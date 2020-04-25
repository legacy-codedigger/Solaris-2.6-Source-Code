#ifndef lint
#pragma ident "@(#)pfglayout.c 1.7 96/04/29 SMI"
#endif

/*
 * Copyright (c) 1994-1995 by Sun MicroSystems, Inc.
 * All rights reserved.
 */

/*
 * Module:	pfglayout.c
 * Group:	installtool
 * Description:
 */

#include "pf.h"
#include "pfg.h"
#include "pfgLayout_ui.h"
WidgetList	widget_list;
WidgetList	entry_widget_list;
WidgetList	new_entry_widget_list;
static Widget	layout_dialog;
static Widget	table_row_column;
static Widget	layout_table_entry[25];
static Widget	partitions[25];
int		num_table_entries;
Dimension	spacing;
static Widget	options_widgets[25];
static Widget	disk_choice[25];
static Profile  profile;
char		*selected_dev, *dev_name, *dev_size, *dev_mntpt, *dev_next;

void	line_selected(Widget, XtPointer, XtPointer);
void	create_new_table_entry(Widget, XtPointer, XtPointer);
void	createLayoutTableEntries(Widget,  Defmnt_t **);
void	createNewTableEntry(Widget, int);
void	alignColumnHeadings(WidgetList);
void	set_to_max_partname_width(void);
void	get_disk_and_slice_lists(WidgetList);
void	set_disk_choice(Widget, XtPointer, XtPointer);
void	set_disk_choice_any(Widget, XtPointer, XtPointer);
void	set_slice_choice(Widget, XtPointer, XtPointer);

Widget
pfgCreateLayout(void)
{
	Defmnt_t	**MountList;

	/* used to restore data on cancel */
/* 	Defmnt_t	**origMountList; */
	Widget		temp_widget;

	DISKPARTITIONING(&profile) = LAYOUT_DEFAULT;

	/*
	 * call the generated function tu_initial_client_query to
	 * create the screen
	 */
	layout_dialog =  tu_autoLayoutDialog_widget("layout_dialog",
				pfgTopLevel, &widget_list);

	XmAddWMProtocolCallback(pfgShell(layout_dialog), pfgWMDeleteAtom,
		(XtCallbackProc)pfgExit, NULL);

	XtVaSetValues(pfgShell(layout_dialog),
		"mwmDecorations", MWM_DECOR_BORDER | MWM_DECOR_TITLE,
		XmNdeleteResponse, XmDO_NOTHING,
		NULL);

	xm_SetNoResize(pfgTopLevel, pfgShell(layout_dialog));

/* 	origMountList = get_dfltmnt_list(NULL); */
	(void) get_dfltmnt_list(NULL);

	/*
	 * reset the mount list to the default for the machine type
	 * selected, set the preservice slice(s) to SELECTED
	 */

	pfgResetDefaults();
	MountList = get_dfltmnt_list(NULL);

	/* get the widget id of the main row column */
	table_row_column = pfgGetNamedWidget(widget_list, "main_table_rc");

	/* create the table entries as children of the row column */
	createLayoutTableEntries(table_row_column, MountList);

	set_to_max_partname_width();

	/* set the XmNlabelStrings for the layout_dialog pushbuttons */
	pfgSetWidgetString(widget_list, "autoLayoutContinue", PFG_CONTINUE);
	pfgSetWidgetString(widget_list, "autoLayoutCancel", PFG_CANCEL);
	pfgSetWidgetString(widget_list, "autoLayoutExit", PFG_EXIT);
	pfgSetWidgetString(widget_list, "autoLayoutHelp", PFG_HELP);
	pfgSetWidgetString(widget_list, "basic_button", PFG_AUTO_BASIC);
	pfgSetWidgetString(widget_list, "options_button", PFG_AUTO_OPTIONS);

	/* manage the basic button */
	temp_widget = pfgGetNamedWidget(widget_list, "basic_button");
	XtManageChild(temp_widget);


	/* set the XmNvalue for the layout_dialog text string */
	pfgSetWidgetString(widget_list, "panelhelpText", MSG_AUTOLAYOUT);

	/* set the labels for the column headings */
	pfgSetWidgetString(widget_list, "slice_name_label", PFG_SLICENAME);
	pfgSetWidgetString(widget_list, "rec_size_label", PFG_RECSIZE);
	pfgSetWidgetString(widget_list, "create_size_label", PFG_CREATESIZE);
	pfgSetWidgetString(widget_list, "disks_label", PFG_DISKLABEL);
	pfgSetWidgetString(widget_list, "slices_label", PFG_SLICELABEL);


	alignColumnHeadings(widget_list);

	XtManageChild(layout_dialog);

	return (layout_dialog);
}

void
get_disk_and_slice_lists(WidgetList widget_list)
{
	Disk_t	*disk_ptr;
	Widget	parent;
	XmString	name_string;
	Widget		temp_widget;
	int		count;
	char		*base_string;
	char		slice_string[14];
	int		index;


	base_string = "slice_choice";
	parent = pfgGetNamedWidget(widget_list, "disk_option_list");
	index = 1;
	for (disk_ptr = first_disk(); disk_ptr;
		disk_ptr = next_disk(disk_ptr)) {
		if (disk_not_okay(disk_ptr))
			continue;
		if (disk_selected(disk_ptr)) {
			name_string =
				XmStringCreateLocalized(disk_name(disk_ptr));
			disk_choice[index] =
				XtVaCreateManagedWidget("disk_choice",
					xmPushButtonWidgetClass,
					parent,
					XmNlabelString, name_string,
					NULL);

			XtAddCallback(disk_choice[index], XmNactivateCallback,
						set_disk_choice, index);
			index++;
		}

		/* set the locked status for each of the slices */
		for (count = 0; count <= LAST_STDSLICE; count++) {
			if (slice_locked(disk_ptr, count)) {
				(void) sprintf(slice_string, "%s%d",
					base_string, count);
				temp_widget = pfgGetNamedWidget(widget_list,
							slice_string);
				XtSetSensitive(temp_widget, False);

			} else {
				/* the slice is not locked */
				if (debug)
					(void) printf("slice %d not locked\n",
					count);

			}

		}
	}

}

/* ARGSUSED */
void
set_disk_choice(Widget w, XtPointer which_disk, XtPointer callD)
{
	int	disk_index;

	/*
	 * which_disk is the index into the disk_choice array, zero
	 * is the ANY disk choice, otherwise we know which disk from
	 * the global list disk_choice
	 */

	disk_index = (int)which_disk;

	if (debug) {
		(void) printf("the selected disk is disk_choice%d\n",
			disk_index);
	}

}

/* ARGSUSED */
void
set_disk_choice_any(Widget w, XtPointer clientD, XtPointer callD)
{
	int disk_index;

	/*
	 * this is the 0th item in the disk list, set disk_choice[0]
	 * to this widget id
	 */

	disk_index = 0;

	/* the widget that activated this callback */
	disk_choice[disk_index] = w;

	if (debug) {
		(void) printf("the selected disk is disk_choice%d\n",
			disk_index);
	}
}

/* ARGSUSED */
void
set_slice_choice(Widget w, XtPointer which_slice, XtPointer callD)
{
	/* the slice name, as a char*, is passed in as client data */
	if (debug) {
		(void) printf("the selected slice is slice_choice%s\n",
			(char *)which_slice);
	}
}

void
set_to_max_partname_width(void)
{
	int	count;
	Dimension	width;
	Dimension	max_width;

	width = 0;
	max_width = 0;

	for (count = 0; count <= num_table_entries; count++) {
		if (partitions[count] != 0) {
			XtVaGetValues(partitions[count],
				XmNwidth, &width,
				NULL);
			if (width >= max_width)
				max_width = width;
		}
	}

	max_width = max_width + spacing;

	/* set the width of all widget in partitions[] to max_width */
	for (count = 0; count <= num_table_entries; count++) {
		if (partitions[count] != 0) {
			XtVaSetValues(partitions[count],
				XmNwidth, max_width,
				NULL);
		}
	}
}

void
alignColumnHeadings(WidgetList widgets)
{
	Widget label_widget;
	Widget temp_widget;
	Dimension width;
	Dimension spacing;
	Dimension margin_width;
	Dimension widget_offset;
	Dimension option_width;

	temp_widget = pfgGetNamedWidget(widgets, "layout_dialog");
	XtVaGetValues(temp_widget,
		XmNmarginWidth, &margin_width,
		NULL);

	temp_widget = pfgGetNamedWidget(entry_widget_list, "basic_table_entry");
	XtVaGetValues(temp_widget,
		XmNhorizontalSpacing, &spacing,
		NULL);

	temp_widget = pfgGetNamedWidget(entry_widget_list, "select_slice_name");
	XtVaGetValues(temp_widget,
		XmNwidth, &width,
		NULL);

	widget_offset = margin_width + spacing;
	label_widget = pfgGetNamedWidget(widgets, "slice_name_label");
	XtVaSetValues(label_widget,
		XmNwidth, width,
		XmNleftOffset, widget_offset,
		NULL);

	temp_widget = pfgGetNamedWidget(entry_widget_list, "recommended_size");
	XtVaGetValues(temp_widget,
		XmNwidth, &width,
		NULL);

	label_widget = pfgGetNamedWidget(widgets, "rec_size_label");
	XtVaSetValues(label_widget,
		XmNwidth, width,
		XmNleftOffset, spacing,
		NULL);

	temp_widget = pfgGetNamedWidget(entry_widget_list, "create_size");
	XtVaGetValues(temp_widget,
		XmNwidth, &width,
		NULL);

	label_widget = pfgGetNamedWidget(widgets, "create_size_label");
	XtVaSetValues(label_widget,
		XmNwidth, width,
		XmNleftOffset, spacing,
		NULL);

	temp_widget = pfgGetNamedWidget(entry_widget_list, "disk_option");
	XtVaGetValues(temp_widget,
		XmNwidth, &option_width,
		NULL);

	temp_widget = pfgGetNamedWidget(entry_widget_list, "disk_option_list");
	XtVaGetValues(temp_widget,
		XmNwidth, &width,
		NULL);

	label_widget = pfgGetNamedWidget(widgets, "disks_label");
	XtVaSetValues(label_widget,
		XmNwidth, width + option_width,
		XmNleftOffset, widget_offset,
		NULL);

	temp_widget = pfgGetNamedWidget(entry_widget_list, "slice_option");
	XtVaGetValues(temp_widget,
		XmNwidth, &option_width,
		NULL);

	temp_widget = pfgGetNamedWidget(entry_widget_list, "slice_option_list");
	XtVaGetValues(temp_widget,
		XmNwidth, &width,
		NULL);

	label_widget = pfgGetNamedWidget(widgets, "slices_label");
	XtVaSetValues(label_widget,
		XmNwidth, width + option_width,
		XmNleftOffset, widget_offset,
		NULL);

}

/* ARGSUSED */
void
createLayoutTableEntries(Widget table_widget, Defmnt_t **mountList)
{
	XmString	toggle_label;
	Widget		line_toggle;
	int		sensitivity;
	char		*mnt_name;
	int		count;
	char		*size_auto;
	Widget		partition_name;
	Widget		options_form;
	int		partition_size;
	char		partition_size_string[6];
	int		mb_size;

	for (count = 0; mountList[count] != NULL; count++) {
		if (mountList[count]->status == DFLT_IGNORE &&
				mountList[count]->allowed == 0) {
			partitions[count] = 0;
			options_widgets[count] = 0;
			continue;
		}
		toggle_label = XmStringCreateLocalized(mountList[count]->name);
		partition_size = get_default_fs_size(mountList[count]->name,
			NULL, DONTROLLUP);
		mb_size = sectors_to_mb(partition_size);
		(void) sprintf(partition_size_string, "%d", mb_size);
		mnt_name = xstrdup(mountList[count]->name);
		sensitivity = mountList[count]->status == DFLT_SELECT &&
				mountList[count]->allowed == 0 ? False : True;
		layout_table_entry[count] =
			tu_full_table_entry_widget("layout_entry",
				table_row_column, &entry_widget_list);
		size_auto = "AUTO";
		pfgSetWidgetString(entry_widget_list, "create_size", size_auto);
		pfgSetWidgetString(entry_widget_list, "recommended_size",
						partition_size_string);
		pfgSetWidgetString(entry_widget_list, "any_disk",
			PFG_ANYSTRING);
		pfgSetWidgetString(entry_widget_list, "any_slice",
			PFG_ANYSTRING);
		/* set up the labels for the basic slice lists */
		pfgSetWidgetString(entry_widget_list, "slice_choice0", "0");
		pfgSetWidgetString(entry_widget_list, "slice_choice1", "1");
		pfgSetWidgetString(entry_widget_list, "slice_choice2", "2");
		pfgSetWidgetString(entry_widget_list, "slice_choice3", "3");
		pfgSetWidgetString(entry_widget_list, "slice_choice4", "4");
		pfgSetWidgetString(entry_widget_list, "slice_choice5", "5");
		pfgSetWidgetString(entry_widget_list, "slice_choice6", "6");
		pfgSetWidgetString(entry_widget_list, "slice_choice7", "7");
		line_toggle = pfgGetNamedWidget(entry_widget_list,
			"select_slice_name");
		XtVaGetValues(line_toggle,
			XmNspacing, &spacing,
			NULL);
		XtVaSetValues(line_toggle,
			XmNset, mountList[count]->status == DFLT_SELECT ?
				True : False,
			XmNlabelString, toggle_label,
			XmNuserData, mnt_name,
			XmNsensitive, sensitivity,
			NULL);

		XtAddCallback(line_toggle, XmNvalueChangedCallback,
				line_selected, mountList);

		XmStringFree(toggle_label);

		options_form = pfgGetNamedWidget(entry_widget_list,
					"options_table_entry");
		options_widgets[count] = options_form;
		get_disk_and_slice_lists(entry_widget_list);

		/* get the widget id of the partition name entry */
		partition_name = pfgGetNamedWidget(entry_widget_list,
						"select_slice_name");
		/* fill in the array with the widgets */
		partitions[count] = partition_name;
		num_table_entries = count;
	}

	/* create the next entry, this is an editable entry */
	createNewTableEntry(table_row_column, num_table_entries);
}

void
createNewTableEntry(Widget parent, int num_entries)
{
	Widget	edit_partition;
	Widget  option_form;

	layout_table_entry[num_entries + 1] =
		tu_full_editable_entry_widget("new_entry",
			parent, &new_entry_widget_list);
	pfgSetWidgetString(new_entry_widget_list, "any_disk", PFG_ANYSTRING);
	pfgSetWidgetString(new_entry_widget_list, "any_slice", PFG_ANYSTRING);
	/* set up the labels for the basic slice lists */
	pfgSetWidgetString(new_entry_widget_list, "slice_choice0", "0");
	pfgSetWidgetString(new_entry_widget_list, "slice_choice1", "1");
	pfgSetWidgetString(new_entry_widget_list, "slice_choice2", "2");
	pfgSetWidgetString(new_entry_widget_list, "slice_choice3", "3");
	pfgSetWidgetString(new_entry_widget_list, "slice_choice4", "4");
	pfgSetWidgetString(new_entry_widget_list, "slice_choice5", "5");
	pfgSetWidgetString(new_entry_widget_list, "slice_choice6", "6");
	pfgSetWidgetString(new_entry_widget_list, "slice_choice7", "7");

	/* get the widget id of the partition name form */
	edit_partition =
		pfgGetNamedWidget(new_entry_widget_list, "blank_name_entry");
	option_form =
		pfgGetNamedWidget(new_entry_widget_list, "options_table_entry");
	options_widgets[num_entries + 1] = option_form;

	/* add this to the partitions widget array */
	partitions[num_entries + 1] = edit_partition;

	/* update the total num items in the list */
	num_table_entries = num_entries + 1;

	get_disk_and_slice_lists(new_entry_widget_list);
}

/* ARGSUSED */
void
create_new_table_entry(
	Widget new_filesys_name, XtPointer clientD, XtPointer callD)
{
	char	*new_name = NULL;

	new_name = XmTextFieldGetString(new_filesys_name);

	if (new_name != NULL) {
		createNewTableEntry(table_row_column, num_table_entries);
	}
}

/* ARGSUSED */
void
line_selected(Widget toggle_button, XtPointer clientD, XtPointer callD)
{
	XmToggleButtonCallbackStruct	*toggle_state;

	char		*mntpt_name;
	Defmnt_t	mountList;
	int		error_ret;

	/* LINTED [pointer cast] */
	toggle_state = (XmToggleButtonCallbackStruct *) callD;

	XtVaGetValues(toggle_button,
		XmNuserData, &mntpt_name,
		NULL);

	if (mntpt_name != NULL) {
		if (toggle_state->set) {
			get_dfltmnt_ent(&mountList, mntpt_name);
			mountList.status = DFLT_SELECT;
			error_ret = set_dfltmnt_ent(&mountList, mntpt_name);
			if (error_ret != D_OK) {
				pfgWarning(layout_dialog, pfErUNSUPPORTEDFS);
			}
		} else {
			get_dfltmnt_ent(&mountList, mntpt_name);
			if (mountList.status == DFLT_SELECT &&
				mountList.allowed == 0) {
				pfgWarning(layout_dialog, pfErREQUIREDFS);
				return;
			}
			if (mntpt_name[0] == '/') {
				mountList.status = DFLT_DONTCARE;
			} else {
				mountList.status = DFLT_IGNORE;
			}

			error_ret = set_dfltmnt_ent(&mountList, mntpt_name);
			if (error_ret != D_OK) {
				pfgWarning(layout_dialog, pfErREQUIREDFS);
			}
		}
	}
}

/* callback function definitions */
/* ARGSUSED */
void
set_show_basic(Widget w, XtPointer clientD, XtPointer callD)
{
	Widget basic_widget;
	int	count;


	if ((strcmp(XtName(w), "options_button")) == 0) {
		/* if  options then set to basic */
		XtUnmanageChild(w);
		basic_widget = pfgGetNamedWidget(widget_list, "basic_button");
		XtManageChild(basic_widget);

		for (count = 0; count <= num_table_entries; count++) {
			if (options_widgets[count] != 0) {
				XtUnmanageChild(options_widgets[count]);
			}
		}
	}

	/* get the width of the scrolled window */
	/*
	 * if options then manage the options fields, set the
	 * scrolled window to its wide width,
	 * if basic then unmanage the options fields, and set
	 * the width to its small width
	 */

}

/* ARGSUSED */
void
set_show_options(Widget w, XtPointer clientD, XtPointer callD)
{
	Widget option_widget;
	int	count;


	if ((strcmp(XtName(w), "basic_button")) == 0) {
		/* if basic then set to options */
		XtUnmanageChild(w);
		option_widget = pfgGetNamedWidget(widget_list,
			"options_button");
		XtManageChild(option_widget);

		for (count = 0; count <= num_table_entries; count++) {
			if (options_widgets[count] != 0) {
				XtManageChild(options_widgets[count]);
			}
		}
	}
}

/* ARGSUSED */
void
layoutContinueCB(Widget w, XtPointer clientD, XtPointer callD)
{
	int error_ret;

	pfgNullDisks();

	/*
	 * SET ALL THE FIELDS IN THE PROFILE STRUCTURE
	 */

	/* set the busy cursor since this can take a while */
	/* pfgBusy(pfgShell(w)); */
	/* set up the default partitioning of the disk */
	error_ret = pfgInitializeDisks();
	if (error_ret == -1) {
		/* error occurred during auto layout */
		if (pfgQuery(layout_dialog, pfQAUTOFAIL) == False) {
			pfgNullDisks();
			return;
		} else {
			pfgNullDisks();
		}

	}

	pfgBuildLayoutArray();
	free(entry_widget_list);

	/* unmanage the window and its popup shell */
	XtUnmanageChild(pfgShell(w));

	/* destroy the widget and shell */
	XtDestroyWidget(pfgShell(w));

	pfgSetAction(parAContinue);
}

/* ARGSUSED */
void
layoutCancelCB(Widget w, XtPointer origMountList, XtPointer callD)
{
	int error_ret;

	/* turn off the busy cursor if it was on */
	/* pfgBusy((Widget)NULL); */

	/* unmanage the window and its popup shell */
	XtUnmanageChild(pfgShell(w));

	/* destroy the widget and shell */
	XtDestroyWidget(pfgShell(w));

	/* LINTED [pointer cast] */
	error_ret = set_dfltmnt_list((Defmnt_t **) origMountList);
	if (error_ret != D_OK) {
		pfgDiskError(w, "set_dfltmnt_list", error_ret);
	}
}

/*
 * the exit and help callbacks are globally defined for
 * all windows so they don't need to be defined here
 */
