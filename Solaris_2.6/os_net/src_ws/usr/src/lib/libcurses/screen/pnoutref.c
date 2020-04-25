/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)pnoutref.c	1.5	92/07/14 SMI"	/* SVr4.0 1.11	*/

#include	"curses_inc.h"

/* wnoutrefresh for pads. */

pnoutrefresh(pad, pby, pbx, sby, sbx, sey, sex)
WINDOW	*pad;
int	pby, pbx, sby, sbx, sey, sex;
{
    extern	int	wnoutrefresh();

    return (_prefresh(wnoutrefresh, pad, pby, pbx, sby, sbx, sey, sex));
}
