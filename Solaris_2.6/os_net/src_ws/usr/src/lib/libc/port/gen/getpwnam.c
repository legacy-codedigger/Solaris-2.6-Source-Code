/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	Copyright (c) 1991, 1992 Sun Microsystems, Inc.	*/

#ident	"@(#)getpwnam.c	1.17	92/10/01 SMI"        /* SVr4.0 1.19  */

/*LINTLIBRARY*/
#ifdef __STDC__
	#pragma weak getpwnam = _getpwnam
	#pragma weak getpwuid = _getpwuid
	#pragma weak getpwent = _getpwent
	#pragma weak fgetpwent = _fgetpwent
#endif
#include "synonyms.h"
#include "shlib.h"
#include <pwd.h>
#include <nss_dbdefs.h>
#include <stdio.h>

#ifdef	NSS_INCLUDE_UNSAFE

/*
 * Ye olde non-reentrant interface (MT-unsafe, caveat utor)
 */

/*
 * Don't free this, even on an endpwent(), because bitter experience shows
 *   that there's production code that does getXXXbyYYY(), then endXXXent(),
 *   and then continues to use the pointer it got back.
 */
static nss_XbyY_buf_t *buffer;
#define GETBUF()	\
	NSS_XbyY_ALLOC(&buffer, sizeof (struct passwd), NSS_BUFLEN_PASSWD)
	/* === ?? set ENOMEM on failure?  */

struct passwd *
getpwuid(uid)
	uid_t	uid;
{
	nss_XbyY_buf_t	*b = GETBUF();

	return (b == 0 ? 0 : getpwuid_r(uid, b->result, b->buffer, b->buflen));
}

struct passwd *
getpwnam(nam)
	const char	*nam;
{
	nss_XbyY_buf_t	*b = GETBUF();

	return (b == 0 ? 0 : getpwnam_r(nam ,b->result, b->buffer, b->buflen));
}

struct passwd *
getpwent()
{
	nss_XbyY_buf_t	*b = GETBUF();

	return (b == 0 ? 0 : getpwent_r(b->result, b->buffer, b->buflen));
}

struct passwd *
fgetpwent(f)
	FILE *f;
{
	nss_XbyY_buf_t	*b = GETBUF();

	return (b == 0 ? 0 : fgetpwent_r(f, b->result, b->buffer, b->buflen));
}

#endif	NSS_INCLUDE_UNSAFE
