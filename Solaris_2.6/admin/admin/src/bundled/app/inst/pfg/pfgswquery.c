#ifndef lint
#pragma ident "@(#)pfgswquery.c 1.28 96/08/07 SMI"
#endif

/*
 * Copyright (c) 1994-1996 by Sun MicroSystems, Inc.
 * All rights reserved.
 */

/*
 * Module:	pfgswquery.c
 * Group:	installtool
 * Description:
 */

#include "pf.h"
#include "pfg.h"

#include "pfgSwQuery_ui.h"

Widget
pfgCreateSwQuery(void)
{
	Widget swquery_dialog;
	WidgetList widget_list;

	swquery_dialog = tu_swquery_dialog_widget("swquery_dialog",
		pfgTopLevel, &widget_list);

	XmAddWMProtocolCallback(pfgShell(swquery_dialog), pfgWMDeleteAtom,
	    (XtCallbackProc) pfgExit, NULL);

	XtVaSetValues(pfgShell(swquery_dialog),
	    XmNtitle, TITLE_UPG_CUSTOM_SWQUERY,
	    XmNdeleteResponse, XmDO_NOTHING,
	    NULL);
	xm_SetNoResize(pfgTopLevel, swquery_dialog);

	pfgSetWidgetString(widget_list, "panelhelpText",
		MSG_UPG_CUSTOM_SWQUERY);
	pfgSetWidgetString(widget_list, "continueButton", PFG_CONTINUE);
	pfgSetWidgetString(widget_list, "gobackButton", PFG_GOBACK);
	pfgSetWidgetString(widget_list, "customizeButton", PFG_CUSTOMIZE);
	pfgSetWidgetString(widget_list, "exitButton", PFG_EXIT);
	pfgSetWidgetString(widget_list, "helpButton", PFG_HELP);

	free(widget_list);

	XtManageChild(swquery_dialog);

	return (swquery_dialog);

}


/* ARGSUSED */
void
swQueryCustomCB(Widget w, XtPointer clientD, XtPointer callD)
{

	pfgBusy(pfgShell(w));
	(void) pfgCreateSoftware(pfgShell(w));
}


/* ARGSUSED */
void
swQueryContinueCB(Widget button, XtPointer clientD, XtPointer callD)
{
	int err;

	pfgBusy(pfgShell(button));

	/*
	 * At this point, we have to find out if there really is
	 * enough space on the system to hold all the currently seelcted
	 * software.
	 * A call to verify_fs_layout along with the parDsrAnalyze
	 * progress bar has already been made, so the assumption here is
	 * that this is a fast call, so there is no progress bar here...
	 */
	err = DsrFSAnalyzeSystem(FsSpaceInfo, NULL, NULL, NULL);
	if (err == SP_ERR_NOT_ENOUGH_SPACE) {
		/* There are failed file systems.
		 * Now that we will be entering DSR,
		 * create the slice list.
		 */
		if (DsrSLUICreate(&DsrALHandle, &DsrSLHandle, FsSpaceInfo)) {
			pfAppError(NULL,
				"Internal DSR error - can't create slice list");
			pfgCleanExit(EXIT_INSTALL_FAILURE, (void *) NULL);
		}
		if (get_trace_level() > 2) {
			DsrSLPrint(DsrSLHandle, DEBUG_LOC);
		}

		pfgSetAction(parADsrSpaceReq);
	} else {
		/* there are no failed file systems */
		pfgSetAction(parAContinue);
	}
}


/* ARGSUSED */
void
swQueryGobackCB(Widget w, XtPointer clientD, XtPointer callD)
{
	pfgBusy(pfgShell(w));
	pfgSetAction(parAGoback);
}
