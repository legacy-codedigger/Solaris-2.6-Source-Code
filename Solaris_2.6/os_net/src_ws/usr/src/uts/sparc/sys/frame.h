/*
 * Copyright (c) 1987 by Sun Microsystems, Inc.
 */

#ifndef _SYS_FRAME_H
#define	_SYS_FRAME_H

#pragma ident	"@(#)frame.h	1.8	92/07/14 SMI"	/* sys4-3.2L 1.1 */

#ifdef	__cplusplus
extern "C" {
#endif

/*
 * Definition of the sparc stack frame (when it is pushed on the stack).
 */
struct frame {
	int	fr_local[8];		/* saved locals */
	int	fr_arg[6];		/* saved arguments [0 - 5] */
	struct frame	*fr_savfp;	/* saved frame pointer */
	int	fr_savpc;		/* saved program counter */
	char	*fr_stret;		/* struct return addr */
	int	fr_argd[6];		/* arg dump area */
	int	fr_argx[1];		/* array of args past the sixth */
};

#ifdef	__cplusplus
}
#endif

#endif	/* _SYS_FRAME_H */
