#
#ident	"@(#)Makefile	1.9	96/04/20 SMI"
#
# Copyright (c) 1989,1996 by Sun Microsystems, Inc.
# All rights reserved.
#

PROG= du
XPG4PROG= du

include ../Makefile.cmd

CFLAGS += -v
$(XPG4) := CFLAGS += -DXPG4
CPPFLAGS += -D_FILE_OFFSET_BITS=64

.KEEP_STATE:

all: $(PROG) $(XPG4)

install: all $(ROOTPROG) $(ROOTXPG4PROG)

clean:

lint:	lint_PROG

include ../Makefile.targ
