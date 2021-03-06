/*
 * Copyright (c) 1991 Sun Microsystems, Inc.  All Rights Reserved. Sun
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
#ident	"@(#)Client_stubs.c 1.2 92/12/14"
#endif

/*
 * Client_stubs.c - Notify and event callback function stubs.
 * This file was generated by `gxv' from `Client.G'.
 */

#include <stdio.h>
#include <sys/param.h>
#include <sys/types.h>
#include <xview/xview.h>
#include <xview/panel.h>
#include <xview/textsw.h>
#include <xview/xv_xrect.h>
#include <group.h>
#include "swmtool.h"
#include "defs.h"
#include "ui.h"


/*
 * Notify callback function for `ClientApply'.
 */
void
Client_ClientWin_ClientApply_notify_callback(Panel_item item, Event *event)
{
	Client_ClientWin_objects *ip = (Client_ClientWin_objects *) xv_get(item, XV_KEY_DATA, INSTANCE);
	
#ifdef XV_DEBUG
	fputs("swmtool: Client_ClientWin_ClientApply_notify_callback\n", stderr);
#endif /* XV_DEBUG */
	
	/* gxv_start_connections DO NOT EDIT THIS SECTION */

	{
		SetClientInfo(item);
	}
	
	/* gxv_end_connections */

}

/*
 * Notify callback function for `ClientReset'.
 */
void
Client_ClientWin_ClientReset_notify_callback(Panel_item item, Event *event)
{
	Client_ClientWin_objects *ip = (Client_ClientWin_objects *) xv_get(item, XV_KEY_DATA, INSTANCE);
	
#ifdef XV_DEBUG
	fputs("swmtool: Client_ClientWin_ClientReset_notify_callback\n", stderr);
#endif /* XV_DEBUG */
	
	/* gxv_start_connections DO NOT EDIT THIS SECTION */

	{
		ResetClientInfo(item);
	}
	
	/* gxv_end_connections */

}

