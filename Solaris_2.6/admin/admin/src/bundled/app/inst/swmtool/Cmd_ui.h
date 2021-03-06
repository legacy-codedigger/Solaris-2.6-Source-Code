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
#ident	"@(#)Cmd_ui.h 1.1 92/10/08"
#endif

#ifndef	Cmd_HEADER
#define	Cmd_HEADER

/*
 * Cmd_ui.h - User interface object and function declarations.
 * This file was generated by `gxv' from `Cmd.G'.
 * DO NOT EDIT BY HAND.
 */

#ifndef SVR4
EXTERN_FUNCTION( char *bindtextdomain, (const char *, const char *) );
EXTERN_FUNCTION( char *dgettext, (const char *, const char *) );
#else
#include <locale.h>
#endif

extern Attr_attribute	INSTANCE;


typedef struct {
	Xv_opaque	CmdWin;
	Xv_opaque	Term;
} Cmd_CmdWin_objects;

extern Cmd_CmdWin_objects	*Cmd_CmdWin_objects_initialize(Cmd_CmdWin_objects *, Xv_opaque);

extern Xv_opaque	Cmd_CmdWin_CmdWin_create(Cmd_CmdWin_objects *, Xv_opaque);
extern Xv_opaque	Cmd_CmdWin_Term_create(Cmd_CmdWin_objects *, Xv_opaque);
#endif
