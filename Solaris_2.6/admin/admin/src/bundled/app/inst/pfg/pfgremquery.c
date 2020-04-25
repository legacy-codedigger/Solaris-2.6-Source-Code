#ifndef lint
#pragma ident "@(#)pfgremquery.c 1.18 96/04/29 SMI"
#endif

/*
 * Copyright (c) 1994-1995 by Sun MicroSystems, Inc.
 * All rights reserved.
 */

/*
 * Module:	pfgremquery.c
 * Group:	installtool
 * Description:
 */

#include "pf.h"
#include "pfg.h"

#include "pfgRemquery_ui.h"

Widget
pfgCreateRemquery(void)
{
	Widget remquery_dialog;
	WidgetList widget_list;

	remquery_dialog = tu_remquery_dialog_widget("remquery_dialog",
		pfgTopLevel, &widget_list);

	XmAddWMProtocolCallback(pfgShell(remquery_dialog), pfgWMDeleteAtom,
	    (XtCallbackProc) pfgExit, NULL);

	XtVaSetValues(pfgShell(remquery_dialog),
	    XmNtitle, TITLE_MOUNTQUERY,
	    XmNdeleteResponse, XmDO_NOTHING,
	    NULL);
	xm_SetNoResize(pfgTopLevel, remquery_dialog);

	pfgSetWidgetString(widget_list, "panelhelpText", MSG_MOUNTQUERY);
	pfgSetWidgetString(widget_list, "continueButton", PFG_CONTINUE);
	pfgSetWidgetString(widget_list, "gobackButton", PFG_GOBACK);
	pfgSetWidgetString(widget_list, "remoteButton", PFG_RQ_EDIT);
	pfgSetWidgetString(widget_list, "exitButton", PFG_EXIT);
	pfgSetWidgetString(widget_list, "helpButton", PFG_HELP);

	free(widget_list);

	XtManageChild(remquery_dialog);

	return (remquery_dialog);
}


/* ARGSUSED */
void
remqueryRemoteCB(Widget w, XtPointer clientD, XtPointer callD)
{
	pfgBusy(pfgShell(w));

	(void) pfgCreateRemote(pfgShell(w));
}


/* ARGSUSED */
void
remqueryContinueCB(Widget w, XtPointer clientD, XtPointer callD)
{
	pfgSetAction(parAContinue);
}


/* ARGSUSED */
void
remqueryGobackCB(Widget w, XtPointer clientD, XtPointer callD)
{
	pfgSetAction(parAGoback);
}
