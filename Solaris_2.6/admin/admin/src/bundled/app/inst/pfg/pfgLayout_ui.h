
/****************************************************************
 *
 * Name      : pfgLayout_ui.h
 * Creator   : mtraub
 *
 * File is generated by TeleUSE
 * Version : TeleUSE v3.0.1 / Solaris 2.3
 *
 ****************************************************************/

#ifndef _PFGLAYOUT_UI_H_
#define _PFGLAYOUT_UI_H_

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>

#include "tu_runtime.h"


/* widget creation functions */

extern Widget tu_autoLayoutDialog_widget(char * name,
                                         Widget parent,
                                         Widget ** warr_ret);

extern Widget tu_full_table_entry_widget(char * name,
                                         Widget parent,
                                         Widget ** warr_ret);

extern Widget tu_editable_table_entry_widget(char * name,
                                             Widget parent,
                                             Widget ** warr_ret);

extern Widget tu_full_editable_entry_widget(char * name,
                                            Widget parent,
                                            Widget ** warr_ret);

#define WI_AUTOLAYOUTDIALOG 0
#define WI_PANELHELPTEXT 1
#define WI_HEADER_FORM 2
#define WI_BASIC_COLUMN_LABELS 3
#define WI_SLICE_NAME_LABEL 4
#define WI_REC_SIZE_LABEL 5
#define WI_CREATE_SIZE_LABEL 6
#define WI_OPTIONS_COLUMN_LABELS 7
#define WI_DISKS_LABEL 8
#define WI_SLICES_LABEL 9
#define WI_AUTOLAYOUTFORM 10
#define WI_AUTOLAYOUTSCROLLWIN 11
#define WI_MAIN_TABLE_RC 12
#define WI_BASIC_BUTTON 13
#define WI_OPTIONS_BUTTON 14
#define WI_AUTOLAYOUTMESSAGEBOX 15
#define WI_AUTOLAYOUTCONTINUE 16
#define WI_BUTTON_NOT_USED 17
#define WI_AUTOLAYOUTCANCEL 18
#define WI_AUTOLAYOUTEXIT 19
#define WI_AUTOLAYOUTHELP 20

#define WI_FULL_TABLE_ENTRY 0
#define WI_BASIC_TABLE_ENTRY 1
#define WI_SELECT_SLICE_NAME 2
#define WI_RECOMMENDED_SIZE 3
#define WI_CREATE_SIZE 4
#define WI_OPTIONS_TABLE_ENTRY 5
#define WI_DISK_OPTION 6
#define WI_DISK_OPTION_LIST 7
#define WI_ANY_DISK 8
#define WI_SLICE_OPTION 9
#define WI_SLICE_OPTION_LIST 10
#define WI_ANY_SLICE 11
#define WI_SLICE_CHOICE0 12
#define WI_SLICE_CHOICE1 13
#define WI_SLICE_CHOICE2 14
#define WI_SLICE_CHOICE3 15
#define WI_SLICE_CHOICE4 16
#define WI_SLICE_CHOICE5 17
#define WI_SLICE_CHOICE6 18
#define WI_SLICE_CHOICE7 19

#define WI_EDITABLE_TABLE_ENTRY 0
#define WI_RECOMMENDED_SIZE1 1
#define WI_CREATE_SIZE1 2
#define WI_BLANK_NAME_ENTRY 3
#define WI_SELECT_SLICE_NAME1 4
#define WI_NEW_SLICE_NAME 5

#define WI_FULL_EDITABLE_ENTRY 0
#define WI_EDITABLE_TABLE_ENTRY1 1
#define WI_RECOMMENDED_SIZE2 2
#define WI_CREATE_SIZE2 3
#define WI_BLANK_NAME_ENTRY1 4
#define WI_SELECT_SLICE_NAME2 5
#define WI_NEW_SLICE_NAME1 6
#define WI_OPTIONS_TABLE_ENTRY1 7
#define WI_DISK_OPTION1 8
#define WI_DISK_OPTION_LIST1 9
#define WI_ANY_DISK1 10
#define WI_SLICE_OPTION1 11
#define WI_SLICE_OPTION_LIST1 12
#define WI_ANY_SLICE1 13
#define WI_SLICE_CHOICE8 14
#define WI_SLICE_CHOICE9 15
#define WI_SLICE_CHOICE10 16
#define WI_SLICE_CHOICE11 17
#define WI_SLICE_CHOICE12 18
#define WI_SLICE_CHOICE13 19
#define WI_SLICE_CHOICE14 20
#define WI_SLICE_CHOICE15 21


#endif
