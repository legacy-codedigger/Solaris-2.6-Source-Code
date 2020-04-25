
/****************************************************************
 *
 * Name      : pfgPlatformSelector_ui.h
 * Creator   : mtraub
 *
 * File is generated by TeleUSE
 * Version : TeleUSE v3.0.1 / Solaris 2.3
 *
 ****************************************************************/

#ifndef _PFGPLATFORMSELECTOR_UI_H_
#define _PFGPLATFORMSELECTOR_UI_H_

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>

#include "tu_runtime.h"


/* widget creation functions */

extern Widget tu_platform_selector_widget(char * name,
                                          Widget parent,
                                          Widget ** warr_ret);

extern Widget tu_entry_table_widget(char * name,
                                    Widget parent,
                                    Widget ** warr_ret);

extern Widget tu_entry_pair_widget(char * name,
                                   Widget parent,
                                   Widget ** warr_ret);

#define WI_PLATFORM_SELECTOR 0
#define WI_PS_MESSAGEBOX 1
#define WI_PS_CONTINUE 2
#define WI_PS_GOBACK 3
#define WI_PS_CANCEL 4
#define WI_PS_EXIT 5
#define WI_PS_HELP 6
#define WI_PS_PANELHELPTEXT 7
#define WI_COLUMN_FORM 8
#define WI_OS_LABEL 9
#define WI_ARCH_LABEL 10
#define WI_LOCALE_LABEL 11
#define WI_MNTPOINT_LABEL 12
#define WI_SIZE_LABEL 13
#define WI_FORM 14
#define WI_PLATFORM_SCROLL 15
#define WI_ENTRY_TABLE 16

#define WI_ENTRY_TABLE1 0

#define WI_ENTRY_PAIR 0


#endif