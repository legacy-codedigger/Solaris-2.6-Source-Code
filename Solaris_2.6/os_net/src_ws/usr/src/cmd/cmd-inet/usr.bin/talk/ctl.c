/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)ctl.c	1.6	94/10/04 SMI"	/* SVr4.0 1.2	*/

/*
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * 		PROPRIETARY NOTICE (Combined)
 *
 * This source code is unpublished proprietary information
 * constituting, or derived under license from AT&T's UNIX(r) System V.
 * In addition, portions of such source code were derived from Berkeley
 * 4.3 BSD under license from the Regents of the University of
 * California.
 *
 *
 *
 * 		Copyright Notice
 *
 * Notice of copyright on this source code product does not indicate
 * publication.
 *
 * 	(c) 1986,1987,1988.1989  Sun Microsystems, Inc
 * 	(c) 1983,1984,1985,1986,1987,1988,1989  AT&T.
 *	All rights reserved.
 *
 */


/*
 * This file handles haggling with the various talk daemons to
 * get a socket to talk to. sockt is opened and connected in
 * the progress
 */

#include "talk_ctl.h"
#include <libintl.h>

struct sockaddr_in daemon_addr = { AF_INET };
struct sockaddr_in ctl_addr = { AF_INET };
struct sockaddr_in my_addr = { AF_INET };

/* inet addresses of the two machines */
struct in_addr my_machine_addr;
struct in_addr rem_machine_addr;

u_short daemon_port;	/* port number of the talk daemon */

int ctl_sockt;
int sockt;
int invitation_waiting = 0;

CTL_MSG msg;

void
open_sockt()
{
	int length;

	my_addr.sin_addr = my_machine_addr;
	my_addr.sin_port = 0;

	sockt = socket(AF_INET, SOCK_STREAM, 0);

	if (sockt <= 0) {
		p_error(gettext("Bad socket"));
	}

	if (bind(sockt, (struct sockaddr *)&my_addr, sizeof (my_addr)) != 0) {
		p_error(gettext("Binding local socket"));
	}

	length = sizeof (my_addr);

	if (getsockname(sockt, (struct sockaddr *)&my_addr, &length) == -1) {
		p_error(gettext("Bad address for socket"));
	}
}

	/* open the ctl socket */

void
open_ctl()
{
	int length;

	ctl_addr.sin_port = 0;
	ctl_addr.sin_addr = my_machine_addr;

	ctl_sockt = socket(AF_INET, SOCK_DGRAM, 0);

	if (ctl_sockt <= 0) {
		p_error(gettext("Bad socket"));
	}

	if (bind(ctl_sockt, (struct sockaddr *)&ctl_addr, sizeof (ctl_addr))
		!= 0) {
		p_error(gettext("Couldn't bind to control socket"));
	}

	length = sizeof (ctl_addr);
	if (getsockname(ctl_sockt, (struct sockaddr *)&ctl_addr, &length)
		== -1) {
		p_error(gettext("Bad address for ctl socket"));
	}
}

/* print_addr is a debug print routine */

void
print_addr(addr)
struct sockaddr_in addr;
{
	int i;

	printf("addr = %x, port = %o, family = %o zero = ",
		addr.sin_addr, (int)addr.sin_port, addr.sin_family);

	for (i = 0; i < 8; i++) {
		printf("%o ", (int)addr.sin_zero[i]);
	}
	putchar('\n');
}
