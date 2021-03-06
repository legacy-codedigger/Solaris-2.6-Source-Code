/*
 * Copyright (c) 1995, by Sun Microsystems, Inc.
 * All rights reserved.
 */

#ident	"@(#)tgetent.c 1.1	95/12/22 SMI"

/*
 * tgetent.c
 *
 * XCurses Library
 *
 * Copyright 1990, 1995 by Mortice Kern Inc.  All rights reserved.
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

#ifdef M_RCSID
#ifndef lint
static char rcsID[] = "$Header: /rd/src/libc/xcurses/rcs/tgetent.c 1.1 1995/07/21 14:20:22 ant Exp $";
#endif
#endif

#include <private.h>

/*
 * Return  if termcap file (terminfo database) cannot be opened;
 * 0 if the terminal name is not present in the termcap file; 1 if
 * all goes well.
 */
int
(tgetent)(buffer, name)
char *buffer, *name;
{
	int err;

#ifdef M_CURSES_TRACE
	__m_trace("tgetent(%p, %p = \"%s\")", buffer, name, name);
#endif

	(void) setupterm(name, fileno(stdout), &err);

	return __m_return_int("tgetent", err);
}
