/*
 * Copyright (c) 1991, by Sun Microsystems, Inc.
 */

#pragma ident "@(#)prom_reboot.c	1.7	94/06/10 SMI"

#include <sys/promif.h>
#include <sys/promimpl.h>
#include <sys/bootsvcs.h>

/*ARGSUSED*/
void
prom_reboot(char *bootstr)
{
#ifdef KADB
	printf("Press <CTL>-<ALT>-<DEL> to reboot.\n");
	while (1)
		prom_getchar();
#else
	reset();
#endif
}
