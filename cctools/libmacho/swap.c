/*
 * Copyright (c) 1999 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * Portions Copyright (c) 1999 Apple Computer, Inc.  All Rights
 * Reserved.  This file contains Original Code and/or Modifications of
 * Original Code as defined in and that are subject to the Apple Public
 * Source License Version 1.1 (the "License").  You may not use this file
 * except in compliance with the License.  Please obtain a copy of the
 * License at http://www.apple.com/publicsource and read it before using
 * this file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON- INFRINGEMENT.  Please see the
 * License for the specific language governing rights and limitations
 * under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */
#import <mach-o/swap.h>
#import <string.h>

void
swap_fat_header(
struct fat_header *fat_header,
enum NXByteOrder target_byte_sex)
{
	fat_header->magic     = NXSwapLong(fat_header->magic);
	fat_header->nfat_arch = NXSwapLong(fat_header->nfat_arch);
}

void
swap_fat_arch(
struct fat_arch *fat_archs,
unsigned long nfat_arch,
enum NXByteOrder target_byte_sex)
{
    unsigned long i;

	for(i = 0; i < nfat_arch; i++){
	    fat_archs[i].cputype    = NXSwapLong(fat_archs[i].cputype);
	    fat_archs[i].cpusubtype = NXSwapLong(fat_archs[i].cpusubtype);
	    fat_archs[i].offset     = NXSwapLong(fat_archs[i].offset);
	    fat_archs[i].size       = NXSwapLong(fat_archs[i].size);
	    fat_archs[i].align      = NXSwapLong(fat_archs[i].align);
	}
}

void
swap_mach_header(
struct mach_header *mh,
enum NXByteOrder target_byte_sex)
{
	mh->magic = NXSwapLong(mh->magic);
	mh->cputype = NXSwapLong(mh->cputype);
	mh->cpusubtype = NXSwapLong(mh->cpusubtype);
	mh->filetype = NXSwapLong(mh->filetype);
	mh->ncmds = NXSwapLong(mh->ncmds);
	mh->sizeofcmds = NXSwapLong(mh->sizeofcmds);
	mh->flags = NXSwapLong(mh->flags);
}

void
swap_load_command(
struct load_command *lc,
enum NXByteOrder target_byte_sex)
{
	lc->cmd = NXSwapLong(lc->cmd);
	lc->cmdsize = NXSwapLong(lc->cmdsize);
}

void
swap_segment_command(
struct segment_command *sg,
enum NXByteOrder target_byte_sex)
{
	/* segname[16] */
	sg->cmd = NXSwapLong(sg->cmd);
	sg->cmdsize = NXSwapLong(sg->cmdsize);
	sg->vmaddr = NXSwapLong(sg->vmaddr);
	sg->vmsize = NXSwapLong(sg->vmsize);
	sg->fileoff = NXSwapLong(sg->fileoff);
	sg->filesize = NXSwapLong(sg->filesize);
	sg->maxprot = NXSwapLong(sg->maxprot);
	sg->initprot = NXSwapLong(sg->initprot);
	sg->nsects = NXSwapLong(sg->nsects);
	sg->flags = NXSwapLong(sg->flags);
}

void
swap_section(
struct section *s,
unsigned long nsects,
enum NXByteOrder target_byte_sex)
{
    unsigned long i;

	for(i = 0; i < nsects; i++){
	    /* sectname[16] */
	    /* segname[16] */
	    s[i].addr = NXSwapLong(s[i].addr);
	    s[i].size = NXSwapLong(s[i].size);
	    s[i].offset = NXSwapLong(s[i].offset);
	    s[i].align = NXSwapLong(s[i].align);
	    s[i].reloff = NXSwapLong(s[i].reloff);
	    s[i].nreloc = NXSwapLong(s[i].nreloc);
	    s[i].flags = NXSwapLong(s[i].flags);
	    s[i].reserved1 = NXSwapLong(s[i].reserved1);
	    s[i].reserved2 = NXSwapLong(s[i].reserved2);
	}
}

void
swap_symtab_command(
struct symtab_command *st,
enum NXByteOrder target_byte_sex)
{
	st->cmd = NXSwapLong(st->cmd);
	st->cmdsize = NXSwapLong(st->cmdsize);
	st->symoff = NXSwapLong(st->symoff);
	st->nsyms = NXSwapLong(st->nsyms);
	st->stroff = NXSwapLong(st->stroff);
	st->strsize = NXSwapLong(st->strsize);
}

void
swap_dysymtab_command(
struct dysymtab_command *dyst,
enum NXByteOrder target_byte_sex)
{
	dyst->cmd = NXSwapLong(dyst->cmd);
	dyst->cmdsize = NXSwapLong(dyst->cmdsize);
	dyst->ilocalsym = NXSwapLong(dyst->ilocalsym);
	dyst->nlocalsym = NXSwapLong(dyst->nlocalsym);
	dyst->iextdefsym = NXSwapLong(dyst->iextdefsym);
	dyst->nextdefsym = NXSwapLong(dyst->nextdefsym);
	dyst->iundefsym = NXSwapLong(dyst->iundefsym);
	dyst->nundefsym = NXSwapLong(dyst->nundefsym);
	dyst->tocoff = NXSwapLong(dyst->tocoff);
	dyst->ntoc = NXSwapLong(dyst->ntoc);
	dyst->modtaboff = NXSwapLong(dyst->modtaboff);
	dyst->nmodtab = NXSwapLong(dyst->nmodtab);
	dyst->extrefsymoff = NXSwapLong(dyst->extrefsymoff);
	dyst->nextrefsyms = NXSwapLong(dyst->nextrefsyms);
	dyst->indirectsymoff = NXSwapLong(dyst->indirectsymoff);
	dyst->nindirectsyms = NXSwapLong(dyst->nindirectsyms);
	dyst->extreloff = NXSwapLong(dyst->extreloff);
	dyst->nextrel = NXSwapLong(dyst->nextrel);
	dyst->locreloff = NXSwapLong(dyst->locreloff);
	dyst->nlocrel = NXSwapLong(dyst->nlocrel);
}

void
swap_symseg_command(
struct symseg_command *ss,
enum NXByteOrder target_byte_sex)
{
	ss->cmd = NXSwapLong(ss->cmd);
	ss->cmdsize = NXSwapLong(ss->cmdsize);
	ss->offset = NXSwapLong(ss->offset);
	ss->size = NXSwapLong(ss->size);
}

void
swap_fvmlib_command(
struct fvmlib_command *fl,
enum NXByteOrder target_byte_sex)
{
	fl->cmd = NXSwapLong(fl->cmd);
	fl->cmdsize = NXSwapLong(fl->cmdsize);
	fl->fvmlib.name.offset = NXSwapLong(fl->fvmlib.name.offset);
	fl->fvmlib.minor_version = NXSwapLong(fl->fvmlib.minor_version);
	fl->fvmlib.header_addr = NXSwapLong(fl->fvmlib.header_addr);
}

void
swap_dylib_command(
struct dylib_command *dl,
enum NXByteOrder target_byte_sex)
{
	dl->cmd = NXSwapLong(dl->cmd);
	dl->cmdsize = NXSwapLong(dl->cmdsize);
	dl->dylib.name.offset = NXSwapLong(dl->dylib.name.offset);
	dl->dylib.timestamp = NXSwapLong(dl->dylib.timestamp);
	dl->dylib.current_version = NXSwapLong(dl->dylib.current_version);
	dl->dylib.compatibility_version =
				NXSwapLong(dl->dylib.compatibility_version);
}

void
swap_prebound_dylib_command(
struct prebound_dylib_command *pbdylib,
enum NXByteOrder target_byte_sex)
{
	pbdylib->cmd = NXSwapLong(pbdylib->cmd);
	pbdylib->cmdsize = NXSwapLong(pbdylib->cmdsize);
	pbdylib->name.offset = NXSwapLong(pbdylib->name.offset);
	pbdylib->nmodules = NXSwapLong(pbdylib->nmodules);
	pbdylib->linked_modules.offset =
		NXSwapLong(pbdylib->linked_modules.offset);
}

void
swap_dylinker_command(
struct dylinker_command *dyld,
enum NXByteOrder target_byte_sex)
{
	dyld->cmd = NXSwapLong(dyld->cmd);
	dyld->cmdsize = NXSwapLong(dyld->cmdsize);
	dyld->name.offset = NXSwapLong(dyld->name.offset);
}

void
swap_fvmfile_command(
struct fvmfile_command *ff,
enum NXByteOrder target_byte_sex)
{
	ff->cmd = NXSwapLong(ff->cmd);
	ff->cmdsize = NXSwapLong(ff->cmdsize);
	ff->name.offset = NXSwapLong(ff->name.offset);
	ff->header_addr = NXSwapLong(ff->header_addr);
}


void
swap_thread_command(
struct thread_command *ut,
enum NXByteOrder target_byte_sex)
{
	ut->cmd = NXSwapLong(ut->cmd);
	ut->cmdsize = NXSwapLong(ut->cmdsize);
}

void
swap_ident_command(
struct ident_command *id_cmd,
enum NXByteOrder target_byte_sex)
{
	id_cmd->cmd = NXSwapLong(id_cmd->cmd);
	id_cmd->cmdsize = NXSwapLong(id_cmd->cmdsize);
}

void
swap_nlist(
struct nlist *symbols,
unsigned long nsymbols,
enum NXByteOrder target_byte_sex)
{
    unsigned long i;

	for(i = 0; i < nsymbols; i++){
	    symbols[i].n_un.n_strx = NXSwapLong(symbols[i].n_un.n_strx);
	    /* n_type */
	    /* n_sect */
	    symbols[i].n_desc = NXSwapShort(symbols[i].n_desc);
	    symbols[i].n_value = NXSwapLong(symbols[i].n_value);
	}
}

void
swap_ranlib(
struct ranlib *ranlibs,
unsigned long nranlibs,
enum NXByteOrder target_byte_sex)
{
    unsigned long i;

	for(i = 0; i < nranlibs; i++){
	    ranlibs[i].ran_un.ran_strx = NXSwapLong(ranlibs[i].ran_un.ran_strx);
	    ranlibs[i].ran_off = NXSwapLong(ranlibs[i].ran_off);
	}
}

void
swap_relocation_info(
struct relocation_info *relocs,
unsigned long nrelocs,
enum NXByteOrder target_byte_sex)
{
    unsigned long i;
    enum NXByteOrder host_byte_sex;
    unsigned long to_host_byte_sex, scattered;

    struct swapped_relocation_info {
	long	r_address;
	union {
	    struct {
		unsigned int
		    r_type:4,
		    r_extern:1,
		    r_length:2,
		    r_pcrel:1,
		    r_symbolnum:24;
	    } fields;
	    unsigned long word;
	} u;
    } sr;

    struct swapped_scattered_relocation_info {
	unsigned long word;
	long	r_value;
    } *ssr;

	host_byte_sex = NXHostByteOrder();
	to_host_byte_sex = target_byte_sex == host_byte_sex;

	for(i = 0; i < nrelocs; i++){
	    if(to_host_byte_sex)
		scattered = (NXSwapLong(relocs[i].r_address) & R_SCATTERED) != 0;
	    else
		scattered = ((relocs[i].r_address) & R_SCATTERED) != 0;
	    if(scattered == FALSE){
		if(to_host_byte_sex){
		    memcpy(&sr, relocs + i, sizeof(struct relocation_info));
		    sr.r_address = NXSwapLong(sr.r_address);
		    sr.u.word = NXSwapLong(sr.u.word);
		    relocs[i].r_address = sr.r_address;
		    relocs[i].r_symbolnum = sr.u.fields.r_symbolnum;
		    relocs[i].r_pcrel = sr.u.fields.r_pcrel;
		    relocs[i].r_length = sr.u.fields.r_length;
		    relocs[i].r_extern = sr.u.fields.r_extern;
		    relocs[i].r_type = sr.u.fields.r_type;
		}
		else{
		    sr.r_address = relocs[i].r_address;
		    sr.u.fields.r_symbolnum = relocs[i].r_symbolnum;
		    sr.u.fields.r_length = relocs[i].r_length;
		    sr.u.fields.r_pcrel = relocs[i].r_pcrel;
		    sr.u.fields.r_extern = relocs[i].r_extern;
		    sr.u.fields.r_type = relocs[i].r_type;
		    sr.r_address = NXSwapLong(sr.r_address);
		    sr.u.word = NXSwapLong(sr.u.word);
		    memcpy(relocs + i, &sr, sizeof(struct relocation_info));
		}
	    }
	    else{
		ssr = (struct swapped_scattered_relocation_info *)(relocs + i);
		ssr->word = NXSwapLong(ssr->word);
		ssr->r_value = NXSwapLong(ssr->r_value);
	    }
	}
}

void
swap_indirect_symbols(
unsigned long *indirect_symbols,
unsigned long nindirect_symbols,
enum NXByteOrder target_byte_sex)
{
    unsigned long i;

	for(i = 0; i < nindirect_symbols; i++)
	    indirect_symbols[i] = NXSwapLong(indirect_symbols[i]);
}

void
swap_dylib_reference(
struct dylib_reference *refs,
unsigned long nrefs,
enum NXByteOrder target_byte_sex)
{
    struct swapped_dylib_reference {
	union {
	    struct {
		unsigned long
		    flags:8,
		    isym:24;
	    } fields;
	    unsigned long word;
	} u;
    } sref;

    unsigned long i;
    enum NXByteOrder host_byte_sex;

	host_byte_sex = NXHostByteOrder();

	for(i = 0; i < nrefs; i++){
	    if(target_byte_sex == host_byte_sex){
		memcpy(&sref, refs + i, sizeof(struct swapped_dylib_reference));
		sref.u.word = NXSwapLong(sref.u.word);
		refs[i].flags = sref.u.fields.flags;
		refs[i].isym = sref.u.fields.isym;
	    }
	    else{
		sref.u.fields.isym = refs[i].isym;
		sref.u.fields.flags = refs[i].flags;
		sref.u.word = NXSwapLong(sref.u.word);
		memcpy(refs + i, &sref, sizeof(struct swapped_dylib_reference));
	    }
	}

}

void
swap_dylib_module(
struct dylib_module *mods,
unsigned long nmods,
enum NXByteOrder target_byte_sex)
{
    unsigned long i;

	for(i = 0; i < nmods; i++){
	    mods[i].module_name = NXSwapLong(mods[i].module_name);
	    mods[i].iextdefsym  = NXSwapLong(mods[i].iextdefsym);
	    mods[i].nextdefsym  = NXSwapLong(mods[i].nextdefsym);
	    mods[i].irefsym     = NXSwapLong(mods[i].irefsym);
	    mods[i].nrefsym     = NXSwapLong(mods[i].nrefsym);
	    mods[i].ilocalsym   = NXSwapLong(mods[i].ilocalsym);
	    mods[i].nlocalsym   = NXSwapLong(mods[i].nlocalsym);
	    mods[i].iextrel     = NXSwapLong(mods[i].iextrel);
	    mods[i].nextrel     = NXSwapLong(mods[i].nextrel);
	    mods[i].iinit       = NXSwapLong(mods[i].iinit);
	    mods[i].ninit       = NXSwapLong(mods[i].ninit);
	}
}

void
swap_dylib_table_of_contents(
struct dylib_table_of_contents *tocs,
unsigned long ntocs,
enum NXByteOrder target_byte_sex)
{
    unsigned long i;

	for(i = 0; i < ntocs; i++){
	    tocs[i].symbol_index = NXSwapLong(tocs[i].symbol_index);
	    tocs[i].module_index = NXSwapLong(tocs[i].module_index);
	}
}
