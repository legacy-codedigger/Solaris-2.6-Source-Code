/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)strncat.c	1.7	92/07/14 SMI"	/* SVr4.0 1.9	*/

/*	3.0 SID #	1.2	*/
/*LINTLIBRARY*/
/*
 * Concatenate s2 on the end of s1.  S1's space must be large enough.
 * At most n characters are moved.
 * Return s1.
 */

#include "synonyms.h"
#include <string.h>

char *
strncat(s1, s2, n)
register char *s1;
#ifdef __STDC__
register const char *s2;
#else
register char *s2;
#endif
register size_t n;
{
	char *os1 = s1;

	n++;
	while(*s1++)
		;
	--s1;
	while(*s1++ = *s2++)
		if(--n == 0) {
			s1[-1] = '\0';
			break;
		}
	return(os1);
}
