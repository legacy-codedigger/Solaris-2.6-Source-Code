/*
 *	Copyright (c) 1988-1991 by Sun Microsystems, Inc.
 */

#ident	"@(#)samuldiv64.s	1.4	96/06/18 SMI"	/* From SunOS 4.1 1.6 */
#include <sys/asm_linkage.h>

#if	defined(lint)

/*
 * Stolen from usr/src/uts/sparc/crt.s
 */

#else

!
!
! 64 bit arithmetic support routines stolen by disassembly from libm.
! There happens to be some C versions kicking around, but they
! looked like a pain to figure out how to compile.
!
! Like other routines, these are called via direct calls as
! generated by the compiler.
!
! I won't vouch for the divide/remainder routines yet, so they will
! remain commented out until needed.
! 
	RTENTRY(__div64)
	save	%sp, -96, %sp
	sll	%i3, 1, %o0
	or	%i3, %o0, %o0
	srl	%o0, 1, %o0
	orcc	%i2, %o0, %g0
	bne	1f	! L23488
	mov	0, %l3
	mov	0, %i3
	ba	.div64out	! L237b0
	mov	0, %i2
1:
	sll	%i1, 1, %o0
	or	%i1, %o0, %o0
	srl	%o0, 1, %o0
	orcc	%i0, %o0, %g0
	bge,a	1f	!L234b4
	sll	%i3, 1, %o0
	subcc	%g0, %i1, %i1
	subx	%g0, %i0, %o0
	mov	%o0, %i0
	mov	1, %l3
	sll	%i3, 1, %o0
1:
	or	%i3, %o0, %o0
	srl	%o0, 1, %o0
	orcc	%i2, %o0, %g0
	bge,a	1f	!L234dc
	mov	%i3, %l5
	subcc	%g0, %i3, %i3
	subx	%g0, %i2, %o0
	mov	%o0, %i2
	xor	%l3, 1, %l3
	mov	%i3, %l5
1:
	mov	%i1, %l1
	cmp	%l1, %l5
	mov	%i2, %l4
	mov	%i0, %l0
	subxcc 	%l0, %l4, %g0
	bgeu,a 	1f	!0x23504
	orcc	%i0, %i2, %g0
	mov	0, %i3
	ba	.div64out
	mov	0, %i2
1:
	bne,a	1f	!0x23524
	mov	0, %o1
	mov	%i3, %o1
	call	.udiv
	mov	%i1, %o0
	mov	%o0, %i1
	ba	.L23790
	mov	%g0, %i0
1:
	cmp	%l1, %o1
	sethi	%hi(0x20000000), %o0
	subxcc 	%l0, %o0, %g0
	mov	0, %l7
	mov	%l4, %i2
	mov	%l5, %i3
	mov	%i0, %i4
	blu	.L23650
	mov	%i1, %i5
	mov	0, %i5
	sethi	%hi(0x20000000), %o0
	mov	%o0, %i4
	cmp	%i3, %i5
	subxcc 	%i2, %i4, %g0
	bgeu,a 	.L23588
	mov	%i1, %i5
.L23564:
	srl	%i3, 30, %o0
	sll	%i3, 2, %i3
	sll	%i2, 2, %i2
	or	%i2, %o0, %i2
	cmp	%i3, %i5
	subxcc 	%i2, %i4, %g0
	blu	.L23564
	add	%l7, 1, %l7
	mov	%i1, %i5
.L23588:
	cmp	%i3, %i5
	mov	%i0, %i4
	subxcc 	%i2, %i4, %g0
	bgeu	.L235d4
	mov	0, %l6
.L2359c:
	sll	%i3, 1, %o2
	or	%i3, %o2, %o2
	srl	%o2, 1, %o2
	orcc	%i2, %o2, %g0
	bl,a	.L235d8
	subcc	%i1, %i3, %i5
	srl	%i3, 31, %o0
	sll	%i3, 1, %i3
	sll	%i2, 1, %i2
	or	%i2, %o0, %i2
	cmp	%i3, %i5
	subxcc 	%i2, %i4, %g0
	blu	.L2359c
	add	%l6, 1, %l6
.L235d4:
	subcc	%i1, %i3, %i5
.L235d8:
	subx	%i0, %i2, %o2
	mov	%o2, %i4
	mov	0, %i0
	mov	1, %i1
.L235e8:
	subcc	%l6, 1, %l6
	bneg,a 	.L23690
	subcc	%l7, 1, %l7
	sll	%i0, 1, %i0
	srl	%i1, 31, %o0
	or	%i0, %o0, %i0
	sll	%i2, 31, %o0
	srl	%i3, 1, %i3
	or	%o0, %i3, %i3
	sll	%i5, 1, %o0
	or	%i5, %o0, %o0
	srl	%o0, 1, %o0
	orcc	%i4, %o0, %g0
	srl	%i2, 1, %i2
	bl	.L2363c
	sll	%i1, 1, %i1
	subcc	%i5, %i3, %i5
	subx	%i4, %i2, %i4
	addcc	%i1, 1, %i1
	ba	.L235e8
	addx	%i0, 0, %i0
.L2363c:
	addcc	%i5, %i3, %i5
	addx	%i4, %i2, %i4
	subcc	%i1, 1, %i1
	ba	.L235e8
	subx	%i0, 0, %i0
.L23650:
	mov	%i5, %l5
	mov	%i4, %l4
	mov	0, %i1
	mov	0, %i0
	srl	%i3, 30, %o0
1:
	sll	%i3, 2, %i3
	sll	%i2, 2, %i2
	or	%i2, %o0, %i2
	subcc	%i3, %l5, %o1
	subxcc 	%i2, %l4, %o0
	blu	2f	! 23684
	add	%l7, 1, %l7
	orcc	%o0, %o1, %g0
2:
	bleu,a 	1b	! 23664
	srl	%i3, 30, %o0
.L2368c:
	subcc	%l7, 1, %l7
.L23690:
	bneg,a 	.L23774
	sll	%i5, 1, %o0
	sll	%i0, 2, %i0
	srl	%i1, 30, %o0
	or	%i0, %o0, %i0
	sll	%i2, 31, %o0
	srl	%i3, 1, %i3
	or	%o0, %i3, %i3
	sll	%i5, 1, %o0
	or	%i5, %o0, %o0
	srl	%o0, 1, %o0
	orcc	%i4, %o0, %g0
	srl	%i2, 1, %i2
	bl	.L23720
	sll	%i1, 2, %i1
	subcc	%i5, %i3, %i5
	srl	%i3, 1, %i3
	sll	%i2, 31, %o0
	or	%o0, %i3, %i3
	sll	%i5, 1, %o0
	or	%i5, %o0, %o0
	srl	%o0, 1, %o0
	subx	%i4, %i2, %i4
	orcc	%i4, %o0, %g0
	bl	.L2370c
	srl	%i2, 1, %i2
	subcc	%i5, %i3, %i5
	subx	%i4, %i2, %i4
	addcc	%i1, 3, %i1
	ba	.L2368c
	addx	%i0, 0, %i0
.L2370c:
	addcc	%i5, %i3, %i5
	addx	%i4, %i2, %i4
	addcc	%i1, 1, %i1
	ba	.L2368c
	addx	%i0, 0, %i0
.L23720:
	addcc	%i5, %i3, %i5
	srl	%i3, 1, %i3
	sll	%i2, 31, %o0
	or	%o0, %i3, %i3
	sll	%i5, 1, %o0
	or	%i5, %o0, %o0
	srl	%o0, 1, %o0
	addx	%i4, %i2, %i4
	orcc	%i4, %o0, %g0
	bl	.L23760
	srl	%i2, 1, %i2
	subcc	%i5, %i3, %i5
	subx	%i4, %i2, %i4
	subcc	%i1, 1, %i1
	ba	.L2368c
	subx	%i0, 0, %i0
.L23760:
	addcc	%i5, %i3, %i5
	addx	%i4, %i2, %i4
	subcc	%i1, 3, %i1
	ba	.L2368c
	subx	%i0, 0, %i0
.L23774:
	or	%i5, %o0, %o0
	srl	%o0, 1, %o0
	orcc	%i4, %o0, %g0
	bge,a	.L23794
	orcc	%g0, %l3, %g0
	subcc	%i1, 1, %i1
	subx	%i0, 0, %i0
.L23790:
	orcc	%g0, %l3, %g0
.L23794:
	be,a	.L237ac
	mov	%i0, %i2
	subcc	%g0, %i1, %i3
	subx	%g0, %i0, %o0
	ba	.div64out
	mov	%o0, %i2
.L237ac:
	mov	%i1, %i3
.div64out:
	mov	%i2, %i0
	ret
	restore	%g0, %i3, %o1
	SET_SIZE(__div64)

	RTENTRY(__rem64)
	save	%sp, -96, %sp
	sll	%i1, 1, %o0
	or	%i1, %o0, %o0
	srl	%o0, 1, %o0
	orcc	%i0, %o0, %g0
	bge	1f	! 0x237e8
	mov	0, %l1
	subcc	%g0, %i1, %i1
	subx	%g0, %i0, %o0
	mov	%o0, %i0
	mov	1, %l1
1:
	mov	%i3, %l3
	sll	%l3, 1, %o0
	or	%l3, %o0, %o0
	srl	%o0, 1, %o0
	mov	%i2, %l2
	orcc	%l2, %o0, %g0
	bge,a	1f	! 0x23818
	mov	%l3, %i5
	subcc	%g0, %l3, %l3
	subx	%g0, %l2, %o0
	mov	%o0, %l2
	mov	%l3, %i5
1:
	cmp	%i5, 2
	mov	%l2, %i4
	subxcc 	%i4, 0, %g0
	bgeu,a 	.L23838
	mov	%i1, %l5
	mov	0, %i5
	ba	.L23afc
	mov	0, %i4
.L23838:
	cmp	%l5, %i5
	mov	%i0, %l4
	subxcc 	%l4, %i4, %g0
	bgeu,a 	.L23858
	orcc	%i0, %l2, %g0
	mov	%i0, %i2
	ba	.L23adc
	mov	%i1, %i3
.L23858:
	bne,a	.L23878
	mov	0, %o1
	mov	%l3, %o1
	call	.urem
	mov	%i1, %o0
	mov	%o0, %i3
	ba	.L23adc
	mov	%g0, %i2
.L23878:
	cmp	%l5, %o1
	sethi	%hi(0x20000000), %o0
	subxcc 	%l4, %o0, %g0
	mov	0, %l7
	mov	%i0, %i2
	blu	.L2399c
	mov	%i1, %i3
	mov	0, %i3
	sethi	%hi(0x20000000), %o0
	mov	%o0, %i2
	cmp	%i5, %i3
	subxcc 	%i4, %i2, %g0
	bgeu,a 	.L238d4
	mov	%i1, %i3
1:
	srl	%i5, 30, %o0
	sll	%i5, 2, %i5
	sll	%i4, 2, %i4
	or	%i4, %o0, %i4
	cmp	%i5, %i3
	subxcc 	%i4, %i2, %g0
	blu	1b	! 0x238b0
	add	%l7, 1, %l7
	mov	%i1, %i3
.L238d4:
	cmp	%i5, %i3
	mov	%i0, %i2
	subxcc 	%i4, %i2, %g0
	bgeu	.L23920
	mov	0, %l6
1:
	sll	%i5, 1, %o2
	or	%i5, %o2, %o2
	srl	%o2, 1, %o2
	orcc	%i4, %o2, %g0
	bl,a	.L23924
	subcc	%i1, %i5, %i3
	srl	%i5, 31, %o0
	sll	%i5, 1, %i5
	sll	%i4, 1, %i4
	or	%i4, %o0, %i4
	cmp	%i5, %i3
	subxcc 	%i4, %i2, %g0
	blu	1b ! 0x238e8
	add	%l6, 1, %l6
.L23920:
	subcc	%i1, %i5, %i3
.L23924:
	subx	%i0, %i4, %o2
	mov	%o2, %i2
	mov	0, %i0
	mov	1, %i1
.L23934:
	subcc	%l6, 1, %l6
	bneg,a 	.L239dc
	subcc	%l7, 1, %l7
	sll	%i0, 1, %i0
	srl	%i1, 31, %o0
	or	%i0, %o0, %i0
	sll	%i4, 31, %o0
	srl	%i5, 1, %i5
	or	%o0, %i5, %i5
	sll	%i3, 1, %o0
	or	%i3, %o0, %o0
	srl	%o0, 1, %o0
	orcc	%i2, %o0, %g0
	srl	%i4, 1, %i4
	bl	.L23988
	sll	%i1, 1, %i1
	subcc	%i3, %i5, %i3
	subx	%i2, %i4, %i2
	addcc	%i1, 1, %i1
	ba	.L23934
	addx	%i0, 0, %i0
.L23988:
	addcc	%i3, %i5, %i3
	addx	%i2, %i4, %i2
	subcc	%i1, 1, %i1
	ba	.L23934
	subx	%i0, 0, %i0
.L2399c:
	mov	%i3, %l5
	mov	%i2, %l4
	mov	0, %i1
	mov	0, %i0
	srl	%i5, 30, %o0
.L239b0:
	sll	%i5, 2, %i5
	sll	%i4, 2, %i4
	or	%i4, %o0, %i4
	subcc	%i5, %l5, %o1
	subxcc 	%i4, %l4, %o0
	blu	1f !0x239d0
	add	%l7, 1, %l7
	orcc	%o0, %o1, %g0
1:
	bleu,a 	.L239b0
	srl	%i5, 30, %o0
.L239d8:
	subcc	%l7, 1, %l7
.L239dc:
	bneg,a 	.L23ac0
	sll	%i3, 1, %o0
	sll	%i0, 2, %i0
	srl	%i1, 30, %o0
	or	%i0, %o0, %i0
	sll	%i4, 31, %o0
	srl	%i5, 1, %i5
	or	%o0, %i5, %i5
	sll	%i3, 1, %o0
	or	%i3, %o0, %o0
	srl	%o0, 1, %o0
	orcc	%i2, %o0, %g0
	srl	%i4, 1, %i4
	bl	.L23a6c
	sll	%i1, 2, %i1
	subcc	%i3, %i5, %i3
	srl	%i5, 1, %i5
	sll	%i4, 31, %o0
	or	%o0, %i5, %i5
	sll	%i3, 1, %o0
	or	%i3, %o0, %o0
	srl	%o0, 1, %o0
	subx	%i2, %i4, %i2
	orcc	%i2, %o0, %g0
	bl	.L23a58
	srl	%i4, 1, %i4
	subcc	%i3, %i5, %i3
	subx	%i2, %i4, %i2
	addcc	%i1, 3, %i1
	ba	.L239d8
	addx	%i0, 0, %i0
.L23a58:
	addcc	%i3, %i5, %i3
	addx	%i2, %i4, %i2
	addcc	%i1, 1, %i1
	ba	.L239d8
	addx	%i0, 0, %i0
.L23a6c:
	addcc	%i3, %i5, %i3
	srl	%i5, 1, %i5
	sll	%i4, 31, %o0
	or	%o0, %i5, %i5
	sll	%i3, 1, %o0
	or	%i3, %o0, %o0
	srl	%o0, 1, %o0
	addx	%i2, %i4, %i2
	orcc	%i2, %o0, %g0
	bl	.L23aac
	srl	%i4, 1, %i4
	subcc	%i3, %i5, %i3
	subx	%i2, %i4, %i2
	subcc	%i1, 1, %i1
	ba	.L239d8
	subx	%i0, 0, %i0
.L23aac:
	addcc	%i3, %i5, %i3
	addx	%i2, %i4, %i2
	subcc	%i1, 3, %i1
	ba	.L239d8
	subx	%i0, 0, %i0
.L23ac0:
	or	%i3, %o0, %o0
	srl	%o0, 1, %o0
	orcc	%i2, %o0, %g0
	bge,a	.L23ae0
	orcc	%g0, %l1, %g0
	addcc	%i3, %l3, %i3
	addx	%i2, %l2, %i2
.L23adc:
	orcc	%g0, %l1, %g0
.L23ae0:
	be,a	.L23af8
	mov	%i2, %i4
	subcc	%g0, %i3, %i5
	subx	%g0, %i2, %o0
	ba	.L23afc
	mov	%o0, %i4
.L23af8:
	mov	%i3, %i5
.L23afc:
	mov	%i4, %i0
	ret
	restore	%g0, %i5, %o1
	SET_SIZE(__rem64)

	RTENTRY(__udiv64)
	save	%sp, -96, %sp
	sll	%i3, 1, %o0
	or	%i3, %o0, %o0
	srl	%o0, 1, %o0
	orcc	%i2, %o0, %g0
	bne,a	1f	!0x23b30
	cmp	%i1, %i3
	mov	0, %i3
	ba	.L23dc8
	mov	0, %i2
1:
	subxcc 	%i0, %i2, %g0
	bgeu,a 	.L23b48
	orcc	%i0, %i2, %g0
	mov	0, %i3
	ba	.L23dc8
	mov	0, %i2
.L23b48:
	bne,a	.L23b68
	mov	0, %o1
	mov	%i3, %o1
	call	.udiv
	mov	%i1, %o0
	mov	%o0, %i3
	ba	.L23dc8
	mov	%g0, %i2
.L23b68:
	mov	%i1, %l5
	cmp	%l5, %o1
	sethi	%hi(0x20000000), %o0
	mov	%i0, %l4
	subxcc 	%l4, %o0, %g0
	blu	.L23c88
	mov	0, %l7
	mov	0, %i5
	sethi	%hi(0x20000000), %o0
	mov	%o0, %i4
	cmp	%i3, %i5
	subxcc 	%i2, %i4, %g0
	bgeu,a 	.L23bc4
	mov	%i1, %i5
.L23ba0:
	srl	%i3, 30, %o0
	sll	%i3, 2, %i3
	sll	%i2, 2, %i2
	or	%i2, %o0, %i2
	cmp	%i3, %i5
	subxcc 	%i2, %i4, %g0
	blu	.L23ba0
	add	%l7, 1, %l7
	mov	%i1, %i5
.L23bc4:
	cmp	%i3, %i5
	mov	%i0, %i4
	subxcc 	%i2, %i4, %g0
	bgeu	.L23c10
	mov	0, %l6
.L23bd8:
	sll	%i3, 1, %o2
	or	%i3, %o2, %o2
	srl	%o2, 1, %o2
	orcc	%i2, %o2, %g0
	bl,a	.L23c14
	subcc	%i1, %i3, %i1
	srl	%i3, 31, %o0
	sll	%i3, 1, %i3
	sll	%i2, 1, %i2
	or	%i2, %o0, %i2
	cmp	%i3, %i5
	subxcc 	%i2, %i4, %g0
	blu	.L23bd8
	add	%l6, 1, %l6
.L23c10:
	subcc	%i1, %i3, %i1
.L23c14:
	subx	%i0, %i2, %i0
	mov	1, %i5
	mov	0, %i4
.L23c20:
	subcc	%l6, 1, %l6
	bneg,a 	.L23cc0
	subcc	%l7, 1, %l7
	sll	%i4, 1, %i4
	srl	%i5, 31, %o0
	or	%i4, %o0, %i4
	sll	%i2, 31, %o0
	srl	%i3, 1, %i3
	or	%o0, %i3, %i3
	sll	%i1, 1, %o0
	or	%i1, %o0, %o0
	srl	%o0, 1, %o0
	orcc	%i0, %o0, %g0
	srl	%i2, 1, %i2
	bl	.L23c74
	sll	%i5, 1, %i5
	subcc	%i1, %i3, %i1
	subx	%i0, %i2, %i0
	addcc	%i5, 1, %i5
	ba	.L23c20
	addx	%i4, 0, %i4
.L23c74:
	addcc	%i1, %i3, %i1
	addx	%i0, %i2, %i0
	subcc	%i5, 1, %i5
	ba	.L23c20
	subx	%i4, 0, %i4
.L23c88:
	mov	0, %i5
	mov	0, %i4
	srl	%i3, 30, %o0
.L23c94:
	sll	%i3, 2, %i3
	sll	%i2, 2, %i2
	or	%i2, %o0, %i2
	subcc	%i3, %l5, %o1
	subxcc 	%i2, %l4, %o0
	blu	.L23cb4
	add	%l7, 1, %l7
	orcc	%o0, %o1, %g0
.L23cb4:
	bleu,a 	.L23c94
	srl	%i3, 30, %o0
.L23cbc:
	subcc	%l7, 1, %l7
.L23cc0:
	bneg,a 	.L23da4
	sll	%i1, 1, %o0
	sll	%i4, 2, %i4
	srl	%i5, 30, %o0
	or	%i4, %o0, %i4
	sll	%i2, 31, %o0
	srl	%i3, 1, %i3
	or	%o0, %i3, %i3
	sll	%i1, 1, %o0
	or	%i1, %o0, %o0
	srl	%o0, 1, %o0
	orcc	%i0, %o0, %g0
	srl	%i2, 1, %i2
	bl	.L23d50
	sll	%i5, 2, %i5
	subcc	%i1, %i3, %i1
	srl	%i3, 1, %i3
	sll	%i2, 31, %o0
	or	%o0, %i3, %i3
	sll	%i1, 1, %o0
	or	%i1, %o0, %o0
	srl	%o0, 1, %o0
	subx	%i0, %i2, %i0
	orcc	%i0, %o0, %g0
	bl	.L23d3c
	srl	%i2, 1, %i2
	subcc	%i1, %i3, %i1
	subx	%i0, %i2, %i0
	addcc	%i5, 3, %i5
	ba	.L23cbc
	addx	%i4, 0, %i4
.L23d3c:
	addcc	%i1, %i3, %i1
	addx	%i0, %i2, %i0
	addcc	%i5, 1, %i5
	ba	.L23cbc
	addx	%i4, 0, %i4
.L23d50:
	addcc	%i1, %i3, %i1
	srl	%i3, 1, %i3
	sll	%i2, 31, %o0
	or	%o0, %i3, %i3
	sll	%i1, 1, %o0
	or	%i1, %o0, %o0
	srl	%o0, 1, %o0
	addx	%i0, %i2, %i0
	orcc	%i0, %o0, %g0
	bl	.L23d90
	srl	%i2, 1, %i2
	subcc	%i1, %i3, %i1
	subx	%i0, %i2, %i0
	subcc	%i5, 1, %i5
	ba	.L23cbc
	subx	%i4, 0, %i4
.L23d90:
	addcc	%i1, %i3, %i1
	addx	%i0, %i2, %i0
	subcc	%i5, 3, %i5
	ba	.L23cbc
	subx	%i4, 0, %i4
.L23da4:
	or	%i1, %o0, %o0
	srl	%o0, 1, %o0
	orcc	%i0, %o0, %g0
	bge,a	1f ! 0x23dc4
	mov	%i5, %i3
	subcc	%i5, 1, %i5
	subx	%i4, 0, %i4
	mov	%i5, %i3
1:
	mov	%i4, %i2
.L23dc8:
	mov	%i2, %i0
	ret
	restore	%g0, %i3, %o1
	SET_SIZE(__udiv64)

	RTENTRY(__urem64)
	save	%sp, -96, %sp
	cmp	%i3, 2
	subxcc 	%i2, 0, %g0
	bgeu,a 	1f ! 0x23df4
	cmp	%i1, %i3
	mov	0, %i1
	ba	.L24098
	mov	0, %i0
1:
	subxcc 	%i0, %i2, %g0
	blu	.L24098
	orcc	%i0, %i2, %g0
	bne,a	.L23e20
	mov	%i1, %i5
	mov	%i3, %o1
	call	.urem
	mov	%i1, %o0
	mov	%o0, %o1
	ba	.L24090
	mov	%g0, %o0
.L23e20:
	mov	%i5, %l3
	mov	0, %o1
	mov	%i0, %i4
	mov	%i4, %l2
	cmp	%l3, %o1
	sethi	%hi(0x20000000), %o0
	subxcc 	%l2, %o0, %g0
	mov	%i2, %i0
	mov	%i3, %i1
	blu	.L23f50
	mov	0, %l5
	mov	0, %l7
	sethi	%hi(0x20000000), %o0
	mov	%o0, %l6
	cmp	%i1, %l7
	subxcc 	%i0, %l6, %g0
	bgeu,a 	.L23e8c
	mov	%i5, %l7
.L23e68:
	srl	%i1, 30, %o0
	sll	%i1, 2, %i1
	sll	%i0, 2, %i0
	or	%i0, %o0, %i0
	cmp	%i1, %l7
	subxcc 	%i0, %l6, %g0
	blu	.L23e68
	add	%l5, 1, %l5
	mov	%i5, %l7
.L23e8c:
	cmp	%i1, %l7
	mov	%i4, %l6
	subxcc 	%i0, %l6, %g0
	bgeu	.L23ed8
	mov	0, %l4
.L23ea0:
	sll	%i1, 1, %o2
	or	%i1, %o2, %o2
	srl	%o2, 1, %o2
	orcc	%i0, %o2, %g0
	bl,a	.L23edc
	subcc	%i5, %i1, %i5
	srl	%i1, 31, %o0
	sll	%i1, 1, %i1
	sll	%i0, 1, %i0
	or	%i0, %o0, %i0
	cmp	%i1, %l7
	subxcc 	%i0, %l6, %g0
	blu	.L23ea0
	add	%l4, 1, %l4
.L23ed8:
	subcc	%i5, %i1, %i5
.L23edc:
	subx	%i4, %i0, %i4
	mov	1, %l7
	mov	0, %l6
.L23ee8:
	subcc	%l4, 1, %l4
	bneg,a 	.L23f88
	subcc	%l5, 1, %l5
	sll	%l6, 1, %l6
	srl	%l7, 31, %o0
	or	%l6, %o0, %l6
	sll	%i0, 31, %o0
	srl	%i1, 1, %i1
	or	%o0, %i1, %i1
	sll	%i5, 1, %o0
	or	%i5, %o0, %o0
	srl	%o0, 1, %o0
	orcc	%i4, %o0, %g0
	srl	%i0, 1, %i0
	bl	.L23f3c
	sll	%l7, 1, %l7
	subcc	%i5, %i1, %i5
	subx	%i4, %i0, %i4
	addcc	%l7, 1, %l7
	ba	.L23ee8
	addx	%l6, 0, %l6
.L23f3c:
	addcc	%i5, %i1, %i5
	addx	%i4, %i0, %i4
	subcc	%l7, 1, %l7
	ba	.L23ee8
	subx	%l6, 0, %l6
.L23f50:
	mov	0, %l7
	mov	0, %l6
	srl	%i1, 30, %o0
.L23f5c:
	sll	%i1, 2, %i1
	sll	%i0, 2, %i0
	or	%i0, %o0, %i0
	subcc	%i1, %l3, %o1
	subxcc 	%i0, %l2, %o0
	blu	.L23f7c
	add	%l5, 1, %l5
	orcc	%o0, %o1, %g0
.L23f7c:
	bleu,a 	.L23f5c
	srl	%i1, 30, %o0
.L23f84:
	subcc	%l5, 1, %l5
.L23f88:
	bneg,a 	.L2406c
	sll	%i5, 1, %o0
	sll	%l6, 2, %l6
	srl	%l7, 30, %o0
	or	%l6, %o0, %l6
	sll	%i0, 31, %o0
	srl	%i1, 1, %i1
	or	%o0, %i1, %i1
	sll	%i5, 1, %o0
	or	%i5, %o0, %o0
	srl	%o0, 1, %o0
	orcc	%i4, %o0, %g0
	srl	%i0, 1, %i0
	bl	.L24018
	sll	%l7, 2, %l7
	subcc	%i5, %i1, %i5
	srl	%i1, 1, %i1
	sll	%i0, 31, %o0
	or	%o0, %i1, %i1
	sll	%i5, 1, %o0
	or	%i5, %o0, %o0
	srl	%o0, 1, %o0
	subx	%i4, %i0, %i4
	orcc	%i4, %o0, %g0
	bl	.L24004
	srl	%i0, 1, %i0
	subcc	%i5, %i1, %i5
	subx	%i4, %i0, %i4
	addcc	%l7, 3, %l7
	ba	.L23f84
	addx	%l6, 0, %l6
.L24004:
	addcc	%i5, %i1, %i5
	addx	%i4, %i0, %i4
	addcc	%l7, 1, %l7
	ba	.L23f84
	addx	%l6, 0, %l6
.L24018:
	addcc	%i5, %i1, %i5
	srl	%i1, 1, %i1
	sll	%i0, 31, %o0
	or	%o0, %i1, %i1
	sll	%i5, 1, %o0
	or	%i5, %o0, %o0
	srl	%o0, 1, %o0
	addx	%i4, %i0, %i4
	orcc	%i4, %o0, %g0
	bl	.L24058
	srl	%i0, 1, %i0
	subcc	%i5, %i1, %i5
	subx	%i4, %i0, %i4
	subcc	%l7, 1, %l7
	ba	.L23f84
	subx	%l6, 0, %l6
.L24058:
	addcc	%i5, %i1, %i5
	addx	%i4, %i0, %i4
	subcc	%l7, 3, %l7
	ba	.L23f84
	subx	%l6, 0, %l6
.L2406c:
	or	%i5, %o0, %o0
	srl	%o0, 1, %o0
	orcc	%i4, %o0, %g0
	bge,a	.L2408c
	mov	%i5, %o1
	addcc	%i5, %i3, %i5
	addx	%i4, %i2, %i4
	mov	%i5, %o1
.L2408c:
	mov	%i4, %o0
.L24090:
	mov	%o0, %i0
	mov	%o1, %i1
.L24098:
	ret
	restore
	SET_SIZE(__urem64)

	RTENTRY(__mul64)
	save	%sp, -96, %sp
	sll	%i1, 1, %o0
	or	%i1, %o0, %o0
	srl	%o0, 1, %o0
	orcc	%i0, %o0, %g0
	mov	0, %l7
	mov	0, %i4
	bge	1f
	mov	0, %i5
	subcc	%g0, %i1, %i1
	subx	%g0, %i0, %o0
	mov	%o0, %i0
	mov	1, %l7
1:	sll	%i3, 1, %o0
	or	%i3, %o0, %o0
	srl	%o0, 1, %o0
	orcc	%i2, %o0, %g0
	bge,a	1f
	orcc	%g0, %i3, %l5
	subcc	%g0, %i3, %i3
	subx	%g0, %i2, %o0
	mov	%o0, %i2
	xor	%l7, 1, %l7
	orcc	%g0, %i3, %l5
1:	mov	%i2, %l4
	be	2f
	mov	%i1, %l6
	orcc	%g0, %l6, %g0
	be,a	1f
	orcc	%g0, %i0, %g0
	mov	%l5, %o1
	call	.umul
	mov	%l6, %o0
	mov	%o0, %i5
	mov	%o1, %i4
	orcc	%g0, %i0, %g0
1:	be,a	3f
	orcc	%g0, %l4, %g0
	mov	%l5, %o1
	call	.umul
	mov	%i0, %o0
	mov	%g0, %o5
	addcc	%i5, %o5, %i5
	addx	%i4, %o0, %i4
2:	orcc	%g0, %l4, %g0
3:	be,a	1f
	orcc	%g0, %l7, %g0
	orcc	%g0, %l6, %g0
	be,a	1f
	orcc	%g0, %l7, %g0
	mov	%l4, %o1
	call	.umul
	mov	%l6, %o0
	mov	%g0, %o5
	addcc	%i5, %o5, %i5
	addx	%i4, %o0, %i4
	orcc	%g0, %l7, %g0
1:	be,a	1f
	mov	%i4, %i0
	subcc	%g0, %i5, %i5
	subx	%g0, %i4, %o0
	mov	%o0, %i4
	mov	%i4, %i0
1:	ret
	restore	%g0, %i5, %o1
	SET_SIZE(__mul64)

	RTENTRY(__umul64)
	save	%sp, -96, %sp
	orcc	%g0, %i3, %l6
	mov	0, %i4
	mov	0, %i5
	mov	%i1, %l7
	be	2f
	mov	%i2, %l5
	orcc	%g0, %l7, %g0
	be,a	1f
	orcc	%g0, %i0, %o0
	mov	%l6, %o1
	call	.umul
	mov	%l7, %o0
	mov	%o0, %i5
	mov	%o1, %i4
	orcc	%g0, %i0, %o0
1:	be,a	1f
	orcc	%g0, %l5, %g0
	call	.umul
	mov	%l6, %o1
	mov	%g0, %o3
	addcc	%i5, %o3, %i5
	addx	%i4, %o0, %i4
2:	orcc	%g0, %l5, %g0
1:	be,a	1f
	mov	%i4, %i0
	orcc	%g0, %l7, %g0
	be,a	1f
	mov	%i4, %i0
	mov	%l5, %o1
	call	.umul
	mov	%l7, %o0
	mov	%g0, %o3
	addcc	%i5, %o3, %i5
	addx	%i4, %o0, %i4
	mov	%i4, %i0
1:	ret
	restore	%g0, %i5, %o1
	SET_SIZE(__umul64)

#endif /* lint */
