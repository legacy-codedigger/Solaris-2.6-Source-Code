/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)standout.c	1.1	90/04/27 SMI"	/* SVr4.0 1.1	*/

/*******************************************************************

		PROPRIETARY NOTICE (Combined)

This source code is unpublished proprietary information
constituting, or derived under license from AT&T's UNIX(r) System V.
In addition, portions of such source code were derived from Berkeley
4.3 BSD under license from the Regents of the University of
California.



		Copyright Notice 

Notice of copyright on this source code product does not indicate 
publication.

	(c) 1986,1987,1988,1989  Sun Microsystems, Inc
	(c) 1983,1984,1985,1986,1987,1988,1989  AT&T.
	          All rights reserved.
********************************************************************/ 

/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#ifndef lint
static	char sccsid[] = "@(#)standout.c 1.6 88/02/08 SMI"; /* from UCB 5.1 85/06/07 */
#endif not lint

/*
 * routines dealing with entering and exiting standout mode
 *
 */

# include	"curses.ext"

/*
 * enter standout mode
 */
char *
wstandout(win)
reg WINDOW	*win;
{
	if (!SO && !UC)
		return FALSE;

	win->_flags |= _STANDOUT;
	return (SO ? SO : UC);
}

/*
 * exit standout mode
 */
char *
wstandend(win)
reg WINDOW	*win;
{
	if (!SO && !UC)
		return FALSE;

	win->_flags &= ~_STANDOUT;
	return (SE ? SE : UC);
}
