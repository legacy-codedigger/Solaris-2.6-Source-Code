#ifndef lint
#pragma ident "@(#)pfgprequery.c 1.36 96/04/29 SMI"
#endif

/*
 * Copyright (c) 1994-1995 by Sun MicroSystems, Inc.
 * All rights reserved.
 */

/*
 * Module:	pfgprequery.c
 * Group:	installtool
 * Description:
 */

#include "pf.h"
#include "pfg.h"
#include "pfglocales.h"

#include "pfgPrequery_ui.h"

static Widget prequery_dialog;

Widget
pfgCreatePreQuery(void)
{
	WidgetList widget_list;

	prequery_dialog = tu_prequery_dialog_widget("prequery_dialog",
		pfgTopLevel, &widget_list);

	XmAddWMProtocolCallback(pfgShell(prequery_dialog), pfgWMDeleteAtom,
	    (XtCallbackProc) pfgExit, NULL);

	XtVaSetValues(pfgShell(prequery_dialog),
	    XmNtitle, TITLE_PREQUERY,
	    XmNdeleteResponse, XmDO_NOTHING,
	    NULL);
	xm_SetNoResize(pfgTopLevel, prequery_dialog);

	pfgSetWidgetString(widget_list, "panelhelpText", MSG_PREQUERY);
	pfgSetWidgetString(widget_list, "continueButton", PFG_CONTINUE);
	pfgSetWidgetString(widget_list, "gobackButton", PFG_GOBACK);
	pfgSetWidgetString(widget_list, "preserveButton", PFG_PQ_PRESERVE);
	pfgSetWidgetString(widget_list, "exitButton", PFG_EXIT);
	pfgSetWidgetString(widget_list, "helpButton", PFG_HELP);

	free(widget_list);

	XtManageChild(prequery_dialog);

	return (prequery_dialog);
}


/* ARGSUSED */
void
prequeryPreserveCB(Widget w, XtPointer clientD, XtPointer callD)
{
	pfgBusy(pfgShell(w));

	(void) pfgCreatePreserve(pfgShell(prequery_dialog));
}


/* ARGSUSED */
void
prequeryContinueCB(Widget w, XtPointer clientD, XtPointer callD)
{
	pfgSetAction(parAContinue);
}


/* ARGSUSED */
void
prequeryGobackCB(Widget w, XtPointer clientD, XtPointer callD)
{
	pfgSetAction(parAGoback);
}
