#ifndef lint
#pragma ident "@(#)pfgclients.c 1.34 96/07/26 SMI"
#endif

/*
 * Copyright (c) 1994-1996 by Sun MicroSystems, Inc.
 * All rights reserved.
 */

/*
 * Module:	pfgclients.c
 * Group:	installtool
 * Description:
 */

#include "pf.h"
#include "pfg.h"

#include "pfgClients_ui.h"

static void disklessContinueCB(Widget, XtPointer, XtPointer);
static void disklessToggleCB(Widget, XtPointer, XtPointer);

#define	TEXTLENGTH 5

Widget
pfgCreateClients(void)
{
	Widget diskless_dialog;

	Arch *arch, *ptr;
	char tmp[TEXTLENGTH];
	char *nativeArch;
	Dimension width, height;
	Widget checkBox, toggleButton;
	XmString toggleString;

	WidgetList widget_list;

	diskless_dialog = tu_diskless_dialog_widget("diskless_dialog",
		pfgTopLevel, &widget_list);


	XmAddWMProtocolCallback(pfgShell(diskless_dialog), pfgWMDeleteAtom,
	    (XtCallbackProc) pfgExit, NULL);

	XtVaSetValues(pfgShell(diskless_dialog),
	    XmNtitle, TITLE_CLIENTS,
	    XmNdeleteResponse, XmDO_NOTHING,
	    NULL);

	pfgSetWidgetString(widget_list, "panelhelpText", MSG_CLIENTS);
	pfgSetWidgetString(widget_list, "continueButton", PFG_CONTINUE);
	pfgSetWidgetString(widget_list, "gobackButton", PFG_GOBACK);
	pfgSetWidgetString(widget_list, "exitButton", PFG_EXIT);
	pfgSetWidgetString(widget_list, "helpButton", PFG_HELP);

	pfgSetWidgetString(widget_list, "clientNumberLabel", PFG_CL_CLIENTS);
	(void) sprintf(tmp, "%d", getNumClients());
	pfgSetWidgetString(widget_list, "clientNumberText", tmp);

	pfgSetWidgetString(widget_list, "clientSwapLabel", PFG_CL_SWAP);
	(void) sprintf(tmp, "%d", getSwapPerClient());
	pfgSetWidgetString(widget_list, "clientSwapText", tmp);

	pfgSetWidgetString(widget_list, "architectureLabel", PFG_CL_ARCH);

	checkBox = pfgGetNamedWidget(widget_list, "architectureRowColumn");

	arch = get_all_arches(NULL);
	nativeArch = get_default_arch();
	for (ptr = arch; ptr != NULL; ptr = ptr->a_next) {
		toggleString = XmStringCreateLocalized(ptr->a_arch);
		toggleButton = XtVaCreateManagedWidget("Toggle",
			xmToggleButtonWidgetClass, checkBox,
			XmNlabelString, toggleString,
			XmNset, ptr->a_selected == TRUE ?
				True : False,
			XmNuserData, ptr,
			XmNsensitive, strcmp(ptr->a_arch, nativeArch) ?
				True : False,
			NULL);
		XtAddCallback(toggleButton, XmNvalueChangedCallback,
			disklessToggleCB, NULL);
	}

	XtAddCallback(pfgGetNamedWidget(widget_list, "continueButton"),
		XmNactivateCallback, disklessContinueCB, checkBox);

	XtManageChild(diskless_dialog);
	(void) XmProcessTraversal(
		pfgGetNamedWidget(widget_list, "continueButton"),
		XmTRAVERSE_CURRENT);

	XtVaGetValues(pfgShell(diskless_dialog),
	    XmNwidth, &width,
	    XmNheight, &height,
	    NULL);

	XtVaSetValues(pfgShell(diskless_dialog),
	    XmNminWidth, width,
	    XmNmaxWidth, width,
	    XmNminHeight, height,
	    NULL);

	free(widget_list);
	pfgUnbusy(pfgShell(diskless_dialog));

	return (diskless_dialog);
}

/* ARGSUSED */
void
setSwapCB(Widget swap, XtPointer clientD, XtPointer callD)
{
	char *string;
	int swapSize;

	string = XmTextFieldGetString(swap);

	if (string[0] == '\0') {
		swapSize = 0;
	} else {
		swapSize = atoi(string);
	}
	setSwapPerClient(swapSize);
}

/* ARGSUSED */
void
setNumClientsCB(Widget numClients, XtPointer clientD, XtPointer callD)
{
	char *string;
	int numClientsSize;

	string = XmTextFieldGetString(numClients);
	if (string[0] == '\0') {
		numClientsSize = 0;
	} else {
		numClientsSize = atoi(string);
	}
	setNumClients(numClientsSize);
}

/* ARGSUSED */
static void
disklessToggleCB(Widget toggle, XtPointer client_data,
    XtPointer callD)
{
	XmToggleButtonCallbackStruct *cbs =
		/* LINTED [pointer cast] */
		(XmToggleButtonCallbackStruct *) callD;

	Module *product = get_current_product();
	Arch *arch;

	XtVaGetValues(toggle,
		XmNuserData, &arch,
		NULL);
	if (arch) {
		if (cbs->set == True) {
			select_arch(product, arch->a_arch);
		} else {
			deselect_arch(product, arch->a_arch);
		}
		mark_arch(product);
	}
}



/* ARGSUSED */
static void
disklessContinueCB(Widget w, XtPointer checkBox, XtPointer callD)
{
	int numChildren;
	WidgetList children;
	Module *product;

	XtVaGetValues(checkBox,
		XmNnumChildren, &numChildren,
		XmNchildren, &children,
		NULL);
	product = get_current_product();
	if (product == NULL) {
		(void) fprintf(stderr,
			"pfgClient:disklessContinueCB: Unable to determine product to install");
		pfgCleanExit(EXIT_INSTALL_FAILURE, (void *) NULL);
	}
	mark_arch(product);
	pfgSetAction(parAContinue);
	pfgBusy(pfgShell(w));
}

/* ARGSUSED */
void
disklessGobackCB(Widget w, XtPointer clientD, XtPointer callD)
{
	pfgSetAction(parAGoback);
}
