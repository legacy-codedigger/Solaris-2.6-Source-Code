#ifndef lint
#pragma ident "@(#)pfgintro.c 1.12 96/07/26 SMI"
#endif

/*
 * Copyright (c) 1994-1995 by Sun MicroSystems, Inc.
 * All rights reserved.
 */

/*
 * Module:	pfgintro.c
 * Group:	installtool
 * Description:
 */

#include "pf.h"
#include "pfg.h"

#include "pfgIntro_ui.h"

Widget
pfgCreateIntro(parWin_t win)
{
	Widget dialog;
	WidgetList widget_list;
	char *title;
	char *msg;

	dialog = tu_intro_dialog_widget("intro_dialog",
		pfgTopLevel, &widget_list);
	switch (win) {
	case parIntro:
		title = TITLE_INTRO;
		msg = MSG_INTRO;
		XtAddCallback(pfgGetNamedWidget(widget_list, "helpButton"),
			XmNactivateCallback,
			pfgHelp,
			(XtPointer)"navigate.t");
		break;
	case parIntroInitial:
		title = TITLE_INTRO_INITIAL;
		msg = MSG_INTRO_INITIAL;
		XtAddCallback(pfgGetNamedWidget(widget_list, "helpButton"),
			XmNactivateCallback,
			pfgHelp,
			(XtPointer)"initial.t");
		write_debug(GUI_DEBUG_L1, "intro initial - prev %d",
			history_prev());
		break;
	}

	/*
	 * if there was a window before this one, then put up the go
	 * back button, and o/w skip it.
	 */
	if (history_prev()) {
		XtManageChild(pfgGetNamedWidget(widget_list,
			"gobackButton"));
		pfgSetStandardButtonStrings(widget_list, ButtonGoback, NULL);
	} else {
		XtUnmanageChild(pfgGetNamedWidget(widget_list,
			"gobackButton"));
	}

	/* normal buttons */
	pfgSetStandardButtonStrings(widget_list,
		ButtonContinue, ButtonExit, ButtonHelp, NULL);

	XmAddWMProtocolCallback(pfgShell(dialog), pfgWMDeleteAtom,
		(XtCallbackProc) pfgExit, NULL);

	XtVaSetValues(pfgShell(dialog),
		"mwmDecorations", MWM_DECOR_BORDER | MWM_DECOR_TITLE,
		XmNtitle, title,
		XmNdeleteResponse, XmDO_NOTHING,
		NULL);
	xm_SetNoResize(pfgTopLevel, pfgShell(dialog));

	pfgSetWidgetString(widget_list, "panelhelpText", msg);

	XtManageChild(dialog);

	(void) XmProcessTraversal(
		pfgGetNamedWidget(widget_list, "continueButton"),
		XmTRAVERSE_CURRENT);

	free(widget_list);
	return (dialog);
}

/* ARGSUSED */
void
introContinueCB(Widget w, XtPointer clientD, XtPointer callD)
{
	/*
	 * MMT 093094 Add pfgBusy wrapper around callback functions
	 * since this action can be time consuming, the user is
	 * shown the stopwatch cursor to indicate that the
	 * application is busy
	 */

	/*
	 * note - this window gets destroyed, do I don't worry about
	 * turning the busy cursor off - DT
	 */
	pfgBusy(pfgShell(w));

	pfgSetAction(parAContinue);
}

/* ARGSUSED */
void
introGobackCB(Widget w, XtPointer clientD, XtPointer callD)
{
	pfgSetAction(parAGoback);
}
