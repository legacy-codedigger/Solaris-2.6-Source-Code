/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)bcmp.c	1.4	94/11/18 SMI"	/* SVr4.0 1.1	*/

/*
 *
 *              PROPRIETARY NOTICE(Combined)
 *
 * This source code is unpublished proprietary information
 * constituting, or derived under license from AT&T's UNIX(r) System V.
 * In addition, portions of such source code were derived from Berkeley
 * 4.3 BSD under license from the Regents of the University of
 * California.
 *
 *
 *
 *              Copyright Notice
 *
 * Notice of copyright on this source code product does not indicate
 * publication.
 *
 *      (c) 1986, 1987, 1988, 1989  Sun Microsystems, Inc
 *      (c) 1983, 1984, 1985, 1986, 1987, 1988, 1989  AT&T.
 *              All rights reserved.
*/

#if !defined(lint) && defined(SCCSIDS)
static	char sccsid[] = "@(#)bcmp.c 1.6 88/02/08 SMI";
			/* from UCB X.X XX/XX/XX */
#endif

int
bcmp(char *s1, char *s2, int len)
{
	if (_memcmp(s1, s2, len) == 0)
		return (0);
	else
		return (1);
}
