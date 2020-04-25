/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)dashos.c	1.4	90/07/11 SMI"	/* SVr4.0 1.5	*/
/* EMACS_MODES: !fill, lnumb, !overwrite, !nodelete, !picture */

#include "string.h"

#include "lp.h"

#define issep(X)	strchr(LP_WS, X)

/**
 ** dashos() - PARSE -o OPTIONS, (char *) --> (char **)
 **/

char **
#if	defined(__STDC__)
dashos (
	char *			o
)
#else
dashos (o)
	register char		*o;
#endif
{
	register char		quote,
				c,
				*option;

	char			**list	= 0;

	if (!o)
		return(0);

	while (*o) {

		while (*o && issep(*o))
			o++;

		for (option = o; *o && !issep(*o); o++)
			if (strchr(LP_QUOTES, (quote = *o)))
				for (o++; *o && *o != quote; o++)
					if (*o == '\\' && o[1])
						o++;

		if (option < o) {
			c = *o;
			*o = 0;
			addlist (&list, option);
			*o = c;
		}

	}
	return (list);
}
