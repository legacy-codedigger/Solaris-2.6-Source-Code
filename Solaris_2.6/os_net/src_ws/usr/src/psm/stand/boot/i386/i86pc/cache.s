#ifdef	lint

#include <sys/types.h>

void rcache(void) { return; }
void savcr0(void) { return; }
void flcache(void) { return; }
int dcache(void) { return(1); }

/*ARGSUSED*/
void flush_l2(ulong *physaddr) { return; }
#else

/	Copyrighted as an unpublished work.
/       (c) Copyright 1992 Sun Microsystems, Inc.
/	All rights reserved.

/	RESTRICTED RIGHTS

/	These programs are supplied under a license.  They may be used,
/	disclosed, and/or copied only as permitted under such license
/	agreement.  Any copy must contain the above copyright notice and
/	this restricted rights notice.  Use, copying, and/or disclosure
/	of the programs is strictly prohibited unless otherwise provided
/	in the license agreement.


	.file   "cache.s"

	.ident "@(#)cache.s	1.6	96/04/30 SMI"

	.text

/       Functions to disable, enable, and flush the i486 on-chip cache.
/       These functions are specific to the 80486 cpu, and should NOT
/       be called when running on a 80386.

	.set    DISAB_MASK, 0x60000000  / cr0 mask (OR) to disable cache
	.set    ENAB_MASK,  0x9FFFFFFF  /          (AND)   enable cache

/       Disable the i486 on-chip cache and flush it (flush also any
/       external cache).
/       Returns the value of cr0 after cache is disabled.

	.globl  dcache
dcache:
	call    flcache                 / flush on-chip & external caches
					/   do this BEFORE setting bits in CR0
					/   to avoid a system panic
	movl    %cr0, %eax
/	movl    %eax, save_cr0
	orl     $DISAB_MASK, %eax

	movl    %eax, %cr0              / Disable cache
	call    flcache                 / flush on-chip & external caches
					/   make sure last lines of cache clear
	ret

/       Flush the i486 on chip cache, and any external cache.
/       A special-function bus cycle is generated by the WBINVD instruction
/       which indicates that external cache should write-back its contents to
/       main memory.
/
/       Since the WBINVD instruction is not currenty known to the assembler,
/       its opcode is placed inline as a 16-bit data value.

	.globl  flcache
flcache:
	.value  0x090F                  / WBINVD opcode
					/ Write-back and invalidate cache
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	ret

/       Enable the i486 on-chip cache.
/       Returns the value of cr0 after cache enabled.  Previous value
/       is saved in 'save_cr0' global.

	.globl  ecache
ecache:
	movl    %cr0, %eax
	movl    %eax, save_cr0
	andl    $ENAB_MASK, %eax
	movl    %eax, %cr0
	call    flcache                 / flush on-chip & external caches
	ret

/	Save away the value of cr0, to later restore cache status
	.globl	savcr0
savcr0:
	movl    %cr0, %eax
	movl    %eax, save_cr0
	ret

/       Restore cr0 register from the 'save_cr0' global.
/       Returns the restored value.

	.globl  rcache
rcache:
	movl    save_cr0, %eax
	movl    %eax, %cr0
	ret

/	Force a dirty block flush from cache line 0.
/	This is designed for large, direct mapped 2nd level
/	caches. Take the pointer % 64K and read that offset
/	every 64k between 1Mb and 2Mb. Handles a linear
/	cache up to 1Mb or a 2-way cache up to 512K, 4-way 256K, etc.

#define M64K	0x10000
#define M1MEG	0x100000
#define M2MEG	0x200000

	.globl	flush_l2

flush_l2:

	pushl	%esi
	pushl	%ebx
	pushl	%ecx

	movl	16(%esp), %ecx		/ physaddr of interest
	andl	$M64K-1, %ecx		/ physaddr % 64k

	movl	$M1MEG, %esi		/ start at 1Mb
	addl	%ecx, %esi		/ ..plus line offset
	movl	$M64K, %ebx		/ increment by 64k
1:
	movl	(%esi), %eax		/ read the alias
	addl	%ebx, %esi		/ increment pointer
	cmpl	$M2MEG, %esi		/ bail at 2M
	jb	1b
	movl	%esi, %eax
	popl	%ecx
	popl	%ebx
	popl	%esi
	ret

	.data

/       Variable to hold the saved value of the cr0 register.

	.globl  save_cr0
save_cr0:
	.long   0

#endif	/* !lint */
