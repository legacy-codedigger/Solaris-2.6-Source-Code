/*
 * Copyright (c) 1991 by Sun Microsystems, Inc.
 */

#ifndef	_SYS_BUFMOD_H
#define	_SYS_BUFMOD_H

#pragma ident	"@(#)bufmod.h	1.6	93/09/08 SMI"

#ifdef	__cplusplus
extern "C" {
#endif

/*
 * Definitions for the STREAMS Buffering module.
 *
 * The module gathers incoming (read-side) messages together into
 * "chunks" and passes completed chunks up to the next module in
 * line.  The gathering process is controlled by two ioctl-settable
 * parameters:
 *
 * timeout	The maximum delay after passing the previous chunk
 *		upward before passing the current one up, even if the
 *		chunk isn't full.  If the timeout value passed in is
 *		a null pointer, the timeout is infinite (as in the
 *		select system call); this is the default.
 * chunksize	The maximum size of a chunk of accumulated messages,
 *		unless a single message exceeds the chunksize, in
 *		which case it's passed up in a chunk containing only
 *		that message.  Note that a given message's size includes
 *		the length of any leading M_PROTO blocks it may have.
 *
 * There is one important side-effect: setting the timeout to zero
 * (polling) will force the chunksize to zero, regardless of its
 * previous setting.
 */

/*
 * Ioctls.
 */
#define	SBIOC	('B' << 8)
#define	SBIOCSTIME	(SBIOC|1)	/* set timeout info */
#define	SBIOCGTIME	(SBIOC|2)	/* get timeout info */
#define	SBIOCCTIME	(SBIOC|3)	/* clear timeout */
#define	SBIOCSCHUNK	(SBIOC|4)	/* set chunksize */
#define	SBIOCGCHUNK	(SBIOC|5)	/* get chunksize */
#define	SBIOCSSNAP	(SBIOC|6)	/* set snapshot length */
#define	SBIOCGSNAP	(SBIOC|7)	/* get snapshot length */
#define	SBIOCSFLAGS	(SBIOC|8)	/* set buffering modes */
#define	SBIOCGFLAGS	(SBIOC|9)	/* get buffering modes */

/*
 * Default chunk size.
 */
#define	SB_DFLT_CHUNK	8192	/* arbitrary */

/*
 * buffering flags
 */

#define	SB_SEND_ON_WRITE	1	/* return buffered read data on write */
#define	SB_NO_HEADER		2	/* don't add header structure to data */
#define	SB_NO_PROTO_CVT		4	/* don't convert proto to data */
#define	SB_DEFER_CHUNK		8	/* fast response time buffering */
#define	SB_NO_DROPS		16	/* Don't drop messages */

/*
 * buffering state
 */
#define	SB_FRCVD	1	/* first message in time window received */

/*
 * When adding a given message to an accumulating chunk, the module
 * first converts any leading M_PROTO data block to M_DATA.
 * It then constructs an sb_hdr (defined below), prepends it to
 * the message, and pads the result out to force its length to a
 * multiple of a machine-dependent alignment size guaranteed to be
 * at least sizeof (u_long).  It then adds the padded message to the
 * chunk.
 *
 * sb_origlen is the original length of the message after the M_PROTO => M_DATA
 * conversion, but before truncating or adding the header.
 *
 * sb_msglen is the length of the message after truncation, but before
 * adding the header.
 *
 * sb_totlen is the length of the message after truncation, and including
 * both the header itself and the trailing padding bytes.
 *
 * sb_drops is the cumulative number of messages dropped by the module
 * due to stream read-side flow control or resource exhaustion.
 *
 * sb_timestamp is the packet arrival time expressed as a 'struct timeval'.
 */
struct sb_hdr {
	u_int	sbh_origlen;
	u_int	sbh_msglen;
	u_int	sbh_totlen;
	u_int	sbh_drops;
	struct	timeval	sbh_timestamp;
};

#ifdef	__cplusplus
}
#endif

#endif	/* _SYS_BUFMOD_H */
