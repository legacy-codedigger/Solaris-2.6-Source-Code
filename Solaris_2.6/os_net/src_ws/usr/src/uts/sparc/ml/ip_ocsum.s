/*
 * Copyright (c) 1991, by Sun Microsystems, Inc.
 */

#ident	"@(#)ip_ocsum.s	1.2	91/12/12 SMI"

#include <sys/asm_linkage.h>

#if defined(lint)
#include <sys/types.h>
#endif	/* lint */

/*
 * ip_ocsum(address, halfword_count, sum)
 * Do a 16 bit one's complement sum of a given number of (16-bit)
 * halfwords. The halfword pointer must not be odd.
 *	%o0 address; %o1 count; %o2 sum accumulator; %o4 temp
 * 	%g2 and %g3 used in main loop
 *
 * (from @(#)ocsum.s 1.3 89/02/24 SMI)
 *
 */

#if defined(lint) 

/* ARGSUSED */
unsigned int
ip_ocsum(u_short *address, int halfword_count, unsigned int sum)
{ return (0); }

#else	/* lint */

	ENTRY(ip_ocsum)
	cmp	%o1, 31		! less than 62 bytes?
	bl,a	.dohw		!   just do halfwords
	tst	%o1		! delay slot, test count

	btst	31, %o0		! (delay slot)
	bz	2f		! if 32 byte aligned, skip
	nop

	!
	! Do first halfwords until 32-byte aligned
	!
1:
	lduh	[%o0], %g2	! read data
	add	%o0, 2, %o0	! increment address
	add	%o2, %g2, %o2	! add to accumulator, don't need carry yet
	btst	31, %o0		! 32 byte aligned?
	bnz	1b
	sub	%o1, 1, %o1	! decrement count
	!
	! loop to add in 32 byte chunks
	! The loads and adds are staggered to help avoid load/use
	! interlocks on highly pipelined implementations, and double
	! loads are used for 64-bit wide memory systems.
	!
2:
	sub	%o1, 16, %o1	! decrement count to aid testing
4:
	ldd	[%o0], %g2	! read data
	ldd	[%o0+8], %o4	! read more data
	addcc	%o2, %g2, %o2	! add to accumulator
	addxcc	%o2, %g3, %o2	! add to accumulator with carry
	ldd	[%o0+16], %g2	! read more data
	addxcc	%o2, %o4, %o2	! add to accumulator with carry
	addxcc	%o2, %o5, %o2	! add to accumulator with carry
	ldd	[%o0+24], %o4	! read more data
	addxcc	%o2, %g2, %o2	! add to accumulator with carry
	addxcc	%o2, %g3, %o2	! add to accumulator with carry
	addxcc	%o2, %o4, %o2	! add to accumulator
	addxcc	%o2, %o5, %o2	! add to accumulator with carry
	addxcc	%o2, 0, %o2	! if final carry, add it in
	subcc	%o1, 16, %o1	! decrement count (in halfwords)
	bge	4b
	add	%o0, 32, %o0	! delay slot, increment address
	
	add	%o1, 16, %o1	! add back in
	!
	! Do any remaining halfwords
	!
	b	.dohw
	tst	%o1		! delay slot, for more to do

3:
	add	%o0, 2, %o0	! increment address
	addcc	%o2, %g2, %o2	! add to accumulator
	addxcc	%o2, 0, %o2	! if carry, add it in
	subcc	%o1, 1, %o1	! decrement count
.dohw:
	bg,a	3b		! more to do?
	lduh	[%o0], %g2	! read data

	!
	! at this point the 32-bit accumulator
	! has the result that needs to be returned in 16-bits
	!
	sll	%o2, 16, %o4	! put low halfword in high halfword %o4
	addcc	%o4, %o2, %o2	! add the 2 halfwords in high %o2, set carry
	srl	%o2, 16, %o2	! shift to low halfword
	retl			! return
	addxcc	%o2, 0, %o0	! add in carry if any. result in %o0
	SET_SIZE(ip_ocsum)

#endif 	/* lint */


/*
 * ip_ocsum_copy(address, halfword_count, sum, dest)
 * Do a 16 bit one's complement sum of a given number of (16-bit)
 * halfwords. The halfword pointer must not be odd.
 *	%o0 address; %o1 count; %o2 accumulator; %o3 dest; %o4, %o5 temp
 * 	%g2 and %g3 used in main loop
 * Note: Assumes dest has the same relative allignment as address.
 */

#if defined(lint)

/* ARGSUSED */
unsigned int
ip_ocsum_copy(u_short *address, int halfword_count, 
    unsigned int sum, u_short *dest)
{ return (0); }

#else	/* lint */

	ENTRY(ip_ocsum_copy)
	cmp	%o1, 31		! less than 62 bytes?
	bl,a	.dohw_copy	!   just do halfwords
	tst	%o1		! delay slot, test count

	btst	31, %o0		! (delay slot)
	bz	2f		! if 32 byte aligned, skip
	nop

	!
	! Do first halfwords until 32-byte aligned
	!
1:
	lduh	[%o0], %g2	! read data
	add	%o0, 2, %o0	! increment address
	add	%o2, %g2, %o2	! add to accumulator, don't need carry yet
	sth	%g2, [%o3]	! *dest++
	add	%o3, 2, %o3
	btst	31, %o0		! 32 byte aligned?
	bnz	1b
	sub	%o1, 1, %o1	! decrement count
	!
	! loop to add in 32 byte chunks
	! The loads and adds are staggered to help avoid load/use
	! interlocks on highly pipelined implementations, and double
	! loads are used for 64-bit wide memory systems.
	!
2:
	sub	%o1, 16, %o1	! decrement count to aid testing
4:
	ldd	[%o0], %g2	! read data
	ldd	[%o0+8], %o4	! read more data
	addcc	%o2, %g2, %o2	! add to accumulator
	addxcc	%o2, %g3, %o2	! add to accumulator with carry
	std	%g2, [%o3]	! store to dest
	ldd	[%o0+16], %g2	! read more data
	addxcc	%o2, %o4, %o2	! add to accumulator with carry
	addxcc	%o2, %o5, %o2	! add to accumulator with carry
	std	%o4, [%o3+8]	! store to dest
	ldd	[%o0+24], %o4	! read more data
	addxcc	%o2, %g2, %o2	! add to accumulator with carry
	addxcc	%o2, %g3, %o2	! add to accumulator with carry
	std	%g2, [%o3+16]	! store to dest
	addxcc	%o2, %o4, %o2	! add to accumulator
	addxcc	%o2, %o5, %o2	! add to accumulator with carry
	std	%o4, [%o3+24]	! store to dest
	addxcc	%o2, 0, %o2	! if final carry, add it in
	add	%o3, 32, %o3	! increment dest
	subcc	%o1, 16, %o1	! decrement count (in halfwords)
	bge	4b
	add	%o0, 32, %o0	! delay slot, increment address
	
	add	%o1, 16, %o1	! add back in
	!
	! Do any remaining halfwords
	!
	b	.dohw_copy
	tst	%o1		! delay slot, for more to do

3:
	add	%o0, 2, %o0	! increment address
	addcc	%o2, %g2, %o2	! add to accumulator
	sth	%g2, [%o3]	! *dest++
	add	%o3, 2, %o3
	addxcc	%o2, 0, %o2	! if carry, add it in
	subcc	%o1, 1, %o1	! decrement count
.dohw_copy:
	bg,a	3b		! more to do?
	lduh	[%o0], %g2	! read data

	!
	! at this point the 32-bit accumulator
	! has the result that needs to be returned in 16-bits
	!
	sll	%o2, 16, %o4	! put low halfword in high halfword %o4
	addcc	%o4, %o2, %o2	! add the 2 halfwords in high %o2, set carry
	srl	%o2, 16, %o2	! shift to low halfword
	retl			! return
	addxcc	%o2, 0, %o0	! add in carry if any. result in %o0
	SET_SIZE(ip_ocsum_copy)

#endif	/* lint */
