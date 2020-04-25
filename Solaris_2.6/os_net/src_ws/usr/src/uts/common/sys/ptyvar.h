/*
 * Copyright (c) 1989 by Sun Microsystems, Inc.
 */

/*
 * Pseudo-tty driver data structures.
 */

#ifndef	_SYS_PTYVAR_H
#define	_SYS_PTYVAR_H

#pragma ident	"@(#)ptyvar.h	1.8	93/08/24 SMI"

#ifdef	__cplusplus
extern "C" {
#endif

struct pty {
	long	pt_flags;		/* flag bits */
	mblk_t	*pt_stuffqfirst;	/* head of queue for ioctls */
	mblk_t	*pt_stuffqlast;		/* tail of queue for ioctls */
	int	pt_stuffqlen;		/* number of bytes of queued ioctls */
	tty_common_t pt_ttycommon;	/* data common to all tty drivers */
	int	pt_wbufcid;		/* id of pending write-side bufcall */
	struct proc *pt_selr;		/* proc selecting on controller read */
	struct proc *pt_selw;		/* proc selecting on controller write */
	struct proc *pt_sele;		/* proc selecting on exception */
	dev_t	pt_sdev;		/* XXX dev no for the slave */
	struct stdata *pt_stream;	/* XXX stream for the slave */
	short	pt_pgrp;		/* controller side process group */
	u_char	pt_send;		/* pending message to controller */
	u_char	pt_ucntl;		/* pending iocontrol for controller */
};

#define	PF_RCOLL	0x00000001	/* > 1 process selecting for read */
#define	PF_WCOLL	0x00000002	/* > 1 process selecting for write */
#define	PF_ECOLL	0x00000004	/* > 1 process selecting for excep. */
#define	PF_NBIO		0x00000008	/* non-blocking I/O on controller */
#define	PF_ASYNC	0x00000010	/* asynchronous I/O on controller */
#define	PF_WOPEN	0x00000020	/* waiting for open to complete */
#define	PF_CARR_ON	0x00000040	/* "carrier" is on (cntlr. is open) */
#define	PF_SLAVEGONE	0x00000080	/* slave was open, but is now closed */
#define	PF_PKT		0x00000100	/* packet mode */
#define	PF_STOPPED	0x00000200	/* user told stopped */
#define	PF_REMOTE	0x00000400	/* remote and flow controlled input */
#define	PF_NOSTOP	0x00000800	/* slave is doing XON/XOFF */
#define	PF_UCNTL	0x00001000	/* user control mode */
#define	PF_43UCNTL	0x00002000	/* real 4.3 user control mode */

/*
 * M_CTL message types.
 */
#define	MC_NOCANON	0	/* module below saying it will canonicalize */
#define	MC_DOCANON	1	/* module below saying it won't canonicalize */
#define	MC_CANONQUERY	2	/* module above asking whether module below */
				/* canonicalizes */
#define	MC_SERVICEIMM	3	/* tell the ZS driver to return input */
				/* immediately */
#define	MC_SERVICEDEF	4	/* tell the ZS driver it can wait */
#define	MC_NOIFLAG	5	/* module below saying don't do i flags */
#define	MC_NOOFLAG	6	/* module below saying don't do o flags */
#define	MC_NOLFLAG	7	/* module below saying don't do l flags */


/*
 * Thus stuff is from the 4.1 termios.h, but we don't want to
 * put it in the real system, so we hide it here.
 */

/*
 * Sun version of winsize.
 */
struct ttysize {
	int	ts_lines;		/* number of lines on terminal */
	int	ts_cols;		/* number of columns on terminal */
};


#define	TIOCPKT		_IOW('t', 112, int)	/* pty: set/clear packet mode */
#define		TIOCPKT_DATA		0x00	/* data packet */
#define		TIOCPKT_FLUSHREAD	0x01	/* flush data not yet written */
						/* to controller */
#define		TIOCPKT_FLUSHWRITE	0x02	/* flush data read from */
						/* controller but not yet */
						/* processed */
#define		TIOCPKT_STOP		0x04	/* stop output */
#define		TIOCPKT_START		0x08	/* start output */
#define		TIOCPKT_NOSTOP		0x10	/* no more ^S, ^Q */
#define		TIOCPKT_DOSTOP		0x20	/* now do ^S, ^Q */
#define		TIOCPKT_IOCTL		0x40	/* "ioctl" packet */

#define	TIOCUCNTL	_IOW('t', 102, int)	/* pty: set/clr usr cntl mode */
#define	TIOCTCNTL	_IOW('t', 32, int)	/* pty: set/clr intercept */
						/* ioctl mode */
#define	TIOCISPACE	_IOR('t', 128, int)	/* space left in input queue */
#define	TIOCISIZE	_IOR('t', 129, int)	/* size of input queue */

#define	TIOCSSIZE	_IOW('t', 37, struct ttysize) /* set tty size */
#define	TIOCGSIZE	_IOR('t', 38, struct ttysize) /* get tty size */

#ifdef _KERNEL

/*
 * pty_softc, npty, ptcph, and pty_initspace are defined in tty_ptyconf.c
 */

extern int	npty;
extern struct	pty *pty_softc;
extern struct	pollhead ptcph;
extern void	pty_initspace(void);

/*
 * define the Berkeley style naming convention
 */
#define	PTY_BANKS	"pqrstuvwxyz"
#define	PTY_DIGITS	"0123456789abcdef"
#endif /* _KERNEL */

#ifdef	__cplusplus
}
#endif

#endif	/* _SYS_PTYVAR_H */
