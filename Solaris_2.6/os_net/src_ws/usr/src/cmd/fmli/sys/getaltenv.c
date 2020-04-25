/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*
 * Copyright  (c) 1985 AT&T
 *	All Rights Reserved
 */
#ident	"@(#)getaltenv.c	1.4	92/07/14 SMI"       /* SVr4.0 1.3 */

#include	<stdio.h>
#include	<string.h>
#include	"wish.h"
#include	"var_arrays.h"

char **Altenv = NULL;
char *getaltenv();

/* LES: replace with MACRO
char *
getAltenv(name)
char *name;
{
	return(getaltenv(Altenv, name));
}
*/

char *
getaltenv(the_env, name)
char **the_env;
char *name;
{
	int i;

	if (the_env && ((i = findaltenv(the_env, name)) != FAIL))
		return(strchr(the_env[i], '=') + 1);
	return(NULL);
}

findaltenv(the_env, name)
char **the_env;
char *name;
{
	int i, len;
	int	lcv;

	len = strlen(name);
	lcv = array_len(the_env);
	for (i = 0; i < lcv; i++)
		if ((strncmp(name, the_env[i], len) == 0) && (the_env[i][len] == '='))
			return(i);
	return(FAIL);
}
