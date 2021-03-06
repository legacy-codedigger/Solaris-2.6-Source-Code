
/****************************************************************
 *
 * Name      : pfgDSRFSSummary_ui.h
 *
 * File is generated by TeleUSE
 * Version : TeleUSE v3.0.2 / Solaris 2.4
 *
 * Copyright (c) 1994-1995 by Sun MicroSystems, Inc.
 * All rights reserved.
 *
 ****************************************************************/

#ifndef _PFGDSRFSSUMMARY_UI_H_
#define _PFGDSRFSSUMMARY_UI_H_

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>

#include "tu_runtime.h"


/* widget creation functions */

extern Widget tu_dsr_fssumm_filesys_entry_widget(char * name,
                                                 Widget parent,
                                                 Widget ** warr_ret);

extern Widget tu_dsr_fssumm_dialog_widget(char * name,
                                          Widget parent,
                                          Widget ** warr_ret);

#define WI_DSR_FSSUMM_FILESYS_ENTRY 0
#define WI_FSVALUE 1
#define WI_NEWSLICEVALUE 2
#define WI_NEWSIZEVALUE 3
#define WI_WHATHAPPENEDVALUE 4
#define WI_ORIGSLICEVALUE 5
#define WI_ORIGSIZEVALUE 6

#define WI_DSR_FSSUMM_DIALOG 0
#define WI_PANELHELPTEXT 1
#define WI_SUMMARYFORM 2
#define WI_FSCOLUMNLABEL 3
#define WI_NEWSLICECOLUMNLABEL 4
#define WI_NEWSIZECOLUMNLABEL 5
#define WI_WHATHAPPENEDCOLUMNLABEL 6
#define WI_ORIGSLICECOLUMNLABEL 7
#define WI_ORIGSIZECOLUMNLABEL 8
#define WI_FSSUMMSCROLLEDWINDOW 9
#define WI_FSSUMMROWCOLUMN 10
#define WI_MESSAGEBOX 11
#define WI_CONTINUEBUTTON 12
#define WI_GOBACKBUTTON 13
#define WI_CHANGEBUTTON 14
#define WI_EXITBUTTON 15
#define WI_HELPBUTTON 16


#endif
