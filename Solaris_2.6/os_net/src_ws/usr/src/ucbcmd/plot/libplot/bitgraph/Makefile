#
#ident	"@(#)Makefile	1.2	92/12/16 SMI"
#
# Copyright (c) 1989 by Sun Microsystems, Inc.
#

LIBRARY= ../libplotbg.a

OBJECTS = arc.o box.o circle.o close.o cont.o dot.o erase.o label.o \
	line.o linemod.o move.o open.o point.o space.o 

# include library definitions
include ../../../../lib/Makefile.lib

BUILD.AR = $(AR) $(ARFLAGS) $@ `$(LORDER) $(AROBJS) | $(TSORT)`

# TXTS allows the AT&T makefile to be bootstrapped into the NSE.

CLOBBERFILES= $(LIBRARY)

.KEEP_STATE:

all: $(LIBS)

include ../../../../lib/Makefile.targ

