
/****************************************************************
 *
 * Name      : pfgCyl_ui.h
 *
 * File is generated by TeleUSE
 * Version : TeleUSE v3.0.2 / Solaris 2.4
 *
 * Copyright (c) 1994-1995 by Sun MicroSystems, Inc.
 * All rights reserved.
 *
 ****************************************************************/

#ifndef _PFGCYL_UI_H_
#define _PFGCYL_UI_H_

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>

#include "tu_runtime.h"


/* widget creation functions */

extern Widget tu_cyl_dialog_widget(char * name,
                                   Widget parent,
                                   Widget ** warr_ret);

extern Widget tu_cyl_slice_entry_widget(char * name,
                                        Widget parent,
                                        Widget ** warr_ret);

#define WI_CYL_DIALOG 0
#define WI_PANELHELPTEXT 1
#define WI_CYLFORM 2
#define WI_MAINTEXT 3
#define WI_RECOMMENDEDFORM 4
#define WI_RECOMMENDEDLABEL 5
#define WI_RECOMMENDEDFRAME 6
#define WI_RECOMMENDEDVALUE 7
#define WI_MINIMUMFORM 8
#define WI_MINIMUMLABEL 9
#define WI_MINIMUMFRAME 10
#define WI_MINIMUMVALUE 11
#define WI_CYLMATRIXFRAME 12
#define WI_CYLMATRIXFORM 13
#define WI_CYLENTRYROWCOLUMN 14
#define WI_DISKLABEL 15
#define WI_HEADINGLABEL 16
#define WI_LABELROWCOLUMN 17
#define WI_OVERHEADLABEL 18
#define WI_ALLOCATEDLABEL 19
#define WI_FREELABEL 20
#define WI_CAPACITYLABEL 21
#define WI_VALUEROWCOLUMN 22
#define WI_OVERHEADVALUE 23
#define WI_ALLOCATEDVALUE 24
#define WI_FREEVALUE 25
#define WI_CAPACITYVALUE 26
#define WI_MESSAGEBOX 27
#define WI_OKBUTTON 28
#define WI_LOADBUTTON 29
#define WI_CANCELBUTTON 30
#define WI_HELPBUTTON 31
#define WI_BUTTON5 32

#define WI_CYL_SLICE_ENTRY 0
#define WI_SLICELABEL 1
#define WI_MOUNTTEXT 2
#define WI_SIZETEXT 3
#define WI_STARTTEXT 4
#define WI_ENDTEXT 5


#endif
