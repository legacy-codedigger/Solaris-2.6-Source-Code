/*
 * Copyright (c) 1991 Sun Microsystems, Inc.  All Rights Reserved. Sun
 * considers its source code as an unpublished, proprietary trade secret, and
 * it is available only under strict license provisions.  This copyright
 * notice is placed here only to protect Sun in the event the source is
 * deemed a published work.  Dissassembly, decompilation, or other means of
 * reducing the object code to human readable form is prohibited by the
 * license agreement under which this code is provided to the user or company
 * in possession of this copy.
 *
 * RESTRICTED RIGHTS LEGEND: Use, duplication, or disclosure by the Government
 * is subject to restrictions as set forth in subparagraph (c)(1)(ii) of the
 * Rights in Technical Data and Computer Software clause at DFARS 52.227-7013
 * and in similar clauses in the FAR and NASA FAR Supplement.
 */

#ifndef lint
#ident	"@(#)Base_ui.h 1.4 93/04/06"
#endif

#ifndef	Base_HEADER
#define	Base_HEADER

/*
 * Base_ui.h - User interface object and function declarations.
 * This file was generated by `gxv' from `Base.G'.
 * DO NOT EDIT BY HAND.
 */

#ifndef SVR4
EXTERN_FUNCTION( char *bindtextdomain, (const char *, const char *) );
EXTERN_FUNCTION( char *dgettext, (const char *, const char *) );
#else
#include <locale.h>
#endif

extern Attr_attribute	INSTANCE;

extern Xv_opaque	Base_FileMenu_create(caddr_t, Xv_opaque);
extern Xv_opaque	Base_ViewMenu_create(caddr_t, Xv_opaque);
extern Xv_opaque	Base_EditMenu_create(caddr_t, Xv_opaque);
extern Xv_opaque	Base_BaseMenu_create(caddr_t, Xv_opaque);

typedef struct {
	Xv_opaque	BaseWin;
	Xv_opaque	BaseCtrl;
	Xv_opaque	BaseMode;
	Xv_opaque	BaseModeGrp;
	Xv_opaque	BaseFile;
	Xv_opaque	BaseView;
	Xv_opaque	BaseEdit;
	Xv_opaque	BaseProps;
	Xv_opaque	BaseTop;
	Xv_opaque	BaseDoit;
	Xv_opaque	BaseBottom;
	Xv_opaque	BaseCat;
	Xv_opaque	BaseCatGrp;
	Xv_opaque	Levels;
	Xv_opaque	Mods;
} Base_BaseWin_objects;

extern Base_BaseWin_objects	*Base_BaseWin_objects_initialize(Base_BaseWin_objects *, Xv_opaque);

extern Xv_opaque	Base_BaseWin_BaseWin_create(Base_BaseWin_objects *, Xv_opaque);
extern Xv_opaque	Base_BaseWin_BaseCtrl_create(Base_BaseWin_objects *, Xv_opaque);
extern Xv_opaque	Base_BaseWin_BaseMode_create(Base_BaseWin_objects *, Xv_opaque);
extern Xv_opaque	Base_BaseWin_BaseModeGrp_create(Base_BaseWin_objects *, Xv_opaque);
extern Xv_opaque	Base_BaseWin_BaseFile_create(Base_BaseWin_objects *, Xv_opaque);
extern Xv_opaque	Base_BaseWin_BaseView_create(Base_BaseWin_objects *, Xv_opaque);
extern Xv_opaque	Base_BaseWin_BaseEdit_create(Base_BaseWin_objects *, Xv_opaque);
extern Xv_opaque	Base_BaseWin_BaseProps_create(Base_BaseWin_objects *, Xv_opaque);
extern Xv_opaque	Base_BaseWin_BaseTop_create(Base_BaseWin_objects *, Xv_opaque);
extern Xv_opaque	Base_BaseWin_BaseDoit_create(Base_BaseWin_objects *, Xv_opaque);
extern Xv_opaque	Base_BaseWin_BaseBottom_create(Base_BaseWin_objects *, Xv_opaque);
extern Xv_opaque	Base_BaseWin_BaseCat_create(Base_BaseWin_objects *, Xv_opaque);
extern Xv_opaque	Base_BaseWin_BaseCatGrp_create(Base_BaseWin_objects *, Xv_opaque);
extern Xv_opaque	Base_BaseWin_Levels_create(Base_BaseWin_objects *, Xv_opaque);
extern Xv_opaque	Base_BaseWin_Mods_create(Base_BaseWin_objects *, Xv_opaque);
#endif
