
/****************************************************************
 *
 * Name      : pfgOs.c
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
#include "pfgOs_ui.h"


#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/MessageB.h>
#include <Xm/MwmUtil.h>
#include <Xm/PushB.h>
#include <Xm/ScrolledW.h>
#include <Xm/Text.h>
#include <Xm/ToggleB.h>


/* Local variables used when selecting screen & display */
static Display * sDisplay = NULL;
static Screen  * sScreen  = NULL;

/* forward declaration */
static void class_init(void);

extern void pfgExit(Widget widget,
                    tu_ccb_arg_p closure,
                    XtPointer calldata);
extern void unregister_as_dropsite(Widget widget,
                                   tu_ccb_arg_p closure,
                                   XtPointer calldata);
extern void osGobackCB(Widget widget,
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
  XtInitializeWidgetClass(xmMessageBoxWidgetClass);
  XtInitializeWidgetClass(xmPushButtonWidgetClass);
  XtInitializeWidgetClass(xmToggleButtonWidgetClass);
}



/****************************************************************
 *
 *  Main C code for presentation component
 *
 ****************************************************************/

/**************************************************************
 * tu_os_dialog_widget:
 **************************************************************/
Widget tu_os_dialog_widget(char    * name,
                           Widget    parent,
                           Widget ** warr_ret)
{
  Arg args[26];
  Widget widget_array[15];
  XtTranslations ttbl;
  int n;
  /* Make sure the classes used are initialized */
  class_init();

  /***************** object of type : XmFormDialog *****************/
  widget_array[WI_OS_DIALOG] =
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
    XmCreateText(widget_array[WI_OS_DIALOG], "panelhelpText", args, n);

  /***************** osForm : XmForm *****************/
  n = 0;
  XtSetArg(args[n], XmNfractionBase, 6); n++;
  XtSetArg(args[n], XmNrubberPositioning, False); n++;
  XtSetArg(args[n], XmNresizePolicy, XmRESIZE_ANY); n++;
  widget_array[WI_OSFORM] =
    XmCreateForm(widget_array[WI_OS_DIALOG], "osForm", args, n);

  /***************** osLabelForm : XmForm *****************/
  n = 0;
  XtSetArg(args[n], XmNrubberPositioning, False); n++;
  XtSetArg(args[n], XmNresizePolicy, XmRESIZE_ANY); n++;
  XtSetArg(args[n], XmNfractionBase, 2); n++;
  XtSetArg(args[n], XmNresizable, True); n++;
  widget_array[WI_OSLABELFORM] =
    XmCreateForm(widget_array[WI_OSFORM], "osLabelForm", args, n);

  /***************** osVersionLabel : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNalignment, XmALIGNMENT_BEGINNING); n++;
  XtSetArg(args[n], XmNresizable, True); n++;
  widget_array[WI_OSVERSIONLABEL] =
    XmCreateLabel(widget_array[WI_OSLABELFORM], "osVersionLabel", args, n);

  /***************** osSliceLabel : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNalignment, XmALIGNMENT_BEGINNING); n++;
  XtSetArg(args[n], XmNresizable, True); n++;
  widget_array[WI_OSSLICELABEL] =
    XmCreateLabel(widget_array[WI_OSLABELFORM], "osSliceLabel", args, n);

  /***************** osScrolledWindow : XmScrolledWindow *****************/
  n = 0;
  XtSetArg(args[n], XmNscrollBarDisplayPolicy, XmAS_NEEDED); n++;
  XtSetArg(args[n], XmNscrollingPolicy, XmAUTOMATIC); n++;
  XtSetArg(args[n], XmNresizable, True); n++;
  widget_array[WI_OSSCROLLEDWINDOW] =
    XmCreateScrolledWindow(widget_array[WI_OSFORM], "osScrolledWindow", args, n);

  /***************** osSlicesForm : XmForm *****************/
  n = 0;
  XtSetArg(args[n], XmNrubberPositioning, False); n++;
  XtSetArg(args[n], XmNresizePolicy, XmRESIZE_ANY); n++;
  XtSetArg(args[n], XmNfractionBase, 2); n++;
  widget_array[WI_OSSLICESFORM] =
    XmCreateForm(widget_array[WI_OSSCROLLEDWINDOW], "osSlicesForm", args, n);

  /***************** messageBox : XmMessageBox *****************/
  n = 0;
  XtSetArg(args[n], XmNdialogType, XmDIALOG_TEMPLATE); n++;
  widget_array[WI_MESSAGEBOX] =
    XmCreateMessageBox(widget_array[WI_OS_DIALOG], "messageBox", args, n);

  /***************** continueButton : XmPushButton *****************/
  widget_array[WI_CONTINUEBUTTON] =
    XmCreatePushButton(widget_array[WI_MESSAGEBOX], "continueButton", NULL, 0);

  /***************** gobackButton : XmPushButton *****************/
  widget_array[WI_GOBACKBUTTON] =
    XmCreatePushButton(widget_array[WI_MESSAGEBOX], "gobackButton", NULL, 0);

  /***************** exitButton : XmPushButton *****************/
  widget_array[WI_EXITBUTTON] =
    XmCreatePushButton(widget_array[WI_MESSAGEBOX], "exitButton", NULL, 0);

  /***************** helpButton : XmPushButton *****************/
  widget_array[WI_HELPBUTTON] =
    XmCreatePushButton(widget_array[WI_MESSAGEBOX], "helpButton", NULL, 0);

  /***************** button5 : XmPushButton *****************/
  widget_array[WI_BUTTON5] =
    XmCreatePushButton(widget_array[WI_MESSAGEBOX], "button5", NULL, 0);

  /* Terminate the widget array */
  widget_array[14] = NULL;


  /***************** object of type : XmFormDialog *****************/
  n = 0;
  XtSetArg(args[n], XmNinitialFocus, widget_array[WI_CONTINUEBUTTON]); n++;
  XtSetValues(widget_array[WI_OS_DIALOG], args, n);


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

  /***************** osForm : XmForm *****************/
  n = 0;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, widget_array[WI_PANELHELPTEXT]); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNbottomWidget, widget_array[WI_MESSAGEBOX]); n++;
  XtSetValues(widget_array[WI_OSFORM], args, n);


  /***************** osLabelForm : XmForm *****************/
  n = 0;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_POSITION); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_POSITION); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_NONE); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_NONE); n++;
  XtSetArg(args[n], XmNleftPosition, 1); n++;
  XtSetArg(args[n], XmNrightPosition, 5); n++;
  XtSetValues(widget_array[WI_OSLABELFORM], args, n);


  /***************** osVersionLabel : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_POSITION); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightPosition, 1); n++;
  XtSetValues(widget_array[WI_OSVERSIONLABEL], args, n);

  XtManageChild(widget_array[WI_OSVERSIONLABEL]);

  /***************** osSliceLabel : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_POSITION); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_NONE); n++;
  XtSetArg(args[n], XmNtopOffset, 0); n++;
  XtSetArg(args[n], XmNleftOffset, 0); n++;
  XtSetArg(args[n], XmNleftPosition, 1); n++;
  XtSetValues(widget_array[WI_OSSLICELABEL], args, n);

  XtManageChild(widget_array[WI_OSSLICELABEL]);
  XtManageChild(widget_array[WI_OSLABELFORM]);

  /***************** osScrolledWindow : XmScrolledWindow *****************/
  n = 0;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_POSITION); n++;
  XtSetArg(args[n], XmNleftPosition, 1); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_POSITION); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, widget_array[WI_OSLABELFORM]); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightPosition, 5); n++;
  XtSetValues(widget_array[WI_OSSCROLLEDWINDOW], args, n);

  XtManageChild(widget_array[WI_OSSLICESFORM]);
  XtManageChild(widget_array[WI_OSSCROLLEDWINDOW]);
  XtManageChild(widget_array[WI_OSFORM]);

  /***************** messageBox : XmMessageBox *****************/
  n = 0;
  XtSetArg(args[n], XmNdefaultButton, widget_array[WI_CONTINUEBUTTON]); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetValues(widget_array[WI_MESSAGEBOX], args, n);

  XtManageChild(widget_array[WI_CONTINUEBUTTON]);
  XtAddCallback(widget_array[WI_GOBACKBUTTON],
                XmNactivateCallback,
                (XtCallbackProc)osGobackCB,
                (XtPointer)NULL);

  XtManageChild(widget_array[WI_GOBACKBUTTON]);
  XtAddCallback(widget_array[WI_EXITBUTTON],
                XmNactivateCallback,
                (XtCallbackProc)pfgExit,
                (XtPointer)NULL);

  XtManageChild(widget_array[WI_EXITBUTTON]);
  XtAddCallback(widget_array[WI_HELPBUTTON],
                XmNactivateCallback,
                pfgHelp,
                (XtPointer)"version.r");

  XtManageChild(widget_array[WI_HELPBUTTON]);
  XtManageChild(widget_array[WI_MESSAGEBOX]);

  /*
   *  Allocate memory for the widget array to return
   */
  if (warr_ret != NULL) {
    *warr_ret = (Widget *) malloc(sizeof(Widget)*15);
    (void) memcpy((char *)*warr_ret,
                  (char *)widget_array,
           sizeof(Widget)*15);
  }

  /*
   *   Fix for SMI's X/NeWS server
   */
  tu_ol_fix_hierarchy(widget_array[WI_OS_DIALOG]);

  /*
   *   Invoke the create callbacks
   */
  tu_widcre_invoke_hooks(widget_array[WI_OS_DIALOG]);

  /*
   *   Return the first created widget.
   */
  return widget_array[WI_OS_DIALOG];
}


/**************************************************************
 * tu_osSliceForm_widget:
 *   XmForm is a container widget with no input semantics. Constraints are placed on its children to define attachments for each of the child's four sides. These attachments can be to the XmForm, to another child, to a relative position or to the initial position of the child. The attachments determine the layout behavior of XmForm when resizing occurs.
 **************************************************************/
Widget tu_osSliceForm_widget(char    * name,
                             Widget    parent,
                             Widget ** warr_ret)
{
  Arg args[19];
  Widget widget_array[4];
  int n;
  /* Make sure the classes used are initialized */
  class_init();

  /***************** object of type : XmForm *****************/
  n = 0;
  XtSetArg(args[n], XmNrubberPositioning, False); n++;
  XtSetArg(args[n], XmNresizePolicy, XmRESIZE_ANY); n++;
  XtSetArg(args[n], XmNfractionBase, 2); n++;
  widget_array[WI_OSSLICEFORM] =
    XmCreateForm(parent, name, args, n);

  /***************** osSliceToggle : XmToggleButton *****************/
  n = 0;
  XtSetArg(args[n], XmNalignment, XmALIGNMENT_BEGINNING); n++;
  XtSetArg(args[n], XmNresizable, True); n++;
  widget_array[WI_OSSLICETOGGLE] =
    XmCreateToggleButton(widget_array[WI_OSSLICEFORM], "osSliceToggle", args, n);

  /***************** osSliceNameLabel : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNmarginWidth, 0); n++;
  XtSetArg(args[n], XmNmarginHeight, 0); n++;
  XtSetArg(args[n], XmNresizable, True); n++;
  widget_array[WI_OSSLICENAMELABEL] =
    XmCreateLabel(widget_array[WI_OSSLICEFORM], "osSliceNameLabel", args, n);

  /* Terminate the widget array */
  widget_array[3] = NULL;


  /***************** osSliceToggle : XmToggleButton *****************/
  n = 0;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_NONE); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetValues(widget_array[WI_OSSLICETOGGLE], args, n);

  XtManageChild(widget_array[WI_OSSLICETOGGLE]);

  /***************** osSliceNameLabel : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftOffset, 55); n++;
  XtSetArg(args[n], XmNleftWidget, widget_array[WI_OSSLICETOGGLE]); n++;
  XtSetValues(widget_array[WI_OSSLICENAMELABEL], args, n);

  XtManageChild(widget_array[WI_OSSLICENAMELABEL]);
  XtManageChild(widget_array[WI_OSSLICEFORM]);

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
  tu_ol_fix_hierarchy(widget_array[WI_OSSLICEFORM]);

  /*
   *   Invoke the create callbacks
   */
  tu_widcre_invoke_hooks(widget_array[WI_OSSLICEFORM]);

  /*
   *   Return the first created widget.
   */
  return widget_array[WI_OSSLICEFORM];
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
  if (strcmp(temp, "os_dialog") == 0){
    w = tu_os_dialog_widget(name, parent, (Widget **)retval);
  }
  else if (strcmp(temp, "osSliceForm") == 0){
    w = tu_osSliceForm_widget(name, parent, (Widget **)retval);
  }

  sDisplay = NULL;
  sScreen = NULL;
  return w;
}

