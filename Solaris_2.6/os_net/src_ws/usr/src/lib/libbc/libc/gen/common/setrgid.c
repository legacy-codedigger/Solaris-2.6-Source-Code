#pragma ident	"@(#)setrgid.c	1.3	92/07/21 SMI"
	  /* from UCB 4.1 83/06/30 */

setrgid(rgid)
	int rgid;
{

	return (setregid(rgid, -1));
}
