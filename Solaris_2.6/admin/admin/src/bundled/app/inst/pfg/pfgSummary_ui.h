
/****************************************************************
 *
 * Name      : pfgSummary_ui.h
 *
 * File is generated by TeleUSE
 * Version : TeleUSE v3.0.2 / Solaris 2.4
 *
 * Copyright (c) 1994-1995 by Sun MicroSystems, Inc.
 * All rights reserved.
 *
 ****************************************************************/

#ifndef _PFGSUMMARY_UI_H_
#define _PFGSUMMARY_UI_H_

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>

#include "tu_runtime.h"


/* widget creation functions */

extern Widget tu_summary_dialog_widget(char * name,
                                       Widget parent,
                                       Widget ** warr_ret);

extern Widget tu_scrolled_error_window_widget(char * name,
                                              Widget parent,
                                              Widget ** warr_ret);

#define WI_SUMMARY_DIALOG 0
#define WI_PANELHELPTEXT 1
#define WI_SUMMARYFORM 2
#define WI_PROFILELABEL 3
#define WI_SUMMARYSCROLLEDWINDOW 4
#define WI_SUMMARYLABEL 5
#define WI_MESSAGEBOX 6
#define WI_CONTINUEBUTTON 7
#define WI_CHANGEBUTTON 8
#define WI_EXITBUTTON 9
#define WI_HELPBUTTON 10
#define WI_BUTTON5 11

#define WI_SCROLLED_ERROR_WINDOW 0
#define WI_PANELHELPTEXT1 1
#define WI_SCROLLEDTEXT 2
#define WI_MESSAGEBOX1 3
#define WI_ERROROKBUTTON 4
#define WI_ERRORCANCELBUTTON 5
#define WI_BUTTON3 6
#define WI_BUTTON4 7
#define WI_BUTTON6 8


#endif
