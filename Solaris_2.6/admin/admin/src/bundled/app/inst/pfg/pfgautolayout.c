#ifndef lint
#pragma ident "@(#)pfgautolayout.c 1.38 96/09/17 SMI"
#endif

/*
 * Copyright (c) 1994-1996 by Sun MicroSystems, Inc.
 * All rights reserved.
 */

/*
 * Module:	pfgautolayout.c
 * Group:	installtool
 * Description:
 */

#include "pf.h"
#include "pfg.h"

#include "pfgAutolayout_ui.h"

static void autolayoutCancelCB(Widget, XtPointer, XtPointer);
static void autolayoutOkCB(Widget, XtPointer, XtPointer);
static void checkCB(Widget, XtPointer, XtPointer);
static void createToggleButtons(Widget parent, Defmnt_t **);

/*
 * flag that indicates if software library should be
 * initialized/reinitialized
 */
static Widget autolayout_dialog;

Widget
pfgCreateAutoLayout(Widget parent)
{
	Dimension radioWidth;
	Defmnt_t **MountList;
	Defmnt_t **origMountList; /* used to restore if user cancels screen */
	WidgetList widget_list;
	Widget checkBox;

	autolayout_dialog = tu_autolayout_dialog_widget("autolayout_dialog",
		parent, &widget_list);

	XmAddWMProtocolCallback(pfgShell(autolayout_dialog), pfgWMDeleteAtom,
	    (XtCallbackProc) pfgExit, NULL);

	XtVaSetValues(pfgShell(autolayout_dialog),
	    XmNtitle, TITLE_AUTOLAYOUT,
	    XmNdeleteResponse, XmDO_NOTHING,
	    NULL);
	xm_SetNoResize(pfgTopLevel, autolayout_dialog);

	origMountList = get_dfltmnt_list(NULL);

	/*
	 * reset the mount list to the default for the machine type
	 * selected.  Set preserved slice to SELECTED
	 */
	pfgResetDefaults();
	MountList = get_dfltmnt_list(NULL);

	checkBox = pfgGetNamedWidget(widget_list, "autolayoutCheckBox");

	createToggleButtons(checkBox, MountList);

	XtVaGetValues(checkBox,
	    XmNwidth, &radioWidth,
	    NULL);

	write_debug(GUI_DEBUG_L1, "radio width = %d", radioWidth);

	pfgSetWidgetString(widget_list, "panelhelpText", MSG_AUTOLAYOUT);
	pfgSetWidgetString(widget_list, "createLabel", PFG_SY_HEADING);
	pfgSetWidgetString(widget_list, "continueButton", PFG_CONTINUE);
	pfgSetWidgetString(widget_list, "cancelButton", PFG_CANCEL);
	pfgSetWidgetString(widget_list, "helpButton", PFG_HELP);

	XtAddCallback(
		pfgGetNamedWidget(widget_list, "continueButton"),
		XmNactivateCallback, autolayoutOkCB, checkBox);
	XtAddCallback(
		pfgGetNamedWidget(widget_list, "cancelButton"),
		XmNactivateCallback, autolayoutCancelCB, origMountList);


	XtManageChild(autolayout_dialog);

	(void) XmProcessTraversal(
		pfgGetNamedWidget(widget_list, "continueButton"),
		XmTRAVERSE_CURRENT);

	free(widget_list);
	return (autolayout_dialog);
}


/*
 * Function: toggleButton
 *
 * Input:
 *   w - radio button widget who's value changed
 *   checkBox - checkBox widget
 *   state - state information of the radio button
 * Output:
 *   checkBox - the XmNuserData field is set to point to the
 *	currently selected meta cluster
 * Return:
 *  NONE
 * Description:
 *  This function is the callback for the radio buttons.  The module
 *  representing the selected meta cluster is stored in the checkBox
 *  XmNuserData field.
 */

/* ARGSUSED */
void
checkCB(Widget toggleButton, XtPointer clientD, XtPointer callD)
{
	XmToggleButtonCallbackStruct *state =
		/* LINTED [pointer cast] */
		(XmToggleButtonCallbackStruct *) callD;

	char *name;
	Defmnt_t mountList;
	int err;

	XtVaGetValues(toggleButton,
	    XmNuserData, &name,
	    NULL);

	if (name != NULL) {
		if (state->set) {
			get_dfltmnt_ent(&mountList, name);
			mountList.status = DFLT_SELECT;
			err = set_dfltmnt_ent(&mountList, name);
			if (err != D_OK) {
				pfgWarning(autolayout_dialog,
					pfErUNSUPPORTEDFS);
			}
		} else {
			get_dfltmnt_ent(&mountList, name);
			if (mountList.status == DFLT_SELECT &&
			    mountList.allowed == 0) {
				pfgWarning(autolayout_dialog, pfErREQUIREDFS);
				return;
			}
			if (name[0] == '/') {
				mountList.status = DFLT_DONTCARE;
			} else {
				mountList.status = DFLT_IGNORE;
			}
			err = set_dfltmnt_ent(&mountList, name);
			if (err != D_OK) {
				pfgWarning(autolayout_dialog, pfErREQUIREDFS);
			}
		}
	}
}

/* ARGSUSED */
void
autolayoutOkCB(Widget w, XtPointer checkBox, XtPointer callD)
{
	int err;

	pfgBusy(pfgShell(w));

	pfgNullDisks();

	/* sets up default partitioning of disk */
	err = pfgInitializeDisks();
	if (err != D_OK) {
		/* error performing auto layout */
		if (pfgQuery(autolayout_dialog, pfQAUTOFAIL) == False) {
			pfgUnbusy(pfgShell(w));
			return;
		} else {
			pfgNullDisks();
			pfgSetManualDefaultMounts();
		}
	}
	pfgBuildLayoutArray();
	XtUnmanageChild(pfgShell(w));
	XtDestroyWidget(pfgShell(w));
	pfgSetAction(parAContinue);
}


/* ARGSUSED */
void
autolayoutCancelCB(Widget w, XtPointer origMountList, XtPointer callD)
{
	int err;

	pfgUnbusy(pfgShell(XtParent(pfgShell(w))));

	XtUnmanageChild(pfgShell(w));
	XtDestroyWidget(pfgShell(w));
	/* LINTED [pointer cast] */
	err = set_dfltmnt_list((Defmnt_t **) origMountList);
	if (err != D_OK) {
		pfgDiskError(w, "set_dfltmnt_list", err);
	}
}


void
createToggleButtons(Widget parent, Defmnt_t ** mountList)
{
	XmString label;
	Widget toggleButton;
	char *name;
	Dimension radioWidth;
	int i;
	int sensitivity;

	for (i = 0; mountList[i] != NULL; i++) {
		if (mountList[i]->status == DFLT_IGNORE &&
		    mountList[i]->allowed == 0) {
			continue;
		}

		label = XmStringCreateLocalized(mountList[i]->name);
		name = xstrdup(mountList[i]->name);
		sensitivity = mountList[i]->status == DFLT_SELECT &&
			mountList[i]->allowed == 0 ? False : True;
		toggleButton = XtVaCreateManagedWidget("autoToggleButton",
			xmToggleButtonWidgetClass, parent,
			XmNset, mountList[i]->status == DFLT_SELECT ?
				True : False,
			XmNlabelString, label,
			XmNuserData, name,
			XmNsensitive, sensitivity,
			NULL);
		XmStringFree(label);

		XtAddCallback(toggleButton, XmNvalueChangedCallback,
		    checkCB, mountList);
	}

	XtVaGetValues(toggleButton,
	    XmNwidth, &radioWidth,
	    NULL);
	XtVaSetValues(parent,
	    XmNwidth, radioWidth,
	    NULL);
}
