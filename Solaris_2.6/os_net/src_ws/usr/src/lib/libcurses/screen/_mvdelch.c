/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)_mvdelch.c	1.5	92/07/14 SMI"	/* SVr4.0 1.1	*/

#define		NOMACROS
#include	"curses_inc.h"

mvdelch(y, x)
int	y, x;
{
    return (wmove(stdscr, y, x)==ERR?ERR:wdelch(stdscr));
}
