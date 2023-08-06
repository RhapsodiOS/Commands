/*
 * dpkg - main program for package management
 * remove.c - functionality for removing packages
 *
 * Copyright (C) 1995 Ian Jackson <iwj10@cus.cam.ac.uk>
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2,
 * or (at your option) any later version.
 *
 * This is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with dpkg; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include <dpkg.h>
#include <dpkg-db.h>
#include <dpkg-var.h>
#include <myopt.h>

#include "filesdb.h"
#include "main.h"
#include "dpkg-int.h"
#include "config.h"

#ifndef ELOOP
#define ELOOP -1
#endif

static void checkforremoval(struct pkginfo *pkgtoremove,
                            struct pkginfo *pkgdepcheck, /* may be virtual pkg */
                            int *rokp, struct varbuf *raemsgs) {
  struct deppossi *possi;
  struct pkginfo *depender;
  int before, ok;
  
  for (possi= pkgdepcheck->installed.depended; possi; possi= possi->nextrev) {
    if (possi->up->type != dep_depends && possi->up->type != dep_predepends) continue;
    depender= possi->up->up;
    debug(dbg_depcon,"checking depending package `%s'",depender->name);
    if (depender->status != stat_installed) continue;
    if (ignore_depends(depender)) {
      debug(dbg_depcon,"ignoring depending package `%s'\n",depender->name);
      continue;
    }
    if (dependtry > 1) { if (findbreakcycle(pkgdepcheck,0)) sincenothing= 0; }
    before= raemsgs->used;
    ok= dependencies_ok(depender,pkgtoremove,raemsgs);
    if (ok == 0 && depender->clientdata->istobe == itb_remove) ok= 1;
    if (ok == 1) raemsgs->used= before; /* Don't burble about reasons for deferral */
    if (ok < *rokp) *rokp= ok;
  }
}

void deferred_remove(struct pkginfo *pkg) {
  struct varbuf raemsgs;
  int rok;
  struct dependency *dep;

  debug(dbg_general,"deferred_remove package %s",pkg->name);
  
  if (pkg->status == stat_notinstalled) {
    fprintf(stderr,
            _("dpkg - warning: ignoring request to remove %.250s which isn't installed.\n"),
            pkg->name);
    pkg->clientdata->istobe= itb_normal;
    return;
  } else if (!f_pending &&
             pkg->status == stat_configfiles &&
             cipaction->arg != act_purge) {
    fprintf(stderr,
            _("dpkg - warning: ignoring request to remove %.250s, only the config\n"
            " files of which are on the system.  Use --purge to remove them too.\n"),
            pkg->name);
    pkg->clientdata->istobe= itb_normal;
    return;
  }

  assert(pkg->installed.valid);
  if (pkg->installed.essential && pkg->status != stat_configfiles)
    forcibleerr(fc_removeessential, _("This is an essential package -"
                " it should not be removed."));

  if (!f_pending)
    pkg->want= (cipaction->arg == act_purge) ? want_purge : want_deinstall;
  if (!f_noact) modstatdb_note(pkg);

  debug(dbg_general,"checking dependencies for remove `%s'",pkg->name);
  varbufinit(&raemsgs);
  rok= 2;
  checkforremoval(pkg,pkg,&rok,&raemsgs);
  for (dep= pkg->installed.depends; dep; dep= dep->next) {
    if (dep->type != dep_provides) continue;
    debug(dbg_depcon,"checking virtual package `%s'",dep->list->ed->name);
    checkforremoval(pkg,dep->list->ed,&rok,&raemsgs);
  }

  if (rok == 1) {
    varbuffree(&raemsgs);
    pkg->clientdata->istobe= itb_remove;
    add_to_queue(pkg);
    return;
  } else if (rok == 0) {
    sincenothing= 0;
    varbufaddc(&raemsgs,0);
    fprintf(stderr,
            _("dpkg: dependency problems prevent removal of %s:\n%s"),
            pkg->name, raemsgs.buf);
    ohshit(_("dependency problems - not removing"));
  } else if (raemsgs.used) {
    varbufaddc(&raemsgs,0);
    fprintf(stderr,
            _("dpkg: %s: dependency problems, but removing anyway as you request:\n%s"),
            pkg->name, raemsgs.buf);
  }
  varbuffree(&raemsgs);
  sincenothing= 0;

  if (pkg->eflag & eflagf_reinstreq)
    forcibleerr(fc_removereinstreq,
                _("Package is in a very bad inconsistent state - you should\n"
                " reinstall it before attempting a removal."));

  ensure_allinstfiles_available();
  filesdbinit();

  if (f_noact) {
    printf(_("Would remove or purge %s ...\n"),pkg->name);
    pkg->status= stat_notinstalled;
    pkg->clientdata->istobe= itb_normal;
    return;
  }

  oldconffsetflags(pkg->installed.conffiles);
    
  printf(_("Removing %s ...\n"),pkg->name);
  if (pkg->status == stat_halfconfigured || pkg->status == stat_installed) {

    if (pkg->status == stat_installed || pkg->status == stat_halfconfigured) {
      pkg->status= stat_halfconfigured;
      modstatdb_note(pkg);
      push_cleanup(cu_prermremove,~ehflag_normaltidy, 0,0, 1,(void*)pkg);
      /* submsg: maintainer_script_installed */
      maintainer_script_installed(pkg, PRERMFILE, _("pre-removal"),
                                  "remove", (char*)0);
    }

    pkg->status= stat_unpacked; /* Will turn into halfinstalled soon ... */
  }

  removal_bulk(pkg);
}

static void push_leftover(struct fileinlist **leftoverp,
                          struct filenamenode *namenode) {
  struct fileinlist *newentry;
  newentry= nfmalloc(sizeof(struct fileinlist));
  newentry->next= *leftoverp;
  newentry->namenode= namenode;
  *leftoverp= newentry;
}

void removal_bulk(struct pkginfo *pkg) {
  /* This is used both by deferred_remove in this file, and at
   * the end of process_archive in archives.c if it needs to finish
   * removing a conflicting package.
   */

  static struct varbuf fnvb, removevb;
  
  int before, r, foundpostrm, removevbbase;
  int infodirbaseused, conffnameused, conffbasenamelen, pkgnameused;
  char *conffbasename;
  struct reversefilelistiter rlistit;
  struct conffile *conff, **lconffp;
  struct fileinlist *searchfile, *leftover;
  struct stat stab;
  DIR *dsd;
  struct dirent *de;
  char *p;
  const char *const *ext;
  const char *postrmfilename;
  struct filenamenode *namenode;

  debug(dbg_general,"removal_bulk package %s",pkg->name);
  
  if (pkg->status == stat_halfinstalled || pkg->status == stat_unpacked) {
    pkg->status= stat_halfinstalled;
    modstatdb_note(pkg);
    push_checkpoint(~ehflag_bombout, ehflag_normaltidy);

    reversefilelist_init(&rlistit,pkg->clientdata->files);
    leftover= 0;
    while ((namenode= reversefilelist_next(&rlistit))) {
      debug(dbg_eachfile, "removal_bulk `%s' flags=%o",
            namenode->name, namenode->flags);
      if (namenode->flags & fnnf_old_conff) {
        push_leftover(&leftover,namenode);
        continue;
      }
      varbufreset(&fnvb);
      varbufaddstr(&fnvb,instdir);
      varbufaddstr(&fnvb,namenodetouse(namenode,pkg)->name);
      before= fnvb.used;
      
      varbufaddstr(&fnvb,DPKGTEMPEXT);
      varbufaddc(&fnvb,0);
      debug(dbg_eachfiledetail, "removal_bulk cleaning temp `%s'", fnvb.buf);
      
      ensure_pathname_nonexisting(fnvb.buf);
      
      fnvb.used= before;
      varbufaddstr(&fnvb,DPKGNEWEXT);
      varbufaddc(&fnvb,0);
      debug(dbg_eachfiledetail, "removal_bulk cleaning new `%s'", fnvb.buf);
      ensure_pathname_nonexisting(fnvb.buf);
      
      fnvb.used= before;
      varbufaddc(&fnvb,0);
      if (!stat(fnvb.buf,&stab) && S_ISDIR(stab.st_mode)) {
        debug(dbg_eachfiledetail, "removal_bulk is a directory");
        /* Only delete a directory or a link to one if we're the only
         * package which uses it.  Other files should only be listed
         * in this package (but we don't check).
         */
        if (isdirectoryinuse(namenode,pkg)) continue;
      }
      debug(dbg_eachfiledetail, "removal_bulk removing `%s'", fnvb.buf);
      if (!rmdir(fnvb.buf) || errno == ENOENT || errno == ELOOP) continue;
      if (errno == ENOTEMPTY) {
        fprintf(stderr,
                _("dpkg - warning: while removing %.250s, directory `%.250s' not empty "
                "so not removed.\n"),
                pkg->name, namenode->name);
        push_leftover(&leftover,namenode);
        continue;
      } else if (errno == EBUSY || errno == EPERM) {
        fprintf(stderr, _("dpkg - warning: while removing %.250s,"
                " unable to remove directory `%.250s':"
                " %s - directory may be a mount point ?\n"),
                pkg->name, namenode->name, strerror(errno));
        push_leftover(&leftover,namenode);
        continue;
      }
      if (errno != ENOTDIR) ohshite(_("cannot remove `%.250s'"),fnvb.buf);
      debug(dbg_eachfiledetail, "removal_bulk unlinking `%s'", fnvb.buf);
      {
        /*
         * If file to remove is a device or s[gu]id, change its mode
         * so that a malicious user cannot use it even if it's linked
         * to another file
         */
        struct stat stat_buf;
        if (stat(fnvb.buf,&stat_buf)==0) {
          if (S_ISCHR(stat_buf.st_mode) || S_ISBLK(stat_buf.st_mode)) {
            chmod(fnvb.buf,0);
          }
          if (stat_buf.st_mode & (S_ISUID|S_ISGID)) {
            chmod(fnvb.buf,stat_buf.st_mode & ~(S_ISUID|S_ISGID));
          }
        }
      }
   if (unlink(fnvb.buf)) ohshite(_("cannot remove file `%.250s'"),fnvb.buf);
    }
    write_filelist_except(pkg,leftover,0);
    /* submsg: maintainer_script_installed */
    maintainer_script_installed(pkg, POSTRMFILE, _("post-removal"),
                                "remove", (char*)0);
    varbufreset(&fnvb);
    varbufaddstr(&fnvb,admindir);
    varbufaddstr(&fnvb,"/");
    varbufaddstr(&fnvb,INFODIR);
    varbufaddstr(&fnvb,"/");
    infodirbaseused= fnvb.used;
    varbufaddc(&fnvb,0);
    dsd= opendir(fnvb.buf); if (!dsd) ohshite(_("cannot read info directory"));
    push_cleanup(cu_closedir,~0, 0,0, 1,(void*)dsd);
    foundpostrm= 0;

    debug(dbg_general, "removal_bulk cleaning info directory");

    while ((de= readdir(dsd)) != 0) {
      debug(dbg_veryverbose, "removal_bulk info file `%s'", de->d_name);
      if (de->d_name[0] == '.') continue;
      p= strrchr(de->d_name,'.'); if (!p) continue;
      if (strlen(pkg->name) != p-de->d_name ||
          strncmp(de->d_name,pkg->name,p-de->d_name)) continue;
      debug(dbg_stupidlyverbose, "removal_bulk info this pkg");
      /* We need the postrm and list files for --purge. */
      if (!strcmp(p+1,LISTFILE)) continue;
      if (!strcmp(p+1,POSTRMFILE)) { foundpostrm=1; continue; }
      debug(dbg_stupidlyverbose, "removal_bulk info not postrm or list");
      fnvb.used= infodirbaseused;
      varbufaddstr(&fnvb,de->d_name);
      varbufaddc(&fnvb,0);
      if (unlink(fnvb.buf))
        ohshite(_("unable to delete control info file `%.250s'"),fnvb.buf);
      debug(dbg_scripts, "removal_bulk info unlinked %s",fnvb.buf);
    }
    pop_cleanup(ehflag_normaltidy); /* closedir */
    
    pkg->status= stat_configfiles;
    pkg->installed.essential= 0;
    modstatdb_note(pkg);
    push_checkpoint(~ehflag_bombout, ehflag_normaltidy);

  } else {

    postrmfilename= pkgadminfile(pkg,POSTRMFILE);
    if (!lstat(postrmfilename,&stab)) foundpostrm= 1;
    else if (errno == ENOENT) foundpostrm= 0;
    else ohshite(_("unable to check existence of `%.250s'"),postrmfilename);

  }
  
  debug(dbg_general, "removal_bulk purging? foundpostrm=%d",foundpostrm);
  
  if (!foundpostrm && !pkg->installed.conffiles) {
    /* If there are no config files and no postrm script then we
     * go straight into `purge'.
     */
    debug(dbg_general, "removal_bulk no postrm, no conffiles, purging");
    pkg->want= want_purge;

  } else if (pkg->want == want_purge) {

    printf(_("Purging configuration files for %s ...\n"),pkg->name);
    ensure_packagefiles_available(pkg); /* We may have modified this above. */

    /* We're about to remove the configuration, so remove the note
     * about which version it was ...
     */
    blankversion(&pkg->configversion);
    modstatdb_note(pkg);
    
    /* Remove from our list any conffiles that aren't ours any more or
     * are involved in diversions, except if we are the package doing the
     * diverting.
     */
    for (lconffp= &pkg->installed.conffiles;
         (conff= *lconffp) != 0;
         lconffp= &conff->next) {
      for (searchfile= pkg->clientdata->files;
           searchfile && strcmp(searchfile->namenode->name,conff->name);
           searchfile= searchfile->next);
      if (!searchfile) {
        debug(dbg_conff,"removal_bulk conffile not ours any more `%s'",conff->name);
        *lconffp= conff->next;
      } else if (searchfile->namenode->divert &&
                 (searchfile->namenode->divert->camefrom ||
                  (searchfile->namenode->divert->useinstead &&
                   searchfile->namenode->divert->pkg != pkg))) {
        debug(dbg_conff,"removal_bulk conffile `%s' ignored due to diversion\n",
              conff->name);
        *lconffp= conff->next;
      } else {
        debug(dbg_conffdetail,"removal_bulk set to new conffile `%s'",conff->name);
        conff->hash= (char*)NEWCONFFILEFLAG; /* yes, cast away const */
      }
    }
    modstatdb_note(pkg);
    
    for (conff= pkg->installed.conffiles; conff; conff= conff->next) {
      varbufreset(&fnvb);
      r= conffderef(pkg, &fnvb, conff->name);
      debug(dbg_conffdetail, "removal_bulk conffile `%s' (= `%s')",
            conff->name, r == -1 ? "<r==-1>" : fnvb.buf);
      if (r == -1) continue;
      conffnameused= fnvb.used-1;
      if (unlink(fnvb.buf) && errno != ENOENT && errno != ENOTDIR)
        ohshite(_("cannot remove old config file `%.250s' (= `%.250s')"),
                conff->name, fnvb.buf);
      p= strrchr(fnvb.buf,'/'); if (!p) continue;
      *p= 0;
      varbufreset(&removevb);
      varbufaddstr(&removevb,fnvb.buf);
      varbufaddc(&removevb,'/');
      removevbbase= removevb.used;
      varbufaddc(&removevb,0);
      dsd= opendir(removevb.buf);
      if (!dsd) {
        int e=errno;
        debug(dbg_conffdetail, "removal_bulk conffile no dsd %s %s",
              fnvb.buf, strerror(e)); errno= e;
        if (errno == ENOENT || errno == ENOTDIR) continue;
        ohshite(_("cannot read config file dir `%.250s' (from `%.250s')"),
                fnvb.buf, conff->name);
      }
      debug(dbg_conffdetail, "removal_bulk conffile cleaning dsd %s", fnvb.buf);
      push_cleanup(cu_closedir,~0, 0,0, 1,(void*)dsd);
      *p= '/';
      conffbasenamelen= strlen(++p);
      conffbasename= fnvb.buf+conffnameused-conffbasenamelen;
      while ((de= readdir(dsd)) != 0) {
        debug(dbg_stupidlyverbose, "removal_bulk conffile dsd entry=`%s'"
              " conffbasename=`%s' conffnameused=%d conffbasenamelen=%d",
              de->d_name, conffbasename, conffnameused, conffbasenamelen);
        if (!strncmp(de->d_name,conffbasename,conffbasenamelen)) {
          debug(dbg_stupidlyverbose, "removal_bulk conffile dsd entry starts right");
          for (ext= REMOVECONFFEXTS; *ext; ext++)
            if (!strcmp(*ext,de->d_name+conffbasenamelen)) goto yes_remove;
          p= de->d_name+conffbasenamelen;
          if (*p++ == '~') {
            while (*p && isdigit(*p)) p++;
            if (*p == '~' && !*++p) goto yes_remove;
          }
        }
        debug(dbg_stupidlyverbose, "removal_bulk conffile dsd entry starts wrong");
        if (de->d_name[0] == '#' &&
            !strncmp(de->d_name+1,conffbasename,conffbasenamelen) &&
            !strcmp(de->d_name+1+conffbasenamelen,"#"))
          goto yes_remove;
        debug(dbg_stupidlyverbose, "removal_bulk conffile dsd entry not it");
        continue;
      yes_remove:
        removevb.used= removevbbase;
        varbufaddstr(&removevb,de->d_name); varbufaddc(&removevb,0);
        debug(dbg_conffdetail, "removal_bulk conffile dsd entry removing `%s'",
              removevb.buf);
        if (unlink(removevb.buf) && errno != ENOENT && errno != ENOTDIR)
          ohshite(_("cannot remove old backup config file `%.250s' (of `%.250s')"),
                  removevb.buf, conff->name);
      }
      pop_cleanup(ehflag_normaltidy); /* closedir */
    
    }
    
    pkg->installed.conffiles= 0;
    modstatdb_note(pkg);
        
    /* submsg: maintainer_script_installed */
    maintainer_script_installed(pkg, POSTRMFILE, _("post-removal"),
                                "purge", (char*)0);

    /* fixme: retry empty directories */

  }

  if (pkg->want == want_purge) {

    /* ie, either of the two branches above. */
    varbufreset(&fnvb);
    varbufaddstr(&fnvb,admindir);
    varbufaddstr(&fnvb,"/");
    varbufaddstr(&fnvb,INFODIR);
    varbufaddstr(&fnvb,"/");
    varbufaddstr(&fnvb,pkg->name);
    pkgnameused= fnvb.used;
    
    varbufaddstr(&fnvb,".");
    varbufaddstr(&fnvb,LISTFILE);
    varbufaddc(&fnvb,0);
    debug(dbg_general, "removal_bulk purge done, removing list `%s'",fnvb.buf);
    if (unlink(fnvb.buf) && errno != ENOENT) ohshite(_("cannot remove old files list"));
    
    fnvb.used= pkgnameused;
    varbufaddstr(&fnvb,".");
    varbufaddstr(&fnvb,POSTRMFILE);
    varbufaddc(&fnvb,0);
    debug(dbg_general, "removal_bulk purge done, removing postrm `%s'",fnvb.buf);
    if (unlink(fnvb.buf) && errno != ENOENT) ohshite(_("can't remove old postrm script"));

    pkg->status= stat_notinstalled;

    /* This will mess up reverse links, but if we follow them
     * we won't go back because pkg->status is stat_notinstalled.
     */
    pkg->installed.depends= 0;
    pkg->installed.essential= 0;
    pkg->installed.description= pkg->installed.maintainer= 0;
    pkg->installed.source= pkg->installed.installedsize= 0;
    blankversion(&pkg->installed.version);
    pkg->installed.arbs= 0;
  }
      
  pkg->eflag= eflagv_ok;
  modstatdb_note(pkg);

  debug(dbg_general, "removal done");
}

