#
#ident	"@(#)Makefile	1.11	93/05/25 SMI"
#
# Copyright (c) 1990 by Sun Microsystems, Inc.
#

SGIDPROG=	netstat
PROG=		$(SGIDPROG)

OBJS=		netstat.o unix.o
SRCS= $(OBJS:%.o=%.c)

include ../../../Makefile.cmd

LDLIBS +=	-lsocket -lnsl -lkvm -lelf -lkstat

.KEEP_STATE:

all: $(SGIDPROG) $(NPROG) 

ROOTSGIDPROG=   $(SGIDPROG:%=$(ROOTBIN)/%)
$(ROOTSGIDPROG) :=      FILEMODE=       02555
$(ROOTSGIDPROG) :=      GROUP=          sys

$(PROG): $(OBJS)
	$(LINK.c) $(OBJS) -o $@ $(LDLIBS)
	$(POST_PROCESS)

install: all $(ROOTSGIDPROG) 

clean:
	$(RM) $(OBJS)

lint:	lint_SRCS

include ../../../Makefile.targ

