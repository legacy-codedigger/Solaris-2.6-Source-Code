#ifndef lint
#pragma ident "@(#)pfgallocatesvcquery.c 1.5 96/06/17 SMI"
#endif

/*
 * Copyright (c) 1994-1996 by Sun MicroSystems, Inc.
 * All rights reserved.
 */

/*
 * Module:	pfgallocatesvcquery.c
 * Group:	installtool
 * Description:
 */

#include "pf.h"
#include "pfg.h"
#include "pfgAllocateSvcQuery_ui.h"

static void allocatesvcContinueCB(Widget, XtPointer, XtPointer);
static void allocatesvcGobackCB(Widget, XtPointer, XtPointer);
static void allocatesvcAllocateCB(Widget, XtPointer, XtPointer);

Widget
pfgCreateAllocateSvcQuery(void)
{
	Widget allocatesvc_dialog;
	WidgetList widget_list;
	Widget default_button;

	allocatesvc_dialog = tu_allocatesvc_dialog_widget("allocatesvc_dialog",
		pfgTopLevel, &widget_list);

	XmAddWMProtocolCallback(pfgShell(allocatesvc_dialog), pfgWMDeleteAtom,
	    (XtCallbackProc) pfgExit, NULL);

	XtVaSetValues(pfgShell(allocatesvc_dialog),
		XmNtitle, TITLE_ALLOCATE_SVC_QUERY,
		XmNdeleteResponse, XmDO_NOTHING,
		NULL);
	xm_SetNoResize(pfgTopLevel, allocatesvc_dialog);

	pfgSetWidgetString(widget_list, "panelhelpText",
		MSG_ALLOCATE_SVC_QUERY);
	pfgSetWidgetString(widget_list, "allocateButton", PFG_ALLOCATE);
	pfgSetStandardButtonStrings(widget_list,
		ButtonContinue, ButtonGoback, ButtonExit, ButtonHelp,
		NULL);

	switch (get_machinetype()) {
	case MT_STANDALONE:
		default_button = pfgGetNamedWidget(widget_list,
			"continueButton");
		break;
	case MT_SERVER:
		default_button = pfgGetNamedWidget(widget_list,
			"allocateButton");
		break;
	default:
		default_button = pfgGetNamedWidget(widget_list,
			"continueButton");
		break;
	}

	XtAddCallback(pfgGetNamedWidget(widget_list, "continueButton"),
		XmNactivateCallback, allocatesvcContinueCB, NULL);
	XtAddCallback(pfgGetNamedWidget(widget_list, "gobackButton"),
		XmNactivateCallback, allocatesvcGobackCB, NULL);
	XtAddCallback(pfgGetNamedWidget(widget_list, "allocateButton"),
		XmNactivateCallback, allocatesvcAllocateCB, NULL);

	/* set up the default button */
	XtVaSetValues(pfgGetNamedWidget(widget_list, "messageBox"),
		XmNdefaultButton, default_button,
		NULL);

	XtManageChild(allocatesvc_dialog);

	free(widget_list);

	return (allocatesvc_dialog);
}

/* ARGSUSED */
void
allocatesvcContinueCB(Widget continueButton, XtPointer client_data,
	XtPointer call_data)
{
	setSystemType(MT_STANDALONE);
	pfgSetAction(parAContinue);
}

/* ARGSUSED */
void
allocatesvcAllocateCB(Widget continueButton, XtPointer client_data,
	XtPointer call_data)
{
	setSystemType(MT_SERVER);
	pfgSetAction(parAAllocateSvc);
}

/* ARGSUSED */
void
allocatesvcGobackCB(Widget w, XtPointer client, XtPointer cbs)
{
	pfgSetAction(parAGoback);
}
