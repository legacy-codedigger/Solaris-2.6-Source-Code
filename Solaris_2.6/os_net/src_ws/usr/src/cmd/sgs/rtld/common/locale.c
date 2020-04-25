/*
 *	Copyright (c) 1996 by Sun Microsystems, Inc.
 *	All rights reserved.
 */
#pragma ident	"@(#)locale.c	1.4	96/06/07 SMI"

/*
 * Messaging support.  To minimize ld.so.1's overhead, messaging support isn't
 * enabled until we need to contruct a message - Note that we don't rely on the
 * application to signify whether messaging is applicable, as many message
 * conditions (such as relocations) are generated before the application gains
 * control.
 *
 * This code implements a very trimmed down version of the capabilities found
 * via setlocale(3c), textdomain(3i) and gettext(3i).  Dragging in the original
 * routines from libc/libintl isn't possible as they cause all i18n support to
 * be included which is far too expensive for ld.so.1.
 */
#include	"_synonyms.h"

#include	<sys/types.h>
#include	<sys/mman.h>
#include	<sys/stat.h>
#include	<string.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<fcntl.h>
#include	<limits.h>
#include	"_rtld.h"
#include	"msg.h"

/*
 * A message object file (as generated by msgfmt(1)) consists of a message
 * header, followed by a message list, followed by the msgid strings and then
 * the msgstr strings.  None of this is defined in any OSNET available headers
 * so we have our own local definitions :-(
 */
typedef struct {
	int	hdr_midlst;		/* middle message no. */
	int	hdr_lstcnt;		/* total no. of message in the file */
	int	hdr_msgidsz;		/* size of msgids (in bytes) */
	int	hdr_msgstrsz;		/* size of msgstrs (in bytes) */
	int	hdr_lstsz;		/* size of message list (in bytes) */
} Msghdr;

typedef struct {
	int	lst_less;
	int	lst_more;
	int	lst_idoff;
	int	lst_stroff;
} Msglst;

#define	LEAFINDICATOR	-99

/*
 * Define a local structure for maintaining the domains we care about.
 */
typedef struct {
	const char *	dom_name;
	const Msghdr *	dom_msghdr;
} Domain;


/*
 * Perform a binary search of a message file (described by the Msghdr) for a
 * msgid (string).  Given a match return the associated msgstr, otherwise
 * return the original msgid.
 */
static const char *
msgid_to_msgstr(const Msghdr * msghdr, const char * msgid)
{
	const Msglst *	list, * _list;
	const char *	ids, * strs, * _msgid;
	int		off, var;

	/*
	 * Establish pointers to the message list (we actually start the search
	 * in the middle of this list (hdr->midlst), the msgid strings (ids)
	 * and the msgstr strings (strs).
	 */
	list = (const Msglst *)&msghdr[1];
	ids = (const char *)&list[msghdr->hdr_lstcnt];
	strs = (const char *)&ids[msghdr->hdr_msgidsz];

	off = msghdr->hdr_midlst;

	do {
		_list = list + off;
		_msgid = ids + _list->lst_idoff;

		if ((var = strcmp(_msgid, msgid)) == 0)
			return (strs + _list->lst_stroff);

		if (var < 0) {
			if ((off = _list->lst_less) == LEAFINDICATOR)
				return (msgid);
		} else {
			if ((off = _list->lst_more) == LEAFINDICATOR)
				return (msgid);
		}
	} while (off);
	/* NOTREACHED */
}

/*
 * Open a message file. Following the model of setlocale(3c) we obtain the
 * message file for the specified locale.  Normally this is:
 *
 *	/usr/lib/locale/`locale'/LC_MESSAGES/`domain'.mo
 *
 * The locale was determined during initial environment processing (see
 * readenv()), which was determined from an LC_ALL, LC_MESSAGES or LANG
 * setting.  If no locale has been specified, or any file processing errors
 * occur, internationalization is basically disabled.
 */
static const Msghdr *
open_mofile(const char * domain)
{
	char		path[PATH_MAX];
	int		fd;
	struct stat	status;
	const Msghdr *	msghdr;

	(void) sprintf(path, MSG_ORIG(MSG_FMT_MSGFILE), locale, domain);

	if ((fd = open(path, O_RDONLY, 0)) == -1)
		return ((Msghdr *)-1);

	if (fstat(fd, &status) == -1)
		return ((Msghdr *)-1);

	/* LINTED */
	if ((msghdr = (Msghdr *)mmap(0, status.st_size, PROT_READ, MAP_SHARED,
	    fd, 0)) == (Msghdr *)-1)
		return ((Msghdr *)-1);

	(void) close(fd);
	return (msghdr);
}

/*
 * Two interfaces are established to support our internationalization.
 * gettext(3i) calls originate from all link-editor libraries, and thus the
 * SUNW_OST_SGS domain is assumed.  _dgettext() calls originate from
 * dependencies such as libelf and libc.
 *
 * Presently we support two domains (libc's strerror() uses SUNW_OST_OSLIB).
 * If ld.so.1's dependencies evolve to require more then the `domain' array
 * maintained below can be enlarged or made more dynamic in nature.
 */
const char *
_dgettext(const char * domain, const char * msgid)
{
	static int	domaincnt = 0;
	static Domain *	domains, * _domain;
	int		cnt;

	if (locale == 0)
		return (msgid);

	/*
	 * Determine if we've initialized any domains yet.
	 */
	if (domaincnt == 0) {
		if ((domains = (Domain *)calloc(sizeof (Domain), 2)) == 0)
			return (msgid);
		domains[0].dom_name = MSG_ORIG(MSG_SUNW_OST_SGS);
		domains[1].dom_name = MSG_ORIG(MSG_SUNW_OST_OSLIB);
		domaincnt = 2;
	}

	/*
	 * Determine which domain we need.
	 */
	for (cnt = 0, _domain = domains; cnt < domaincnt; _domain++, cnt++) {
		if (_domain->dom_name == domain)
			break;
		if (strcmp(_domain->dom_name, domain) == 0)
			break;
	}
	if (cnt == domaincnt)
		return (msgid);

	/*
	 * Determine if the domain has been initialized yet.
	 */
	if (_domain->dom_msghdr == 0)
		_domain->dom_msghdr = open_mofile(_domain->dom_name);
	if (_domain->dom_msghdr == (Msghdr *)-1)
		return (msgid);

	return (msgid_to_msgstr(_domain->dom_msghdr, msgid));
}

char *
gettext(const char * msgid)
{
	return ((char *)_dgettext(MSG_ORIG(MSG_SUNW_OST_SGS), msgid));
}

/*
 * The sgsmsg.1l use requires the following interface.
 */
const char *
_rtld_msg(int mid)
{
	return ((char *)_dgettext(MSG_ORIG(MSG_SUNW_OST_SGS), MSG_ORIG(mid)));
}
