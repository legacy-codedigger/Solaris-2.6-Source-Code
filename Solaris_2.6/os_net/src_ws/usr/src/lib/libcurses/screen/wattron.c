/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)wattron.c	1.5	92/07/14 SMI"	/* SVr4.0 1.11	*/

#include	"curses_inc.h"

wattron(win,a)
WINDOW	*win;
chtype	a;
{
    /* if 'a' contains color information, then if we are on color terminal */
    /* erase color information from window attribute, otherwise erase      */
    /* color information from 'a'					   */

    if (a & A_COLOR)
        if (cur_term->_pairs_tbl)
            win->_attrs &= ~A_COLOR;
	else
	    a &= ~A_COLOR;

    win->_attrs |= (a & A_ATTRIBUTES);
    return (1);
}