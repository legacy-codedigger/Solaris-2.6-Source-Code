#
# uts/sparc/tirdwr/Makefile
# Copyright (c) 1991-1994, by Sun Microsystems, Inc.
#
#ident	"@(#)Makefile	1.7	96/05/24 SMI"
#
#	This makefile drives the production of the tirdwr streams kernel
#	module.
#
#	sparc architecture dependent
#

#
#	Path to the base of the uts directory tree (usually /usr/src/uts).
#
UTSBASE	= ../..

#
#	Define the module and object file sets.
#
MODULE		= tirdwr
OBJECTS		= $(TIRDWR_OBJS:%=$(OBJS_DIR)/%)
LINTS		= $(TIRDWR_OBJS:%.o=$(LINTS_DIR)/%.ln)
ROOTMODULE	= $(ROOT_STRMOD_DIR)/$(MODULE)

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
