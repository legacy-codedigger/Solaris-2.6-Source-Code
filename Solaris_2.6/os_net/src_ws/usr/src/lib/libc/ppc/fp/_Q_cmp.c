/*
 * Copyright (c) 1988, 1989, 1991, by Sun Microsystems, Inc.
 */

#ident	"@(#)_Q_cmp.c	1.7	94/09/02 SMI"

#include "synonyms.h"
#include "_Qquad.h"
#include "_Qglobals.h"

extern  _Q_set_exception();

enum fcc_type
_q_cmp(const quadruple *x, const quadruple *y)
{
	unpacked	px, py, pz;
	enum fcc_type	fcc;
	unsigned 	cexc = 0;
	_fp_unpack(&px, x, fp_op_extended, &cexc);
	_fp_unpack(&py, y, fp_op_extended, &cexc);
	fcc = _fp_compare(&px, &py, 0, &cexc); /* quiet NaN unexceptional */
	_Q_set_exception(cexc);
	return (fcc);
}
