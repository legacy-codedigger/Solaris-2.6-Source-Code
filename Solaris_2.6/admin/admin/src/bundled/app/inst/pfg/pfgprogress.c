#ifndef lint
#pragma ident "@(#)pfgprogress.c 1.34 96/07/31 SMI"
#endif

/*
 * Copyright (c) 1994-1996 by Sun MicroSystems, Inc.
 * All rights reserved.
 */

/*
 * Module:	pfgprogress.c
 * Group:	installtool
 * Description:
 *	This is the upgrade progress bar display module for the
 *	initial install path.
 */

#include <unistd.h>
#include <Xm/Scale.h>

#include "pfg.h"
#include "pfgProgress_ui.h"

/*
 * client data structure that is passed to the SystemUpdate backend
 * for the initial path
 * and is then passed back into all the update callbacks.
 */
typedef struct {
	Widget	toplevel;
	Widget	dialog;
	WidgetList widget_list;
	int totalK;
	int doneK;
} pfgSUInitialData;
static pfgSUInitialData *pfgSUData = NULL;

/*
 * static functions
 */
static int
pfgSystemUpdateInitialCB(void *client_data, void *call_data);

static void
SUInitialUpdateBeginCB(pfgSUInitialData *pfg_su_data);

static void
SUInitialUpdateEndCB(pfgSUInitialData *pfg_su_data);

static void
SUInitialPkgAddBeginCB(pfgSUInitialData *pfg_su_data, char *pkgdir);

static void
SUInitialPkgAddEndCB(pfgSUInitialData *pfg_su_data, char *pkgdir);


/*
 * Function:	pfgCreateProgress
 * Description:
 *	Creation routine; creates the initial install path progress display
 * Scope:	<PRIVATE|INTERNAL|PUBLIC>
 * Parameters:	none
 * Return: none
 * Globals:
 *	Allocates and initializes static global
 *	(pfgSUInitialData *pfgSUData) for application data passed to
 *	backend.
 * Notes:
 */
void
pfgCreateProgress(void)
{
	write_debug(GUI_DEBUG_L1, "Entering pfgCreateProgress");

	/* create dialog widget */
	pfgSUData = (pfgSUInitialData *)
		xcalloc(sizeof (pfgSUInitialData));
	pfgSUData->toplevel = pfgTopLevel;
	pfgSUData->dialog = tu_progress_dialog_widget("progress_dialog",
		pfgSUData->toplevel, &pfgSUData->widget_list);

	XmAddWMProtocolCallback(pfgShell(pfgSUData->dialog), pfgWMDeleteAtom,
		(XtCallbackProc) pfgExit, NULL);

	XtVaSetValues(pfgShell(pfgSUData->dialog),
		XmNtitle, TITLE_PROGRESS,
		XmNdeleteResponse, XmDO_NOTHING,
		NULL);
	xm_SetNoResize(pfgSUData->toplevel, pfgSUData->dialog);

	pfgSetWidgetString(pfgSUData->widget_list, "panelhelpText",
		MSG_PROGRESS);
	pfgSetWidgetString(pfgSUData->widget_list, "installLabel",
		LABEL_PROGRESS_PARTITIONING);
	pfgSetWidgetString(pfgSUData->widget_list, "packageLabel", " ");

	XtManageChild(pfgSUData->dialog);

	pfgBusy(pfgShell(pfgSUData->dialog));

	xm_ForceEventUpdate(pfgAppContext, pfgSUData->toplevel);
}

/*
 * Function:	pfgSystemUpdateInitial
 * Description:
 *	Initial install path System update wrapper...
 * Scope:	PUBLIC
 * Parameters:	none
 * Return:
 * Globals:
 *	pfgSUInitialData *pfgSUData
 *		Application data to be passed to the backend.
 *		This data should have been alloced and initialized in the
 *		pfgCreateProgress call above.
 * Notes:
 */
parAction_t
pfgSystemUpdateInitial(void)
{
	TSUData su_data;
	TSUError ret;

	write_debug(GUI_DEBUG_L1, "Entering pfgSystemUpdate");

	if (!pfgSUData)
		return (parAExit);

	su_data.Operation = SI_INITIAL_INSTALL;
	su_data.Info.Initial.prod = get_current_product();
	su_data.Info.Initial.cfs = pfGetRemoteFS();
	su_data.Info.Initial.SoftUpdateCallback = pfgSystemUpdateInitialCB;
	su_data.Info.Initial.ApplicationData = (void *) pfgSUData;

	ret = SystemUpdate(&su_data);
	if (ret == SUSuccess) {
		/* the installation was successful, continue */
		return (parAContinue);
	} else {
		/* the installation failed, exit */
		pfAppError(NULL, SUGetErrorText(ret));
		return (parAExit);
	}
}

/*
 * Function:	pfgSystemUpdateInitialCB
 * Description:
 *	Main top level SystemUpdate callback for the initial install path.
 * Scope:	INTERNAL
 * Parameters:
 *	void *client_data
 *		application data
 *	void *call_data
 *		SystemUpdate provided data
 * Return:
 *	SUCCESS
 *	FAILURE
 * Globals:	None
 * Notes:
 */
static int
pfgSystemUpdateInitialCB(void *client_data, void *call_data)
{
	pfgSUInitialData *pfg_su_data =
		(pfgSUInitialData *) client_data;
	TSoftUpdateStateData *cb_data =
		(TSoftUpdateStateData *) call_data;

	if (!pfg_su_data || !cb_data)
		return (FAILURE);

	write_debug(GUI_DEBUG_L1, "SU Initial: State = %d", cb_data->State);

	switch (cb_data->State) {
	case SoftUpdateBegin:
		SUInitialUpdateBeginCB(pfg_su_data);
		break;
	case SoftUpdateEnd:
		SUInitialUpdateEndCB(pfg_su_data);
		break;
	case SoftUpdatePkgAddBegin:
		SUInitialPkgAddBeginCB(
			pfg_su_data,
			cb_data->Data.PkgAddBegin.PkgDir);
		break;
	case SoftUpdatePkgAddEnd:
		SUInitialPkgAddEndCB(
			pfg_su_data,
			cb_data->Data.PkgAddEnd.PkgDir);
		break;
	case SoftUpdateInteractivePkgAdd:
		write_debug(GUI_DEBUG_L1, "SoftUpdateInteractivePkgAdd");
		break;
	default:
		return (FAILURE);
	}

	return (SUCCESS);
}

static void
SUInitialUpdateBeginCB(pfgSUInitialData *pfg_su_data)
{
	pfgSetWidgetString(pfg_su_data->widget_list,
		"installLabel", LABEL_PROGRESS_INSTALL);

	XtSetSensitive(pfgGetNamedWidget(pfg_su_data->widget_list,
		"progressScale"), True);
	pfg_su_data->totalK = get_total_kb_to_install();
}

static void
SUInitialUpdateEndCB(pfgSUInitialData *pfg_su_data)
{
	Display * dis = XtDisplay(pfg_su_data->toplevel);

	pfgSetWidgetString(pfg_su_data->widget_list, "installLabel",
		LABEL_PROGRESS_COMPLETE);
	pfgSetWidgetString(pfg_su_data->widget_list, "packageLabel", " ");
	XmScaleSetValue(pfgGetNamedWidget(pfg_su_data->widget_list,
		"progressScale"), 100);

	XBell(dis, 50);
	xm_ForceDisplayUpdate(pfg_su_data->toplevel, pfg_su_data->dialog);
	(void) sleep(APP_PROGRESS_PAUSE_TIME);
	XtUnmanageChild(pfg_su_data->dialog);

	free(pfg_su_data);
	pfgSUData = NULL;
}

static void
SUInitialPkgAddBeginCB(pfgSUInitialData *pfg_su_data, char *pkgdir)
{
	char *id;

	id = pkgid_from_pkgdir(pkgdir);

	write_debug(GUI_DEBUG_L1, "SUInitialPkgAddBeginCB (%s=%s)", pkgdir, id);

	if (! *id)
		return;

	pfgSetWidgetString(pfg_su_data->widget_list, "packageLabel",
		pfPackagename(id));

	pfg_su_data->doneK += get_size_in_kbytes(id);

	xm_ForceDisplayUpdate(pfg_su_data->toplevel, pfg_su_data->dialog);
}

static void
SUInitialPkgAddEndCB(pfgSUInitialData *pfg_su_data, char *pkgdir)
{
	int val;
	write_debug(GUI_DEBUG_L1, "SUInitialPkgAddEndCB: (%s)", pkgdir);

	val = pfg_su_data->doneK * 100 / pfg_su_data->totalK;
	if (val > 100)
		val = 100;
	else if (val < 0)
		val = 0;

	XmScaleSetValue(pfgGetNamedWidget(pfg_su_data->widget_list,
		"progressScale"), val);

	xm_ForceDisplayUpdate(pfg_su_data->toplevel, pfg_su_data->dialog);
	if (GetSimulation(SIM_EXECUTE))
		(void) sleep(1);
}
