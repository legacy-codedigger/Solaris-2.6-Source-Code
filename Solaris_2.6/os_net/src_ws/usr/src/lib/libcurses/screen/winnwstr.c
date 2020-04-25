/*  Copyright (c) 1988 AT&T */
/*    All Rights Reserved   */

/*  THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T */
/*  The copyright notice above does not evidence any    */
/*  actual or intended publication of such source code. */

#ident  "@(#)winnwstr.c 1.1 93/05/05 SMI"
 
#include	"curses_inc.h"

/*
 * Copy n chars(wchar_t) in window win from current cursor position to end
 * of window into char buffer str.  Return the number of chars copied.
 */

winnwstr(win, wstr, ncols)
register	WINDOW	*win;
register	wchar_t	*wstr;
register	int	ncols;
{
    register	int	counter = 0;
    int			cy = win->_cury;
    register	chtype	*ptr = &(win->_y[cy][win->_curx]),
			*pmax = &(win->_y[cy][win->_maxx]);
    wchar_t		wc;
    int			eucw, scrw, s;
    char		*cp, cbuf[CSMAX+1];


    while (ISCBIT(*ptr))
	ptr--;

    if (ncols < -1)
	ncols = MAXINT;

    while (counter < ncols)
    {
	wc = RBYTE(*ptr);
	scrw = mbscrw(wc);
	eucw = mbeucw(wc);
	cp = cbuf;
	for (s = 0; s < scrw; s++, ptr++)
	{
	    if ((wc = RBYTE(*ptr)) == MBIT)
		continue;
	    *cp++ = wc;
	    if ((wc = LBYTE(*ptr) | MBIT) == MBIT)
		continue;
	    *cp++ = wc;
	}
	*cp = (char)0;

	if (_curs_mbtowc(&wc, cbuf, CSMAX) <= 0)
		break;
	
	*wstr++ = wc;
	counter++;

	if (ptr >= pmax)
	{
	    if (++cy == win->_maxy)
		break;

	    ptr = &(win->_y[cy][0]);
	    pmax = ptr + win->_maxx;
	}
    }
    if (counter < ncols)
	    *wstr = (wchar_t)0;

    return (counter);
}
