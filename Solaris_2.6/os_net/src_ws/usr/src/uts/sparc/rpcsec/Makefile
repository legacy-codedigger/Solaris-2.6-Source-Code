#
# Copyright (c) 1995, by Sun Microsystems, Inc.
# All rights reserved.
#
#pragma ident   "@(#)Makefile 1.4     96/05/24 SMI"
#
#	This makefile drives the production of the kernel rpcsec module.
#
#	sparc implementation architecture dependent
#
#
#	Path to the base of the uts directory tree (usually /usr/src/uts).
#
UTSBASE	= ../..

#
#	Define the module and object file sets.
#
MODULE		= rpcsec
OBJECTS		= $(RPCSEC_OBJS:%=$(OBJS_DIR)/%)
LINTS		= $(RPCSEC_OBJS:%.o=$(LINTS_DIR)/%.ln)
ROOTMODULE	= $(ROOT_MISC_DIR)/$(MODULE)

#
#	Include common rules.
#
include $(UTSBASE)/sparc/Makefile.sparc

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

all:		$(MODULE)

lint:		$(MODULE).lint

install:  	$(MODULE) $(ROOTMODULE)

#
#	Include common targets.
#
include $(UTSBASE)/sparc/Makefile.targ
