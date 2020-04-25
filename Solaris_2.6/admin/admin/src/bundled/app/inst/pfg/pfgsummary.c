#ifndef lint
#pragma ident "@(#)pfgsummary.c 1.70 96/07/30 SMI"
#endif

/*
 * Copyright (c) 1994-1996 by Sun MicroSystems, Inc.
 * All rights reserved.
 */

/*
 * Module:	pfgsummary.c
 * Group:	installtool
 * Description:
 */

#include "pf.h"
#include "pfg.h"
#include "pfgSummary_ui.h"

#define	ERR_MSG_STRLEN	200

static XmString gen_summary(void);
Widget		summary_dialog;
Widget		error_dialog;
WidgetList	error_widget_list;
WidgetList	widget_list;

/* create and populate summary screen */
Widget
pfgCreateSummary(void)
{
	Dimension width, height;
	XmString xmstr;
	char *helpString;

	summary_dialog = tu_summary_dialog_widget("summary_dialog",
		pfgTopLevel, &widget_list);

	XmAddWMProtocolCallback(pfgShell(summary_dialog), pfgWMDeleteAtom,
		(XtCallbackProc) pfgExit, NULL);

	XtVaSetValues(pfgShell(summary_dialog),
		XmNtitle, TITLE_PROFILE,
		XmNdeleteResponse, XmDO_NOTHING,
		NULL);

	/* summary text */
	xmstr = gen_summary();
	XtVaSetValues(pfgGetNamedWidget(widget_list, "summaryLabel"),
		XmNlabelString, xmstr,
		NULL);
	XmStringFree(xmstr);

	/* main message text */
	helpString = (char *) xmalloc(
		strlen(MSG_SUMMARY) +
		strlen(MSG_SUMMARY_CLIENT_SERVICES) +
		strlen(BOOTOBJ_SUMMARY_NOTE) + 1);

	/* add client services note, if applicable */
	if (get_machinetype() == MT_SERVER) {
		(void) sprintf(helpString, "%s%s",
		MSG_SUMMARY,
		MSG_SUMMARY_CLIENT_SERVICES);
	} else {
		(void) sprintf(helpString, "%s", MSG_SUMMARY);
	}

	/* add BIOS note, if applicable */
	if (IsIsa("i386") &&
		(BootobjCompare(CFG_EXIST, CFG_CURRENT, 0) != D_OK))
		(void) sprintf(helpString, "%s%s",
			helpString, BOOTOBJ_SUMMARY_NOTE);

	pfgSetWidgetString(widget_list, "panelhelpText", helpString);

	if (pfgState & AppState_UPGRADE)
		pfgSetWidgetString(widget_list, "continueButton",
			PFG_SM_BEGIN_UP);
	else
		pfgSetWidgetString(widget_list, "continueButton",
			PFG_SM_BEGIN);

	pfgSetWidgetString(widget_list, "profileLabel", PFG_SM_SUMMARY);

	pfgSetStandardButtonStrings(widget_list,
		ButtonChange, ButtonExit, ButtonHelp, NULL);

	XtManageChild(summary_dialog);

	(void) XmProcessTraversal(
		pfgGetNamedWidget(widget_list, "continueButton"),
		XmTRAVERSE_CURRENT);

	XtVaGetValues(pfgShell(summary_dialog),
		XmNwidth, &width,
		XmNheight, &height,
		NULL);

	XtVaSetValues(pfgShell(summary_dialog),
		XmNminWidth, width,
		XmNminHeight, height,
		NULL);

	return (summary_dialog);
}

/* callbacks */

/* ARGSUSED */
void
summaryContinueCB(Widget w, XtPointer client, XtPointer callD)
{
	int		num_errors;
	int		error_found, warning_found;
	Errmsg_t	*error_list;
	char		error_message[ERR_MSG_STRLEN];
	char		*buf;

	/*
	 * check the state of the disks before continuing
	 * it is possible that the user configured the disks
	 * such that the install will fail or the system
	 * will not be bootable after an install, the user must
	 * select Change at this time and "fix" the disks
	 */

	if (pfgState & AppState_UPGRADE) {
		/*
		 * don't do check_disks() in upgrade.
		 * In particular, in upgrade, it is OK to have duplicate
		 * mount pnts on the same disk (e.g. 2 '/''s on the
		 * same disk.
		 */
		num_errors = 0;
	} else {
		num_errors = check_disks();
		write_debug(GUI_DEBUG_L1,
			"%d errors and/or warnings found by check_disks().",
			num_errors);
	}
	if (num_errors > 0) {
		/*
		 * some combination of errors and/or warnings was found
		 * walk the list and find out if there are any errors,
		 * if there are only warnings the user will be allowed
		 * to procede, if there is an error and/or warning an
		 * error dialog will be popped up
		 */
		error_found = 0;
		warning_found = 0;
		buf = xmalloc((int)(ERR_MSG_STRLEN * num_errors));
		*buf = '\0';
		WALK_LIST(error_list, get_error_list()) {
			if (error_list->code < 0) {
				error_found = 1;
				(void) sprintf(error_message, PFG_ERROR,
							error_list->msg);
				(void) strcat(buf, error_message);
				(void) strcat(buf, "\n");
			} else if (error_list->code > 0) {
				warning_found = 1;
				(void) sprintf(error_message, PFG_WARNING,
							error_list->msg);
				(void) strcat(buf, error_message);
				(void) strcat(buf, "\n");
			}
		}

		free_error_list();

		if (error_found == 1 || warning_found == 1) {
			/*
			 * pop up the error message ,
			 * the user will have to select Change to fix
			 * the disk state, if there are only warnings
			 * the error window will be displayed one time
			 * for the user, if they choose Begin
			 * Installation/Upgrade it means they have accepted
			 * the warnings and warning_already_issued will be set,
			 * the warning will not be shown again and the
			 * installation process will continue
			 */

			error_dialog = tu_scrolled_error_window_widget(
				"error_dialog", summary_dialog,
				&error_widget_list);
			XtVaSetValues(pfgShell(error_dialog),
				XmNtitle, TITLE_WARNING,
				XmNdeleteResponse, XmDO_NOTHING,
				NULL);
			pfgSetWidgetString(error_widget_list,
				"panelhelpText",
				PFG_ER_DISKERROR);
			pfgSetWidgetString(error_widget_list,
				"errorOkbutton",
				PFG_OKAY);
			pfgSetWidgetString(error_widget_list,
				"errorCancelbutton",
				PFG_CANCEL);
			pfgSetWidgetString(error_widget_list,
				"scrolledText",
				buf);
			XtManageChild(error_dialog);
			free(buf);
		}


	} else {
		/*
		 * no errors were found at all, just continue
		 *
		 * set the busy cursor ON
		 * w is the pushbutton widget from which the callback
		 * was initiated, we want the dialog shell for
		 * that widget, this is the button labeled either Begin Upgrade
		 * or Begin Installation
		 */
		pfgBusy(pfgShell(w));
		pfgSetAction(parAContinue);
		free(widget_list);
	}

}

/* ARGSUSED */
void
summaryChangeCB(Widget w, XtPointer client, XtPointer callD)
{
	pfgSetAction(parAChange);
}

/* ARGSUSED */
void
diskcheckerrorOkCB(Widget w, XtPointer client, XtPointer callD)
{
	pfgBusy(pfgShell(w));
	XtUnmanageChild(error_dialog);
	pfgSetAction(parAContinue);
	free(error_widget_list);
}

/* ARGSUSED */
void
diskcheckerrorCancelCB(Widget w, XtPointer client, XtPointer callD)
{
	XtUnmanageChild(error_dialog);
	free(error_widget_list);
}


/* XmStringConcat takes two strings and mallocs a third */
static XmString
gen_summary(void)
{
	int i, adds = 0, cans = 0;
	XmString xm1, xm2, xm3;
	pfSw_t *sw;
	Remote_FS *rem, *remotefs;
	char *str;
	char sub[1000], buf[1000];
	struct disk *d;
	Arch *arch;
	Module *module, *mod, *product;
	int languages = False;
	char	name[32];
	char	part_char;
	int	index;
	DsrSLListExtraData *LLextra;

	MachineType machineType;

	write_debug(GUI_DEBUG_L1, "Entering gen_summary");

	(void) sprintf(buf, PFG_SM_BASE);

	xm1 = XmStringCreateLtoR(buf, XmFONTLIST_DEFAULT_TAG);

	/*
	 * install option
	 */
	if (pfgState & AppState_UPGRADE) {
		if (pfgState & AppState_UPGRADE_DSR)
			str = INSTALL_TYPE_UPGRADE_DSR_STR;
		else
			str = INSTALL_TYPE_UPGRADE_STR;
	} else {
		str = INSTALL_TYPE_INITIAL_STR;
	}

	(void) sprintf(buf, PFG_SM_ITYPE, str);
	xm2 = XmStringCreateLtoR(buf, XmFONTLIST_DEFAULT_TAG);
	xm3 = XmStringConcat(xm1, xm2);
	XmStringFree(xm1);
	XmStringFree(xm2);
	xm1 = xm3;

	/*
	 * Boot device
	 */
	BootobjGetAttribute(CFG_CURRENT,
		BOOTOBJ_DISK, name,
		BOOTOBJ_DEVICE_TYPE, &part_char,
		BOOTOBJ_DEVICE, &index,
		NULL);

	if (part_char == 'p' || index == -1)
		(void) sprintf(buf, PFG_SM_BOOTDEVICE_PPC, name);
	else if (index != -1 && part_char == 's')
		(void) sprintf(buf, PFG_SM_BOOTDEVICE, name, part_char, index);

	xm2 = XmStringCreateLtoR(buf, XmFONTLIST_DEFAULT_TAG);
	xm3 = XmStringConcat(xm1, xm2);
	XmStringFree(xm1);
	XmStringFree(xm2);
	xm1 = xm3;

	/*
	 * - disk to be upgraded
	 * - backup media to use
	 */
	if (pfgState & AppState_UPGRADE) {
		/* upgrade disk */
		(void) sprintf(buf, "%s\n  %s %s\n",
			APP_SUMMARY_UPG_TARGET,
			(SliceGetSelected(UpgradeSlices, NULL))->release,
			(SliceGetSelected(UpgradeSlices, NULL))->slice);
		xm2 = XmStringCreateLtoR(buf, XmFONTLIST_DEFAULT_TAG);
		xm3 = XmStringConcat(xm1, xm2);
		XmStringFree(xm1);
		XmStringFree(xm2);
		xm1 = xm3;

	}

	/* DSR upgrade media */
	if (pfgState & AppState_UPGRADE_DSR) {
		(void) LLGetSuppliedListData(DsrSLHandle, NULL,
			(TLLData *)&LLextra);
		(void) sprintf(buf, "%s\n  %s: %s\n",
			APP_SUMMARY_DSR_BACKUP_MEDIA,
			DsrALMediaTypeStr(LLextra->history.media_type),
			LLextra->history.media_device);

		xm2 = XmStringCreateLtoR(buf, XmFONTLIST_DEFAULT_TAG);
		xm3 = XmStringConcat(xm1, xm2);
		XmStringFree(xm1);
		XmStringFree(xm2);
		xm1 = xm3;
	}

	/* display machine type */
	if (!(pfgState & AppState_UPGRADE)) {
		machineType = get_machinetype();
		switch (machineType) {
		case MT_STANDALONE:
		default:
			(void) sprintf(buf, PFG_SM_NO_CLIENT_SERVICES);
			break;
		case MT_SERVER:
			(void) sprintf(buf, PFG_SM_CLIENT_SERVICES);

			(void) sprintf(sub, PFG_SM_NUMCLIENTS, getNumClients());
			(void) strcat(buf, sub);
			(void) sprintf(sub, PFG_SM_SWAP_PER_CLIENT,
				getSwapPerClient());
			(void) strcat(buf, sub);
			(void) sprintf(sub, PFG_SM_ROOT_PER_CLIENT,
				getRootPerClient());
			(void) strcat(buf, sub);
			(void) strcat(buf, PFG_SM_ARCH);
			for (arch = get_all_arches((Module *) NULL); arch;
				arch = arch->a_next) {
				if (arch->a_selected == SELECTED) {
					(void) sprintf(sub, "         %s\n",
					    arch->a_arch);
					(void) strcat(buf, sub);
				}
			}
			break;
		}
		xm2 = XmStringCreateLtoR(buf, XmFONTLIST_DEFAULT_TAG);
		xm3 = XmStringConcat(xm1, xm2);
		XmStringFree(xm1);
		XmStringFree(xm2);
		xm1 = xm3;
	}
	buf[0] = '\0';

	/* check if any locales are selected */
	for (mod = get_all_locales(); mod; mod = get_next(mod)) {
		if (mod->info.locale->l_selected == SELECTED) {
			languages = True;
			break;
		}
	}
	if (languages) {

		xm2 = XmStringCreateLtoR(PFG_SM_LANGS,
			XmFONTLIST_DEFAULT_TAG);
		xm3 = XmStringConcat(xm1, xm2);
		XmStringFree(xm1);
		XmStringFree(xm2);
		xm1 = xm3;

		for (mod = get_all_locales(); mod; mod = get_next(mod)) {
			if (mod->info.locale->l_selected == SELECTED) {
				(void) sprintf(buf, "   %s\n",
					mod->info.locale->l_language);
				xm2 = XmStringCreateLtoR(buf,
					XmFONTLIST_DEFAULT_TAG);
				xm3 = XmStringConcat(xm1, xm2);
				XmStringFree(xm1);
				XmStringFree(xm2);
				xm1 = xm3;
			}
		}
	}
	if (pfgState & AppState_UPGRADE) {
		module = (Module *)get_local_metacluster();
	} else {
		module = pfGetCurrentMeta();
	}
	if (module) {
		product = (Module *)get_current_product();
		(void) sprintf(buf, PFG_SM_SWBASE,
			product->info.prod->p_name,
			product->info.prod->p_version,
			module->info.mod->m_name);
		xm2 = XmStringCreateLtoR(buf, XmFONTLIST_DEFAULT_TAG);
		xm3 = XmStringConcat(xm1, xm2);
		XmStringFree(xm1);
		XmStringFree(xm2);
		xm1 = xm3;
	}
	/* any sw adds and cancellations? */
	for (sw = pfGetClusterList(); sw; sw = sw->next)
		sw->delta ? ++adds : ++cans;
	for (sw = pfGetPackageList(); sw; sw = sw->next)
		sw->delta ? ++adds : ++cans;

	if (adds) {		/* could be in block above */
		xm2 = XmStringCreateLtoR(PFG_SM_INCL, XmFONTLIST_DEFAULT_TAG);
		xm3 = XmStringConcat(xm1, xm2);
		XmStringFree(xm1);
		XmStringFree(xm2);
		xm1 = xm3;

		for (sw = pfGetClusterList(); sw; sw = sw->next) {
			if (!sw->delta)
				continue;
			(void) sprintf(buf, "         %s\n",
				pfClustername(sw->name));
			xm2 = XmStringCreateLtoR(buf, XmFONTLIST_DEFAULT_TAG);
			xm3 = XmStringConcat(xm1, xm2);
			XmStringFree(xm1);
			XmStringFree(xm2);
			xm1 = xm3;
		}
		for (sw = pfGetPackageList(); sw; sw = sw->next) {
			if (!sw->delta)
				continue;
			(void) sprintf(buf, "         %s\n",
				pfPackagename(sw->name));
			xm2 = XmStringCreateLtoR(buf, XmFONTLIST_DEFAULT_TAG);
			xm3 = XmStringConcat(xm1, xm2);
			XmStringFree(xm1);
			XmStringFree(xm2);
			xm1 = xm3;
		}
	}
	if (cans) {		/* could be in block above */
		xm2 = XmStringCreateLtoR(PFG_SM_EXCL, XmFONTLIST_DEFAULT_TAG);
		xm3 = XmStringConcat(xm1, xm2);
		XmStringFree(xm1);
		XmStringFree(xm2);
		xm1 = xm3;

		for (sw = pfGetClusterList(); sw; sw = sw->next) {
			if (sw->delta)
				continue;
			(void) sprintf(buf, "         %s\n",
				pfClustername(sw->name));
			xm2 = XmStringCreateLtoR(buf, XmFONTLIST_DEFAULT_TAG);
			xm3 = XmStringConcat(xm1, xm2);
			XmStringFree(xm1);
			XmStringFree(xm2);
			xm1 = xm3;
		}
		for (sw = pfGetPackageList(); sw; sw = sw->next) {
			if (sw->delta)
				continue;
			(void) sprintf(buf, "         %s\n",
				pfPackagename(sw->name));
			xm2 = XmStringCreateLtoR(buf, XmFONTLIST_DEFAULT_TAG);
			xm3 = XmStringConcat(xm1, xm2);
			XmStringFree(xm1);
			XmStringFree(xm2);
			xm1 = xm3;
		}
	}

	/*
	 * File System and Disk Layout summary
	 * - displayed in DSR upgrade and initial
	 */
	if ((pfgState & AppState_UPGRADE_DSR) ||
		!(pfgState & AppState_UPGRADE)) {
		xm2 = XmStringCreateLtoR(APP_SUMMARY_FSLAYOUT,
			XmFONTLIST_DEFAULT_TAG);
		xm3 = XmStringConcat(xm1, xm2);
		XmStringFree(xm1);
		XmStringFree(xm2);
		xm1 = xm3;

		set_units(D_MBYTE);
		WALK_DISK_LIST(d) {
			if (!disk_selected(d))
				continue;

			for (i = 0; i <= LAST_STDSLICE; ++i) {
				if (!slice_size(d, i))
					continue;
				(void) sprintf(buf,
					"  %-*.*s %ss%d %5d MB %s\n",
					UI_FS_DISPLAY_LENGTH,
					UI_FS_DISPLAY_LENGTH,
					slice_mntpnt(d, i),
					disk_name(d), i,
					(int) blocks2size(d, slice_size(d, i),
						ROUNDDOWN),
					(slice_preserved(d, i) ?
						PFG_PRESERVED : ""));

				xm2 = XmStringCreateLtoR(buf,
					XmFONTLIST_DEFAULT_TAG);
				xm3 = XmStringConcat(xm1, xm2);
				XmStringFree(xm1);
				XmStringFree(xm2);
				xm1 = xm3;
			}
		}
	}

	remotefs = pfGetRemoteFS();
	if (remotefs) {
		xm2 = XmStringCreateLtoR(PFG_SM_RFS, XmFONTLIST_DEFAULT_TAG);
		xm3 = XmStringConcat(xm1, xm2);
		XmStringFree(xm1);
		XmStringFree(xm2);
		xm1 = xm3;

		for (rem = remotefs; rem; rem = rem->c_next) {
			(void) sprintf(buf, "  %s %s:%s\n",
				rem->c_mnt_pt,
				rem->c_hostname,
				rem->c_export_path);
			xm2 = XmStringCreateLtoR(buf, XmFONTLIST_DEFAULT_TAG);
			xm3 = XmStringConcat(xm1, xm2);
			XmStringFree(xm1);
			XmStringFree(xm2);
			xm1 = xm3;
		}
	}
	return (xm1);
}
