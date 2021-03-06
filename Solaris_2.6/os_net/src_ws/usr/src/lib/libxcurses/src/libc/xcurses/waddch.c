/*
 * Copyright (c) 1995, by Sun Microsystems, Inc.
 * All rights reserved.
 */

#ident	"@(#)waddch.c 1.1	95/12/22 SMI"

/*
 * waddch.c
 * 
 * XCurses Library
 *
 * Copyright 1990, 1995 by Mortice Kern Systems Inc.  All rights reserved.
 *
 * This Software is unpublished, valuable, confidential property of
 * Mortice Kern Systems Inc.  Use is authorized only in accordance
 * with the terms and conditions of the source licence agreement
 * protecting this Software.  Any unauthorized use or disclosure of
 * this Software is strictly prohibited and will result in the
 * termination of the licence agreement.
 *
 * If you have any questions, please consult your supervisor.
 */

#if M_RCSID
#ifndef lint
static char rcsID[] = "$Header: /rd/src/libc/xcurses/rcs/waddch.c 1.1 1995/06/07 14:08:34 ant Exp $";
#endif
#endif

#include <private.h>

int
waddch(w, ch)
WINDOW *w;
chtype ch;
{
	int code;
	cchar_t cc;

#ifdef M_CURSES_TRACE
	__m_trace("waddch(%p, %lx)", w, ch);
#endif

	(void) __m_chtype_cc(ch, &cc);
	code = wadd_wch(w, &cc);

	return __m_return_code("waddch", code);
}
