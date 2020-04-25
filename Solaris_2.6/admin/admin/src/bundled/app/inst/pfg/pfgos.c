#ifndef lint
#pragma ident "@(#)pfgos.c 1.30 96/10/07 SMI"
#endif

/*
 * Copyright (c) 1994-1996 by Sun MicroSystems, Inc.
 * All rights reserved.
 */

/*
 * Module:	pfgos.c
 * Group:	installtool
 * Description:
 */

#include "pf.h"
#include "pfg.h"

#include "pfgOs_ui.h"

void osContinueCB(Widget, XtPointer, XtPointer);
void osGobackCB(Widget, XtPointer, XtPointer);

static Widget os_dialog;
static Widget *slice_toggle_array;
static WidgetList *slice_toggle_array_widgetlist;
static int num_slices;
static char *old_slice = NULL;

static void
os_change_toggle_states(Widget w, XtPointer clientD,
	XmToggleButtonCallbackStruct *state);


/*
 * Function: pfgCreateOs
 * Description:
 *	Create the upgrade window that presents the multiple
 *	slices that we know about that can be upgraded and let the
 *	user pick which one to upgrade.
 * Scope:	public
 * Parameters:  none
 * Return:	[Widget] - the dialog widget
 * Globals:
 * Notes:
 *	- the window is set up so that it can resize vertically, but
 *	  not horizontally, so that the slice list can be expanded
 *	  downward if tehre are lots of them.
 */
Widget
pfgCreateOs(void)
{
	WidgetList widget_list;
	XmString labelString;
	Widget osSlicesForm;
	Widget slice_toggle;
	Widget slice_label;
	int i;
	Dimension max_width;
	Dimension width;
	Dimension height;

	/* get the dialog widget & the dialog widget list from teleuse */
	os_dialog = tu_os_dialog_widget("os_dialog",
		pfgTopLevel, &widget_list);

	/* set up exit callback off window manager close */
	XmAddWMProtocolCallback(pfgShell(os_dialog), pfgWMDeleteAtom,
		(XtCallbackProc) pfgExit, NULL);

	/* set title */
	XtVaSetValues(pfgShell(os_dialog),
		XtNtitle, TITLE_OS_MULTIPLE,
		XmNdeleteResponse, XmDO_NOTHING,
		NULL);

	/* get form we will put slice list into */
	osSlicesForm = pfgGetNamedWidget(widget_list, "osSlicesForm");

	/* how many upgradeable disks are there? */
	num_slices = SliceGetTotalNumUpgradeable(UpgradeSlices);

	/* make the first slice selected if one isn't already selected */
	SliceSelectOne(UpgradeSlices);

	/* make an array to hold the slice list of toggles */
	slice_toggle_array = (Widget *) xmalloc(sizeof (Widget) * num_slices);
	slice_toggle_array_widgetlist = (WidgetList *)
		xmalloc(sizeof (WidgetList) * num_slices);

	/*
	 * Loop through slices and set up a list of slice forms.
	 * Each slice form has a toggle button with the Solaris OS (release)
	 * name and a corresponding label with the slice name this
	 * OS's root file system is on.
	 */
	for (i = 0, max_width = 0; i < num_slices; i++) {
		/* create the string to assign to the release toggle */
		labelString = XmStringCreateLocalized(UpgradeSlices[i].release);

		/* get the slice toggle and label widget from teleuse */
		slice_toggle_array[i] =  tu_osSliceForm_widget("osSliceForm",
			osSlicesForm,
			&slice_toggle_array_widgetlist[i]);
		slice_toggle =
			pfgGetNamedWidget(slice_toggle_array_widgetlist[i],
			"osSliceToggle");
		slice_label =
			pfgGetNamedWidget(slice_toggle_array_widgetlist[i],
			"osSliceNameLabel");

		/*
		 * if this is the first slice form, attach it to the
		 * surrounding form on top.
		 * otherwise, attach it to the slice form above it.
		 */
		if (i == 0) {
			XtVaSetValues(slice_toggle_array[i],
				XmNtopAttachment, XmATTACH_FORM,
				NULL);
		} else {
			XtVaSetValues(slice_toggle_array[i],
				XmNtopAttachment, XmATTACH_WIDGET,
				XmNtopWidget, slice_toggle_array[i-1],
				NULL);
		}

		/* set the label on slice toggle and select it if appropriate */
		XtVaSetValues(slice_toggle,
			XmNlabelString, labelString,
			XmNuserData, &UpgradeSlices[i],
			XmNset, UpgradeSlices[i].selected ? True : False,
			NULL);

		/* find widest toggle width so far */
		XtVaGetValues(slice_toggle,
			XmNwidth, &width,
			XmNheight, &height,
			NULL);
		if (width > max_width)
			max_width = width;

		/* add toggle callbacks */
		XtAddCallback(slice_toggle,
			XmNvalueChangedCallback,
			(XtCallbackProc) os_change_toggle_states,
			(XtPointer) NULL);

		/* set label on disk slice label */
		XtVaSetValues(slice_label,
			XmNlabelString,
				XmStringCreateLocalized(
					UpgradeSlices[i].slice),
			XmNheight, height,
			NULL);

		XmStringFree(labelString);

		if (UpgradeSlices[i].failed)
			XtSetSensitive(slice_toggle_array[i], False);

	}

	/* set all the toggles to be the width of the widest one */
	for (i = 0; i < num_slices; i++) {
		XtVaSetValues(
			pfgGetNamedWidget(slice_toggle_array_widgetlist[i],
			"osSliceToggle"),
			XmNwidth, max_width,
			NULL);
	}

	pfgSetWidgetString(widget_list, "panelhelpText", MSG_OS);
	pfgSetWidgetString(widget_list, "osVersionLabel", OS_VERSION_LABEL);
	pfgSetWidgetString(widget_list, "osSliceLabel", LABEL_SLICE);
	pfgSetStandardButtonStrings(widget_list,
		ButtonContinue, ButtonGoback, ButtonExit, ButtonHelp,
		NULL);

	XtAddCallback(pfgGetNamedWidget(widget_list, "continueButton"),
		XmNactivateCallback, osContinueCB, NULL);

	XtManageChild(os_dialog);

	(void) XmProcessTraversal(
		pfgGetNamedWidget(widget_list, "continueButton"),
		XmTRAVERSE_CURRENT);

	free(widget_list);

	XtVaGetValues(pfgShell(os_dialog),
	    XmNwidth, &width,
	    XmNheight, &height,
	    NULL);

	XtVaSetValues(pfgShell(os_dialog),
	    XmNminWidth, width,
	    XmNmaxWidth, width,
	    XmNminHeight, height,
	    NULL);

	SlicePrintDebugInfo(UpgradeSlices);
	return (os_dialog);
}

/* ARGSUSED */
static void
os_change_toggle_states(Widget w, XtPointer clientD,
	XmToggleButtonCallbackStruct *state)
{
	Widget		toggle;
	int		i;

	/* make sure all other toggles are set to false */
	for (i = 0; i < num_slices; i++) {
		toggle = pfgGetNamedWidget(slice_toggle_array_widgetlist[i],
			"osSliceToggle");
		if (toggle != w) {
			XmToggleButtonSetState(toggle, False, False);
		}
	}
}

/* ARGSUSED */
void
osGobackCB(Widget w, XtPointer client, XtPointer cbs)
{
	pfgBusy(pfgShell(os_dialog));
	pfgSetAction(parAGoback);
}

/* ARGSUSED */
void
osContinueCB(Widget button, XtPointer client_data, XtPointer call_data)
{
	Widget toggle;
	Widget osSliceForm;
	int i;
	UpgOs_t *new_slice;
	parAction_t action;
	TChildAction status;

	/* unselect the previous slice */
	SliceSetUnselected(UpgradeSlices);

	/* find the newly selected slice */
	for (i = 0, new_slice = NULL; i < num_slices; i++) {
		toggle = pfgGetNamedWidget(slice_toggle_array_widgetlist[i],
			"osSliceToggle");

		if (XmToggleButtonGetState(toggle)) {
			osSliceForm = pfgGetNamedWidget(
				slice_toggle_array_widgetlist[i],
				"osSliceForm");
			new_slice = &UpgradeSlices[i];
			new_slice->selected = 1;
			break;
		}
	}

	if (!new_slice) {
		/* they haven't selected a slice */
		pfgWarning(os_dialog, pfErNOUPGRADEDISK);
	} else {
		pfgBusy(pfgShell(os_dialog));

		/*
		 * initialize sw lib, etc. with the new slice to
		 * upgrade.
		 */
		status = AppParentStartUpgrade(
			&FsSpaceInfo,
			UpgradeSlices,
			&pfgState,
			pfgExit,
			pfgParentReinit,
			(void *) &pfgParentReinitData);

		if (pfgState & AppState_UPGRADE_CHILD) {
			pfgSetAction(parAContinue);
			return;
		} else {
			/* we're in the parent */

			if (status != ChildUpgSliceFailure) {
				/*
				 * Anything but a ChildUpgSliceFailure means
				 * we're either ok and should continue or
				 * we're hosed and can't try any more slices
				 * and should just exit or go into an
				 * initial install.
				 */
				action = AppParentContinueUpgrade(
					status, &pfgState, pfgCleanExit);

				/*
				 * If we haven't exitted yet, then
				 * the selected slice to upgrade is OK
				 * and we can proceed with the upgrade,
				 * or we want to go onto the initial
				 * path instead.
				 */

				if (old_slice)
					free(old_slice);
				old_slice = xstrdup(new_slice->slice);

#if 0
				pfgUnbusy(pfgShell(os_dialog));
#endif
				pfgSetAction(action);
				return;
			}

			/*
			 * A ChildUpgSliceFailure means this slice failed and
			 * there are more possible slices to try and that
			 * we should try another.
			 * Set the currently selected slice insensitive
			 * and select another one for them.
			 */

			/* make unsuccessful insensitive */
			XmToggleButtonSetState(
				toggle, False, False);
			XtSetSensitive(osSliceForm, False);

			/* pick another slice */
			SliceSelectOne(UpgradeSlices);

			/* update the newly selected toggle */
			(void) SliceGetSelected(UpgradeSlices, &i);
			toggle = pfgGetNamedWidget(
				slice_toggle_array_widgetlist[i],
				"osSliceToggle");
			XmToggleButtonSetState(
				toggle, True, False);

			/* drop back to the screen */
			pfgUnbusy(pfgShell(os_dialog));
			return;
		}
	}
}
