#
# uts/i86/doorfs/Makefile
# Copyright (c) 1991-1994, by Sun Microsystems, Inc.
#
#ident	"@(#)Makefile 95/08/10 1.2 SMI"
#
#	This makefile drives the production of the semsys driver kernel module.
#
#	x86 architecture dependent
#

#
#	Path to the base of the uts directory tree (usually /usr/src/uts).
#
UTSBASE	= ../..

DOORFS_OBJS	+= door_vnops.o	door_sys.o door_support.o

#
#	Define the module and object file sets.
#
MODULE		= doorfs
OBJECTS		= $(DOORFS_OBJS:%=$(OBJS_DIR)/%)
LINTS		= $(DOORFS_OBJS:%.o=$(LINTS_DIR)/%.ln)
ROOTMODULE	= $(ROOT_SYS_DIR)/$(MODULE)

#
#	Include common rules.
#

include $(UTSBASE)/i86/Makefile.i86

#
#	Overrides.
#
MODSTUBS_DIR	 = $(OBJS_DIR)
$(MODSTUBS_O)	:= AS_CPPFLAGS +=
CLEANFILES	+= $(MODSTUBS_O)

CLOBBERFILES	+= $(MODULE)
CFLAGS		+= -v

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
include $(UTSBASE)/i86/Makefile.targ

#
# rules
#
$(OBJS_DIR)/door_sys.o:      $(UTSBASE)/common/fs/doorfs/door_sys.c
	$(COMPILE.c) -o $@ $(UTSBASE)/common/fs/doorfs/door_sys.c

$(OBJS_DIR)/door_vnops.o:     $(UTSBASE)/common/fs/doorfs/door_vnops.c
	$(COMPILE.c) -o $@ $(UTSBASE)/common/fs/doorfs/door_vnops.c
 
$(OBJS_DIR)/door_support.o:   $(UTSBASE)/i86/os/door_support.c
	$(COMPILE.c) -o $@ $(UTSBASE)/i86/os/door_support.c

#
# Lint targets
#
$(LINTS_DIR)/door_sys.ln:    $(UTSBASE)/common/fs/doorfs/door_sys.c
	@($(LHEAD) $(LINT.c) $(UTSBASE)/common/fs/doorfs/door_sys.c $(LTAIL))
 
$(LINTS_DIR)/door_vnops.ln:   $(UTSBASE)/common/fs/doorfs/door_vnops.c
	@($(LHEAD) $(LINT.c) $(UTSBASE)/common/fs/doorfs/door_vnops.c $(LTAIL))
 
$(LINTS_DIR)/door_support.ln: $(UTSBASE)/i86/os/door_support.c
	@($(LHEAD) $(LINT.c) $(UTSBASE)/i86/os/door_support.c $(LTAIL))

