#
# uts/i86/winlock/Makefile
# Copyright (c) 1991-1994, by Sun Microsystems, Inc.
#
#ident	"@(#)Makefile	1.3	96/05/27 SMI"
#
#	This makefile drives the production of winlock kernel module.
#
#	architecture independent

#
#	Path to the base of the uts directory tree (usually /usr/src/uts).
#
UTSBASE	= ../..

#
#	Define the module and object file sets.
#
MODULE		= winlock
OBJECTS		= $(WINLOCK_OBJS:%=$(OBJS_DIR)/%)
LINTS		= $(WINLOCK_OBJS:%.o=$(LINTS_DIR)/%.ln)
ROOTMODULE	= $(USR_DRV_DIR)/$(MODULE)
CONF_SRCDIR	= $(UTSBASE)/common/io

#
#	Include common rules.
#
include $(UTSBASE)/i86/Makefile.i86

#
#	Overrides.
#
CLOBBERFILES	+= $(MODULE)

#
# lint pass one enforcement
#  
CFLAGS += -v

#
#	Default build targets.
#
.KEEP_STATE:

all:		$(MODULE) $(SRC_CONFILE)

lint:		$(MODULE).lint


install:	$(MODULE) $(ROOTMODULE) $(ROOT_CONFFILE)

#
#	Include common targets.
#
include $(UTSBASE)/i86/Makefile.targ
