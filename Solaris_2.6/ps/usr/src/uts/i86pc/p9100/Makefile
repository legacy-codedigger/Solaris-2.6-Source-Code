#
# uts/i86pc/p9100/Makefile
# Copyright (c) 1995 by Sun Microsystems, Inc.
#
#ident  "@(#)Makefile 1.2     96/04/12 SMI"
#
#       This makefile drives the production of Weitek p9100 display driver
#	kernel module.
#
#       i86pc implementation architecture dependent
#

#
#	Path to the base of the uts directory tree (usually /usr/src/uts).
#
UTSBASE	= ../..

#
#	Define the module and object file sets.
#
MODULE          = p9100
OBJECTS         = $(P9100_OBJS:%=$(OBJS_DIR)/%)
LINTS           = $(P9100_OBJS:%.o=$(LINTS_DIR)/%.ln)
ROOTMODULE	= $(ROOT_DRV_DIR)/$(MODULE)
CONF_SRCDIR     = $(UTSBASE)/i86pc/io/p9100

#
#	Include common rules.
#
include $(UTSBASE)/i86pc/Makefile.i86pc
 
#
#       Override defaults to build a unique, local modstubs.o.
#

CLOBBERFILES	+= $(MODULE)

#
#	Default build targets.
#
.KEEP_STATE:

all:		$(MODULE)

lint:		$(MODULE).lint


install:  	$(MODULE) $(ROOTMODULE) $(ROOT_CONFFILE)

#
#	Include common targets.
#
include $(UTSBASE)/i86pc/Makefile.targ
