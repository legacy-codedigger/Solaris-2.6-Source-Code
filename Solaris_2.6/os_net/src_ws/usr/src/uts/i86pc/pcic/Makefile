#
# uts/i86pc/pcic/Makefile
# Copyright (c) 1994 by Sun Microsystems, Inc.
#ident "@(#)Makefile	1.5	95/06/27 SMI"
#
#	This makefile drives the PCIC style PCMCIA adapter
#	It is mostly a standard driver
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
MODULE		= pcic
OBJECTS		= $(PCIC_OBJS:%=$(OBJS_DIR)/%)
LINTS		= $(PCIC_OBJS:%.o=$(LINTS_DIR)/%.ln)
ROOTMODULE	= $(ROOT_DRV_DIR)/$(MODULE)
CONF_SRCDIR	= $(UTSBASE)/common/io

#
#	Include common rules.
#
include $(UTSBASE)/i86pc/Makefile.i86pc

CLOBBERFILES	+= $(MODULE)

#
# lint pass one enforcement
#
CFLAGS		+= -v

#
#	Default build targets.
#
.KEEP_STATE:

all:		$(MODULE) $(SRC_CONFILE)

lint:		$(MODULE).lint

install:  	$(MODULE) $(ROOTMODULE) $(ROOT_CONFFILE)

#
#	Include common targets.
#
include $(UTSBASE)/i86pc/Makefile.targ
