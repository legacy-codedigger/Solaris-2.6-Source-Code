#ifndef lint
#pragma ident "@(#)pfgfilesys.c 1.59 96/10/16 SMI"
#endif

/*
 * Copyright (c) 1994-1996 by Sun MicroSystems, Inc.
 * All rights reserved.
 */

/*
 * Module:	pfgfilesys.c
 * Group:	installtool
 * Description:
 */

#include "pf.h"
#include "pfg.h"

#include "pfgFilesys_ui.h"

/* static functions */

static void popupSpaceWindow(Widget, Space*);
static void pfgSizeScrolledWindowCB(Widget w, XtPointer client, XtPointer callD);

static Widget filesys_dialog;
WidgetList widget_list;
WidgetList space_widget_list;

Widget
pfgCreateFilesys(void)
{
	filesys_dialog = tu_filesys_dialog_widget("filesys_dialog",
		pfgTopLevel, &widget_list);

	XmAddWMProtocolCallback(pfgShell(filesys_dialog), pfgWMDeleteAtom,
		(XtCallbackProc) pfgExit, NULL);

	XtVaSetValues(pfgShell(filesys_dialog),
		XmNtitle, TITLE_FILESYS,
		XmNdeleteResponse, XmDO_NOTHING,
		XmNallowShellResize, True,
		NULL);
	XtAddCallback(pfgShell(filesys_dialog), XmNpopupCallback, 
		pfgSizeScrolledWindowCB, (XtPointer)NULL);

	pfgSetWidgetString(widget_list, "filesysColumnLabel", PFG_FS_FILESYS);
	pfgSetWidgetString(widget_list, "diskColumnLabel", PFG_FS_DISK);
	pfgSetWidgetString(widget_list, "sizeColumnLabel", PFG_FS_SIZE);
	pfgSetWidgetString(widget_list, "optionsColumnLabel", PFG_FS_OPTIONS);

	pfgSetWidgetString(widget_list, "panelhelpText", MSG_FILESYS);
	pfgSetWidgetString(widget_list, "continueButton", PFG_CONTINUE);
	pfgSetWidgetString(widget_list, "gobackButton", PFG_GOBACK);
	pfgSetWidgetString(widget_list, "customizeButton", PFG_CUSTOMIZE);
	pfgSetWidgetString(widget_list, "exitButton", PFG_EXIT);
	pfgSetWidgetString(widget_list, "helpButton", PFG_HELP);

	XtManageChild(filesys_dialog);

	(void) XmProcessTraversal(
		pfgGetNamedWidget(widget_list, "continueButton"),
		XmTRAVERSE_CURRENT);

	return (filesys_dialog);
}

static void
pfgSizeScrolledWindowCB(Widget w, XtPointer client, XtPointer callD)
{
	Dimension width, height;

	pfgUpdateFilesysSummary();
	(void) xm_SizeScrolledWindowToWorkArea(pfgGetNamedWidget(widget_list, 
		"filesysScrolledWindow"), True, False);

        XtVaGetValues(pfgShell(filesys_dialog),
            XmNwidth, &width,
            XmNheight, &height,
            NULL);

        XtVaSetValues(pfgShell(filesys_dialog),
            XmNminWidth, width,
            XmNmaxWidth, width,
            XmNminHeight, height,
            NULL);
}


/* callbacks */

/* ARGSUSED */
void
filesysContinueCB(Widget w, XtPointer client, XtPointer callD)
{
	int	ret;
	char	diskname[32];
	char	part_char;
	int	dev_index;
	Disk_t	*dp;
	int	slice;
	char	root_slicename[32];
	char	root_diskname[32];
	char	buf[32];
	char	diskbuf[32];
	Space	*space_status;
	int	status;


	WALK_DISK_LIST(dp) {
		if (disk_selected(dp)) {
			WALK_SLICES(slice) {
				if (streq(slice_mntpnt(dp, slice), ROOT)) {
					(void) sprintf(root_slicename, "%ss%d",
						disk_name(dp), slice);
					(void) sprintf(root_diskname, "%s",
						disk_name(dp));
				}
			}
		}
	}

	/*
	 * make sure the boot device is set to the disk that contains the root
	 * filesystem
	 */
	WALK_DISK_LIST(dp) {
		if (disk_selected(dp)) {
			WALK_SLICES(slice) {
				if (strcmp(slice_mntpnt(dp, slice), ROOT) == 0) {
					status = SdiskobjRootSetBoot(dp, slice);
					if (status != D_OK) {
						pfgDiskError(filesys_dialog,
							NULL, status);
					}
				}
			}
		}
	}

	/*
	 * get the name of the committed boot device
	 */
	(void) BootobjGetAttribute(CFG_CURRENT,
		BOOTOBJ_DISK, diskname,
		BOOTOBJ_DEVICE, &dev_index,
		BOOTOBJ_DEVICE_TYPE, &part_char,
		NULL);

	if ((strcmp(diskname, "") != 0) && dev_index != -1 && IsIsa("sparc")) {
		(void) sprintf(buf, "%s%c%d", diskname,
			part_char, dev_index);
	} else if ((strcmp(diskname, "") != 0) && dev_index != -1 && IsIsa("ppc")) {
		(void) sprintf(buf, "%ss0", diskname);
	} else if ((strcmp(diskname, "") != 0) && dev_index != -1 && IsIsa("i386")) {
		(void) sprintf(diskbuf, "%s", diskname);
	} else if ((strcmp(diskname, "") != 0) && dev_index == -1) {
		(void) sprintf(diskbuf, "%s", diskname);
	}
	/*
	 * a NULL diskname means we could not read the prom
	 */
	if (strcmp(diskname, "") != 0) {
		if ((strcmp(diskname, root_diskname) != 0) ||
			(!IsIsa("i386") && (strcmp(diskname, root_diskname) == 0)
			&& (strcmp(root_slicename, buf) != 0))) {
					ret = BootobjDiffersQuery(root_slicename);
					if (ret == TRUE) {
						return;
					}

		}
	}


	/*
	 * Make sure each file system has at least the minimum amount
	 * of space we require to lay it out.
	 */
	if (space_status = ResobjIsComplete(RESSIZE_MINIMUM))
		popupSpaceWindow(pfgShell(w), space_status);
	else {
		pfgSetAction(parAContinue);
		free(widget_list);
	}
}

/* ARGSUSED */
void
filesysGobackCB(Widget w, XtPointer client, XtPointer callD)
{
	pfgSetAction(parAGoback);

	free(widget_list);
}

/* ARGSUSED */
void
filesysCustomizeCB(Widget w, XtPointer client, XtPointer callD)
{
	/* set busy cursor on self */
	pfgBusy(pfgShell(w));

	(void) pfgCreateDisks(filesys_dialog);
}

/* ARGSUSED */
void
pfgUpdateFilesysSummary(void)
{
	int 		i;
	char 		buf[100];
	struct disk	*d;
	int		child;
	int		num_children;
	WidgetList	children;
	Widget		rc;
	WidgetList	*entry_list;
	WidgetList	col;
	int             num_entries;
        int             entry;

	write_debug(GUI_DEBUG_L1, "Entering pfgUpdateFilesysSummary");

	rc = pfgGetNamedWidget(widget_list, "filesysRowColumn");

	XtVaGetValues(rc,
		XmNnumChildren, &num_children,
		XmNchildren, &children,
		NULL);

	if (num_children > 0) {
		for (child = 0; child < num_children; child++)
			XtDestroyWidget(children[child]);
	}

	num_entries = 0;
	for (d = first_disk(); d; d = next_disk(d)) {
		if (disk_selected(d)) {
			for (i = 0; i <= LAST_STDSLICE; ++i) { 	
				if (slice_size(d, i))
				    num_entries++;
			}
		}
	}

        entry_list = (WidgetList *)xmalloc(sizeof(WidgetList) * num_entries);
        col = (WidgetList)xmalloc(sizeof(Widget) * (num_entries + 1));
        entry = 0;

	set_units(D_MBYTE);
	for (d = first_disk(); d; d = next_disk(d)) {
		if (disk_selected(d)) {
			for (i = 0; i <= LAST_STDSLICE; ++i) {
				if (!slice_size(d, i))
					continue;

				(void) tu_filesys_entry_widget(
					"filesys_entry", rc,
					&entry_list[entry]);

				(void) sprintf(buf, "     %-14s",
					slice_mntpnt(d, i));
				pfgSetWidgetString(entry_list[entry],
					"filesysFilesysLabel", buf);
				(void) sprintf(buf, "  %ss%d  ",
					disk_name(d), i);
				pfgSetWidgetString(entry_list[entry],
					"filesysDiskLabel", buf);
				(void) sprintf(buf, "  %5d MB  ",
					(int) blocks2size(d, slice_size(d, i),
						ROUNDDOWN));
				pfgSetWidgetString(entry_list[entry],
					"filesysSizeLabel", buf);
				pfgSetWidgetString(entry_list[entry],
					"filesysOptionsLabel",
					(slice_preserved(d, i) ?
						PFG_PRESERVED : ""));
				entry++;
			}
		}
	}

	/*
	 * now, we align the table, based on the widths above.
	 */
	for (i=0; i<num_entries; i++)
		col[i] = pfgGetNamedWidget(entry_list[i], 
			"filesysFilesysLabel");
	col[num_entries] = NULL;
	(void) xm_AlignWidgetCols(pfgGetNamedWidget(widget_list, "filesysColumnLabel"),
		col);

	for (i=0; i<num_entries; i++)
		col[i] = pfgGetNamedWidget(entry_list[i], 
			"filesysDiskLabel");
	col[num_entries] = NULL;
	xm_AlignWidgetCols(pfgGetNamedWidget(widget_list, "diskColumnLabel"),
		col);

	for (i=0; i<num_entries; i++)
		col[i] = pfgGetNamedWidget(entry_list[i], 
			"filesysSizeLabel");
	col[num_entries] = NULL;
	xm_AlignWidgetCols(pfgGetNamedWidget(widget_list, "sizeColumnLabel"),
		col);

	for (i=0; i<num_entries; i++)
		col[i] = pfgGetNamedWidget(entry_list[i], 
			"filesysOptionsLabel");
	col[num_entries] = NULL;
	xm_AlignWidgetCols(pfgGetNamedWidget(widget_list, "optionsColumnLabel"),
		col);

	for (i=0; i<num_entries; i++)
	    free(entry_list[i]);
	free(entry_list);
	free(col);
}

static void
popupSpaceWindow(Widget parent, Space* status)
{
	int		j = 0;
	Widget		space_dialog;
	char		buf[100];
	Widget		rc;
	WidgetList	entry_list;
	Dimension	filesys_width = 0;
	Dimension	size_width = 0;

	write_debug(GUI_DEBUG_L1, "Entering popupSpaceWindow");

	space_dialog = tu_space_dialog_widget("space_dialog",
		parent, &space_widget_list);

	XmAddWMProtocolCallback(pfgShell(space_dialog), pfgWMDeleteAtom,
	    (XtCallbackProc) pfgExit, NULL);

	XtVaSetValues(pfgShell(space_dialog),
		XmNtitle, TITLE_WARNING,
		XmNdeleteResponse, XmDO_NOTHING,
		NULL);

	pfgSetWidgetString(space_widget_list, "panelhelpText", MSG_SPACE);
	pfgSetWidgetString(space_widget_list, "continueButton", PFG_OKAY);
	pfgSetWidgetString(space_widget_list, "cancelButton", PFG_CANCEL);

	pfgSetWidgetString(space_widget_list, "filesysColumnLabel",
		PFG_FS_FILESYS);
	pfgSetWidgetString(space_widget_list, "minsizeColumnLabel",
		PFG_FS_MINSIZE);

	rc = pfgGetNamedWidget(space_widget_list, "spaceRowColumn");

	for (j = 0; status[j].name[0] != '\0'; j++) {
		(void) tu_space_entry_widget(
			"space_entry", rc,
			&entry_list);

		(void) sprintf(buf, "     %-14s", status[j].name);
		pfgSetWidgetString(entry_list, "spaceFilesysLabel", buf);
		(void) sprintf(buf, "  %5.0f MB  ",
			(float) sectors_to_mb(status[j].required));
		pfgSetWidgetString(entry_list, "spaceMinsizeLabel", buf);

		/*
		 * get the widths of each of the table entries.
		 * this is used to align the column headers.
		 * right now, I do it for every entry in the
		 * table. it's really only needed for one of
		 * the entires.
		 */
		XtVaGetValues(pfgGetNamedWidget(entry_list,
			"spaceFilesysLabel"),
			XmNwidth, &filesys_width,
			NULL);
		XtVaGetValues(pfgGetNamedWidget(entry_list,
			"spaceMinsizeLabel"),
			XmNwidth, &size_width,
			NULL);
		free(entry_list);
	}

	/*
	 * now, we align the table, based on the widths above.
	 */
	XtVaSetValues(pfgGetNamedWidget(space_widget_list,
		"filesysColumnLabel"),
		XmNwidth, filesys_width,
		NULL);
	XtVaSetValues(pfgGetNamedWidget(space_widget_list,
		"minsizeColumnLabel"),
		XmNwidth, size_width,
		NULL);

	free(space_widget_list);

	pfgBusy(pfgShell(parent));

	XtManageChild(space_dialog);
}

/* ARGSUSED */
void
filesysSpaceContinueCB(Widget w, XtPointer client, XtPointer callD)
{
	pfgSetAction(parAContinue);

	pfgUnbusy(pfgShell(XtParent(pfgShell(w))));

	free(widget_list);
}

/* ARGSUSED */
void
filesysSpaceCancelCB(Widget w, XtPointer client, XtPointer callD)
{
	XtUnmanageChild(pfgShell(w));
	XtDestroyWidget(pfgShell(w));

	pfgUnbusy(pfgShell(XtParent(pfgShell(w))));
}
