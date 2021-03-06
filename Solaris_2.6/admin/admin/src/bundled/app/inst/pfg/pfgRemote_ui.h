
/****************************************************************
 *
 * Name      : pfgRemote_ui.h
 *
 * File is generated by TeleUSE
 * Version : TeleUSE v3.0.2 / Solaris 2.4
 *
 * Copyright (c) 1994-1995 by Sun MicroSystems, Inc.
 * All rights reserved.
 *
 ****************************************************************/

#ifndef _PFGREMOTE_UI_H_
#define _PFGREMOTE_UI_H_

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>

#include "tu_runtime.h"


/* widget creation functions */

extern Widget tu_remote_dialog_widget(char * name,
                                      Widget parent,
                                      Widget ** warr_ret);

#define WI_REMOTE_DIALOG 0
#define WI_PANELHELPTEXT 1
#define WI_FORM 2
#define WI_SERVERLABELTEXT 3
#define WI_SERVERLABEL 4
#define WI_SERVERTEXT 5
#define WI_IPLABELTEXT 6
#define WI_IPLABEL 7
#define WI_IPTEXT 8
#define WI_FILESYSLABELTEXT 9
#define WI_FILESYSLABEL 10
#define WI_FILESYSTEXT 11
#define WI_REMOTELIST 12
#define WI_MOUNTPOINTLABELTEXT 13
#define WI_MOUNTPOINTLABEL 14
#define WI_MOUNTPOINTTEXT 15
#define WI_BUTTONROWCOLUMN 16
#define WI_ADDBUTTON 17
#define WI_REMOVEBUTTON 18
#define WI_TESTBUTTON 19
#define WI_REMOTELABEL 20
#define WI_SUMMARYLIST 21
#define WI_MESSAGEBOX 22
#define WI_CONTINUEBUTTON 23
#define WI_CANCELBUTTON 24
#define WI_HELPBUTTON 25
#define WI_BUTTON4 26
#define WI_BUTTON5 27


#endif
