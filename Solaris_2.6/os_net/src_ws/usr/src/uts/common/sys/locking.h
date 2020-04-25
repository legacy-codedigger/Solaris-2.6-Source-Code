/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	Copyright (c) 1987, 1988 Microsoft Corporation	*/
/*	  All Rights Reserved	*/

/*	This Module contains Proprietary Information of Microsoft  */
/*	Corporation and should be treated as Confidential.	   */

#ifndef _SYS_LOCKING_H
#define	_SYS_LOCKING_H

#pragma ident	"@(#)locking.h	1.10	93/11/04 SMI"	/* SVr4.0 1.2	*/

#ifdef	__cplusplus
extern "C" {
#endif

/*
 *   Flag values for XENIX locking() system call (os/xsys.c)
 */

#define	LK_UNLCK  0	/* unlock request */
#define	LK_LOCK   1	/* lock request */
#define	LK_NBLCK  20	/* non-blocking lock request */
#define	LK_RLCK   3	/* read permitted only lock request */
#define	LK_NBRLCK 4	/* non-blocking read only lock request */

#ifdef	__cplusplus
}
#endif

#endif	/* _SYS_LOCKING_H */
