
/****************************************************************
 *
 * Name      : pfgDSRFSSummary.c
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
#include "pfgDSRFSSummary_ui.h"


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

extern void dsr_fssumm_continue_cb(Widget widget,
                                   tu_ccb_arg_p closure,
                                   XtPointer calldata);
extern void dsr_fssumm_goback_cb(Widget widget,
                                 tu_ccb_arg_p closure,
                                 XtPointer calldata);
extern void dsr_fssumm_change_cb(Widget widget,
                                 tu_ccb_arg_p closure,
                                 XtPointer calldata);
extern void pfgExit(Widget widget,
                    tu_ccb_arg_p closure,
                    XtPointer calldata);
extern void unregister_as_dropsite(Widget widget,
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
  XtInitializeWidgetClass(xmLabelWidgetClass);
  XtInitializeWidgetClass(xmTextWidgetClass);
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
 * tu_dsr_fssumm_filesys_entry_widget:
 **************************************************************/
Widget tu_dsr_fssumm_filesys_entry_widget(char    * name,
                                          Widget    parent,
                                          Widget ** warr_ret)
{
  Arg args[17];
  Widget widget_array[8];
  int n;
  /* Make sure the classes used are initialized */
  class_init();

  /***************** object of type : XmForm *****************/
  n = 0;
  XtSetArg(args[n], XmNrubberPositioning, False); n++;
  XtSetArg(args[n], XmNresizePolicy, XmRESIZE_ANY); n++;
  widget_array[WI_DSR_FSSUMM_FILESYS_ENTRY] =
    XmCreateForm(parent, name, args, n);

  /***************** fsValue : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNalignment, XmALIGNMENT_BEGINNING); n++;
  XtSetArg(args[n], XmNresizable, True); n++;
  widget_array[WI_FSVALUE] =
    XmCreateLabel(widget_array[WI_DSR_FSSUMM_FILESYS_ENTRY], "fsValue", args, n);

  /***************** newSliceValue : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNresizable, True); n++;
  widget_array[WI_NEWSLICEVALUE] =
    XmCreateLabel(widget_array[WI_DSR_FSSUMM_FILESYS_ENTRY], "newSliceValue", args, n);

  /***************** newSizeValue : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNalignment, XmALIGNMENT_END); n++;
  XtSetArg(args[n], XmNresizable, True); n++;
  widget_array[WI_NEWSIZEVALUE] =
    XmCreateLabel(widget_array[WI_DSR_FSSUMM_FILESYS_ENTRY], "newSizeValue", args, n);

  /***************** whatHappenedValue : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNalignment, XmALIGNMENT_END); n++;
  XtSetArg(args[n], XmNresizable, True); n++;
  widget_array[WI_WHATHAPPENEDVALUE] =
    XmCreateLabel(widget_array[WI_DSR_FSSUMM_FILESYS_ENTRY], "whatHappenedValue", args, n);

  /***************** origSliceValue : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNresizable, True); n++;
  widget_array[WI_ORIGSLICEVALUE] =
    XmCreateLabel(widget_array[WI_DSR_FSSUMM_FILESYS_ENTRY], "origSliceValue", args, n);

  /***************** origSizeValue : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNalignment, XmALIGNMENT_END); n++;
  XtSetArg(args[n], XmNresizable, True); n++;
  widget_array[WI_ORIGSIZEVALUE] =
    XmCreateLabel(widget_array[WI_DSR_FSSUMM_FILESYS_ENTRY], "origSizeValue", args, n);

  /* Terminate the widget array */
  widget_array[7] = NULL;


  /***************** fsValue : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_NONE); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_NONE); n++;
  XtSetValues(widget_array[WI_FSVALUE], args, n);

  XtManageChild(widget_array[WI_FSVALUE]);

  /***************** newSliceValue : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, widget_array[WI_FSVALUE]); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_NONE); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_NONE); n++;
  XtSetValues(widget_array[WI_NEWSLICEVALUE], args, n);

  XtManageChild(widget_array[WI_NEWSLICEVALUE]);

  /***************** newSizeValue : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, widget_array[WI_NEWSLICEVALUE]); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_NONE); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_NONE); n++;
  XtSetValues(widget_array[WI_NEWSIZEVALUE], args, n);

  XtManageChild(widget_array[WI_NEWSIZEVALUE]);

  /***************** whatHappenedValue : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, widget_array[WI_NEWSIZEVALUE]); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_NONE); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_NONE); n++;
  XtSetValues(widget_array[WI_WHATHAPPENEDVALUE], args, n);

  XtManageChild(widget_array[WI_WHATHAPPENEDVALUE]);

  /***************** origSliceValue : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, widget_array[WI_WHATHAPPENEDVALUE]); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_NONE); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_NONE); n++;
  XtSetValues(widget_array[WI_ORIGSLICEVALUE], args, n);

  XtManageChild(widget_array[WI_ORIGSLICEVALUE]);

  /***************** origSizeValue : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, widget_array[WI_ORIGSLICEVALUE]); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_NONE); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_NONE); n++;
  XtSetValues(widget_array[WI_ORIGSIZEVALUE], args, n);

  XtManageChild(widget_array[WI_ORIGSIZEVALUE]);
  XtManageChild(widget_array[WI_DSR_FSSUMM_FILESYS_ENTRY]);

  /*
   *  Allocate memory for the widget array to return
   */
  if (warr_ret != NULL) {
    *warr_ret = (Widget *) malloc(sizeof(Widget)*8);
    (void) memcpy((char *)*warr_ret,
                  (char *)widget_array,
           sizeof(Widget)*8);
  }

  /*
   *   Fix for SMI's X/NeWS server
   */
  tu_ol_fix_hierarchy(widget_array[WI_DSR_FSSUMM_FILESYS_ENTRY]);

  /*
   *   Invoke the create callbacks
   */
  tu_widcre_invoke_hooks(widget_array[WI_DSR_FSSUMM_FILESYS_ENTRY]);

  /*
   *   Return the first created widget.
   */
  return widget_array[WI_DSR_FSSUMM_FILESYS_ENTRY];
}


/**************************************************************
 * tu_dsr_fssumm_dialog_widget:
 **************************************************************/
Widget tu_dsr_fssumm_dialog_widget(char    * name,
                                   Widget    parent,
                                   Widget ** warr_ret)
{
  Arg args[26];
  Widget widget_array[18];
  XtTranslations ttbl;
  int n;
  /* Make sure the classes used are initialized */
  class_init();

  /***************** object of type : XmFormDialog *****************/
  n = 0;
  XtSetArg(args[n], XmNrubberPositioning, False); n++;
  XtSetArg(args[n], XmNresizePolicy, XmRESIZE_ANY); n++;
  widget_array[WI_DSR_FSSUMM_DIALOG] =
    XmCreateFormDialog(parent, name, args, n);

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
    XmCreateText(widget_array[WI_DSR_FSSUMM_DIALOG], "panelhelpText", args, n);

  /***************** summaryForm : XmForm *****************/
  n = 0;
  XtSetArg(args[n], XmNrubberPositioning, False); n++;
  XtSetArg(args[n], XmNresizePolicy, XmRESIZE_ANY); n++;
  XtSetArg(args[n], XmNresizable, True); n++;
  widget_array[WI_SUMMARYFORM] =
    XmCreateForm(widget_array[WI_DSR_FSSUMM_DIALOG], "summaryForm", args, n);

  /***************** fsColumnLabel : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNresizable, True); n++;
  widget_array[WI_FSCOLUMNLABEL] =
    XmCreateLabel(widget_array[WI_SUMMARYFORM], "fsColumnLabel", args, n);

  /***************** newSliceColumnLabel : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNresizable, True); n++;
  widget_array[WI_NEWSLICECOLUMNLABEL] =
    XmCreateLabel(widget_array[WI_SUMMARYFORM], "newSliceColumnLabel", args, n);

  /***************** newSizeColumnLabel : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNalignment, XmALIGNMENT_CENTER); n++;
  XtSetArg(args[n], XmNresizable, True); n++;
  widget_array[WI_NEWSIZECOLUMNLABEL] =
    XmCreateLabel(widget_array[WI_SUMMARYFORM], "newSizeColumnLabel", args, n);

  /***************** whatHappenedColumnLabel : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNresizable, True); n++;
  widget_array[WI_WHATHAPPENEDCOLUMNLABEL] =
    XmCreateLabel(widget_array[WI_SUMMARYFORM], "whatHappenedColumnLabel", args, n);

  /***************** origSliceColumnLabel : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNresizable, True); n++;
  widget_array[WI_ORIGSLICECOLUMNLABEL] =
    XmCreateLabel(widget_array[WI_SUMMARYFORM], "origSliceColumnLabel", args, n);

  /***************** origSizeColumnLabel : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNresizable, True); n++;
  widget_array[WI_ORIGSIZECOLUMNLABEL] =
    XmCreateLabel(widget_array[WI_SUMMARYFORM], "origSizeColumnLabel", args, n);

  /***************** fsSummScrolledWindow : XmScrolledWindow *****************/
  n = 0;
  XtSetArg(args[n], XmNscrollBarDisplayPolicy, XmAS_NEEDED); n++;
  XtSetArg(args[n], XmNscrollingPolicy, XmAUTOMATIC); n++;
  XtSetArg(args[n], XmNresizable, True); n++;
  widget_array[WI_FSSUMMSCROLLEDWINDOW] =
    XmCreateScrolledWindow(widget_array[WI_SUMMARYFORM], "fsSummScrolledWindow", args, n);

  /***************** fsSummRowColumn : XmRowColumn *****************/
  widget_array[WI_FSSUMMROWCOLUMN] =
    XmCreateRowColumn(widget_array[WI_FSSUMMSCROLLEDWINDOW], "fsSummRowColumn", NULL, 0);

  /***************** messageBox : XmMessageBox *****************/
  n = 0;
  XtSetArg(args[n], XmNdialogType, XmDIALOG_TEMPLATE); n++;
  widget_array[WI_MESSAGEBOX] =
    XmCreateMessageBox(widget_array[WI_DSR_FSSUMM_DIALOG], "messageBox", args, n);

  /***************** continueButton : XmPushButton *****************/
  widget_array[WI_CONTINUEBUTTON] =
    XmCreatePushButton(widget_array[WI_MESSAGEBOX], "continueButton", NULL, 0);

  /***************** gobackButton : XmPushButton *****************/
  widget_array[WI_GOBACKBUTTON] =
    XmCreatePushButton(widget_array[WI_MESSAGEBOX], "gobackButton", NULL, 0);

  /***************** changeButton : XmPushButton *****************/
  widget_array[WI_CHANGEBUTTON] =
    XmCreatePushButton(widget_array[WI_MESSAGEBOX], "changeButton", NULL, 0);

  /***************** exitButton : XmPushButton *****************/
  widget_array[WI_EXITBUTTON] =
    XmCreatePushButton(widget_array[WI_MESSAGEBOX], "exitButton", NULL, 0);

  /***************** helpButton : XmPushButton *****************/
  widget_array[WI_HELPBUTTON] =
    XmCreatePushButton(widget_array[WI_MESSAGEBOX], "helpButton", NULL, 0);

  /* Terminate the widget array */
  widget_array[17] = NULL;


  /***************** object of type : XmFormDialog *****************/
  n = 0;
  XtSetArg(args[n], XmNinitialFocus, widget_array[WI_CONTINUEBUTTON]); n++;
  XtSetValues(widget_array[WI_DSR_FSSUMM_DIALOG], args, n);


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

  /***************** summaryForm : XmForm *****************/
  n = 0;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, widget_array[WI_PANELHELPTEXT]); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNbottomWidget, widget_array[WI_MESSAGEBOX]); n++;
  XtSetArg(args[n], XmNtopOffset, 10); n++;
  XtSetValues(widget_array[WI_SUMMARYFORM], args, n);


  /***************** fsColumnLabel : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_NONE); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_NONE); n++;
  XtSetArg(args[n], XmNleftOffset, 10); n++;
  XtSetValues(widget_array[WI_FSCOLUMNLABEL], args, n);

  XtManageChild(widget_array[WI_FSCOLUMNLABEL]);

  /***************** newSliceColumnLabel : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, widget_array[WI_FSCOLUMNLABEL]); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_NONE); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, widget_array[WI_FSCOLUMNLABEL]); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_NONE); n++;
  XtSetValues(widget_array[WI_NEWSLICECOLUMNLABEL], args, n);

  XtManageChild(widget_array[WI_NEWSLICECOLUMNLABEL]);

  /***************** newSizeColumnLabel : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, widget_array[WI_NEWSLICECOLUMNLABEL]); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_NONE); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, widget_array[WI_FSCOLUMNLABEL]); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_NONE); n++;
  XtSetValues(widget_array[WI_NEWSIZECOLUMNLABEL], args, n);

  XtManageChild(widget_array[WI_NEWSIZECOLUMNLABEL]);

  /***************** whatHappenedColumnLabel : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, widget_array[WI_NEWSIZECOLUMNLABEL]); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_NONE); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, widget_array[WI_FSCOLUMNLABEL]); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_NONE); n++;
  XtSetValues(widget_array[WI_WHATHAPPENEDCOLUMNLABEL], args, n);

  XtManageChild(widget_array[WI_WHATHAPPENEDCOLUMNLABEL]);

  /***************** origSliceColumnLabel : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, widget_array[WI_WHATHAPPENEDCOLUMNLABEL]); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_NONE); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, widget_array[WI_FSCOLUMNLABEL]); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_NONE); n++;
  XtSetValues(widget_array[WI_ORIGSLICECOLUMNLABEL], args, n);

  XtManageChild(widget_array[WI_ORIGSLICECOLUMNLABEL]);

  /***************** origSizeColumnLabel : XmLabel *****************/
  n = 0;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, widget_array[WI_ORIGSLICECOLUMNLABEL]); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_NONE); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, widget_array[WI_FSCOLUMNLABEL]); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_NONE); n++;
  XtSetValues(widget_array[WI_ORIGSIZECOLUMNLABEL], args, n);

  XtManageChild(widget_array[WI_ORIGSIZECOLUMNLABEL]);

  /***************** fsSummScrolledWindow : XmScrolledWindow *****************/
  n = 0;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopWidget, widget_array[WI_FSCOLUMNLABEL]); n++;
  XtSetArg(args[n], XmNrightOffset, 10); n++;
  XtSetArg(args[n], XmNleftWidget, widget_array[WI_FSCOLUMNLABEL]); n++;
  XtSetValues(widget_array[WI_FSSUMMSCROLLEDWINDOW], args, n);

  XtManageChild(widget_array[WI_FSSUMMROWCOLUMN]);
  XtManageChild(widget_array[WI_FSSUMMSCROLLEDWINDOW]);
  XtManageChild(widget_array[WI_SUMMARYFORM]);

  /***************** messageBox : XmMessageBox *****************/
  n = 0;
  XtSetArg(args[n], XmNdefaultButton, widget_array[WI_CONTINUEBUTTON]); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetValues(widget_array[WI_MESSAGEBOX], args, n);

  XtAddCallback(widget_array[WI_CONTINUEBUTTON],
                XmNactivateCallback,
                (XtCallbackProc)dsr_fssumm_continue_cb,
                (XtPointer)NULL);

  XtManageChild(widget_array[WI_CONTINUEBUTTON]);
  XtAddCallback(widget_array[WI_GOBACKBUTTON],
                XmNactivateCallback,
                (XtCallbackProc)dsr_fssumm_goback_cb,
                (XtPointer)NULL);

  XtManageChild(widget_array[WI_GOBACKBUTTON]);
  XtAddCallback(widget_array[WI_CHANGEBUTTON],
                XmNactivateCallback,
                (XtCallbackProc)dsr_fssumm_change_cb,
                (XtPointer)NULL);

  XtManageChild(widget_array[WI_CHANGEBUTTON]);
  XtAddCallback(widget_array[WI_EXITBUTTON],
                XmNactivateCallback,
                (XtCallbackProc)pfgExit,
                (XtPointer)NULL);

  XtManageChild(widget_array[WI_EXITBUTTON]);
  XtAddCallback(widget_array[WI_HELPBUTTON],
                XmNactivateCallback,
                pfgHelp,
                (XtPointer)"filechanges.r");

  XtManageChild(widget_array[WI_HELPBUTTON]);
  XtManageChild(widget_array[WI_MESSAGEBOX]);

  /*
   *  Allocate memory for the widget array to return
   */
  if (warr_ret != NULL) {
    *warr_ret = (Widget *) malloc(sizeof(Widget)*18);
    (void) memcpy((char *)*warr_ret,
                  (char *)widget_array,
           sizeof(Widget)*18);
  }

  /*
   *   Fix for SMI's X/NeWS server
   */
  tu_ol_fix_hierarchy(widget_array[WI_DSR_FSSUMM_DIALOG]);

  /*
   *   Invoke the create callbacks
   */
  tu_widcre_invoke_hooks(widget_array[WI_DSR_FSSUMM_DIALOG]);

  /*
   *   Return the first created widget.
   */
  return widget_array[WI_DSR_FSSUMM_DIALOG];
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
  if (strcmp(temp, "dsr_fssumm_filesys_entry") == 0){
    w = tu_dsr_fssumm_filesys_entry_widget(name, parent, (Widget **)retval);
  }
  else if (strcmp(temp, "dsr_fssumm_dialog") == 0){
    w = tu_dsr_fssumm_dialog_widget(name, parent, (Widget **)retval);
  }

  sDisplay = NULL;
  sScreen = NULL;
  return w;
}

