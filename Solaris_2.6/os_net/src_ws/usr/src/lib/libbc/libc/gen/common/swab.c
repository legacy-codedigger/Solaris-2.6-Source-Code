#pragma ident	"@(#)swab.c	1.3	92/07/21 SMI"
	  /* from UCB 4.2 83/06/27 */

/*
 * Swab bytes
 * Jeffrey Mogul, Stanford
 */

void
swab(from, to, n)
	register char *from, *to;
	register int n;
{
	register unsigned long temp;
	
	if (n <= 1)
		return;
	n >>= 1; n++;
#define	STEP	temp = *from++,*to++ = *from++,*to++ = temp
	/* round to multiple of 8 */
	while ((--n) & 07)
		STEP;
	n >>= 3;
	while (--n >= 0) {
		STEP; STEP; STEP; STEP;
		STEP; STEP; STEP; STEP;
	}
}
