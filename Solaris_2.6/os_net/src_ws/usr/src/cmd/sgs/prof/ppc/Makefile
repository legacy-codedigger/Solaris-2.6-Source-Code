#
# ident	"@(#)Makefile	1.2	94/01/13 SMI"
#
# Copyright (c) 1994 by Sun Microsystems, Inc.
#
# cmd/sgs/prof/ppc/Makefile
#

PROG=		prof

include 	../../../Makefile.cmd

COMOBJS=	prof.o symintOpen.o symintClose.o symintUtil.o \
		symintErr.o symintLoad.o
DEMOBJS=	dem.o cafe_dem.o

OBJS=		$(COMOBJS) $(DEMOBJS)

SRCS=		$(COMOBJS:%.o=../common/%.c) \
		$(DEMOBJS:%.o=../../c++/demangler/common/%.c)
		

INCLIST=	-I../common -I../../include -I../../include/$(MACH)
DEFLIST=	-DELF
CPPFLAGS=	$(INCLIST) $(DEFLIST) $(CPPFLAGS.master)
LDLIBS +=	-lelf
LINTFLAGS +=	$(LDLIBS)
CLEANFILES +=	$(LINTOUT)

$(DEMOBJS):=	INCLIST = -I../../c++/demangler/common
$(DEMOBJS):=	DEFLIST = -DELF_OBJ

%.o:		../common/%.c
		$(COMPILE.c) $<

%.o:		../../c++/demangler/common/%.c
		$(COMPILE.c) $<

.KEEP_STATE:

all:		$(PROG)

$(PROG):	$(OBJS)
		$(LINK.c) $(OBJS) -o $@ $(LDLIBS)
		$(POST_PROCESS)

install:	all $(ROOTCCSBINPROG)

clean:
		$(RM) $(OBJS) $(CLEANFILES)

lint:		$(LINTOUT)

$(LINTOUT):	$(SRCS)
		$(LINT.c) $(SRCS) > $(LINTOUT) 2>&1

include		../../../Makefile.targ
