# Copyright 1990-93 GROUPE BULL -- See license conditions in file COPYRIGHT
#
# XPM Makefile - Arnaud LE HORS
#

CC = cc
AR = ar r
RANLIB = /bin/true
RM = rm -f
# on sysV, define this as cp.
#INSTALL = install -f
INSTALL = cp

CDEBUGFLAGS= -O

# if your system doesn't provide strcasecmp add -DNEED_STRCASECMP
# if your system doesn't provide strdup add -DNEED_STRDUP
# if your system doesn't provide pipe remove -DZPIPE
DEFINES = -DZPIPE  -DSVR4 -DSYSV

DESTBINDIR=/usr/local/bin/X11
#DESTLIBDIR=$(ROOT)/usr/snadm/lib
#DESTINCLUDEDIR=$(ROOT)/usr/include/admin
DESTLIBDIR=../..
DESTINCLUDEDIR=../..

OBJS= data.o create.o misc.o rgb.o scan.o parse.o hashtable.o \
      XpmWrFFrP.o XpmRdFToP.o XpmCrPFrData.o XpmCrDataFrP.o \
      XpmWrFFrI.o XpmRdFToI.o XpmCrIFrData.o XpmCrDataFrI.o \
      XpmCrIFrBuf.o XpmCrPFrBuf.o XpmCrBufFrI.o XpmCrBufFrP.o \
      XpmRdFToData.o XpmWrFFrData.o

X_CFLAGS = -I$(OPENWINHOME)/include
MOTIF_CFLAGS = -I$(MOTIFHOME)/include
CFLAGS= $(CDEBUGFLAGS) $(DEFINES) $(X_CFLAGS) $(MOTIF_CFLAGS)

all: libXpm.a

clean: 
	$(RM) *.o libXpm.a

libXpm.a: $(OBJS)
	$(AR) libXpm.a $(OBJS)
	$(RANLIB) libXpm.a

install: libXpm.a
	$(INSTALL) libXpm.a $(DESTLIBDIR)
	chmod 0664 $(DESTLIBDIR)/libXpm.a 
	cd $(DESTLIBDIR); $(RANLIB) libXpm.a
	$(INSTALL) xpm.h $(DESTINCLUDEDIR) 
	chmod 0664 $(DESTINCLUDEDIR)/xpm.h

# Other dependencies.
XpmCrBufFrI.o:  xpmP.h xpm.h
XpmCrBufFrP.o:  xpmP.h xpm.h
XpmCrDataFI.o:  xpmP.h xpm.h
XpmCrDataFP.o:  xpmP.h xpm.h
XpmCrIFrBuf.o:  xpmP.h xpm.h
XpmCrIFrData.o:  xpmP.h xpm.h
XpmCrPFrBuf.o:  xpmP.h xpm.h
XpmCrPFrData.o:  xpmP.h xpm.h
XpmRdFToData.o:  xpmP.h xpm.h
XpmRdFToI.o:  xpmP.h xpm.h
XpmRdFToP.o:  xpmP.h xpm.h
XpmWrFFrData.o:  xpmP.h xpm.h
XpmWrFFrI.o:  xpmP.h xpm.h
XpmWrFFrP.o:  xpmP.h xpm.h
create.o:  xpmP.h xpm.h
data.o:  xpmP.h xpm.h
hashtable.o:  xpmP.h xpm.h
misc.o:  xpmP.h xpm.h
parse.o:  xpmP.h xpm.h
rgb.o:  xpmP.h xpm.h
scan.o:  xpmP.h xpm.h
