/*
 * Copyright (c) 1995, by Sun Microsystems, Inc.
 * All rights reserved.
 */

#ident	"@(#)vw_scanw.c 1.2	96/02/16 SMI"

/*
 * vw_scan.c
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
static char rcsID[] = "$Header: /rd/src/libc/xcurses/rcs/vw_scanw.c 1.3 1995/07/14 20:48:27 ant Exp $";
#endif
#endif

#include <private.h>
#include <limits.h>
#include <stdarg.h>

int
vw_scanw(w, fmt, ap)
WINDOW *w;
const char *fmt;
#if defined(sun)
va_list ap;
#else
void *ap;
#endif
{
	int code;
	char buffer[LINE_MAX];

#ifdef M_CURSES_TRACE
	__m_trace("vw_scanw(%p, %p = \"%s\", %p)", w, fmt, fmt, ap);
#endif
	if ((code = wgetnstr(w, buffer, (int) sizeof buffer)) == OK)
#if defined(sun)
		(void) m_vsscanf(buffer, fmt, ap);
#else
		(void) m_vsscanf(buffer, fmt, (va_list) ap);
#endif

	return __m_return_code("vw_scanw", code);
}
