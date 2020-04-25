/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)quit.c	1.8	93/03/10 SMI"	/* SVr4.0  1.7.1.1	*/

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <libintl.h>
#include <pkgdev.h>
#include <pkglib.h>
#include "libadm.h"
#include "libinst.h"

extern struct pkgdev pkgdev;
extern int	intfchg, npkgs, failflag, admnflag, nullflag, intrflag,
		warnflag, reboot, ireboot;

/* main.c */
extern void	ckreturn(int retcode);

/* presvr4.c */
extern void	intf_reloc(void);

#define	MSG_REBOOT	"\\n*** IMPORTANT NOTICE ***\\n" \
			"\\tThis machine must now be rebooted in order to " \
			"ensure\\n" \
			"\\tsane operation.  Execute\\n\\t\\tshutdown -y -i6 " \
			"-g0\\n" \
			"\\tand wait for the \"Console Login:\" prompt."

void		trap(int signo), quit(int retcode);

void
trap(int signo)
{
	if ((signo == SIGINT) || (signo == SIGHUP))
		quit(3);
	else
		quit(1);
}

void
quit(int retcode)
{
	(void) signal(SIGINT, SIG_IGN);
	(void) signal(SIGHUP, SIG_IGN);

	if (retcode != 99) {
		ckreturn(retcode);
		if (failflag)
			retcode = 1;
		else if (warnflag)
			retcode = 2;
		else if (intrflag)
			retcode = 3;
		else if (admnflag)
			retcode = 4;
		else if (nullflag)
			retcode = 5;
		else
			retcode = 0;
		if (ireboot)
			retcode += 20;
		if (reboot)
			retcode += 10;
	}

	if (reboot || ireboot)
		ptext(stderr, gettext(MSG_REBOOT));

	if (pkgdev.mount) {
		(void) chdir("/");
		(void) pkgumount(&pkgdev);
	}
	if (npkgs)
		if (npkgs > 1)
			echo(gettext("\n%d packages not processed!\n"), npkgs);
		else
			echo(gettext("\n1 package not processed!\n"));

	if (intfchg)
		intf_reloc();
	exit(retcode);
}
