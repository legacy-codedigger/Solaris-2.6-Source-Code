#ifndef lint
#pragma ident "@(#)pfgprogressbar.c 1.3 96/07/08 SMI"
#endif

/*
 * Copyright (c) 1994-1996 by Sun MicroSystems, Inc.
 * All rights reserved.
 */

/*
 * Module:	pfgprogressbar.c
 * Group:	installtool
 * Description:
 */

#include <unistd.h>

#include "pfg.h"
#include "pfgprogressbar.h"
#include "pfgProgressBar_ui.h"

Widget
pfgProgressBarCreate(
	UIProgressBarInitData *init_data,
	pfgProgressBarDisplayData **display_data,
	int scale_info_cnt)
{
	WidgetList widget_list;
	Widget progressbar_dialog;
	int	cnt;

	if (scale_info_cnt <= 0)
		cnt = 1;
	else
		cnt = scale_info_cnt;

	*display_data = (pfgProgressBarDisplayData *)
		xcalloc(sizeof (pfgProgressBarDisplayData));
	(*display_data)->scale_info = (UIProgressBarScaleInfo *)
		xcalloc(sizeof (UIProgressBarScaleInfo) * cnt);

	/* get the dialog widget & the dialog widget list from teleuse */
	progressbar_dialog = tu_progressbar_dialog_widget(
		"progressbar_dialog", pfgTopLevel, &widget_list);

	/* get the display data to pass back to the caller */
	(*display_data)->dialog = progressbar_dialog;
	(*display_data)->widget_list = widget_list;
	(*display_data)->scale =
		pfgGetNamedWidget(widget_list, "progressScale");
	(*display_data)->main_label =
		pfgGetNamedWidget(widget_list, "mainLabel");
	(*display_data)->detail_label =
		pfgGetNamedWidget(widget_list, "detailLabel");
	(*display_data)->scale_info[0].start = 0;
	(*display_data)->scale_info[0].factor = 1;

	/* set up exit callback off window manager close */
	XmAddWMProtocolCallback(pfgShell(progressbar_dialog), pfgWMDeleteAtom,
		(XtCallbackProc) pfgExit, NULL);

	/* set title */
	XtVaSetValues(pfgShell(progressbar_dialog),
		XtNtitle, init_data->title ? init_data->title : "",
		XmNdeleteResponse, XmDO_NOTHING,
		NULL);

	pfgSetWidgetString(widget_list, "panelhelpText",
		init_data->main_msg ? init_data->main_msg : "");
	pfgSetWidgetString(widget_list, "mainLabel",
		init_data->main_label ? init_data->main_label : "");
	pfgSetWidgetString(widget_list, "detailLabel",
		init_data->detail_label ? init_data->detail_label : "");

	XmScaleSetValue(pfgGetNamedWidget(widget_list, "progressScale"),
		init_data->percent);

	/* manage the dialog */
	xm_SetNoResize(pfgTopLevel, progressbar_dialog);
	XtManageChild(progressbar_dialog);

	/* update the X display prior to going into the progress phase */
	pfgBusy(pfgShell(progressbar_dialog));
	xm_ForceDisplayUpdate(pfgTopLevel, progressbar_dialog);

	return (progressbar_dialog);
}

void
pfgProgressBarUpdate(pfgProgressBarDisplayData *display_data, int pause)
{
	/* force an update of the display */
	xm_ForceDisplayUpdate(pfgTopLevel, display_data->dialog);
	XmUpdateDisplay(pfgTopLevel);

	if (pause)
		(void) sleep(APP_PROGRESS_PAUSE_TIME);
}

void
pfgProgressBarCleanup(pfgProgressBarDisplayData *display_data)
{

	/*
	 * free the teleuse widget list -
	 * make sure this is done last, after the progress call back
	 * is finished  being called since it uses the widget_list
	 */
	free(display_data->widget_list);
	free(display_data->scale_info);
	free(display_data);

	pfgUnbusy(pfgShell(display_data->dialog));
}
