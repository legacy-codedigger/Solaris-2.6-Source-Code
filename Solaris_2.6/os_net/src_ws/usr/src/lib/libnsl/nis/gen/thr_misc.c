/*
 *	thr_misc.c
 *
 *	Copyright (c) 1993 Sun Microsystems Inc
 *	All Rights Reserved.
 */

#pragma ident	"@(#)thr_misc.c	1.9	96/05/22 SMI"

#include "nis_local.h"
#include <errno.h>
#include <rpcsvc/nis.h>
#include <malloc.h>



void *
thr_get_storage(key, size, destructor)
	thread_key_t *key;
	int size;
	void (*destructor)(void *);
{
	void *addr;
	static mutex_t thr_get_storage_lock = DEFAULTMUTEX;

	addr = NULL;
	mutex_lock(&thr_get_storage_lock);
	if (thr_getspecific(*key, &addr) == EINVAL)
		thr_keycreate(key, destructor);

	if (addr == NULL)  {
		if (size > 0) {
			addr = (void *)calloc(1, size);
		} else addr = NULL;
		thr_setspecific(*key, (void *) addr);
	}
	mutex_unlock(&thr_get_storage_lock);
	return (addr);
}

void
thr_sigblock(oset)
	sigset_t *oset;
{
	sigset_t	set = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
	thr_sigsetmask(SIG_SETMASK, &set, oset);
}
