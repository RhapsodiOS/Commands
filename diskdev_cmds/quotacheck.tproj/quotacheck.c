/*
 * Copyright (c) 1999 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * "Portions Copyright (c) 1999 Apple Computer, Inc.  All Rights
 * Reserved.  This file contains Original Code and/or Modifications of
 * Original Code as defined in and that are subject to the Apple Public
 * Source License Version 1.0 (the 'License').  You may not use this file
 * except in compliance with the License.  Please obtain a copy of the
 * License at http://www.apple.com/publicsource and read it before using
 * this file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT.  Please see the
 * License for the specific language governing rights and limitations
 * under the License."
 * 
 * @APPLE_LICENSE_HEADER_END@
 */
/*
 * Copyright (c) 1980, 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Robert Elz at The University of Melbourne.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */


/*
 * Fix up / report on disk quotas & usage
 */
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/queue.h>

#include <ufs/ufs/dinode.h>
#include <ufs/ufs/quota.h>
#include <ufs/ffs/fs.h>

#include <fcntl.h>
#include <fstab.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

#if     REV_ENDIAN_FS
#import "ufs_byte_order.h"
#endif  /* REV_ENDIAN_FS */

char *qfname = QUOTAFILENAME;
char *qfextension[] = INITQFNAMES;
char *quotagroup = QUOTAGROUP;

union {
	struct	fs	sblk;
	char	dummy[MAXBSIZE];
} un;
#define	sblock	un.sblk
long dev_bsize;
long maxino;

struct quotaname {
	long	flags;
	char	grpqfname[MAXPATHLEN + 1];
	char	usrqfname[MAXPATHLEN + 1];
};
#define	HASUSR	1
#define	HASGRP	2

struct fileusage {
	struct	fileusage *fu_next;
	u_long	fu_curinodes;
	u_long	fu_curblocks;
	u_long	fu_id;
	char	fu_name[1];
	/* actually bigger */
};
#define FUHASH 1024	/* must be power of two */
struct fileusage *fuhead[MAXQUOTAS][FUHASH];

int	aflag;			/* all file systems */
int	gflag;			/* check group quotas */
int	uflag;			/* check user quotas */
int	vflag;			/* verbose */
int	fi;			/* open disk file descriptor */
u_long	highid[MAXQUOTAS];	/* highest addid()'ed identifier per type */

#if     REV_ENDIAN_FS
int rev_endian=0;
#endif /* REV_ENDIAN_FS */

struct fileusage *
	 addid __P((u_long, int, char *));
char	*blockcheck __P((char *));
void	 bread __P((daddr_t, char *, long));
int	 chkquota __P((char *, char *, struct quotaname *));
void	 freeinodebuf __P((void));
struct dinode *
	 getnextinode __P((ino_t));
int	 getquotagid __P((void));
int	 hasquota __P((struct fstab *, int, char **));
struct fileusage *
	 lookup __P((u_long, int));
void	*needchk __P((struct fstab *));
int	 oneof __P((char *, char*[], int));
void	 resetinodebuf __P((void));
int	 update __P((char *, char *, int));
void	 usage __P((void));

int
main(argc, argv)
	int argc;
	char *argv[];
{
	register struct fstab *fs;
	register struct passwd *pw;
	register struct group *gr;
	struct quotaname *auxdata;
	int i, argnum, maxrun, errs;
	long done = 0;
	char ch, *name;

	errs = maxrun = 0;
	while ((ch = getopt(argc, argv, "aguvl:")) != EOF) {
		switch(ch) {
		case 'a':
			aflag++;
			break;
		case 'g':
			gflag++;
			break;
		case 'u':
			uflag++;
			break;
		case 'v':
			vflag++;
			break;
		case 'l':
			maxrun = atoi(optarg);
			break;
		default:
			usage();
		}
	}
	argc -= optind;
	argv += optind;
	if ((argc == 0 && !aflag) || (argc > 0 && aflag))
		usage();
	if (!gflag && !uflag) {
		gflag++;
		uflag++;
	}
	if (gflag) {
		setgrent();
#ifdef __APPLE__
		while ((gr = getgrent()) != 0) {
		    /* Ignore negative groupid for nogroup and nobody */
		    if ((long)gr->gr_gid == -1 || (long)gr->gr_gid == -2)
		      continue;
		    (void) addid((u_long)gr->gr_gid, GRPQUOTA, gr->gr_name);
		}
#else
		while ((gr = getgrent()) != 0)
			(void) addid((u_long)gr->gr_gid, GRPQUOTA, gr->gr_name);
#endif /* __APPLE__ */
		endgrent();
	}
	if (uflag) {
		setpwent();
#ifdef __APPLE__
		while ((pw = getpwent()) != 0) {
		    if ((long)pw->pw_uid == -2)   /*Ignore userid for nobody*/
		      continue;
		    (void) addid((u_long)pw->pw_uid, USRQUOTA, pw->pw_name);
		}
#else
		while ((pw = getpwent()) != 0)
			(void) addid((u_long)pw->pw_uid, USRQUOTA, pw->pw_name);
#endif /* __APPLE__ */
		endpwent();
	}
	if (aflag)
		exit(checkfstab(1, maxrun, needchk, chkquota));
	if (setfsent() == 0)
		err(1, "%s: can't open", FSTAB);
	while ((fs = getfsent()) != NULL) {
		if (((argnum = oneof(fs->fs_file, argv, argc)) >= 0 ||
		    (argnum = oneof(fs->fs_spec, argv, argc)) >= 0) &&
		    (auxdata = needchk(fs)) &&
		    (name = blockcheck(fs->fs_spec))) {
			done |= 1 << argnum;
			errs += chkquota(name, fs->fs_file, auxdata);
		}
	}
	endfsent();
	for (i = 0; i < argc; i++)
		if ((done & (1 << i)) == 0)
			fprintf(stderr, "%s not found in %s\n",
				argv[i], FSTAB);
	exit(errs);
}

void
usage()
{
	(void)fprintf(stderr, "usage:\t%s\n\t%s\n",
		"quotacheck -a [-guv]",
		"quotacheck [-guv] filesys ...");
	exit(1);
}

void *
needchk(fs)
	register struct fstab *fs;
{
	register struct quotaname *qnp;
	char *qfnp;

	if (strcmp(fs->fs_vfstype, "ufs") ||
	    strcmp(fs->fs_type, FSTAB_RW))
		return (NULL);
	if ((qnp = malloc(sizeof(*qnp))) == NULL)
		err(1, "%s", strerror(errno));
	qnp->flags = 0;
	if (gflag && hasquota(fs, GRPQUOTA, &qfnp)) {
		strcpy(qnp->grpqfname, qfnp);
		qnp->flags |= HASGRP;
	}
	if (uflag && hasquota(fs, USRQUOTA, &qfnp)) {
		strcpy(qnp->usrqfname, qfnp);
		qnp->flags |= HASUSR;
	}
	if (qnp->flags)
		return (qnp);
	free(qnp);
	return (NULL);
}

/*
 * Scan the specified filesystem to check quota(s) present on it.
 */
int
chkquota(fsname, mntpt, qnp)
	char *fsname, *mntpt;
	register struct quotaname *qnp;
{
	register struct fileusage *fup;
	register struct dinode *dp;
	int cg, i, mode, errs = 0;
	ino_t ino;

	if ((fi = open(fsname, O_RDONLY, 0)) < 0) {
		perror(fsname);
		return (1);
	}
	if (vflag) {
		(void)printf("*** Checking ");
		if (qnp->flags & HASUSR)
			(void)printf("%s%s", qfextension[USRQUOTA],
			    (qnp->flags & HASGRP) ? " and " : "");
		if (qnp->flags & HASGRP)
			(void)printf("%s", qfextension[GRPQUOTA]);
		(void)printf(" quotas for %s (%s)\n", fsname, mntpt);
	}
	sync();
	dev_bsize = 1;
	bread(SBOFF, (char *)&sblock, (long)SBSIZE);

#ifdef __APPLE__
        if (sblock.fs_magic != FS_MAGIC) {
#if     REV_ENDIAN_FS
            byte_swap_sbin(&sblock);
	    if (sblock.fs_magic != FS_MAGIC) {
#endif  /* REV_ENDIAN_FS */
                (void)printf("%s: superblock has bad magic number, skipped",
			     fsname);
                (void)close(fi);
                return (1);
#if     REV_ENDIAN_FS
           } else {
	         if(vflag)
                     (void)printf("Reverse Byte order Filesystem Detected\n");
                 rev_endian=1;
           }
#endif  /* REV_ENDIAN_FS */
	}
#if     REV_ENDIAN_FS
        else rev_endian=0;
#endif  /* REV_ENDIAN_FS */
#endif /* __APPLE__ */

	dev_bsize = sblock.fs_fsize / fsbtodb(&sblock, 1);
	maxino = sblock.fs_ncg * sblock.fs_ipg;
	resetinodebuf();
	for (ino = 0, cg = 0; cg < sblock.fs_ncg; cg++) {
		for (i = 0; i < sblock.fs_ipg; i++, ino++) {
			if (ino < ROOTINO)
				continue;
			if ((dp = getnextinode(ino)) == NULL)
				continue;
			if ((mode = dp->di_mode & IFMT) == 0)
				continue;
			if (qnp->flags & HASGRP) {
				fup = addid((u_long)dp->di_gid, GRPQUOTA,
				    (char *)0);
				fup->fu_curinodes++;
				if (mode == IFREG || mode == IFDIR ||
				    mode == IFLNK)
					fup->fu_curblocks += dp->di_blocks;
			}
			if (qnp->flags & HASUSR) {
				fup = addid((u_long)dp->di_uid, USRQUOTA,
				    (char *)0);
				fup->fu_curinodes++;
				if (mode == IFREG || mode == IFDIR ||
				    mode == IFLNK)
					fup->fu_curblocks += dp->di_blocks;
			}
		}
	}
	freeinodebuf();
	if (qnp->flags & HASUSR)
		errs += update(mntpt, qnp->usrqfname, USRQUOTA);
	if (qnp->flags & HASGRP)
		errs += update(mntpt, qnp->grpqfname, GRPQUOTA);
	close(fi);
	return (errs);
}

/*
 * Update a specified quota file.
 */
int
update(fsname, quotafile, type)
	char *fsname, *quotafile;
	register int type;
{
	register struct fileusage *fup;
	register FILE *qfi, *qfo;
	register u_long id, lastid;
	struct dqblk dqbuf;
	static int warned = 0;
	static struct dqblk zerodqbuf;
	static struct fileusage zerofileusage;

	if ((qfo = fopen(quotafile, "r+")) == NULL) {
		if (errno == ENOENT)
			qfo = fopen(quotafile, "w+");
		if (qfo) {
			(void) fprintf(stderr,
			    "quotacheck: creating quota file %s\n", quotafile);
#define	MODE	(S_IRUSR|S_IWUSR|S_IRGRP)
			(void) fchown(fileno(qfo), getuid(), getquotagid());
			(void) fchmod(fileno(qfo), MODE);
		} else {
			(void) fprintf(stderr,
			    "quotacheck: %s: %s\n", quotafile, strerror(errno));
			return (1);
		}
	}
	if ((qfi = fopen(quotafile, "r")) == NULL) {
		(void) fprintf(stderr,
		    "quotacheck: %s: %s\n", quotafile, strerror(errno));
		(void) fclose(qfo);
		return (1);
	}
	if (quotactl(fsname, QCMD(Q_SYNC, type), (u_long)0, (caddr_t)0) < 0 &&
	    errno == EOPNOTSUPP && !warned && vflag) {
		warned++;
		(void)printf("*** Warning: %s\n",
		    "Quotas are not compiled into this kernel");
	}
	for (lastid = highid[type], id = 0; id <= lastid; id++) {
		if (fread((char *)&dqbuf, sizeof(struct dqblk), 1, qfi) == 0)
			dqbuf = zerodqbuf;
		if ((fup = lookup(id, type)) == 0)
			fup = &zerofileusage;
		if (dqbuf.dqb_curinodes == fup->fu_curinodes &&
		    dqbuf.dqb_curblocks == fup->fu_curblocks) {
			fup->fu_curinodes = 0;
			fup->fu_curblocks = 0;
			fseek(qfo, (long)sizeof(struct dqblk), 1);
			continue;
		}
		if (vflag) {
			if (aflag)
				printf("%s: ", fsname);
			printf("%-8s fixed:", fup->fu_name);
			if (dqbuf.dqb_curinodes != fup->fu_curinodes)
				(void)printf("\tinodes %d -> %d",
					dqbuf.dqb_curinodes, fup->fu_curinodes);
			if (dqbuf.dqb_curblocks != fup->fu_curblocks)
				(void)printf("\tblocks %d -> %d",
					dqbuf.dqb_curblocks, fup->fu_curblocks);
			(void)printf("\n");
		}
		/*
		 * Reset time limit if have a soft limit and were
		 * previously under it, but are now over it.
		 */
		if (dqbuf.dqb_bsoftlimit &&
		    dqbuf.dqb_curblocks < dqbuf.dqb_bsoftlimit &&
		    fup->fu_curblocks >= dqbuf.dqb_bsoftlimit)
			dqbuf.dqb_btime = 0;
		if (dqbuf.dqb_isoftlimit &&
		    dqbuf.dqb_curblocks < dqbuf.dqb_isoftlimit &&
		    fup->fu_curblocks >= dqbuf.dqb_isoftlimit)
			dqbuf.dqb_itime = 0;
		dqbuf.dqb_curinodes = fup->fu_curinodes;
		dqbuf.dqb_curblocks = fup->fu_curblocks;
		fwrite((char *)&dqbuf, sizeof(struct dqblk), 1, qfo);
		(void) quotactl(fsname, QCMD(Q_SETUSE, type), id,
		    (caddr_t)&dqbuf);
		fup->fu_curinodes = 0;
		fup->fu_curblocks = 0;
	}
	fclose(qfi);
	fflush(qfo);
	ftruncate(fileno(qfo),
	    (off_t)((highid[type] + 1) * sizeof(struct dqblk)));
	fclose(qfo);
	return (0);
}

/*
 * Check to see if target appears in list of size cnt.
 */
int
oneof(target, list, cnt)
	register char *target, *list[];
	int cnt;
{
	register int i;

	for (i = 0; i < cnt; i++)
		if (strcmp(target, list[i]) == 0)
			return (i);
	return (-1);
}

/*
 * Determine the group identifier for quota files.
 */
int
getquotagid()
{
	struct group *gr;

	if (gr = getgrnam(quotagroup))
		return (gr->gr_gid);
	return (-1);
}

/*
 * Check to see if a particular quota is to be enabled.
 */
int
hasquota(fs, type, qfnamep)
	register struct fstab *fs;
	int type;
	char **qfnamep;
{
	register char *opt;
	char *cp;
	static char initname, usrname[100], grpname[100];
	static char buf[BUFSIZ];

	if (!initname) {
		(void)snprintf(usrname, sizeof(usrname),
		    "%s%s", qfextension[USRQUOTA], qfname);
		(void)snprintf(grpname, sizeof(grpname),
		    "%s%s", qfextension[GRPQUOTA], qfname);
		initname = 1;
	}
	strcpy(buf, fs->fs_mntops);
	for (opt = strtok(buf, ","); opt; opt = strtok(NULL, ",")) {
		if (cp = strchr(opt, '='))
			*cp++ = '\0';
		if (type == USRQUOTA && strcmp(opt, usrname) == 0)
			break;
		if (type == GRPQUOTA && strcmp(opt, grpname) == 0)
			break;
	}
	if (!opt)
		return (0);
	if (cp)
		*qfnamep = cp;
	else {
		(void)snprintf(buf, sizeof(buf),
		    "%s/%s.%s", fs->fs_file, qfname, qfextension[type]);
		*qfnamep = buf;
	}
	return (1);
}

/*
 * Routines to manage the file usage table.
 *
 * Lookup an id of a specific type.
 */
struct fileusage *
lookup(id, type)
	u_long id;
	int type;
{
	register struct fileusage *fup;

	for (fup = fuhead[type][id & (FUHASH-1)]; fup != 0; fup = fup->fu_next)
		if (fup->fu_id == id)
			return (fup);
	return (NULL);
}

/*
 * Add a new file usage id if it does not already exist.
 */
struct fileusage *
addid(id, type, name)
	u_long id;
	int type;
	char *name;
{
	struct fileusage *fup, **fhp;
	int len;

	if (fup = lookup(id, type))
		return (fup);
	if (name)
		len = strlen(name);
	else
		len = 10;
	if ((fup = calloc(1, sizeof(*fup) + len)) == NULL)
		err(1, "%s", strerror(errno));
	fhp = &fuhead[type][id & (FUHASH - 1)];
	fup->fu_next = *fhp;
	*fhp = fup;
	fup->fu_id = id;
	if (id > highid[type])
		highid[type] = id;
	if (name)
		memmove(fup->fu_name, name, len + 1);
	else
		(void)sprintf(fup->fu_name, "%u", id);
	return (fup);
}

/*
 * Special purpose version of ginode used to optimize pass
 * over all the inodes in numerical order.
 */
ino_t nextino, lastinum;
long readcnt, readpercg, fullcnt, inobufsize, partialcnt, partialsize;
struct dinode *inodebuf;
#define	INOBUFSIZE	56*1024	/* size of buffer to read inodes */

struct dinode *
getnextinode(inumber)
	ino_t inumber;
{
	long size;
	daddr_t dblk;
	static struct dinode *dp;

	if (inumber != nextino++ || inumber > maxino)
		err(1, "bad inode number %d to nextinode", inumber);
	if (inumber >= lastinum) {
		readcnt++;
		dblk = fsbtodb(&sblock, ino_to_fsba(&sblock, lastinum));
		if (readcnt % readpercg == 0) {
			size = partialsize;
			lastinum += partialcnt;
		} else {
			size = inobufsize;
			lastinum += fullcnt;
		}
		bread(dblk, (char *)inodebuf, size);
#if     REV_ENDIAN_FS
		if (rev_endian)
		    byte_swap_dinodecount(inodebuf, size, 0);
#endif /* REV_ENDIAN_FS */ 
		dp = inodebuf;
	}
	return (dp++);
}

/*
 * Prepare to scan a set of inodes.
 */
void
resetinodebuf()
{

	nextino = 0;
	lastinum = 0;
	readcnt = 0;
	inobufsize = blkroundup(&sblock, INOBUFSIZE);
	fullcnt = inobufsize / sizeof(struct dinode);
	readpercg = sblock.fs_ipg / fullcnt;
	partialcnt = sblock.fs_ipg % fullcnt;
	partialsize = partialcnt * sizeof(struct dinode);
	if (partialcnt != 0) {
		readpercg++;
	} else {
		partialcnt = fullcnt;
		partialsize = inobufsize;
	}
	if (inodebuf == NULL &&
	   (inodebuf = malloc((u_int)inobufsize)) == NULL)
		err(1, "%s", strerror(errno));
	while (nextino < ROOTINO)
		getnextinode(nextino);
}

/*
 * Free up data structures used to scan inodes.
 */
void
freeinodebuf()
{

	if (inodebuf != NULL)
		free(inodebuf);
	inodebuf = NULL;
}

/*
 * Read specified disk blocks.
 */
void
bread(bno, buf, cnt)
	daddr_t bno;
	char *buf;
	long cnt;
{

	if (lseek(fi, (off_t)bno * dev_bsize, SEEK_SET) < 0 ||
	    read(fi, buf, cnt) != cnt)
		err(1, "block %ld", bno);
}