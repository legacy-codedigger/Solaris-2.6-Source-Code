
/****************************************************************
 *
 * Name      : pfgOs_ui.h
 *
 * File is generated by TeleUSE
 * Version : TeleUSE v3.0.2 / Solaris 2.4
 *
 * Copyright (c) 1994-1995 by Sun MicroSystems, Inc.
 * All rights reserved.
 *
 ****************************************************************/

#ifndef _PFGOS_UI_H_
#define _PFGOS_UI_H_

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>

#include "tu_runtime.h"


/* widget creation functions */

extern Widget tu_os_dialog_widget(char * name,
                                  Widget parent,
                                  Widget ** warr_ret);

extern Widget tu_osSliceForm_widget(char * name,
                                    Widget parent,
                                    Widget ** warr_ret);

#define WI_OS_DIALOG 0
#define WI_PANELHELPTEXT 1
#define WI_OSFORM 2
#define WI_OSLABELFORM 3
#define WI_OSVERSIONLABEL 4
#define WI_OSSLICELABEL 5
#define WI_OSSCROLLEDWINDOW 6
#define WI_OSSLICESFORM 7
#define WI_MESSAGEBOX 8
#define WI_CONTINUEBUTTON 9
#define WI_GOBACKBUTTON 10
#define WI_EXITBUTTON 11
#define WI_HELPBUTTON 12
#define WI_BUTTON5 13

#define WI_OSSLICEFORM 0
#define WI_OSSLICETOGGLE 1
#define WI_OSSLICENAMELABEL 2


#endif