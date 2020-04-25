/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)ttyadm.c	1.9	95/01/12 SMI"	/* SVr4.0 1.10	*/


# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <ctype.h>
# include <sys/stat.h>
# include "tmstruct.h"
# include "ttymon.h"

char	Scratch[BUFSIZ];

/*
 *	ttyadm 	- format ttymon specific information and
 *		  print it to stdout
 *
 *	Usage: 	ttyadm [options] -d device -s service -l ttylabel
 *		ttyadm -V
 *			
 *		valid options are:
 *		-c
 *		-h
 *		-b
 *		-I
 *		-S y|n
 *		-T term
 *		-r count
 *		-t timeout
 *		-p prompt
 *		-m modules
 *		-i msg
 */

static void usage();
static	int	check_label();

extern	int	check_device();
extern	int	check_cmd();
extern	int	vml();

int
main(int argc, char *argv[])
{
	int c;			/* option letter */
	int errflg = 0;		/* error indicator */

	struct pmtab *ptr;
	char *timeout = "";
	char *count = "";
	char prompt[BUFSIZ];
	char dmsg[BUFSIZ];
	char ttyflags[BUFSIZ], *tf;

	int  dflag = 0;		/* -d seen */
	int  sflag = 0;		/* -s seen */
	int  lflag = 0;		/* -l seen */
	int  mflag = 0;		/* -m seen */

	extern	void 	copystr();
	extern	char	*optarg;
	extern	int	optind;
	extern	int	strcheck();

	if (argc == 1)
		usage();
	if ((ptr = ALLOC_PMTAB) == PNULL) {
		(void)fprintf(stderr, "calloc failed\n");
		exit(1);
	}
	ptr->p_modules = "";
	ptr->p_dmsg = "";
	ptr->p_termtype = "";
	ptr->p_softcar = "";
	ptr->p_prompt = "login\\: ";
	ttyflags[0] = '\0';
	tf = ttyflags;
	while ((c = getopt(argc, argv, "IT:S:Vd:s:chbr:t:l:m:p:i:")) != -1) {
		switch (c) {
		case 'V':
			if ((argc > 2) || (optind < argc))
				usage();
			(void)fprintf(stdout,"%d\n", PMTAB_VERS);
			exit(0);
			break;	/*NOTREACHED*/
		case 'd':
			ptr->p_device = optarg;
			dflag = 1;
			break;
		case 'c':
			tf = strcat(tf,"c");
			break;
		case 'h':
			tf = strcat(tf,"h");
			break;
		case 'b':
			tf = strcat(tf,"b");
			break;
		case 'I':
			tf = strcat(tf,"I");
			break;
		case 'r':
			tf = strcat(tf,"r");
			count = optarg;
			if (strcheck(optarg,NUM) != 0) {
				(void)fprintf(stderr, 
		"Invalid argument for \"-r\" -- positive number expected.\n");
				usage();
			}
			break;
		case 'T':
			ptr->p_termtype = optarg;
			break;
		case 'S':
			switch (*optarg) {
			case 'Y':
			case 'y':
				ptr->p_softcar = "y";
				break;
			case 'N':
			case 'n':
				ptr->p_softcar = "n";
				break;
			default:
				usage();
			}
			break;
		case 's':
			ptr->p_server = optarg;
			sflag = 1;
			break;
		case 't':
			timeout = optarg;
			if (strcheck(optarg,NUM) != 0) {
				(void)fprintf(stderr, 
		"Invalid argument for \"-t\" -- positive number expected.\n");
				usage();
			}
			break;
		case 'l':
			ptr->p_ttylabel = optarg;
			lflag = 1;
			break;
		case 'm':
			ptr->p_modules = optarg;
			mflag = 1;
			break;
		case 'p':
			ptr->p_prompt = prompt;
			copystr(ptr->p_prompt,optarg);
			break;
		case 'i':
			ptr->p_dmsg = dmsg;
			copystr(ptr->p_dmsg,optarg);
			break;
		case '?':
			usage();
			break;
		}
	}
	if (optind < argc)
		usage();

	if ((!dflag) || (!sflag) || (!lflag))
		usage();

	if (check_device(ptr->p_device) != 0)
		errflg++;
	if (check_cmd(ptr->p_server) != 0)
		errflg++;
	if (check_label(ptr->p_ttylabel) != 0)
		errflg++;
	if (mflag && (vml(ptr->p_modules) != 0))
		errflg++;
	if (errflg)
		exit(1);
	(void)fprintf(stdout, "%s:%s:%s:%s:%s:%s:%s:%s:%s:%s:%s:",
			ptr->p_device, ttyflags, count, ptr->p_server,
			timeout, ptr->p_ttylabel, ptr->p_modules,
			ptr->p_prompt, ptr->p_dmsg, ptr->p_termtype,
			ptr->p_softcar);
	return (0);
}

/*
 * usage - print out a usage message
 */

static void
usage()
{
	(void)fprintf(stderr, "Usage:\tttyadm [ options ] -d device -s service -l ttylabel\n");
	(void)fprintf(stderr, "\tttyadm -V\n");
	(void)fprintf(stderr, "\n\tValid options are:\n");
	(void)fprintf(stderr, "\t-c\n");
	(void)fprintf(stderr, "\t-h\n");
	(void)fprintf(stderr, "\t-b\n");
	(void)fprintf(stderr, "\t-I\n");
	(void)fprintf(stderr, "\t-S y|n\n");
	(void)fprintf(stderr, "\t-T term\n");
	(void)fprintf(stderr, "\t-r count\n");
	(void)fprintf(stderr, "\t-t timeout\n");
	(void)fprintf(stderr, "\t-p prompt\n");
	(void)fprintf(stderr, "\t-m modules\n");
	(void)fprintf(stderr, "\t-i msg\n");
	exit(1);
}

/*
 *	check_label	- if ttylabel exists in /etc/ttydefs, return 0
 *			- otherwise, return -1
 */

static int
check_label(ttylabel)
char	*ttylabel;
{
	FILE *fp;
	extern	int	find_label();

	if ((ttylabel == NULL) || (*ttylabel == '\0')) {
		(void)fprintf(stderr, "error -- ttylabel is missing");
		return(-1);
	}
	if ((fp = fopen(TTYDEFS, "r")) == NULL) {
		(void)fprintf(stderr, "error -- \"%s\" does not exist, can't verify ttylabel <%s>\n", TTYDEFS, ttylabel);
		return(-1);
	}
	if (find_label(fp,ttylabel)) {
		(void)fclose(fp);
		return(0);
	}	
	(void)fclose(fp);
	(void)fprintf(stderr,"error -- can't find ttylabel <%s> in \"%s\"\n",
		ttylabel, TTYDEFS);
	return(-1);
}

/*
 *	log	- print a message to stderr
 */

void
log(msg)
char	*msg;
{
	(void)fprintf(stderr,"%s\n", msg);
}