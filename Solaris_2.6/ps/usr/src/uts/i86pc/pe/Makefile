#
# uts/i86pc/pe/Makefile
# Copyright (c) 1994-1995 by Sun Microsystems, Inc.
# All Rights Reserved.
#
#pragma ident  "@(#)Makefile  1.4     95/06/30 SMI"
#
#	This makefile drives the production of the Xircom Pocket Ethernet
#	driver kernel module.
#
#	i86pc architecture dependent
#

#
#	Path to the base of the uts directory tree (usually /usr/src/uts).
#
UTSBASE	= ../..

#
#	Define the module and object file sets.
#
MODULE		= pe
OBJECTS		= $(PE_OBJS:%=$(OBJS_DIR)/%)
LINTS		= $(PE_OBJS:%.o=$(LINTS_DIR)/%.ln)
ROOTMODULE	= $(ROOT_DRV_DIR)/$(MODULE)
CONF_SRCDIR	= $(UTSBASE)/i86pc/io/pe

INC_PATH += -I$(UTSBASE)/i86pc/sys/pe

#
#	Include common rules.
#
include $(UTSBASE)/i86pc/Makefile.i86pc

DEBUG_FLGS      =
$(NOT_RELEASE_BUILD)DEBUG_DEFS	+= $(DEBUG_FLGS)
# EXTRA_OPTIONS and EXTRA_CFLAGS are the correct variables to which
# driver-specific cpp and compiler flags should be appended to the cc
# command (see ../../Makefile.uts and ../../../Makefile.master)
EXTRA_OPTIONS   += -DSUNDDI -Dfar= -Dout=outw -Din=inw
EXTRA_CFLAGS    +=
CLOBBERFILES	+= $(MODULE)

#
#	Default build targets.
#
.KEEP_STATE:

.PARALLEL: $(OBJECTS)

all:		$(MODULE) $(SRC_CONFILE)

lint:		$(MODULE).lint

install:  	$(MODULE) $(ROOTMODULE) $(ROOT_CONFFILE)

#
#	Include common targets.
#
include $(UTSBASE)/i86pc/Makefile.targ
