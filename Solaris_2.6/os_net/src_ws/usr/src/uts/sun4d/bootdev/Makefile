#
# uts/sun4d/bootdev/Makefile
# Copyright (c) 1995 by Sun Microsystems, Inc.
#
#ident   "@(#)Makefile 1.3     96/05/21 SMI"

#
#	This makefile drives the production of the bootdev misc kernel
#	module.
#

#
#	Path to the base of the uts directory tree (usually /usr/src/uts).
#
UTSBASE	= ../..

#
#	Define the module and object file sets.
#
MODULE		= bootdev
OBJECTS		= $(BOOTDEV_OBJS:%=$(OBJS_DIR)/%)
LINTS		= $(BOOTDEV_OBJS:%.o=$(LINTS_DIR)/%.ln)
ROOTMODULE	= $(ROOT_PSM_MISC_DIR)/$(MODULE)

#
#	Include common rules.
#
include $(UTSBASE)/sun4d/Makefile.sun4d

CLOBBERFILES	+= $(MODULE)

#
# lint pass one enforcement
#
CFLAGS += -v

#
#	Default build targets.
#
.KEEP_STATE:

all:		$(MODULE)

lint:		$(MODULE).lint

install:  	$(MODULE) $(ROOTMODULE)

#
#	Include common targets.
#
include $(UTSBASE)/sun4d/Makefile.targ
