#pragma ident	"@(#)aio.c	1.6	96/08/21 SMI"
/*
 * Copyright (c) 1993-1996, by Sun Microsystems, Inc.
 * All Rights reserved.
 *
 * The POSIX async. I/O functionality is
 * implemented in libaio/common/posix_aio.c
 */


#include "synonyms.h"
#include <aio.h>
#include <errno.h>

int
aio_cancel(int fildes, struct aiocb *aiocbp)
{
	return (__aio_cancel(fildes, aiocbp));
}

int
aio_cancel64(int fildes, struct aiocb64 *aiocbp)
{
	return (__aio_cancel64(fildes, aiocbp));
}

#undef aio_error

int
aio_error(const struct aiocb *aiocbp)
{
	return (__aio_error(aiocbp));
}

#undef aio_error64

int
aio_error64(const struct aiocb64 *aiocbp)
{
	return (__aio_error64(aiocbp));
}

int
aio_fsync(int op, struct aiocb *aiocbp)
{
	return (__aio_fsync(aiocbp));
}

int
aio_fsync64(int op, struct aiocb64 *aiocbp)
{
	return (__aio_fsync64(aiocbp));
}

int
aio_read(struct aiocb *aiocbp)
{
	return (__aio_read(aiocbp));
}

int
aio_read64(struct aiocb64 *aiocbp)
{
	return (__aio_read64(aiocbp));
}

#undef aio_return

ssize_t
aio_return(struct aiocb *aiocbp)
{
	return (__aio_return(aiocbp));
}

#undef aio_return64

ssize_t
aio_return64(struct aiocb64 *aiocbp)
{
	return (__aio_return64(aiocbp));
}

int
aio_suspend(const struct aiocb * const list[], int nent,
    const struct timespec *timeout)
{
	return (__aio_suspend(list, nent, timeout));
}

int
aio_suspend64(const struct aiocb64 * const list[], int nent,
    const struct timespec *timeout)
{
	return (__aio_suspend64(list, nent, timeout));
}

int
aio_write(struct aiocb *aiocbp)
{
	return (__aio_write(aiocbp));
}

int
aio_write64(struct aiocb64 *aiocbp)
{
	return (__aio_write64(aiocbp));
}

int
lio_listio(int mode, struct aiocb * const list[], int nent,
	struct sigevent *sig)
{
	return (__lio_listio(mode, list, nent, sig));
}

int
lio_listio64(int mode, struct aiocb64 * const list[], int nent,
	struct sigevent *sig)
{
	return (__lio_listio64(mode, list, nent, sig));
}
