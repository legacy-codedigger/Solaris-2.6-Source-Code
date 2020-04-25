/*
 * Copyright (c) 1992 Sun Microsystems, Inc.  All Rights Reserved. Sun
 * considers its source code as an unpublished, proprietary trade secret, and
 * it is available only under strict license provisions.  This copyright
 * notice is placed here only to protect Sun in the event the source is
 * deemed a published work.  Dissassembly, decompilation, or other means of
 * reducing the object code to human readable form is prohibited by the
 * license agreement under which this code is provided to the user or company
 * in possession of this copy.
 *
 * RESTRICTED RIGHTS LEGEND: Use, duplication, or disclosure by the Government
 * is subject to restrictions as set forth in subparagraph (c)(1)(ii) of the
 * Rights in Technical Data and Computer Software clause at DFARS 52.227-7013
 * and in similar clauses in the FAR and NASA FAR Supplement.
 */

#ifndef lint
#ident	"@(#)ui.h 1.6 93/04/09"
#endif

#ifndef SWM_UI_H
#define	SWM_UI_H

#ifdef XVIEW
#include "xview.h"
#endif

extern u_long	Basescreen;
extern u_long	Loadscreen;
extern u_long	Adminscreen;
extern u_long	Hostscreen;
extern u_long	Termscreen;

/*PRINTFLIKE3*/
extern void msg(u_long, int, char *, ...);
/*PRINTFLIKE1*/
extern void die(char *, ...);
/*PRINTFLIKE2*/
extern void asktoproceed(u_long, char *, ...);
/*PRINTFLIKE4*/
extern int confirm(u_long, char *, char *, char *, ...);
extern void ask_for_mode(void);

#endif	/* !SWM_UI_H */
