
/****************************************************************
 *
 * Name      : pfgProgress_ui.h
 *
 * File is generated by TeleUSE
 * Version : TeleUSE v3.0.2 / Solaris 2.4
 *
 * Copyright (c) 1994-1995 by Sun MicroSystems, Inc.
 * All rights reserved.
 *
 ****************************************************************/

#ifndef _PFGPROGRESS_UI_H_
#define _PFGPROGRESS_UI_H_

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>

#include "tu_runtime.h"


/* widget creation functions */

extern Widget tu_progress_dialog_widget(char * name,
                                        Widget parent,
                                        Widget ** warr_ret);

#define WI_PROGRESS_DIALOG 0
#define WI_PANELHELPTEXT 1
#define WI_PROGRESSFORM 2
#define WI_INSTALLLABEL 3
#define WI_PACKAGELABEL 4
#define WI_PROGRESSSCALE 5
#define WI_MESSAGEBOX 6
#define WI_BUTTON1 7
#define WI_BUTTON2 8
#define WI_BUTTON3 9
#define WI_BUTTON4 10
#define WI_BUTTON5 11


#endif
