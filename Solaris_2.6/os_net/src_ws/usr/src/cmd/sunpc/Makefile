#
#ident	"@(#)Makefile	1.4	92/07/14 SMI"
#
# Copyright (c) 1989 by Sun Microsystems, Inc.
#
# cmd/sunpc/Makefile

SUBDIRS=	other

include ../Makefile.cmd

all:=		TARGET= all
install:=	TARGET= install
clean:=		TARGET= clean
clobber:=	TARGET= clobber
lint:=		TARGET= lint

.KEEP_STATE:

all install clean clobber lint:	$(SUBDIRS)

$(SUBDIRS): FRC
	@cd $@; pwd; $(MAKE) $(TARGET)

FRC:
