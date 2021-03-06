#pragma ident	"@(#)clock.c	1.2	92/07/20 SMI"  /* from S5R2 1.1 */

/*LINTLIBRARY*/

#include <sys/types.h>
#include <sys/times.h>
#include <sys/param.h>	/* for HZ (clock frequency in Hz) */
#define TIMES(B)	(B.tms_utime+B.tms_stime+B.tms_cutime+B.tms_cstime)

extern long times();
static long first;

long
clock()
{
	struct tms buffer;

	if (times(&buffer) == -1L)
		return (0L);
	if (first == 0L)
		first = TIMES(buffer);
	return ((TIMES(buffer) - first) * (1000000L/HZ));
}
