#
# uts/ppc/inst_sync/Makefile
# Copyright (c) 1994 by Sun Microsystems, Inc.
#
#ident	"@(#)Makefile	1.1	91/02/14 SMI"
#
#	This makefile drives the production of the inst_sync system call module.
#
#	ppc processor architecture dependent
#

#
#	Path to the base of the uts directory tree (usually /usr/src/uts).
#
UTSBASE	= ../..

#
#	Define the module and object file sets.
#
MODULE		= inst_sync
OBJECTS		= $(INSTANCE_OBJS:%=$(OBJS_DIR)/%)
LINTS		= $(INSTANCE_OBJS:%.o=$(LINTS_DIR)/%.ln)
ROOTMODULE	= $(ROOT_SYS_DIR)/$(MODULE)

#
#	Include common rules.
#
include $(UTSBASE)/ppc/Makefile.ppc

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
include $(UTSBASE)/ppc/Makefile.targ
