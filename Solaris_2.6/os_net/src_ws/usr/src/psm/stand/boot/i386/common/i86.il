/*
 * Copyright (c) 1992 by Sun Microsystems, Inc.  All Rights Reserved.
 */

#ident "@(#)i86.il	1.3	95/06/08 SMI"
/
/ In-line functions for i86 calls.
/


/
/ in and out
/
	.inline	inb,4
	movl	(%esp), %edx
	xorl    %eax, %eax
	inb	(%dx)
	.end

	.inline	inw,4
	movl	(%esp), %edx
	xorl    %eax, %eax
	inw	(%dx)
	.end

	.inline	inl,4
	movl	(%esp), %edx
	xorl    %eax, %eax
	inl	(%dx)
	.end

	.inline	outb,8
	movl	(%esp), %edx
	movl    4(%esp), %eax
	outb	(%dx)
	.end

	.inline	outw,8
	movl	(%esp), %edx
	movl    4(%esp), %eax
	outw	(%dx)
	.end

	.inline	outl,8
	movl	(%esp), %edx
	movl    4(%esp), %eax
	outl	(%dx)
	.end

/
/ Networking byte order functions (too bad, Intel has the wrong byte order)
/

	.inline	htonl,4
	movl	(%esp), %eax
	xchgb	%ah, %al
	rorl	$16, %eax
	xchgb	%ah, %al
	.end

	.inline	ntohl,4
	movl	(%esp), %eax
	xchgb	%ah, %al
	rorl	$16, %eax
	xchgb	%ah, %al
	.end

	.inline	htons,4
	movl	(%esp), %eax
	xchgb	%ah, %al
	.end

	.inline	ntohs,4
	movl	(%esp), %eax
	xchgb	%ah, %al
	.end

/
/ disable interrupts and return value describing if interrupts were enabled
/
	.inline	clear_int_flag,0
	pushfl
	cli
	popl	%eax
	.end

/
/ restore interrupt enable flag to value returned from 'clear_int_flag' above
/
	.inline restore_int_flag,4
	pushl	(%esp)
	popfl
	.end

/
/ stub function. 
/ i386 arch currently has a unified cache
/
	.inline sync_instruction_memory,8
	nop
	.end
