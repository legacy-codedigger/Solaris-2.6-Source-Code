#
#ident	"@(#)Makefile	1.8	96/04/18 SMI"
#
# Copyright (c) 1989,1996 by Sun Microsystems, Inc.
# All rights reserved.
#
# cmd/fs.d/ufs/repquota/Makefile

FSTYPE=		ufs
LIBPROG=	repquota
ATTMK=		$(LIBPROG)

OTHERINSTALL=	$(ROOTUSRSBIN)/$(LIBPROG)
LINKVALUE=	../lib/fs/$(FSTYPE)/$(LIBPROG)

include		../../Makefile.fstype

CPPFLAGS +=	-D_LARGEFILE64_SOURCE
lint:		lint_PROG

$(ROOTUSRSBIN)/$(LIBPROG):
		-$(RM) $@; $(SYMLINK) $(LINKVALUE) $@
