/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)tmpfile.c	1.18	96/10/24 SMI"	/* SVr4.0 1.12	*/

/*LINTLIBRARY*/
/*
 *	tmpfile - return a pointer to an update file that can be
 *		used for scratch. The file will automatically
 *		go away if the program using it terminates.
 */

#define	unlink		_unlink

#if defined(ABI)
#define	strcpy		_abi_strcpy
#define	strcat		_abi_strcat
#endif

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <thread.h>
#include <synch.h>
#include <mtlib.h>

static char seed[] = { 'a', 'a', 'a', '\0' };
#ifdef _REENTRANT
static mutex_t seed_lk = DEFAULTMUTEX;
#endif _REENTRANT

#define	XS "\bXXXXXX"		/* a '\b' character is prepended to this */
				/* string to avoid conflicts with names */
				/* generated by tmpnam() */
static FILE *
_common(boolean_t large_file)
{
	char	tfname[L_tmpnam];
	FILE	*p;
	char	*q;
	int	mkret;
	mode_t	current_umask;

	(void) strcpy(tfname, P_tmpdir);
	_mutex_lock(&seed_lk);
	(void) strcat(tfname, seed);
	(void) strcat(tfname, XS);

	q = seed;
	while (*q == 'z')
		*q++ = 'a';
	if (*q != '\0')
		++*q;
	_mutex_unlock(&seed_lk);

	if (large_file == B_TRUE) {
		if ((mkret = mkstemp64(tfname)) == -1)
			return (NULL);
	} else {
		if ((mkret = mkstemp(tfname)) == -1)
			return (NULL);
	}

	(void) unlink(tfname);
	current_umask = umask(0777);
	(void) umask(current_umask);
	(void) fchmod(mkret, 0666 & ~current_umask);
	if ((p = fdopen(mkret, "w+")) == NULL) {
		(void) close(mkret);
		return (NULL);
	}

	return (p);
}

FILE *
tmpfile64(void)
{
	return (_common(B_TRUE));
}

FILE *
tmpfile(void)
{
	return (_common(B_FALSE));
}
