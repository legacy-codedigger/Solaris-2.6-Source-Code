/*
 * Copyright (c) 1994, 1996 by
 * Sun Microsystems, Inc.  All Rights Reserved.
 */

#pragma ident "@(#)setuppr.c 1.9	96/07/28 SMI"

/*
 * adb - routines to read a.out+core at startup
 */

#include <procfs.h>

#include "adb.h"
#include <stdio.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/proc.h>
#if !defined(KADB)
#include <sys/ptrace.h>
#endif	/* KADB */
#include <sys/cpuvar.h>
#include <sys/vmmac.h>
#include <sys/file.h>
#include <sys/utsname.h>
#include <sys/auxv.h>
#include <string.h>
#include "symtab.h"
#if !defined(KADB)
#include "fpascii.h"
#endif	/* KADB */

off_t	datbas;			/* offset of the base of the data segment */
off_t	stksiz;			/* stack size in the core image */
off_t   textaddr();		/* address of the text segment */

struct asym *trampsym;
struct map_range f_rng1, f_rng2, d_rng1, d_rng2;

#if !defined(KADB)
#include <kvm.h>
#include <sys/dumphdr.h>

char	*symfil = "a.out";
char	*corfil = "core";

extern kvm_t *kvmd;		/* see main.c */
extern int use_shlib;   /* non-zero __DYNAMIC ==> extended maps for text */
#endif	/* !KADB */

int		textseg;	/* index of text segment header */
int		dataseg = -1;	/* index of data segment header */
Elf32_Shdr	*secthdr;	/* ELF section header */
psinfo_t	core_psinfo;	/* struct prpsinfo from core file */
char		core_platform[SYS_NMLN];
				/* platform upon which core file created */
auxv_t		core_auxv[NUM_AUX_VECTORS];
				/* auxiliary vector from core file */
char *rtld_path;


setsym(void)
{
	off_t loc;
	unsigned long txtbas;
	struct map_range *fsym_rng1, *fsym_rng2;
	int i, symfile_type = AOUT;

	db_printf(4, "setsym: called");

#if !defined(KADB)
	/*
	 * Make sure all the open files are closed and the allocated
	 * space is freed before (re)assigning the maps.
	 */
	fsym_rng1 = txtmap.map_head;
	while (fsym_rng1 != NULL) {
		free(fsym_rng1->mpr_fn);
		if (fsym_rng1->mpr_fd > 0)
			(void) close(fsym_rng1->mpr_fd);
		if (fsym_rng1 == txtmap.map_tail)
			break;
		fsym_rng2 = fsym_rng1;
		fsym_rng1 = fsym_rng1->mpr_next;
		if (fsym_rng2 != &f_rng1 && fsym_rng2 != &f_rng2)
			free(fsym_rng2);
	}

#endif	/* !KADB */
	txtmap.map_head = fsym_rng1 = &f_rng1;
	txtmap.map_tail = fsym_rng2 = &f_rng2;
	fsym_rng1->mpr_next = fsym_rng2;

#if defined(KADB)
	datmap.map_head = txtmap.map_head;
	datmap.map_tail = txtmap.map_tail;
#else	/* !KADB */
	fsym_rng1->mpr_fn = fsym_rng2->mpr_fn  = symfil;
	fsym_rng1->mpr_fd  = fsym_rng2->mpr_fd = fsym = getfile(symfil, 1);

	if (read(fsym, (char *) &filhdr, sizeof (filhdr)) != sizeof (filhdr) ||
		filhdr.e_ident[EI_MAG0] != ELFMAG0 ||
		filhdr.e_ident[EI_MAG1] != ELFMAG1 ||
		filhdr.e_ident[EI_MAG2] != ELFMAG2 ||
		filhdr.e_ident[EI_MAG3] != ELFMAG3 ||
		!(filhdr.e_type == ET_EXEC || filhdr.e_type == ET_REL ||
		filhdr.e_type == ET_DYN) ||
		filhdr.e_version != EV_CURRENT) {
			fsym_rng1->mpr_e = MAXFILE;
			return;
	}

#endif	/* KADB */


	if (filhdr.e_type == ET_REL)
		symfile_type = REL;

	switch (filhdr.e_machine) {
	case EM_PPC:
		if (filhdr.e_ident[EI_CLASS] != ELFCLASS32 ||
			filhdr.e_ident[EI_DATA] != ELFDATA2LSB) {
			(void) printf("%s: erroneous PowerPC "
			    "ELF ident field.\n", symfil);
		}
		break;
	case EM_386:
	case EM_486:
	case EM_SPARC:
	case EM_M32:
	case EM_68K:
	case EM_88K:
	case EM_860:
		(void) printf("%s: non-PowerPC ELF ident field.\n",
								symfil);
		break;
	default:
		(void) printf("%s: unknown machine type %D\n",
						symfil, filhdr.e_machine);
		break;
	}
	db_printf(2, "setsym: filhdr.e_phnum %s 0",
					(filhdr.e_phnum == 0) ? "==" : "!=");
	/* Don't necessarily expect a program header. */
	if (filhdr.e_phnum != 0) {
#if !defined(KADB)
		/* Get space for a copy of the program header. */
		if ((proghdr = (Elf32_Phdr *) malloc(filhdr.e_phentsize *
		    filhdr.e_phnum)) == NULL)
			outofmem();
		/* Seek to program header table and read it. */
		if ((loc = lseek(fsym, filhdr.e_phoff, L_SET) !=
			filhdr.e_phoff) ||
		    (read(fsym, (char *)proghdr, filhdr.e_phentsize *
			    filhdr.e_phnum) !=
			filhdr.e_phentsize * filhdr.e_phnum)) {
			(void) printf("Unable to read program header.\n");
			return;
		}
		for (textseg = 0; textseg < (int) filhdr.e_phnum; textseg++) {
			if (proghdr[textseg].p_type == PT_INTERP) {
				Elf32_Phdr *intp_hdr = &proghdr[textseg];
				rtld_path = (void *)
				    malloc(intp_hdr->p_filesz+1);
				if (rtld_path == NULL)
					outofmem();
				if (((loc = lseek(fsym,
					intp_hdr->p_offset, L_SET)) !=
					intp_hdr->p_offset) ||
				    (read(fsym, (char *)rtld_path,
					intp_hdr->p_filesz)
					!= intp_hdr->p_filesz)) {
					(void) printf("Unable to read "
					    "PT_INTERP path.\n");
					return;
				}
				use_shlib = 1;
				db_printf(2, "setsym: use_shlib=1");
				break;
			}
		}
#endif	/* !KADB */
	/*
	 * XXX - Finding the text segment is a problem:
	 * The ABI doesn't show a certain way of distinguishing
	 * text and data segments.  It says only that text "usually"
	 * has "r-x" access and data "usually" has "rwx" access.
	 * I'm counting on finding text followed by data, which is
	 * the tradition.  This could need work.
	 */
		for (textseg = 0; textseg < (int) filhdr.e_phnum; textseg++) {
			if ((proghdr[textseg].p_type == PT_LOAD) &&
				(proghdr[textseg].p_flags & PF_X))
				break;
		}
		fsym_rng1->mpr_b = proghdr[textseg].p_vaddr;
		fsym_rng1->mpr_e = fsym_rng1->mpr_b + proghdr[textseg].p_filesz;
		fsym_rng1->mpr_f = proghdr[textseg].p_offset;

		dataseg = textseg + 1;	/* follows immediately, right? */
		if (dataseg > (int) filhdr.e_phnum) {
			dataseg = -1;	/* no data? */
			fsym_rng2->mpr_b = 0;
			fsym_rng2->mpr_e = MAXFILE;
			fsym_rng2->mpr_f = 0;
		} else {
			fsym_rng2->mpr_b = proghdr[dataseg].p_vaddr;
			fsym_rng2->mpr_e = fsym_rng2->mpr_b +
				proghdr[dataseg].p_filesz;
			fsym_rng2->mpr_f = proghdr[dataseg].p_offset;
		}
	}
	/* Get space for the section header. */
	if ((secthdr = (Elf32_Shdr *) malloc(filhdr.e_shentsize *
						filhdr.e_shnum)) == NULL) {
		(void) printf("Unable to allocate section header.\n");
#if defined(KADB)
		return;
#else	/* !KADB */
		outofmem();
#endif	/* KADB */
	}
	/*
	 * Seek to section header and read it.
	 */
	if ((loc = lseek(fsym, filhdr.e_shoff, L_SET) == -1) ||
		(read(fsym, (char *)secthdr, filhdr.e_shentsize *
		filhdr.e_shnum) != filhdr.e_shentsize * filhdr.e_shnum)) {
		printf("Unable to read section header.\n");
		return;
	}
#if !defined(KADB)
	/*
	 * If there wasn't a program header, we need to fix up the maps
	 * from information in the section header table.
	 */
	if (filhdr.e_phnum == 0) {

		char *sect_names = NULL;
		int sn;
		int ds = 0, d1s = 0, ts = 0;

		/*
		 * Look for the first string table.  It should be the
		 * section names.  We need the names to distinguish the
		 * interesting sections because the sh_type fields don't
		 * resolve them for this purpose.  Unfortunately, we only
		 * recognize the section names string table by finding
		 * the name ".shstrtab" in the section names string table.
		 */
		for (sn = 0; sn < (int) filhdr.e_shnum; sn++) {
			if (secthdr[sn].sh_type == SHT_STRTAB) {
				if ((sect_names = malloc(secthdr[sn].sh_size))
								== NULL) {
					(void) printf("Unable to allocate "
					    "section name table.\n");
					outofmem();
				}
				if ((loc = lseek(fsym, secthdr[sn].sh_offset,
					L_SET) == -1) ||
					(read(fsym, sect_names, secthdr[sn].
					sh_size) != secthdr[sn].sh_size)) {
					(void) printf("Unable to read section "
					    "names.\n");
					goto bad_shdr;
				}
				loc = (off_t) ((int) sect_names +
					(int) secthdr[sn].sh_name);
				if (strcmp((char *)loc, ".shstrtab") == 0)
					break;		/* found it */
			}
		}
		if (sn == filhdr.e_shnum)
			goto bad_shdr;		/* must not have found it */

		/* Look for the text and data. */

		if (symfile_type == REL) {
			int count = 0;
			/* finds the data section first */
			for (i = 0; i < (int) filhdr.e_shnum; i++) {
				/* since we have no prog headers
				 * then txtmap is currently NULL
				 */
				if (secthdr[i].sh_type == SHT_PROGBITS) {
					if(((secthdr[i].sh_flags & SHF_ALLOC) &&
					(secthdr[i].sh_flags&SHF_WRITE)) ||
					((secthdr[i].sh_flags&SHF_EXECINSTR) &&
					(secthdr[i].sh_flags&SHF_ALLOC)))  {
						count++;
						add_map_range(&txtmap,
							secthdr[i].sh_offset,
							secthdr[i].sh_size +
							secthdr[i].sh_offset,
							secthdr[i].sh_offset,
							symfil);
					}
				}
				if (count == 1)
					txtmap.map_head = txtmap.map_tail;
			}
		} else {
			for (i = 0; i < (int) filhdr.e_shnum; i++) {
				if (secthdr[i].sh_type == SHT_PROGBITS) {
					loc = (off_t) ((int) sect_names +
						(int) secthdr[i].sh_name);
					if (strcmp((char *)loc, ".text") == 0)
						ts = i;
					else if (strcmp((char *)loc,
					    ".data") == 0)
						ds = i;
					else if (strcmp((char *)loc,
					    ".data1") == 0)
						d1s = i;
				}
			}
			if (ts != 0) {
			fsym_rng1->mpr_b = 0;
				fsym_rng1->mpr_e = secthdr[ts].sh_size;
				fsym_rng1->mpr_f = secthdr[ts].sh_offset;
			}
			if (ds != 0) {
				if (secthdr[ds].sh_size == 0 && d1s != 0)
					ds = d1s;
				/*
				 * XXX - data follows text immediately, right?
				 */
				fsym_rng2->mpr_b = fsym_rng1->mpr_e;
				fsym_rng2->mpr_e = fsym_rng2->mpr_b +
					secthdr[ds].sh_size;
				fsym_rng2->mpr_f = secthdr[ds].sh_offset;
			}
		}
	bad_shdr:
		if (sect_names != NULL)
			free(sect_names);
	}
#endif	/* !KADB */
	stinit(fsym, secthdr, filhdr.e_shnum, symfile_type);
#if !defined(KADB)
	if (kernel == NOT_KERNEL)
		if (lookup("sighandler") != NULL) {
			trampsym = cursym;
			db_printf(2, "setsym: 'sighandler'->s_value=%X",
							cursym->s_value);
		} else
			errflg = NULL;	/* no problem if not found */
#endif	/* !KADB */
	db_printf(2, "setsym: sighandler at %X",
			(trampsym == NULL) ? NULL : trampsym->s_value);
}

/*
 * address of the text segment.  Normally this is given
 * by (x).a_entry, but an exception is made for demand
 * paged (ZMAGIC == 0413) files, in which the exec structure
 * is at the beginning of text address space, and the entry
 * point immediately follows.
 */
static off_t
textaddr(fhdr)
	Elf32_Ehdr *fhdr;
{
	return (proghdr ? proghdr[textseg].p_vaddr : 0);
}

#if !defined(KADB)

static int
chk_dumphdr(const struct dumphdr *const dumphdr, const char *dumpname)
{
	if (dumphdr->dump_magic != DUMP_MAGIC) {
		(void) printf("%s: bad dump magic number", dumpname);
		return (-1);
	}
	if (dumphdr->dump_version < DUMP_VERSION) {
		(void) printf("%s: bad dump version number", dumpname);
		return (-1);
	}
	if (!(dumphdr->dump_flags & DF_VALID)) {
		(void) printf("%s: invalid core dump", dumpname);
		return (-1);
	}
	if (!(dumphdr->dump_flags & DF_COMPLETE)) {
		(void) printf("%s: incomplete core dump", dumpname);
		return (-1);
	}
	return (0);
}

static int
readump(const int fcor, const off_t offset, char *buf,
	const int size, const char *const dumpname)
{
	if (lseek(fcor, offset, L_SET) == -1) {
		(void) printf("%s: seek error on the dump header\n", dumpname);
		return (-1);
	}
	if (read(fcor, buf, size) != size) {
		(void) printf("%s: read error on the dump header\n", dumpname);
		return (-1);
	}
	return (0);
}

static int
get_dump_info(const int fcor, const char *const dumpname)
{
	caddr_t kaddr;
	struct dumphdr dumphdr, *dhp;
	struct regs regs;
	char panicstr[256 + 1];
	static int readump(const int, const off_t,  char *,
	    const int, const char *const);
	static int chk_dumphdr(const struct dumphdr *const, const char *const);

	if (readump(fcor, (off_t) 0, (char *) &dumphdr, sizeof (struct dumphdr),
		    dumpname) == -1) {
		(void) printf("%s: unable to read first part of dump header",
		    dumpname);
		return (-1);
	}
	if (chk_dumphdr(&dumphdr, dumpname) == -1) {
		(void) printf("%s: bad dump header\n", dumpname);
		return (-1);
	}

	dhp = (struct dumphdr *) malloc(dumphdr.dump_headersize);
	if (dhp == NULL) {
		(void) printf("cannot allocate dump headr");
		return (-1);
	}

	if (readump(fcor, (off_t) 0, (char *) dhp, dumphdr.dump_headersize,
		    dumpname) == -1) {
		(void) printf("%s: unable to read dump header", dumpname);
		free(dhp);
		return (-1);
	}

	if (readump(fcor, (off_t) dhp->dump_panicstringoff,
		    (char *) &panicstr, 256 + 1, dumpname) == -1) {
		(void) printf("%s: unable to read panic string", dumpname);
		free(dhp);
		return (-1);
	}

	if (dhp->dump_crashtime.tv_sec == 0)
		(void) printf("Invalid system dump time.\n");
	else
		(void) printf("System dumped at: %s",
				ctime(&dhp->dump_crashtime.tv_sec));
#if	XXXPPC
	Curthread = (kthread_id_t) dhp->dump_thread;
	(void) printf("Panic String:\t%s\n"
	    "Running thread at: 0x%X on cpu at: 0x%X\n",
	    panicstr, Curthread, dhp->dump_cpu);
#endif
	free(dhp);

	(void) lookup("panic_reg");
	/*
	 * panic_reg is set to a pointer to all the registers at the
	 * time of crash in kernel, if not set to NULL.  Otherwise,
	 * the registers should be obtained from the t_pcb of the current
	 * thread.
	 */
	if (cursym == 0) {
		(void) printf("Unable to find 'panic_reg' symbol;"
		    " no registers available.\n");
		return (-1);
	}
	db_printf(2, "get_dump_info: panic_reg at 0x%X", cursym->s_value);
	if (kvm_read(kvmd, (long) cursym->s_value, (char *) &kaddr,
	    sizeof (kaddr)) != sizeof (kaddr)) {
		(void) printf("Unable to get the pointer to the registers at"
		    " the time of crash.\n");
		return (-1);
	}
	if (kaddr == NULL) {	/* invalid panic_reg, trap() is not called */
		kernel = CMN_ERR_PANIC;
		db_printf(2, "get_dump_info: kernel=%D", kernel);
		if (get_thread() == -1)
			return (-1);
	} else {
		kernel = TRAPPED_PANIC;
		db_printf(2, "get_dump_info: Pointer to the crash"
		    " registers at 0x%X, kernel=%D", kaddr, kernel);
		if (kvm_read(kvmd, (long) kaddr, (char *) &regs,
		    sizeof (regs)) != sizeof (regs)) {
			(void) printf("Unable to get the registers at"
			    " the time of crash.\n");
			return (-1);
		}
		(void) memcpy(&Prstatus.pr_lwp.pr_reg, &regs,
			sizeof (gregset_t));
		core_to_regs();
	}
	return (0);
}

/*
 * Called only when kernel == LIVE.
 */
static int
get_curthread(void)
{
	extern int errno;
	struct cpu cpu;

	db_printf(4, "get_curthread called");

	datmap.map_head->mpr_e = MAXFILE;
	Curthread = NULL;

	/*
	 * This function is called ONLY when debugging the live kernel
	 * using adb -k (and not kadb).
	 * And its major goal is to set the registers, in case right
	 * off from the start, $r is requested.  i.e. Some reasonable
	 * register values will do.  Obviously, the user can never
	 * expect them to be the CURRENT ones as adb -k is run from
	 * the user level and the kernel is not (cannot be) stopped.
	 * In other words, registers of any valid thread on any active
	 * CPU will do.  Therefore, even in an MP environment using
	 * the first cpu structure is valid enough.
	 */
	(void) lookup("cpus");
	/*
	 * In kernel cpus is an array of cpu's
	 */
	if (cursym == 0) {
		(void) printf("Unable to find 'cpus' symbol;"
			    " no registers available.\n");
		return (-1);
	}
	if (kvm_read(kvmd, (long) cursym->s_value, (char *) &cpu,
	    sizeof (cpu)) != sizeof (cpu)) {
		(void) printf("Unable to get the cpu structure;"
		    " no registers available.\n");
		return (-1);
	}
	Curthread = cpu.cpu_thread;
	(void) printf("Current registers belong to a snapshot of\n"
		    "\tkernel thread at: 0x%X on cpu[0]  at: 0x%X\n"
		    "Use thread_addr$t or proc_addr$p to change"
		    " the snapshot.\n",
		    Curthread, cursym->s_value);
	if (get_thread() == -1)
		return (-1);
	return (0);
}

setcor(void)
{
	struct stat stb;
	Elf32_Ehdr core_ehdr;
	Elf32_Phdr *core_phdr;
	char *noteseg, *offset;
	int nindx;
	int i, so_far = 0;
	struct map_range *fcor_rng1, *fcor_rng2;
	static int get_dump_info(const int, const char *const);
	static int get_curthread(void);

	db_printf(4, "setcor: called");

	/*
	 * Make sure all the open files are closed and the allocated
	 * space is freed before (re)assigning the maps.
	 */
	fcor_rng1 = datmap.map_head;
	while (fcor_rng1 != NULL) {
		free(fcor_rng1->mpr_fn);
		if (fcor_rng1->mpr_fd > 0)
			(void) close(fcor_rng1->mpr_fd);
		if (fcor_rng1 == datmap.map_tail)
			break;
		fcor_rng2 = fcor_rng1;
		fcor_rng1 = fcor_rng1->mpr_next;
		if (fcor_rng2 != &d_rng1 && fcor_rng2 != &d_rng2)
			free(fcor_rng2);
	}

	datmap.map_head = fcor_rng1 = &d_rng1;
	datmap.map_tail = fcor_rng2 = &d_rng2;
	fcor_rng1->mpr_next = fcor_rng2;

	datmap.map_head = fcor_rng1;
	datmap.map_tail = fcor_rng2;
	fcor_rng1->mpr_next = fcor_rng2;

	fcor_rng1->mpr_fn = fcor_rng2->mpr_fn  = corfil;
	fcor_rng1->mpr_fd  = fcor_rng2->mpr_fd = fcor = getfile(corfil, 2);
	fcor_rng1->mpr_e = MAXFILE;	/* liable to be overwritten */

	db_printf(2, "setcor: fcor=%D, kernel=%D", fcor, kernel);

	if (fcor == -1)
		return;
	if (kernel != NOT_KERNEL) {
		if (strcmp(corfil, LIVE_KERNEL_COREFILE) == 0) {
			/* Aleady assigned kernel to LIVE */
			if (get_curthread() == -1)
				(void) printf("Unable to provide "
				    "useful information.\n");
		} else if (get_dump_info(fcor, corfil) == -1)
			(void) printf("Cannot use %s\n", corfil);
		db_printf(2, "setcor: kernel=%D", kernel);
		return;
	}
	fstat(fcor, &stb);

	/* Exhaustive test for rectitude: */
	if (((stb.st_mode&S_IFMT) == S_IFREG) &&
	    (read(fcor, (char *) &core_ehdr, sizeof (core_ehdr)) ==
		sizeof (core_ehdr)) &&
	    (core_ehdr.e_ident[EI_MAG0] == ELFMAG0) &&
	    (core_ehdr.e_ident[EI_MAG1] == ELFMAG1) &&
	    (core_ehdr.e_ident[EI_MAG2] == ELFMAG2) &&
	    (core_ehdr.e_ident[EI_MAG3] == ELFMAG3) &&
	    (core_ehdr.e_ident[EI_CLASS] == ELFCLASS32) &&
	    (core_ehdr.e_ident[EI_DATA] == ELFDATA2LSB) &&
	    (core_ehdr.e_type == ET_CORE) &&
	    (core_ehdr.e_machine == EM_PPC) &&
	    (core_ehdr.e_version == EV_CURRENT)) {

		Elf32_Nhdr *nhdr;
		struct map_range *prev_rng = NULL;

		/* Position to the program header table. */
		if (lseek(fcor, (long) core_ehdr.e_phoff, L_SET) !=
						    core_ehdr.e_phoff) {
			(void) printf("Unable to find core file "
			    "program header.\n");
			goto bad1;
		}
		db_printf(2, "setcor: found core file program header");
		/* Get space for a copy of the program header table. */
		if ((core_phdr = (Elf32_Phdr *) malloc(core_ehdr.e_phentsize *
					    core_ehdr.e_phnum)) == NULL) {
			(void) printf("Unable to allocate core program header");
			fcor_rng1->mpr_e = MAXFILE;
			outofmem();
		}
		db_printf(2, "setcor: allocated core program header");
		/* Read the program header table. */
		if (read(fcor, core_phdr, core_ehdr.e_phentsize *
		    core_ehdr.e_phnum) != core_ehdr.e_phentsize *
		    core_ehdr.e_phnum) {
			(void) printf("Unable to read core program header.\n");
			goto bad1;
		}
		db_printf(2, "setcor: read core program header");
		/* The first segment should be a "note". */
		nindx = 0;
		if (core_phdr[0].p_type != PT_NOTE) {
			(void) printf("Core file state info unavailable.\n");
			goto bad1;
		}
		/* If the second segment is also a "note", then use it */
		if (core_phdr[1].p_type == PT_NOTE)
			nindx = 1;
		db_printf(2, "setcor: core file state info available");
		/* Position to the first segment. */
		if (lseek(fcor, (long) core_phdr[nindx].p_offset, L_SET) !=
		    core_phdr[nindx].p_offset) {
			(void) printf("Unable to find process state "
			    "info in core file.\n");
			goto bad1;
		}
		db_printf(2, "setcor: found process state info in core file");
		/* Get a place to keep the note segment. */
		if ((noteseg = malloc(core_phdr[nindx].p_filesz)) == NULL) {
			(void) printf("Unable to allocate core file state "
			    "info tables");
			fcor_rng1->mpr_e = MAXFILE;
			outofmem();
		}
		db_printf(2, "setcor: allocated core file state info tables");
		/*
		 * Read the note segment.
		 */
		if (read(fcor, noteseg, core_phdr[nindx].p_filesz) !=
		    core_phdr[nindx].p_filesz) {
			(void) printf("Unable to read core file state info.\n");
			goto bad1;
		}
		db_printf(2, "setcor: read core file state info");
		/* there should be a header at the beginning of the segment. */
		nhdr = (Elf32_Nhdr *) noteseg;

		/*
		 * The loop runs until the nhdr pointer reaches the end of
		 * the note segment that we just read.  This will happen as
		 * long as the note segment is well-formed.  Is that too
		 * much to assume?
		 */
		while ((int) nhdr < (int) noteseg +
			(int) core_phdr[nindx].p_filesz) {

			if ((int)nhdr->n_descsz > 0) {
				offset = (char *)(sizeof (Elf32_Nhdr) +
				    (int)nhdr + (((int)nhdr->n_namesz +
				    sizeof (int) - 1) & ~(sizeof (int) - 1)));
				switch (nhdr->n_type) {
				case NT_PSTATUS:
					if (!(so_far & (1<<NT_PSTATUS))) {
						memcpy(&Prstatus, offset,
							sizeof (Prstatus));
						so_far |= (1 << NT_PSTATUS);
						memcpy(&Prfpregs,
						    &Prstatus.pr_lwp.pr_fpreg,
						    sizeof (Prfpregs));
					} else {
						fprintf(stderr,
						"Ignoring another NT_PSTATUS"
						" note segment entry.\n");
					}
					break;
				case NT_PSINFO:
					if (!(so_far & (1<<NT_PSINFO))) {
						memcpy(&core_psinfo, offset,
							sizeof (core_psinfo));
						so_far |= (1 << NT_PSINFO);
					} else {
						fprintf(stderr,
						"Ignoring another NT_PSINFO"
						" note segment entry.\n");
					}
					break;
				case NT_PRXREG:
					if (!(so_far & (1<<NT_PRXREG))) {
						fprintf(stderr,
						"NT_PRXREG currently "
						"unsupported note segment "
						"entry.\n");
						so_far |= (1 << NT_PRXREG);
					}
					break;
				case NT_PLATFORM:
					if (!(so_far & (1<<NT_PLATFORM))) {
						memcpy(core_platform, offset,
							sizeof (core_platform));
						so_far |= (1 << NT_PLATFORM);
					} else {
						fprintf(stderr,
						"Ignoring another NT_PLATFORM"
						" note segment entry.\n");
					}
					break;
				case NT_AUXV:
					if (!(so_far & (1<<NT_AUXV))) {
						memcpy(core_auxv, offset,
							sizeof (core_auxv));
						so_far |= (1 << NT_AUXV);
					} else {
						fprintf(stderr,
						"Ignoring another NT_AUXV"
						" note segment entry.\n");
					}
					break;
				/* recognize but ignore these (for now) */
				case NT_LWPSINFO:
				case NT_LWPSTATUS:
					break;
				default:
					if (!(so_far & (1<<nhdr->n_type))) {
						fprintf(stderr,
						"Ignoring unrecognized"
						" note segment entry %d.\n",
						nhdr->n_type);
						so_far |= (1 << nhdr->n_type);
					}
					break;
				}
			}
			nhdr = (Elf32_Nhdr *)((int) offset +
				(((int)nhdr->n_descsz + sizeof (int) - 1)
				& ~(sizeof (int) -1)));
		}
gotnotes:
		free(noteseg);

		if (so_far & (1<<NT_PLATFORM)) {
			(void) printf("core file '%s' from program '%s'"
				" on platform '%s'\n", corfil,
				core_psinfo.pr_fname, core_platform);
		} else {
			(void) printf("core file '%s' from program '%s'\n",
				corfil, core_psinfo.pr_fname);
		}

		/* Allow for an arbitrary number of segments. */
		fcor_rng2 = fcor_rng1;
		for (i = nindx+1; i < (u_int) core_ehdr.e_phnum; i++) {

			/* Does this entry describes something in the file? */
			if (core_phdr[i].p_filesz) {
				/*
				 * Save start and end addresses in memory
				 * and file offset.
				 */
				fcor_rng2->mpr_b = core_phdr[i].p_vaddr;
				fcor_rng2->mpr_e = fcor_rng2->mpr_b +
					core_phdr[i].p_filesz;
				fcor_rng2->mpr_f = core_phdr[i].p_offset;

				/* Link previous map to the current one. */
				if (prev_rng != NULL)
					prev_rng->mpr_next = fcor_rng2;
				prev_rng = fcor_rng2;
				fcor_rng2 = (struct map_range *)
					calloc(1, sizeof (struct map_range));
				if (fcor_rng2 == NULL)
					outofmem();
				fcor_rng2->mpr_fn = corfil;
				fcor_rng2->mpr_fd = fcor;
				fcor_rng2->mpr_next = NULL;
			}
		}
		free(core_phdr);
		datmap.map_tail = prev_rng;
		core_to_regs();
		db_printf(2, "setcor: read the registers from %s", corfil);
		signo = (int) Prstatus.pr_lwp.pr_cursig;
		if (signo)
			sigprint(signo);
		(void) printf("\n");
	} else

		{
	bad1:
		/*
		 * Either not a regular file, or
		 * core struct not convincing
		 */
		(void) printf("'%s' is not a core file\n", corfil);
		fcor_rng1->mpr_e = MAXFILE;
	}
	if (use_shlib)
#if __ELF
		scan_linkmap();
#else /* !__ELF */
		extend_text_map();
#endif /* !__ELF */
}


#if !defined(__ELF)
/*
 * For kadb's sake, we pretend we can always find
 * the registers on the stack.
 */
regs_not_on_stack() {
	return (0);
}
#endif	/* !__ELF */

#endif	/* !KADB */

#if !defined(__ELF)
/*
 * Get the whole u structure.  We need the pcb for stack tracing.
 * Called by setcor only if we got what looks like a convincing core file.
 */

#if !defined(KADB)
static
#endif	/* !KADB */
read_ustruct(core_fd)
	int core_fd;
{
	off_t pos_was, pos_ustruct, err, lseek();

#if !defined(KADB)
	db_printf(4, "read_ustruct: core_fd=%D", core_fd);
	pos_ustruct = core.c_len + core.c_dsize + core.c_ssize;
	pos_was = lseek(core_fd, 0L, SEEK_CUR);	/* = tell(core_fd); */
	err = lseek(core_fd, pos_ustruct, L_SET);	/* <sys/file.h> */

	if ((err != -1) && (pos_was != -1) &&	/* ok pos'ns */
	    (read(core_fd, (char *)&u, sizeof (u)) == sizeof (u))) {
		db_printf(2, "read_ustruct: read u struct from core file");
		/* go back whence you came */
		(void) lseek(core_fd, pos_was, L_SET);
	} else {
		printf("core file (\"%s\") appears to be truncated.\n", corfil);
		datmap.map_head->mpr_e = MAXFILE;
	}
#endif	/* !KADB */
}
#endif	/* __ELF */

/*
 * Only called if kernel debugging (-k or KADB).  Read in (or map)
 * the u-area for the appropriate process and get the registers.
 */

int
getproc(void)
{
	struct proc proc;
#if !defined(KADB)

	db_printf(4, "getproc: called");

	if (kvm_read(kvmd, (long) Curproc, (char *) &proc, sizeof (proc)) !=
	    sizeof (proc)) {
		(void) printf("Cannot find proc 0x%X\n", Curproc);
		return (-1);
	}
	if (proc.p_stat == SZOMB) {
		(void) printf("0x%X is a zombie process - no useful registers",
		    Curproc);
		return (-1);
	}
	Curthread = proc.p_tlist;
	db_printf(2, "getproc: Curthread=0x%X", Curthread);
	if (get_thread() == -1)
		return (-1);
	return (0);
#endif	/* KADB */
}

/*
 * The address of a thread struct is put in Curthread before get_thread()
 * is called.
 */

int
get_thread(void)
{
#if !defined(KADB)
	label_t *t_pcb, pcb;
	kthread_t *thread = (kthread_t *) NULL;
	static int getpcb(label_t *);

	if (Curthread == NULL)
		return (-1);

	t_pcb = (label_t *)
	    ((unsigned long) Curthread + (unsigned long) &thread->t_pcb);
	if (kvm_read(kvmd, (long) t_pcb, (char *) &pcb, sizeof (pcb)) !=
		sizeof (pcb)) {
		(void) printf("Cannot read current thread's t_pcb at 0x%X\n",
			t_pcb);
		return (-1);
	}
	if (getpcb(&pcb) == -1)
		return (-1);
	return (0);
#endif	/* !KADB */
}

#if !defined(KADB)
/*
 * getpcb() hand crafts as many registers as possible
 * from t_pcb of the Curthread.
 */
static int
getpcb(label_t *regaddr)
{
	register int *rp = &(regaddr->val[0]);
	int spval, bpval;

	db_printf(4, "getpcb: regaddr=%X, kernel=%D", regaddr, kernel);

	switch (kernel) {
	case NOT_KERNEL:
		(void) printf("getpcb: internal error, kernel=%D\n", kernel);
		exit(-1);
	case LIVE:
		/*
		 * On a live kernel Curthread is read from the cpu structure.
		 * t_pcb of the Curthread is an array that only the first two
		 * elements of it are useful for us here:  the first one is eip
		 * and the second esp.  resume() code in the kernel pushes ebp,
		 * esi, edi and ebx (in this particular order).
		 */
	/* XXXPPC */
		break;
	case TRAPPED_PANIC:
		/*
		 * Should never be here.  get_dump_info() has dealt with
		 * this case.  panic_reg kernel global variable is ptr
		 * to all the registers of Curthread.
		 */
		(void) printf("getpcb: internal error, kernel=%D\n", kernel);
		exit(-1);
	case CMN_ERR_PANIC:
		/*
		 * A kernel assertion is failed and the dump is produced by
		 * a call to cmn_err() in the kernel which eventually
		 * calls dumpsys() - bypassing trap().  The important
		 * registers are moved to the setjmp() buffer and saved
		 * in t_pcb of Curthread.
		 */
#if	i386
	/* XXXPPC */
		setreg(EDI, *rp++);
		setreg(ESI, *rp++);
		setreg(EBX, *rp++);
		setreg(EBP, bpval = *rp++);
		setreg(ESP, spval = *rp++);
		setreg(EIP, *rp);
		if (spval == 0 || bpval == 0)
			return (-1);
#endif
		break;
	default:
		(void) printf("getpcb: internal error, kernel=%D\n", kernel);
		exit(-1);
	}


	return (0);
}


create(f)
	char *f;
{
	int fd;

	db_printf(7, "create: f=%s", (f == NULL) ? "NULL" : f);
	if ((fd = creat64(f, 0644)) >= 0) {
		(void) close(fd);
		return (open64(f, wtflag));
	} else
		return (-1);
}

getfile(filnam, cnt)
	char *filnam;
	int cnt;
{
	register int fsym;

	db_printf(5, "getfile: filnam='%s', cnt=%D",
			(filnam == NULL) ? "NULL" : filnam, cnt);
	if (strcmp(filnam, "-") == 0)
	    return (-1);
	fsym = open64(filnam, wtflag);
	if (fsym < 0 && xargc > cnt) {
	    db_printf(9, "getfile: wtflag=%D", wtflag);
	    if (wtflag) {
		fsym = create(filnam);
		if (fsym < 0) {
		    /* try reading only */
		    fsym = open64(filnam, 0);
		    if (fsym >= 0)
			(void) printf("warning: `%s' read-only\n", filnam);
		}
	    }
	    if (fsym < 0)
		(void) printf("cannot open `%s'\n", filnam);
	}
	db_printf(5, "getfile: returns %D", fsym);
	return (fsym);
}

#endif	/* !KADB */

setvar(void)
{
	var[varchk('b')] = (dataseg == -1) ? 0 : proghdr[dataseg].p_vaddr;
	var[varchk('d')] = (dataseg == -1) ? 0 : proghdr[dataseg].p_memsz;
	var[varchk('e')] = filhdr.e_entry;
	var[varchk('m')] = *((int *) filhdr.e_ident);
	var[varchk('t')] = proghdr ? proghdr[textseg].p_memsz : 0;
	var[varchk('s')] = stksiz;
#if !KADB
	var[varchk('F')] = 1; /* XXXPPC _fp_hw; */
#else	/* KADB */
	var[varchk('F')] = 1;
#endif	/* !KADB */
}
