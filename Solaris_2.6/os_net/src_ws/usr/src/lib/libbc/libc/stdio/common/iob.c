#pragma ident	"@(#)iob.c	1.7	92/07/20 SMI"  /* from Sun */

#include <stdio.h>
#include "iob.h"

FILE _iob[NSTATIC] = {
#if pdp11
	{ NULL, 0, NULL, 0, _IOREAD,			0 },	/* stdin */
	{ NULL, 0, NULL, 0, _IOWRT,			1 },	/* stdout */
	{ NULL, 0, NULL, 0, _IOWRT|_IONBF,		2 },	/* stderr */
#else
#if u370
	{ NULL, 0, NULL, 0, _IOREAD,			0 },	/* stdin */
	{ NULL, 0, NULL, 0, _IOWRT,			1 },	/* stdout */
	{ NULL, 0, NULL, 0, _IOWRT|_IONBF,		2 },	/* stderr */
#else	/* just about every other UNIX system in existence */
	{ 0, NULL, NULL, 0, _IOREAD,			0 },	/* stdin */
	{ 0, NULL, NULL, 0, _IOWRT,			1 },	/* stdout */
	{ 0, NULL, NULL, 0, _IOWRT|_IONBF,		2 },	/* stderr */
#endif
#endif
};
