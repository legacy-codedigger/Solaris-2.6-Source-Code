#
# ident	"@(#)Makefile	1.7	94/03/30 SMI"
#
# Copyright (c) 1989 by Sun Microsystems, Inc.
#
# cmd/lp/filter/postscript/download/Makefile
#

include		../../../Makefile.lp

PROG=		download

SRCS =		download.c

OBJS =		$(SRCS:%.c=%.o)

COMMONDIR =	../common

COMMONOBJS =	$(COMMONDIR)/glob.o	\
		$(COMMONDIR)/misc.o	\
		$(COMMONDIR)/tempnam.o

TXTS =		README 

ENCODING =	2
CPPFLAGS =	-DDFLTENCODING=$(ENCODING)	\
		-I. -I$(COMMONDIR)		\
		$(CPPFLAGS.master)

.KEEP_STATE:

all:		$(TXTS) $(PROG)

install:	all $(ROOTLIBLPPOSTPROG)	

$(PROG):	$(OBJS) $(COMMONOBJS)
		$(LINK.c) -o $@ $(OBJS) $(COMMONOBJS) $(LDLIBS)
		$(POST_PROCESS)

$(COMMONOBJS):	$$(@:%.o=%.c)
		cd $(@D); $(MAKE) $(@F)

clean:
		$(RM) $(OBJS)

strip:
		$(STRIP) $(PROG)

lint:		lint_PROG

include		../../../../Makefile.targ
