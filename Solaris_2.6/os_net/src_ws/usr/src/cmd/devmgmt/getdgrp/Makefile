#
#ident	"@(#)Makefile	1.5	92/12/15 SMI"
#
# Copyright (c) 1990 by Sun Microsystems, Inc.
#
# cmd/devmgmt/getdgrp/Makefile
#

PROG= getdgrp

include ../../Makefile.cmd

LDLIBS += -ladm

.KEEP_STATE:

all: $(PROG) 

$(PROG): main.c
	$(LINK.c) -o $@ main.c $(LDLIBS)
	$(POST_PROCESS)

install: all $(ROOTPROG)

clean:

lint:   lint_PROG

include ../../Makefile.targ
