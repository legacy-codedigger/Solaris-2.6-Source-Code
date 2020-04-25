
/****************************************************************
 *
 * Name      : pfgBootDiskSelect.c
 *
 * File is generated by TeleUSE
 * Version : TeleUSE v3.0.2 / Solaris 2.4
 *
 * Copyright (c) 1994-1995 by Sun MicroSystems, Inc.
 * All rights reserved.
 *
 ****************************************************************/

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Protocols.h>

#include "tu_runtime.h"

#include <stdarg.h>

#include <stdio.h>
#include <stdlib.h>
#include "pfgBootDiskSelect_ui.h"


#include <Xm/CascadeB.h>
#include <Xm/CascadeBG.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/MessageB.h>
#include <Xm/MwmUtil.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/ScrolledW.h>
#include <Xm/Text.h>
#include <Xm/ToggleB.h>


/* Local variables used when selecting screen & display */
static Display * sDisplay = NULL;
static Screen  * sScreen  = NULL;

/* forward declaration */
static void class_init(void);

extern void change_toggle_states(Widget widget,
                                 tu_ccb_arg_p closure,
                                 XtPointer calldata);
extern void set_prom_update(Widget widget,
                            tu_ccb_arg_p closure,
                            XtPointer calldata);
extern void bootdiskCancelCB(Widget widget,
                             tu_ccb_arg_p closure,
                             XtPointer calldata);
extern void unregister_as_dropsite(Widget widget,
                                   tu_ccb_arg_p closure,
                                   XtPointer calldata);
extern void bootdiskOkCB(Widget widget,
                         tu_ccb_arg_p closure,
                         XtPointer calldata);
extern void bootdiskResetCB(Widget widget,
                            tu_ccb_arg_p closure,
                            XtPointer calldata);
extern void pfgHelp(Widget widget,
                    XtPointer closure,
                    XtPointer calldata);


/****************************************************************
 * get_constraint_widget:
 ****************************************************************/
static Widget get_constraint_widget(Widget child, Widget parent)
{
  Widget w;

  w = child;
  while (XtParent(w) != parent)
    w = XtParent(w);
  return (w);
}


/****************************************************************
 * put_client_data_arg:
 *    set up a client data argument.
 ****************************************************************/
static tu_ccb_arg_p put_client_data_arg(unsigned int cnt, ...)
{
  va_list pvar;
  int i;
  tu_ccb_arg_p rv;

  rv = (tu_ccb_arg_p) malloc(cnt*sizeof(tu_ccb_arg_t));
  if (rv == NULL) return NULL;

  va_start(pvar, cnt);

  for (i=0;i<cnt;i++) {
    rv[i].name = va_arg(pvar, char *);
    rv[i].value = va_arg(pvar, char *);
    rv[i].next = &rv[i+1];
  }
  va_end(pvar);
  rv[cnt-1].next = NULL;
  return rv;
}



/****************************************************************
 * class_init:
 *     Initializes the classes used by the user interface.
 ****************************************************************/
static void class_init(void)
{
  static int inited = 0;

  if (inited != 0)
    return;

  inited = 1;
  sDisplay = sDisplay;

  XtInitializeWidgetClass(xmFormWidgetClass);
  XtInitializeWidgetClass(xmTextWidgetClass);
  XtInitializeWidgetClass(xmScrolledWindowWidgetClass);
  XtInitializeWidgetClass(xmLabelWidgetClass);
  XtInitializeWidgetClass(xmToggleButtonWidgetClass);
  XtInitializeWidgetClass(xmMessageBoxWidgetClass);
  XtInitializeWidgetClass(xmPushButtonWidgetClass);
  XtInitializeWidgetClass(xmRowColumnWidgetClass);
}



/****************************************************************
 *
 *  Main C code for presentation component
 *
 ****************************************************************/

/**************************************************************
 * tu_bootdisk_dialog2_widget:
 **************************************************************/
Widget tu_bootdisk_dialog2_widget(char    * name,
                                  Widget    parent,
                                  Widget ** warr_ret)
{
  Arg args[26];
  Widget widget_array[20];
  XtTranslations ttbl;
  int n;
  /* Make sure the classes used are initialized */
  class_init();

  /***************** object of type : XmFormDialog *****************/
  widget_array[WI_BOOTDISK_DIALOG2] =
    XmCreateFormDialog(parent, name, NULL, 0);

  /***************** panelhelpText : XmText *****************/
  n = 0;
  XtSetArg(args[n], XmNautoShowCursorPosition, False); n++;
  XtSetArg(args[n], XmNcursorPositionVisible, False); n++;
  XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
  XtSetArg(args[n], XmNtraversalOn, False); n++;
  XtSetArg(args[n], XmNresizeHeight, True); n++;
  XtSetArg(args[n], XmNwordWrap, True); n++;
  XtSetArg(args[n], XmNshadowThickness, 0); n++;
  XtSetArg(args[n], XmNmarginWidth, 18); n++;
  XtSetArg(args[n], XmNmarginHeight, 18); n++;
  XtSetArg(args[n], XmNvalue, "message_text"); n++;
  widget_array[WI_PANELHELPTEXT] =
    XmCreateText(widget_array[WI_BOOTDISK_DIALOG2], "panelhelpText", args, n);

  /***************** disk_form : XmForm *****************/
  n = 0;
  XtSetArg(args[n], XmNrubberPositioning, False); n++;
  XtSetArg(args[n], XmNresizePolicy, XmRESIZE_ANY); n++;
  XtSetArg(args[n], XmNverticalSpacing, 10); n++;
  XtSetArg(args[n], XmNmarginWidth, 20); n++;
  widget_array[WI_DISK_FORM] =
    XmCreateForm(widget_array[WI_BOOTDISK_DIALOG2], "disk_form", args, n);

  /***************** bootdiskScrolledWindow : XmScrolledWindow *****************/
  n = 0;
  XtSetArg(args[n], XmNscrollingPolicy, XmAUTOMATIC); n++;
  XtSetArg(args[n], XmNresizable, True); n++;
  widget_array[WI_BOOTDISKSCROLLEDWINDOW] =
    XmCreateScrolledWindow(widget_array[WI_DISK_FORM], "bootdiskScrolledWindow", args, n);

  /***************** form_list : XmForm *****************/
  widget_array[WI_FORM_LIST] =
    XmCreateForm(widget_array[WI_BOOTDISKSCROLLEDWINDOW], "form_list", NULL, 0);

  /***************** label_form : XmForm *****************/
  n = 0;
  XtSetArg(args[n], XmNrubberPositioning, False); n++;
  XtSetArg(args[n], XmNresizePolicy, XmRESIZE_ANY); n++;
  XtSetArg(args[n], XmNresizable, True); n++;
  widget_array[WI_LABEL_FORM] =
    XmCreateForm(widget_array[WI_DISK_FORM], "label_form", args, n);

  /***************** disk_label : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNresizable, True); n++;
  widget_array[WI_DISK_LABEL] =
    XmCreateLabel(widget_array[WI_LABEL_FORM], "disk_label", args, n);

  /***************** device_label : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNresizable, True); n++;
  widget_array[WI_DEVICE_LABEL] =
    XmCreateLabel(widget_array[WI_LABEL_FORM], "device_label", args, n);

  /***************** devices_form : XmForm *****************/
  n = 0;
  XtSetArg(args[n], XmNrubberPositioning, False); n++;
  XtSetArg(args[n], XmNresizePolicy, XmRESIZE_ANY); n++;
  XtSetArg(args[n], XmNresizable, True); n++;
  widget_array[WI_DEVICES_FORM] =
    XmCreateForm(widget_array[WI_DISK_FORM], "devices_form", args, n);

  /***************** rebootButton : XmToggleButton *****************/
  n = 0;
  XtSetArg(args[n], XmNset, True); n++;
  XtSetArg(args[n], XmNresizable, True); n++;
  widget_array[WI_REBOOTBUTTON] =
    XmCreateToggleButton(widget_array[WI_DEVICES_FORM], "rebootButton", args, n);

  /***************** label_form : XmForm *****************/
  n = 0;
  XtSetArg(args[n], XmNrubberPositioning, False); n++;
  XtSetArg(args[n], XmNresizePolicy, XmRESIZE_ANY); n++;
  XtSetArg(args[n], XmNresizable, True); n++;
  widget_array[WI_LABEL_FORM1] =
    XmCreateForm(widget_array[WI_DEVICES_FORM], "label_form", args, n);

  /***************** existing_label : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNresizable, True); n++;
  widget_array[WI_EXISTING_LABEL] =
    XmCreateLabel(widget_array[WI_LABEL_FORM1], "existing_label", args, n);

  /***************** existing_value : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNresizable, True); n++;
  widget_array[WI_EXISTING_VALUE] =
    XmCreateLabel(widget_array[WI_LABEL_FORM1], "existing_value", args, n);

  /***************** messageBox : XmMessageBox *****************/
  n = 0;
  XtSetArg(args[n], XmNdialogType, XmDIALOG_TEMPLATE); n++;
  widget_array[WI_MESSAGEBOX] =
    XmCreateMessageBox(widget_array[WI_BOOTDISK_DIALOG2], "messageBox", args, n);

  /***************** okButton : XmPushButton *****************/
  n = 0;
  XtSetArg(args[n], XmNshowAsDefault, 1); n++;
  widget_array[WI_OKBUTTON] =
    XmCreatePushButton(widget_array[WI_MESSAGEBOX], "okButton", args, n);

  /***************** cancelButton : XmPushButton *****************/
  widget_array[WI_CANCELBUTTON] =
    XmCreatePushButton(widget_array[WI_MESSAGEBOX], "cancelButton", NULL, 0);

  /***************** resetButton : XmPushButton *****************/
  widget_array[WI_RESETBUTTON] =
    XmCreatePushButton(widget_array[WI_MESSAGEBOX], "resetButton", NULL, 0);

  /***************** customButton : XmPushButton *****************/
  widget_array[WI_CUSTOMBUTTON] =
    XmCreatePushButton(widget_array[WI_MESSAGEBOX], "customButton", NULL, 0);

  /***************** helpButton : XmPushButton *****************/
  widget_array[WI_HELPBUTTON] =
    XmCreatePushButton(widget_array[WI_MESSAGEBOX], "helpButton", NULL, 0);

  /* Terminate the widget array */
  widget_array[19] = NULL;


  /***************** object of type : XmFormDialog *****************/
  n = 0;
  XtSetArg(args[n], XmNinitialFocus, widget_array[WI_OKBUTTON]); n++;
  XtSetArg(args[n], XmNdefaultButton, widget_array[WI_OKBUTTON]); n++;
  XtSetValues(widget_array[WI_BOOTDISK_DIALOG2], args, n);


  /***************** panelhelpText : XmText *****************/
  n = 0;
  ttbl = XtParseTranslationTable("#override\n\
~Ctrl ~Meta<BtnDown>:\n\
~Ctrl ~Meta<BtnUp>:");
  XtOverrideTranslations(widget_array[WI_PANELHELPTEXT], ttbl);
  unregister_as_dropsite(widget_array[WI_PANELHELPTEXT],
                         NULL,
                         NULL);

  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetValues(widget_array[WI_PANELHELPTEXT], args, n);

  XtManageChild(widget_array[WI_PANELHELPTEXT]);

  /***************** disk_form : XmForm *****************/
  n = 0;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, widget_array[WI_PANELHELPTEXT]); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNbottomWidget, widget_array[WI_MESSAGEBOX]); n++;
  XtSetValues(widget_array[WI_DISK_FORM], args, n);


  /***************** bootdiskScrolledWindow : XmScrolledWindow *****************/
  n = 0;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_NONE); n++;
  XtSetArg(args[n], XmNtopWidget, widget_array[WI_LABEL_FORM]); n++;
  XtSetValues(widget_array[WI_BOOTDISKSCROLLEDWINDOW], args, n);

  XtManageChild(widget_array[WI_FORM_LIST]);
  XtManageChild(widget_array[WI_BOOTDISKSCROLLEDWINDOW]);

  /***************** label_form : XmForm *****************/
  n = 0;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_POSITION); n++;
  XtSetArg(args[n], XmNleftPosition, 2); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_NONE); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_NONE); n++;
  XtSetValues(widget_array[WI_LABEL_FORM], args, n);


  /***************** disk_label : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_NONE); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetValues(widget_array[WI_DISK_LABEL], args, n);

  XtManageChild(widget_array[WI_DISK_LABEL]);

  /***************** device_label : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, widget_array[WI_DISK_LABEL]); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_NONE); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftOffset, 30); n++;
  XtSetValues(widget_array[WI_DEVICE_LABEL], args, n);

  XtManageChild(widget_array[WI_DEVICE_LABEL]);
  XtManageChild(widget_array[WI_LABEL_FORM]);

  /***************** devices_form : XmForm *****************/
  n = 0;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, widget_array[WI_BOOTDISKSCROLLEDWINDOW]); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_NONE); n++;
  XtSetArg(args[n], XmNleftOffset, 25); n++;
  XtSetValues(widget_array[WI_DEVICES_FORM], args, n);


  /***************** rebootButton : XmToggleButton *****************/
  n = 0;
  XtAddCallback(widget_array[WI_REBOOTBUTTON],
                XmNvalueChangedCallback,
                (XtCallbackProc)set_prom_update,
                (XtPointer)NULL);

  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_NONE); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetValues(widget_array[WI_REBOOTBUTTON], args, n);

  XtManageChild(widget_array[WI_REBOOTBUTTON]);

  /***************** label_form : XmForm *****************/
  n = 0;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftOffset, 20); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_NONE); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftWidget, widget_array[WI_REBOOTBUTTON]); n++;
  XtSetValues(widget_array[WI_LABEL_FORM1], args, n);


  /***************** existing_label : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_NONE); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetValues(widget_array[WI_EXISTING_LABEL], args, n);

  XtManageChild(widget_array[WI_EXISTING_LABEL]);

  /***************** existing_value : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, widget_array[WI_EXISTING_LABEL]); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetValues(widget_array[WI_EXISTING_VALUE], args, n);

  XtManageChild(widget_array[WI_EXISTING_VALUE]);
  XtManageChild(widget_array[WI_LABEL_FORM1]);
  XtManageChild(widget_array[WI_DEVICES_FORM]);
  XtManageChild(widget_array[WI_DISK_FORM]);

  /***************** messageBox : XmMessageBox *****************/
  n = 0;
  XtSetArg(args[n], XmNdefaultButton, widget_array[WI_OKBUTTON]); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetValues(widget_array[WI_MESSAGEBOX], args, n);

  XtAddCallback(widget_array[WI_OKBUTTON],
                XmNactivateCallback,
                (XtCallbackProc)bootdiskOkCB,
                (XtPointer)NULL);

  XtManageChild(widget_array[WI_OKBUTTON]);
  XtAddCallback(widget_array[WI_CANCELBUTTON],
                XmNactivateCallback,
                (XtCallbackProc)bootdiskCancelCB,
                (XtPointer)NULL);

  XtManageChild(widget_array[WI_CANCELBUTTON]);
  XtAddCallback(widget_array[WI_RESETBUTTON],
                XmNactivateCallback,
                (XtCallbackProc)bootdiskResetCB,
                (XtPointer)NULL);

  XtManageChild(widget_array[WI_RESETBUTTON]);
  XtSetSensitive(widget_array[WI_CUSTOMBUTTON], False);
  XtAddCallback(widget_array[WI_HELPBUTTON],
                XmNactivateCallback,
                pfgHelp,
                (XtPointer)"spotchangedisk.r");

  XtManageChild(widget_array[WI_HELPBUTTON]);
  XtManageChild(widget_array[WI_MESSAGEBOX]);

  /*
   *  Allocate memory for the widget array to return
   */
  if (warr_ret != NULL) {
    *warr_ret = (Widget *) malloc(sizeof(Widget)*20);
    (void) memcpy((char *)*warr_ret,
                  (char *)widget_array,
           sizeof(Widget)*20);
  }

  /*
   *   Fix for SMI's X/NeWS server
   */
  tu_ol_fix_hierarchy(widget_array[WI_BOOTDISK_DIALOG2]);

  /*
   *   Invoke the create callbacks
   */
  tu_widcre_invoke_hooks(widget_array[WI_BOOTDISK_DIALOG2]);

  /*
   *   Return the first created widget.
   */
  return widget_array[WI_BOOTDISK_DIALOG2];
}


/**************************************************************
 * tu_disk_id1_widget:
 **************************************************************/
Widget tu_disk_id1_widget(char    * name,
                          Widget    parent,
                          Widget ** warr_ret)
{
  Arg args[19];
  Widget widget_array[6];
  int n;
  /* Make sure the classes used are initialized */
  class_init();

  /***************** object of type : XmForm *****************/
  n = 0;
  XtSetArg(args[n], XmNrubberPositioning, False); n++;
  XtSetArg(args[n], XmNresizePolicy, XmRESIZE_ANY); n++;
  widget_array[WI_DISK_ID1] =
    XmCreateForm(parent, name, args, n);

  /***************** pulldownMenu : XmPulldownMenu *****************/
  if (XtIsSubclass(widget_array[WI_DISK_ID1], xmCascadeButtonWidgetClass) ||
      XtIsSubclass(widget_array[WI_DISK_ID1], xmCascadeButtonGadgetClass)) { 
    Widget p = XtParent(widget_array[WI_DISK_ID1]);
    Arg smid[1];

    widget_array[WI_PULLDOWNMENU] =
      XmCreatePulldownMenu(p, "pulldownMenu", NULL, 0);
    XtSetArg(smid[0], XmNsubMenuId, widget_array[WI_PULLDOWNMENU]);
    XtSetValues(widget_array[WI_DISK_ID1], smid, 1);
  } else if (XmIsRowColumn(widget_array[WI_DISK_ID1])) { 
    Widget p = XtParent(widget_array[WI_DISK_ID1]);
    unsigned char rct;
    Arg smid[1];
    Arg rarg[1];

    XtSetArg(rarg[0], XmNrowColumnType, &rct);
    XtGetValues(widget_array[WI_DISK_ID1], rarg, 1);
    if (rct == XmMENU_OPTION) {
      widget_array[WI_PULLDOWNMENU] =
        XmCreatePulldownMenu(p, "pulldownMenu", NULL, 0);
      XtSetArg(smid[0], XmNsubMenuId, widget_array[WI_PULLDOWNMENU]);
      XtSetValues(widget_array[WI_DISK_ID1], smid, 1);
    } else { 
      widget_array[WI_PULLDOWNMENU] =
        XmCreatePulldownMenu(widget_array[WI_DISK_ID1], "pulldownMenu", NULL, 0);
    }
  } else {
    widget_array[WI_PULLDOWNMENU] =
      XmCreatePulldownMenu(widget_array[WI_DISK_ID1], "pulldownMenu", NULL, 0);
  }

  /***************** diskToggle1 : XmToggleButton *****************/
  n = 0;
  XtSetArg(args[n], XmNindicatorType, XmONE_OF_MANY); n++;
  XtSetArg(args[n], XmNspacing, 10); n++;
  XtSetArg(args[n], XmNalignment, XmALIGNMENT_BEGINNING); n++;
  XtSetArg(args[n], XmNresizable, True); n++;
  widget_array[WI_DISKTOGGLE1] =
    XmCreateToggleButton(widget_array[WI_DISK_ID1], "diskToggle1", args, n);

  /***************** optionMenu : XmOptionMenu *****************/
  n = 0;
  XtSetArg(args[n], XmNresizable, True); n++;
  widget_array[WI_OPTIONMENU] =
    XmCreateOptionMenu(widget_array[WI_DISK_ID1], "optionMenu", args, n);

  /***************** solar_part_device_label : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNresizable, True); n++;
  widget_array[WI_SOLAR_PART_DEVICE_LABEL] =
    XmCreateLabel(widget_array[WI_DISK_ID1], "solar_part_device_label", args, n);

  /* Terminate the widget array */
  widget_array[5] = NULL;


  /***************** diskToggle1 : XmToggleButton *****************/
  n = 0;
  XtAddCallback(widget_array[WI_DISKTOGGLE1],
                XmNvalueChangedCallback,
                (XtCallbackProc)change_toggle_states,
                (XtPointer)NULL);

  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_NONE); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetValues(widget_array[WI_DISKTOGGLE1], args, n);

  XtManageChild(widget_array[WI_DISKTOGGLE1]);

  /***************** optionMenu : XmOptionMenu *****************/
  n = 0;
  XtSetArg(args[n], XmNsubMenuId, widget_array[WI_PULLDOWNMENU]); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, widget_array[WI_DISKTOGGLE1]); n++;
  XtSetArg(args[n], XmNleftOffset, 30); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_NONE); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetValues(widget_array[WI_OPTIONMENU], args, n);

  XtManageChild(widget_array[WI_OPTIONMENU]);

  /***************** solar_part_device_label : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, widget_array[WI_DISKTOGGLE1]); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_NONE); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftOffset, 30); n++;
  XtSetValues(widget_array[WI_SOLAR_PART_DEVICE_LABEL], args, n);

  XtManageChild(widget_array[WI_DISK_ID1]);

  /*
   *  Allocate memory for the widget array to return
   */
  if (warr_ret != NULL) {
    *warr_ret = (Widget *) malloc(sizeof(Widget)*6);
    (void) memcpy((char *)*warr_ret,
                  (char *)widget_array,
           sizeof(Widget)*6);
  }

  /*
   *   Fix for SMI's X/NeWS server
   */
  tu_ol_fix_hierarchy(widget_array[WI_DISK_ID1]);

  /*
   *   Invoke the create callbacks
   */
  tu_widcre_invoke_hooks(widget_array[WI_DISK_ID1]);

  /*
   *   Return the first created widget.
   */
  return widget_array[WI_DISK_ID1];
}


/**************************************************************
 * tu_promToggle_widget:
 **************************************************************/
Widget tu_promToggle_widget(char    * name,
                            Widget    parent,
                            Widget ** warr_ret)
{
  Arg args[15];
  Widget widget_array[2];
  XmString xms;
  int n;
  /* Make sure the classes used are initialized */
  class_init();

  /***************** object of type : XmToggleButton *****************/
  n = 0;
  XtSetArg(args[n], XmNindicatorType, XmONE_OF_MANY); n++;
  XtSetArg(args[n], XmNspacing, 10); n++;
  XtSetArg(args[n], XmNset, True); n++;
  xms = tu_cvt_string_to_xmstring(" ");
  XtSetArg(args[n], XmNlabelString, xms); n++;
  widget_array[WI_PROMTOGGLE] =
    XmCreateToggleButton(parent, name, args, n);

  if (xms) XmStringFree(xms);

  /* Terminate the widget array */
  widget_array[1] = NULL;

  XtAddCallback(widget_array[WI_PROMTOGGLE],
                XmNvalueChangedCallback,
                (XtCallbackProc)change_toggle_states,
                (XtPointer)NULL);

  XtManageChild(widget_array[WI_PROMTOGGLE]);

  /*
   *  Allocate memory for the widget array to return
   */
  if (warr_ret != NULL) {
    *warr_ret = (Widget *) malloc(sizeof(Widget)*2);
    (void) memcpy((char *)*warr_ret,
                  (char *)widget_array,
           sizeof(Widget)*2);
  }

  /*
   *   Fix for SMI's X/NeWS server
   */
  tu_ol_fix_hierarchy(widget_array[WI_PROMTOGGLE]);

  /*
   *   Invoke the create callbacks
   */
  tu_widcre_invoke_hooks(widget_array[WI_PROMTOGGLE]);

  /*
   *   Return the first created widget.
   */
  return widget_array[WI_PROMTOGGLE];
}



/****************************************************************
 * create_method:
 *     This function creates a widget hierarchy using the
 *     functions generated above.
 ****************************************************************/
static Widget create_method(char               * temp,
                            char               * name,
                            Widget               parent,
                            Display            * disp,
                            Screen             * screen,
                            tu_template_descr  * retval)
{
  Widget w;

  sDisplay = disp;
  sScreen = screen;

  /* check each node against its name and call its
   * create function if appropriate */
  w = NULL;
  if (strcmp(temp, "bootdisk_dialog2") == 0){
    w = tu_bootdisk_dialog2_widget(name, parent, (Widget **)retval);
  }
  else if (strcmp(temp, "disk_id1") == 0){
    w = tu_disk_id1_widget(name, parent, (Widget **)retval);
  }
  else if (strcmp(temp, "promToggle") == 0){
    w = tu_promToggle_widget(name, parent, (Widget **)retval);
  }

  sDisplay = NULL;
  sScreen = NULL;
  return w;
}

