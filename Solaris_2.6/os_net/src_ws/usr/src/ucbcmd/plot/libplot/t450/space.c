/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)space.c	1.1	90/08/19 SMI"	/* SVr4.0 1.1	*/

/*
 * Copyright (c) 1983, 1984 1985, 1986, 1987, 1988, Sun Microsystems, Inc.
 * All Rights Reserved.
 */

# include "con.h"
space(x0,y0,x1,y1){
	botx = -2047.;
	boty = -2047.;
	obotx = x0;
	oboty = y0;
	scalex = 4096./(x1-x0);
	scaley = 4096./(y1-y0);
}
