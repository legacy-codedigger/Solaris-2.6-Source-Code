
/****************************************************************
 *
 * Name      : pfgMeta_ui.h
 *
 * File is generated by TeleUSE
 * Version : TeleUSE v3.0.2 / Solaris 2.4
 *
 * Copyright (c) 1994-1995 by Sun MicroSystems, Inc.
 * All rights reserved.
 *
 ****************************************************************/

#ifndef _PFGMETA_UI_H_
#define _PFGMETA_UI_H_

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>

#include "tu_runtime.h"


/* widget creation functions */

extern Widget tu_meta_dialog_widget(char * name,
                                    Widget parent,
                                    Widget ** warr_ret);

#define WI_META_DIALOG 0
#define WI_PANELHELPTEXT 1
#define WI_METAFORM 2
#define WI_SOFTWAREGROUPLABEL 3
#define WI_RECOMMENDEDSIZELABEL 4
#define WI_METASCROLLEDWINDOW 5
#define WI_METARADIOBOX 6
#define WI_MESSAGEBOX 7
#define WI_CONTINUEBUTTON 8
#define WI_GOBACKBUTTON 9
#define WI_CUSTOMIZEBUTTON 10
#define WI_EXITBUTTON 11
#define WI_HELPBUTTON 12


#endif
