/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *		PROPRIETARY NOTICE (Combined)
 *
 * This source code is unpublished proprietary information
 * constituting, or derived under license from AT&T's UNIX(r) System V.
 * In addition, portions of such source code were derived from Berkeley
 * 4.3 BSD under license from the Regents of the University of
 * California.
 *
 *
 *
 *		Copyright Notice
 *
 * Notice of copyright on this source code product does not indicate
 * publication.
 *
 *	(c) 1986,1987,1988,1989,1994,1996  Sun Microsystems, Inc
 *	(c) 1983,1984,1985,1986,1987,1988,1989  AT&T.
 *		All rights reserved.
 *
 *
 * BIND 4.9.3
 *
 * ++Copyright++ 1983, 1989, 1993
 * -
 * Copyright (c) 1983, 1989, 1993
 *    The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 * 	This product includes software developed by the University of
 * 	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * -
 * Portions Copyright (c) 1993 by Digital Equipment Corporation.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies, and that
 * the name of Digital Equipment Corporation not be used in advertising or
 * publicity pertaining to distribution of the document or software without
 * specific, written prior permission.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND DIGITAL EQUIPMENT CORP. DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS.   IN NO EVENT SHALL DIGITAL EQUIPMENT
 * CORPORATION BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 * --Copyright--
 *
 * End BIND 4.9.3
 */

#ifndef _ARPA_NAMESER_H
#define	_ARPA_NAMESER_H

#pragma ident	"@(#)nameser.h	1.16	96/05/09 SMI"

#include <sys/isa_defs.h>

#ifdef	__cplusplus
extern "C" {
#endif

/* BIND 4.9.3 */
#define	__BIND		19950621	/* interface version stamp */
/* End BIND 4.9.3 */

/*
 * Define constants based on rfc883
 */
#define	PACKETSZ	512		/* maximum packet size */
#define	MAXDNAME	256		/* maximum domain name */
#define	MAXCDNAME	255		/* maximum compressed domain name */
#define	MAXLABEL	63		/* maximum length of domain label */
#define	QFIXEDSZ	4		/* #/bytes of fixed data in query */
#define	RRFIXEDSZ	10		/* #/bytes of fixed data in r record */
/* BIND 4.9.3 */
#define	HFIXEDSZ	12		/* #/bytes of fixed data in header */
#define	INT32SZ		4		/* for systems without 32-bit ints */
#define	INT16SZ		2		/* for systems without 16-bit ints */
#define	INADDRSZ	4		/* for sizeof(struct inaddr) != 4 */
/* End BIND 4.9.3 */

/*
 * Internet nameserver port number
 */
#define	NAMESERVER_PORT	53

/*
 * Currently defined opcodes
 */
#define	QUERY		0x0		/* standard query */
#define	IQUERY		0x1		/* inverse query */
#define	STATUS		0x2		/* nameserver status query */
/* #define xxx		0x3 */		/* 0x3 reserved */
#define	NS_NOTIFY_OP	0x4		/* notify secondary of SOA change */
#ifdef ALLOW_UPDATES
	/* non standard - supports ALLOW_UPDATES stuff from Mike Schwartz */
#define	UPDATEA		0x9		/* add resource record */
#define	UPDATED		0xa		/* delete a specific resource record */
#define	UPDATEDA	0xb		/* delete all named resource record */
#define	UPDATEM		0xc		/* modify a specific resource record */
#define	UPDATEMA	0xd		/* modify all named resource record */
#define	ZONEINIT	0xe		/* initial zone transfer */
#define	ZONEREF		0xf		/* incremental zone referesh */
#endif

/*
 * Currently defined response codes
 */
#define	NOERROR		0		/* no error */
#define	FORMERR		1		/* format error */
#define	SERVFAIL	2		/* server failure */
#define	NXDOMAIN	3		/* non existent domain */
#define	NOTIMP		4		/* not implemented */
#define	REFUSED		5		/* query refused */
#ifdef ALLOW_UPDATES
	/* non standard */
#define	NOCHANGE	0xf		/* update failed to change db */
#endif

/*
 * Type values for resources and queries
 */
#define	T_A		1		/* host address */
#define	T_NS		2		/* authoritative server */
#define	T_MD		3		/* mail destination */
#define	T_MF		4		/* mail forwarder */
#define	T_CNAME		5		/* canonical name */
#define	T_SOA		6		/* start of authority zone */
#define	T_MB		7		/* mailbox domain name */
#define	T_MG		8		/* mail group member */
#define	T_MR		9		/* mail rename name */
#define	T_NULL		10		/* null resource record */
#define	T_WKS		11		/* well known service */
#define	T_PTR		12		/* domain name pointer */
#define	T_HINFO		13		/* host information */
#define	T_MINFO		14		/* mailbox information */
#define	T_MX		15		/* mail routing information */
#define	T_TXT		16		/* text strings */
/* BIND 4.9.3 */
#define	T_RP		17		/* responsible person */
#define	T_AFSDB		18		/* AFS cell database */
#define	T_X25		19		/* X_25 calling address */
#define	T_ISDN		20		/* ISDN calling address */
#define	T_RT		21		/* router */
#define	T_NSAP		22		/* NSAP address */
#define	T_NSAP_PTR	23		/* reverse NSAP lookup (deprecated) */
#define	T_SIG		24		/* security signature */
#define	T_KEY		25		/* security key */
#define	T_PX		26		/* X.400 mail mapping */
#define	T_GPOS		27		/* geographical position (withdrawn) */
#define	T_AAAA		28		/* IP6 Address */
#define	T_LOC		29		/* Location Information */
/* End BIND 4.9.3 */
	/* non standard */
#define	T_UINFO		100		/* user (finger) information */
#define	T_UID		101		/* user ID */
#define	T_GID		102		/* group ID */
#define	T_UNSPEC	103		/* Unspecified format (binary data) */
	/* Query type values which do not appear in resource records */
#define	T_AXFR		252		/* transfer zone of authority */
#define	T_MAILB		253		/* transfer mailbox records */
#define	T_MAILA		254		/* transfer mail agent records */
#define	T_ANY		255		/* wildcard match */

/*
 * Values for class field
 */

#define	C_IN		1		/* the arpa internet */
#define	C_CHAOS		3		/* for chaos net at MIT */
#define	C_HS		4		/* for Hesiod name server at MIT */
	/* Query class values which do not appear in resource records */
#define	C_ANY		255		/* wildcard match */

/*
 * Status return codes for T_UNSPEC conversion routines
 */
#define	CONV_SUCCESS 0
#define	CONV_OVERFLOW -1
#define	CONV_BADFMT -2
#define	CONV_BADCKSUM -3
#define	CONV_BADBUFLEN -4

/*
 * Structure for query header, the order of the fields is machine and
 * compiler dependent, in our case, the bits within a byte are assignd
 * least significant first, while the order of transmition is most
 * significant first.  This requires a somewhat confusing rearrangement.
 */

typedef struct {
	unsigned	id:16;		/* query identification number */
#if defined(_BIT_FIELDS_HTOL) || defined(BIT_ZERO_ON_LEFT)
	/* Bit zero on left:  SPARC and similar architectures */
			/* fields in third byte */
	unsigned	qr:1;		/* response flag */
	unsigned	opcode:4;	/* purpose of message */
	unsigned	aa:1;		/* authoritive answer */
	unsigned	tc:1;		/* truncated message */
	unsigned	rd:1;		/* recursion desired */
			/* fields in fourth byte */
	unsigned	ra:1;		/* recursion available */
	unsigned	pr:1;		/* primary server required (non	*/
					/* standard; not supported in	*/
					/* BIND 4.9.3)			*/
					/* pr not supported in BIND 4.9.3 */
	unsigned	unused:2;	/* unused bits */
	unsigned	rcode:4;	/* response code */
#else
#if defined(_BIT_FIELDS_LTOH) || defined(BIT_ZERO_ON_RIGHT)
	/* Bit zero on right:  Intel x86 and similar architectures */
			/* fields in third byte */
	unsigned	rd:1;		/* recursion desired */
	unsigned	tc:1;		/* truncated message */
	unsigned	aa:1;		/* authoritive answer */
	unsigned	opcode:4;	/* purpose of message */
	unsigned	qr:1;		/* response flag */
			/* fields in fourth byte */
	unsigned	rcode:4;	/* response code */
	unsigned	unused:2;	/* unused bits */
	unsigned	pr:1;		/* primary server required (non	*/
					/* standard; not supported in	*/
					/* BIND 4.9.3)			*/
					/* pr not supported in BIND 4.9.3 */
	unsigned	ra:1;		/* recursion available */
#else
	/* you must determine what the correct bit order is for your compiler */
	UNDEFINED_BIT_ORDER;
#endif
#endif
			/* remaining bytes */
	unsigned	qdcount:16;	/* number of question entries */
	unsigned	ancount:16;	/* number of answer entries */
	unsigned	nscount:16;	/* number of authority entries */
	unsigned	arcount:16;	/* number of resource entries */
} HEADER;

/*
 * Defines for handling compressed domain names
 */
#define	INDIR_MASK	0xc0

/*
 * Structure for passing resource records around.
 */
struct rrec {
	short	r_zone;			/* zone number */
	short	r_class;		/* class number */
	short	r_type;			/* type number */
	u_long	r_ttl;			/* time to live */
	int	r_size;			/* size of data area */
	char	*r_data;		/* pointer to data */
};

extern	u_short	_getshort();
extern	u_int	_getlong();

/*
 * Inline versions of get/put short/long.
 * Pointer is advanced; we assume that both arguments
 * are lvalues and will already be in registers.
 * cp MUST be u_char *.
 */
#define	GETSHORT(s, cp) { \
	(s) = *(cp)++ << 8; \
	(s) |= *(cp)++; \
}

#define	GETLONG(l, cp) { \
	(l) = *(cp)++ << 8; \
	(l) |= *(cp)++; (l) <<= 8; \
	(l) |= *(cp)++; (l) <<= 8; \
	(l) |= *(cp)++; \
}


#define	PUTSHORT(s, cp) { \
	*(cp)++ = (s) >> 8; \
	*(cp)++ = (s); \
}

/*
 * Warning: PUTLONG destroys its first argument.
 */
#define	PUTLONG(l, cp) { \
	(cp)[3] = l; \
	(cp)[2] = (l >>= 8); \
	(cp)[1] = (l >>= 8); \
	(cp)[0] = l >> 8; \
	(cp) += sizeof (u_long); \
}

#ifdef	__cplusplus
}
#endif

#endif /* _ARPA_NAMESER_H */
