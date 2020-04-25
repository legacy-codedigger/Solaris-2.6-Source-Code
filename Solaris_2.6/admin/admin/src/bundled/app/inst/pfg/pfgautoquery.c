#ifndef lint
#pragma ident "@(#)pfgautoquery.c 1.39 96/07/08 SMI"
#endif

/*
 * Copyright (c) 1994-1996 by Sun MicroSystems, Inc.
 * All rights reserved.
 */

/*
 * Module:	pfgautoquery.c
 * Group:	installtool
 * Description:
 */

#include "pf.h"
#include "pfg.h"

#include "pfgAutoquery_ui.h"

static void autoqueryAutoCB(Widget, XtPointer, XtPointer);
static void autoqueryManualCB(Widget, XtPointer, XtPointer);

static Widget autoquery_dialog;
static int first = True;

Widget
pfgCreateAutoQuery(void)
{
	WidgetList widget_list;

	autoquery_dialog = tu_autoquery_dialog_widget("AutoQuery",
						pfgTopLevel, &widget_list);

	XmAddWMProtocolCallback(pfgShell(autoquery_dialog), pfgWMDeleteAtom,
	    (XtCallbackProc) pfgExit, NULL);

	XtVaSetValues(pfgShell(autoquery_dialog),
	    XmNtitle, TITLE_AUTOLAYOUTQRY,
	    XmNdeleteResponse, XmDO_NOTHING,
	    NULL);
	xm_SetNoResize(pfgTopLevel, autoquery_dialog);

	XtAddCallback(
		pfgGetNamedWidget(widget_list, "continueButton"),
		XmNactivateCallback, autoqueryAutoCB, NULL);
	XtAddCallback(
		pfgGetNamedWidget(widget_list, "customizeButton"),
		XmNactivateCallback, autoqueryManualCB, NULL);
	pfgSetWidgetString(widget_list, "panelhelpText",
		MSG_AUTOLAYOUTQRY);
	pfgSetWidgetString(widget_list, "continueButton",
		PFG_AQ_AUTOLAY);
	pfgSetWidgetString(widget_list, "gobackButton", PFG_GOBACK);
	pfgSetWidgetString(widget_list, "customizeButton",
		PFG_AQ_MANLAY);
	pfgSetWidgetString(widget_list, "exitButton", PFG_EXIT);
	pfgSetWidgetString(widget_list, "helpButton", PFG_HELP);

	free(widget_list);

	XtManageChild(autoquery_dialog);

	return (autoquery_dialog);
}

/*
 * This function is called when the user selects the manual layout
 * button.  The user is only given one chance at a manual layout the
 * the first time through the screen.
 */

/* ARGSUSED */
static void
autoqueryManualCB(Widget w, XtPointer clientD, XtPointer callD)
{
	if (first) {
		/* null out all nonpreserved disk slices */
		pfgNullDisks();
		/* set default mount list for manual layout mode */
		pfgSetManualDefaultMounts();
	} else {
		pfgCompareLayout(); /* null out disks not in previous layout */
	}
	first = False;
	/* build layout array */
	pfgBuildLayoutArray();
	pfgSetAction(parAContinue);

}

/*
 * This function is called during a second pass thru the screen
 * when the user chooses continue.
 */

/* ARGSUSED */
static void
autoqueryAutoCB(Widget button, XtPointer clientD, XtPointer callD)
{
	first = False;

	pfgBusy(pfgShell(button));

	(void) pfgCreateAutoLayout(pfgShell(autoquery_dialog));

	/* this is the new auto layout screen */
/* 	pfgCreateLayout(pfgShell(autoquery_dialog)); */
}


/* ARGSUSED */
void
autoqueryGobackCB(Widget w, XtPointer clientD, XtPointer callD)
{
	pfgSetAction(parAGoback);
}
