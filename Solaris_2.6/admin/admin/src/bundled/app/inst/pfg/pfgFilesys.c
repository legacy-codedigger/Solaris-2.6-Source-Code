
/****************************************************************
 *
 * Name      : pfgFilesys.c
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
#include "pfgFilesys_ui.h"


#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/MessageB.h>
#include <Xm/MwmUtil.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/ScrolledW.h>
#include <Xm/Text.h>


/* Local variables used when selecting screen & display */
static Display * sDisplay = NULL;
static Screen  * sScreen  = NULL;

/* forward declaration */
static void class_init(void);

extern void filesysContinueCB(Widget widget,
                              tu_ccb_arg_p closure,
                              XtPointer calldata);
extern void filesysGobackCB(Widget widget,
                            tu_ccb_arg_p closure,
                            XtPointer calldata);
extern void filesysCustomizeCB(Widget widget,
                               tu_ccb_arg_p closure,
                               XtPointer calldata);
extern void pfgExit(Widget widget,
                    tu_ccb_arg_p closure,
                    XtPointer calldata);
extern void unregister_as_dropsite(Widget widget,
                                   tu_ccb_arg_p closure,
                                   XtPointer calldata);
extern void filesysSpaceContinueCB(Widget widget,
                                   tu_ccb_arg_p closure,
                                   XtPointer calldata);
extern void filesysSpaceCancelCB(Widget widget,
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
  XtInitializeWidgetClass(xmLabelWidgetClass);
  XtInitializeWidgetClass(xmScrolledWindowWidgetClass);
  XtInitializeWidgetClass(xmRowColumnWidgetClass);
  XtInitializeWidgetClass(xmMessageBoxWidgetClass);
  XtInitializeWidgetClass(xmPushButtonWidgetClass);
}



/****************************************************************
 *
 *  Main C code for presentation component
 *
 ****************************************************************/

/**************************************************************
 * tu_filesys_dialog_widget:
 **************************************************************/
Widget tu_filesys_dialog_widget(char    * name,
                                Widget    parent,
                                Widget ** warr_ret)
{
  Arg args[26];
  Widget widget_array[16];
  XtTranslations ttbl;
  int n;
  /* Make sure the classes used are initialized */
  class_init();

  /***************** object of type : XmFormDialog *****************/
  widget_array[WI_FILESYS_DIALOG] =
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
    XmCreateText(widget_array[WI_FILESYS_DIALOG], "panelhelpText", args, n);

  /***************** filesysForm : XmForm *****************/
  n = 0;
  XtSetArg(args[n], XmNmarginWidth, 20); n++;
  widget_array[WI_FILESYSFORM] =
    XmCreateForm(widget_array[WI_FILESYS_DIALOG], "filesysForm", args, n);

  /***************** filesysColumnLabel : XmLabel *****************/
  widget_array[WI_FILESYSCOLUMNLABEL] =
    XmCreateLabel(widget_array[WI_FILESYSFORM], "filesysColumnLabel", NULL, 0);

  /***************** diskColumnLabel : XmLabel *****************/
  widget_array[WI_DISKCOLUMNLABEL] =
    XmCreateLabel(widget_array[WI_FILESYSFORM], "diskColumnLabel", NULL, 0);

  /***************** sizeColumnLabel : XmLabel *****************/
  widget_array[WI_SIZECOLUMNLABEL] =
    XmCreateLabel(widget_array[WI_FILESYSFORM], "sizeColumnLabel", NULL, 0);

  /***************** optionsColumnLabel : XmLabel *****************/
  widget_array[WI_OPTIONSCOLUMNLABEL] =
    XmCreateLabel(widget_array[WI_FILESYSFORM], "optionsColumnLabel", NULL, 0);

  /***************** filesysScrolledWindow : XmScrolledWindow *****************/
  n = 0;
  XtSetArg(args[n], XmNscrollingPolicy, XmAUTOMATIC); n++;
  widget_array[WI_FILESYSSCROLLEDWINDOW] =
    XmCreateScrolledWindow(widget_array[WI_FILESYSFORM], "filesysScrolledWindow", args, n);

  /***************** filesysRowColumn : XmRowColumn *****************/
  n = 0;
  XtSetArg(args[n], XmNmarginHeight, 0); n++;
  widget_array[WI_FILESYSROWCOLUMN] =
    XmCreateRowColumn(widget_array[WI_FILESYSSCROLLEDWINDOW], "filesysRowColumn", args, n);

  /***************** messageBox : XmMessageBox *****************/
  n = 0;
  XtSetArg(args[n], XmNdialogType, XmDIALOG_TEMPLATE); n++;
  widget_array[WI_MESSAGEBOX] =
    XmCreateMessageBox(widget_array[WI_FILESYS_DIALOG], "messageBox", args, n);

  /***************** continueButton : XmPushButton *****************/
  widget_array[WI_CONTINUEBUTTON] =
    XmCreatePushButton(widget_array[WI_MESSAGEBOX], "continueButton", NULL, 0);

  /***************** gobackButton : XmPushButton *****************/
  widget_array[WI_GOBACKBUTTON] =
    XmCreatePushButton(widget_array[WI_MESSAGEBOX], "gobackButton", NULL, 0);

  /***************** customizeButton : XmPushButton *****************/
  widget_array[WI_CUSTOMIZEBUTTON] =
    XmCreatePushButton(widget_array[WI_MESSAGEBOX], "customizeButton", NULL, 0);

  /***************** exitButton : XmPushButton *****************/
  widget_array[WI_EXITBUTTON] =
    XmCreatePushButton(widget_array[WI_MESSAGEBOX], "exitButton", NULL, 0);

  /***************** helpButton : XmPushButton *****************/
  widget_array[WI_HELPBUTTON] =
    XmCreatePushButton(widget_array[WI_MESSAGEBOX], "helpButton", NULL, 0);

  /* Terminate the widget array */
  widget_array[15] = NULL;


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

  /***************** filesysForm : XmForm *****************/
  n = 0;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, widget_array[WI_PANELHELPTEXT]); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNbottomWidget, widget_array[WI_MESSAGEBOX]); n++;
  XtSetValues(widget_array[WI_FILESYSFORM], args, n);


  /***************** filesysColumnLabel : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftOffset, 25); n++;
  XtSetValues(widget_array[WI_FILESYSCOLUMNLABEL], args, n);

  XtManageChild(widget_array[WI_FILESYSCOLUMNLABEL]);

  /***************** diskColumnLabel : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, widget_array[WI_FILESYSCOLUMNLABEL]); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, widget_array[WI_FILESYSCOLUMNLABEL]); n++;
  XtSetValues(widget_array[WI_DISKCOLUMNLABEL], args, n);

  XtManageChild(widget_array[WI_DISKCOLUMNLABEL]);

  /***************** sizeColumnLabel : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, widget_array[WI_DISKCOLUMNLABEL]); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, widget_array[WI_DISKCOLUMNLABEL]); n++;
  XtSetValues(widget_array[WI_SIZECOLUMNLABEL], args, n);

  XtManageChild(widget_array[WI_SIZECOLUMNLABEL]);

  /***************** optionsColumnLabel : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, widget_array[WI_SIZECOLUMNLABEL]); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, widget_array[WI_SIZECOLUMNLABEL]); n++;
  XtSetValues(widget_array[WI_OPTIONSCOLUMNLABEL], args, n);

  XtManageChild(widget_array[WI_OPTIONSCOLUMNLABEL]);

  /***************** filesysScrolledWindow : XmScrolledWindow *****************/
  n = 0;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, widget_array[WI_FILESYSCOLUMNLABEL]); n++;
  XtSetValues(widget_array[WI_FILESYSSCROLLEDWINDOW], args, n);

  XtManageChild(widget_array[WI_FILESYSROWCOLUMN]);
  XtManageChild(widget_array[WI_FILESYSSCROLLEDWINDOW]);
  XtManageChild(widget_array[WI_FILESYSFORM]);

  /***************** messageBox : XmMessageBox *****************/
  n = 0;
  XtSetArg(args[n], XmNdefaultButton, widget_array[WI_CONTINUEBUTTON]); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetValues(widget_array[WI_MESSAGEBOX], args, n);

  XtAddCallback(widget_array[WI_CONTINUEBUTTON],
                XmNactivateCallback,
                (XtCallbackProc)filesysContinueCB,
                (XtPointer)NULL);

  XtManageChild(widget_array[WI_CONTINUEBUTTON]);
  XtAddCallback(widget_array[WI_GOBACKBUTTON],
                XmNactivateCallback,
                (XtCallbackProc)filesysGobackCB,
                (XtPointer)NULL);

  XtManageChild(widget_array[WI_GOBACKBUTTON]);
  XtAddCallback(widget_array[WI_CUSTOMIZEBUTTON],
                XmNactivateCallback,
                (XtCallbackProc)filesysCustomizeCB,
                (XtPointer)NULL);

  XtManageChild(widget_array[WI_CUSTOMIZEBUTTON]);
  XtAddCallback(widget_array[WI_EXITBUTTON],
                XmNactivateCallback,
                (XtCallbackProc)pfgExit,
                (XtPointer)NULL);

  XtManageChild(widget_array[WI_EXITBUTTON]);
  XtAddCallback(widget_array[WI_HELPBUTTON],
                XmNactivateCallback,
                pfgHelp,
                (XtPointer)"diskpart.t");

  XtManageChild(widget_array[WI_HELPBUTTON]);
  XtManageChild(widget_array[WI_MESSAGEBOX]);

  /*
   *  Allocate memory for the widget array to return
   */
  if (warr_ret != NULL) {
    *warr_ret = (Widget *) malloc(sizeof(Widget)*16);
    (void) memcpy((char *)*warr_ret,
                  (char *)widget_array,
           sizeof(Widget)*16);
  }

  /*
   *   Fix for SMI's X/NeWS server
   */
  tu_ol_fix_hierarchy(widget_array[WI_FILESYS_DIALOG]);

  /*
   *   Invoke the create callbacks
   */
  tu_widcre_invoke_hooks(widget_array[WI_FILESYS_DIALOG]);

  /*
   *   Return the first created widget.
   */
  return widget_array[WI_FILESYS_DIALOG];
}


/**************************************************************
 * tu_space_dialog_widget:
 **************************************************************/
Widget tu_space_dialog_widget(char    * name,
                              Widget    parent,
                              Widget ** warr_ret)
{
  Arg args[26];
  Widget widget_array[14];
  XtTranslations ttbl;
  int n;
  /* Make sure the classes used are initialized */
  class_init();

  /***************** object of type : XmFormDialog *****************/
  widget_array[WI_SPACE_DIALOG] =
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
  widget_array[WI_PANELHELPTEXT1] =
    XmCreateText(widget_array[WI_SPACE_DIALOG], "panelhelpText", args, n);

  /***************** spaceForm : XmForm *****************/
  n = 0;
  XtSetArg(args[n], XmNmarginWidth, 20); n++;
  widget_array[WI_SPACEFORM] =
    XmCreateForm(widget_array[WI_SPACE_DIALOG], "spaceForm", args, n);

  /***************** filesysColumnLabel : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNrecomputeSize, False); n++;
  widget_array[WI_FILESYSCOLUMNLABEL1] =
    XmCreateLabel(widget_array[WI_SPACEFORM], "filesysColumnLabel", args, n);

  /***************** minsizeColumnLabel : XmLabel *****************/
  widget_array[WI_MINSIZECOLUMNLABEL] =
    XmCreateLabel(widget_array[WI_SPACEFORM], "minsizeColumnLabel", NULL, 0);

  /***************** spaceScrolledWindow : XmScrolledWindow *****************/
  n = 0;
  XtSetArg(args[n], XmNscrollingPolicy, XmAUTOMATIC); n++;
  widget_array[WI_SPACESCROLLEDWINDOW] =
    XmCreateScrolledWindow(widget_array[WI_SPACEFORM], "spaceScrolledWindow", args, n);

  /***************** spaceRowColumn : XmRowColumn *****************/
  widget_array[WI_SPACEROWCOLUMN] =
    XmCreateRowColumn(widget_array[WI_SPACESCROLLEDWINDOW], "spaceRowColumn", NULL, 0);

  /***************** messageBox : XmMessageBox *****************/
  n = 0;
  XtSetArg(args[n], XmNdialogType, XmDIALOG_TEMPLATE); n++;
  widget_array[WI_MESSAGEBOX1] =
    XmCreateMessageBox(widget_array[WI_SPACE_DIALOG], "messageBox", args, n);

  /***************** continueButton : XmPushButton *****************/
  widget_array[WI_CONTINUEBUTTON1] =
    XmCreatePushButton(widget_array[WI_MESSAGEBOX1], "continueButton", NULL, 0);

  /***************** cancelButton : XmPushButton *****************/
  widget_array[WI_CANCELBUTTON] =
    XmCreatePushButton(widget_array[WI_MESSAGEBOX1], "cancelButton", NULL, 0);

  /***************** button3 : XmPushButton *****************/
  widget_array[WI_BUTTON3] =
    XmCreatePushButton(widget_array[WI_MESSAGEBOX1], "button3", NULL, 0);

  /***************** button4 : XmPushButton *****************/
  widget_array[WI_BUTTON4] =
    XmCreatePushButton(widget_array[WI_MESSAGEBOX1], "button4", NULL, 0);

  /***************** button5 : XmPushButton *****************/
  widget_array[WI_BUTTON5] =
    XmCreatePushButton(widget_array[WI_MESSAGEBOX1], "button5", NULL, 0);

  /* Terminate the widget array */
  widget_array[13] = NULL;


  /***************** panelhelpText : XmText *****************/
  n = 0;
  ttbl = XtParseTranslationTable("#override\n\
~Ctrl ~Meta<BtnDown>:\n\
~Ctrl ~Meta<BtnUp>:");
  XtOverrideTranslations(widget_array[WI_PANELHELPTEXT1], ttbl);
  unregister_as_dropsite(widget_array[WI_PANELHELPTEXT1],
                         NULL,
                         NULL);

  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetValues(widget_array[WI_PANELHELPTEXT1], args, n);

  XtManageChild(widget_array[WI_PANELHELPTEXT1]);

  /***************** spaceForm : XmForm *****************/
  n = 0;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, widget_array[WI_PANELHELPTEXT1]); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNbottomWidget, widget_array[WI_MESSAGEBOX1]); n++;
  XtSetValues(widget_array[WI_SPACEFORM], args, n);


  /***************** filesysColumnLabel : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetValues(widget_array[WI_FILESYSCOLUMNLABEL1], args, n);

  XtManageChild(widget_array[WI_FILESYSCOLUMNLABEL1]);

  /***************** minsizeColumnLabel : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, widget_array[WI_FILESYSCOLUMNLABEL1]); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, widget_array[WI_FILESYSCOLUMNLABEL1]); n++;
  XtSetValues(widget_array[WI_MINSIZECOLUMNLABEL], args, n);

  XtManageChild(widget_array[WI_MINSIZECOLUMNLABEL]);

  /***************** spaceScrolledWindow : XmScrolledWindow *****************/
  n = 0;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, widget_array[WI_FILESYSCOLUMNLABEL1]); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetValues(widget_array[WI_SPACESCROLLEDWINDOW], args, n);

  XtManageChild(widget_array[WI_SPACEROWCOLUMN]);
  XtManageChild(widget_array[WI_SPACESCROLLEDWINDOW]);
  XtManageChild(widget_array[WI_SPACEFORM]);

  /***************** messageBox : XmMessageBox *****************/
  n = 0;
  XtSetArg(args[n], XmNdefaultButton, widget_array[WI_CONTINUEBUTTON1]); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetValues(widget_array[WI_MESSAGEBOX1], args, n);

  XtAddCallback(widget_array[WI_CONTINUEBUTTON1],
                XmNactivateCallback,
                (XtCallbackProc)filesysSpaceContinueCB,
                (XtPointer)NULL);

  XtManageChild(widget_array[WI_CONTINUEBUTTON1]);
  XtAddCallback(widget_array[WI_CANCELBUTTON],
                XmNactivateCallback,
                (XtCallbackProc)filesysSpaceCancelCB,
                (XtPointer)NULL);

  XtManageChild(widget_array[WI_CANCELBUTTON]);
  XtManageChild(widget_array[WI_MESSAGEBOX1]);

  /*
   *  Allocate memory for the widget array to return
   */
  if (warr_ret != NULL) {
    *warr_ret = (Widget *) malloc(sizeof(Widget)*14);
    (void) memcpy((char *)*warr_ret,
                  (char *)widget_array,
           sizeof(Widget)*14);
  }

  /*
   *   Fix for SMI's X/NeWS server
   */
  tu_ol_fix_hierarchy(widget_array[WI_SPACE_DIALOG]);

  /*
   *   Invoke the create callbacks
   */
  tu_widcre_invoke_hooks(widget_array[WI_SPACE_DIALOG]);

  /*
   *   Return the first created widget.
   */
  return widget_array[WI_SPACE_DIALOG];
}


/**************************************************************
 * tu_filesys_entry_widget:
 **************************************************************/
Widget tu_filesys_entry_widget(char    * name,
                               Widget    parent,
                               Widget ** warr_ret)
{
  Arg args[16];
  Widget widget_array[6];
  int n;
  /* Make sure the classes used are initialized */
  class_init();

  /***************** object of type : XmForm *****************/
  widget_array[WI_FILESYS_ENTRY] =
    XmCreateForm(parent, name, NULL, 0);

  /***************** filesysFilesysLabel : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNmarginHeight, 0); n++;
  widget_array[WI_FILESYSFILESYSLABEL] =
    XmCreateLabel(widget_array[WI_FILESYS_ENTRY], "filesysFilesysLabel", args, n);

  /***************** filesysDiskLabel : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNmarginHeight, 0); n++;
  widget_array[WI_FILESYSDISKLABEL] =
    XmCreateLabel(widget_array[WI_FILESYS_ENTRY], "filesysDiskLabel", args, n);

  /***************** filesysSizeLabel : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNmarginHeight, 0); n++;
  widget_array[WI_FILESYSSIZELABEL] =
    XmCreateLabel(widget_array[WI_FILESYS_ENTRY], "filesysSizeLabel", args, n);

  /***************** filesysOptionsLabel : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNmarginHeight, 0); n++;
  widget_array[WI_FILESYSOPTIONSLABEL] =
    XmCreateLabel(widget_array[WI_FILESYS_ENTRY], "filesysOptionsLabel", args, n);

  /* Terminate the widget array */
  widget_array[5] = NULL;


  /***************** filesysFilesysLabel : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetValues(widget_array[WI_FILESYSFILESYSLABEL], args, n);

  XtManageChild(widget_array[WI_FILESYSFILESYSLABEL]);

  /***************** filesysDiskLabel : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, widget_array[WI_FILESYSFILESYSLABEL]); n++;
  XtSetValues(widget_array[WI_FILESYSDISKLABEL], args, n);

  XtManageChild(widget_array[WI_FILESYSDISKLABEL]);

  /***************** filesysSizeLabel : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, widget_array[WI_FILESYSDISKLABEL]); n++;
  XtSetValues(widget_array[WI_FILESYSSIZELABEL], args, n);

  XtManageChild(widget_array[WI_FILESYSSIZELABEL]);

  /***************** filesysOptionsLabel : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, widget_array[WI_FILESYSSIZELABEL]); n++;
  XtSetValues(widget_array[WI_FILESYSOPTIONSLABEL], args, n);

  XtManageChild(widget_array[WI_FILESYSOPTIONSLABEL]);
  XtManageChild(widget_array[WI_FILESYS_ENTRY]);

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
  tu_ol_fix_hierarchy(widget_array[WI_FILESYS_ENTRY]);

  /*
   *   Invoke the create callbacks
   */
  tu_widcre_invoke_hooks(widget_array[WI_FILESYS_ENTRY]);

  /*
   *   Return the first created widget.
   */
  return widget_array[WI_FILESYS_ENTRY];
}


/**************************************************************
 * tu_space_entry_widget:
 **************************************************************/
Widget tu_space_entry_widget(char    * name,
                             Widget    parent,
                             Widget ** warr_ret)
{
  Arg args[15];
  Widget widget_array[4];
  int n;
  /* Make sure the classes used are initialized */
  class_init();

  /***************** object of type : XmForm *****************/
  widget_array[WI_SPACE_ENTRY] =
    XmCreateForm(parent, name, NULL, 0);

  /***************** spaceFilesysLabel : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNmarginHeight, 0); n++;
  widget_array[WI_SPACEFILESYSLABEL] =
    XmCreateLabel(widget_array[WI_SPACE_ENTRY], "spaceFilesysLabel", args, n);

  /***************** spaceMinsizeLabel : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNmarginHeight, 0); n++;
  widget_array[WI_SPACEMINSIZELABEL] =
    XmCreateLabel(widget_array[WI_SPACE_ENTRY], "spaceMinsizeLabel", args, n);

  /* Terminate the widget array */
  widget_array[3] = NULL;


  /***************** spaceFilesysLabel : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetValues(widget_array[WI_SPACEFILESYSLABEL], args, n);

  XtManageChild(widget_array[WI_SPACEFILESYSLABEL]);

  /***************** spaceMinsizeLabel : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, widget_array[WI_SPACEFILESYSLABEL]); n++;
  XtSetValues(widget_array[WI_SPACEMINSIZELABEL], args, n);

  XtManageChild(widget_array[WI_SPACEMINSIZELABEL]);
  XtManageChild(widget_array[WI_SPACE_ENTRY]);

  /*
   *  Allocate memory for the widget array to return
   */
  if (warr_ret != NULL) {
    *warr_ret = (Widget *) malloc(sizeof(Widget)*4);
    (void) memcpy((char *)*warr_ret,
                  (char *)widget_array,
           sizeof(Widget)*4);
  }

  /*
   *   Fix for SMI's X/NeWS server
   */
  tu_ol_fix_hierarchy(widget_array[WI_SPACE_ENTRY]);

  /*
   *   Invoke the create callbacks
   */
  tu_widcre_invoke_hooks(widget_array[WI_SPACE_ENTRY]);

  /*
   *   Return the first created widget.
   */
  return widget_array[WI_SPACE_ENTRY];
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
  if (strcmp(temp, "filesys_dialog") == 0){
    w = tu_filesys_dialog_widget(name, parent, (Widget **)retval);
  }
  else if (strcmp(temp, "space_dialog") == 0){
    w = tu_space_dialog_widget(name, parent, (Widget **)retval);
  }
  else if (strcmp(temp, "filesys_entry") == 0){
    w = tu_filesys_entry_widget(name, parent, (Widget **)retval);
  }
  else if (strcmp(temp, "space_entry") == 0){
    w = tu_space_entry_widget(name, parent, (Widget **)retval);
  }

  sDisplay = NULL;
  sScreen = NULL;
  return w;
}

