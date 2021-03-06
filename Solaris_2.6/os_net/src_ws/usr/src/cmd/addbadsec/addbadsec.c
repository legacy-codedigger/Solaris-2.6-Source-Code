/*	Copyright (c) 1990, 1991 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1984, 1986, 1987, 1988, 1989, 1990 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident "@(#)addbadsec.c	1.8	93/04/08 SMI"

/*
 * Copyrighted as an unpublished work.
 * (c) Copyright INTERACTIVE Systems Corporation 1986, 1988, 1990
 * All rights reserved.
 *
 * RESTRICTED RIGHTS
 *
 * These programs are supplied under a license.  They may be used,
 * disclosed, and/or copied only as permitted under such license
 * agreement.  Any copy must contain the above copyright notice and
 * this restricted rights notice.  Use, copying, and/or disclosure
 * of the programs is strictly prohibited unless otherwise provided
 * in the license agreement.
 */


#include <stdio.h>
#include <fcntl.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/mkdev.h>
#include <sys/vtoc.h>
#include <sys/dkio.h>
#include <sys/errno.h>
#include <sys/scsi/generic/commands.h>
#include <sys/scsi/impl/commands.h>
#include <sys/scsi/impl/uscsi.h>
#include "badsec.h"

char    *devname;		/* name of device */
int	devfd;			/* device file descriptor */
struct  dk_geom      	dkg;	/* geometry */
struct  vtoc	vtoc;		/* table of contents */
char	*progname;

extern	struct	badsec_lst *badsl_chain;
extern	int	badsl_chain_cnt;
extern	struct	badsec_lst *gbadsl_chain;
extern	int	gbadsl_chain_cnt;

extern int	errno;

int		alts_fd;

void
main(argc, argv)
int	argc;
char	*argv[];
{
	extern int	optind;
	extern char	*optarg;

	static char     options[] = "Ipa:f:";
	char		numbuf[100];
	char		*nxtarg;
	char		*alts_name;
	minor_t 	minor_val;
	struct stat 	statbuf;
	struct partition	*part = NULL;
	int		alts_slice = -1;
	int		l;
	int		p;
	int		init_flag = 0;
	int		print_flag = 0;
	int 		c;
	int 		i;
	FILE		*badsecfd = NULL;
	struct badsec_lst *blc_p;

	progname = argv[0];
	while ( (c=getopt(argc, argv, options)) != EOF ) {
		switch (c) {
		case 'I':
			init_flag = 1;
			break;
		case 'p':
			print_flag = 1;
			break;
		case 'a':
			nxtarg = optarg;
			for (;*nxtarg != '\0';)
				add_gbad(strtol(nxtarg, &nxtarg, 0));
			break;
		case 'f':
			if ((badsecfd = fopen(optarg, "r")) == NULL) {
				fprintf(stderr, "%s: unable to open %s file\n", progname, optarg);
				exit(1);
			}
			break;
		default:
			giveusage();
			exit(2);
		}
	}

		/* get the last argument -- device stanza */
	if (argc != optind+1) {
		fprintf(stderr, "Missing disk device name\n");
		giveusage();
		exit(3);
	}
	devname = argv[optind];

	if (stat(devname, &statbuf)) {
		fprintf(stderr, "%s: invalid device %s, stat failed\n", progname, devname);
		giveusage();
		exit(4);
	}
	if ((statbuf.st_mode & S_IFMT) != S_IFCHR) {
		fprintf(stderr, "%s: device %s is not character special\n", progname, devname);
		giveusage();
		exit(5);
	}
	minor_val = minor(statbuf.st_rdev);
	/*
	 * NEED A DEFINE FOR THE PHYSICAL BIT (0x10)
	 */
	if ((minor_val & 0x10) == 0) {
		fprintf(stderr, "%s: device %s is not a physical slice\n", progname, devname);
		giveusage();
		exit(6);
	}
	if ((minor_val % V_NUMPAR) != 0) {
		fprintf(stderr, "%s: device %s is not a slice 0 device\n", progname, devname);
		giveusage();
		exit(7);
	}
	if ((devfd=open(devname, O_RDWR)) == -1) {
		fprintf(stderr, "%s: open of %s failed\n", progname ,devname);
		perror("");
		exit(8);
	}
	if ((ioctl (devfd, DKIOCGGEOM, &dkg)) == -1) {
		fprintf(stderr, "%s: unable to get disk geometry.\n", progname);
		perror("");
		exit(9);
	}

	if (ioctl(devfd, DKIOCGVTOC, &vtoc) == -1)
	{
		fprintf(stderr, "%s: could not get VTOC.\n", progname);
		giveusage();
		exit(14);
	}

	if ((vtoc.v_sanity != VTOC_SANE) || (vtoc.v_version != V_VERSION)) {
		fprintf(stderr, "%s: invalid VTOC found.\n", progname);
		giveusage();
		exit(15);
	}
	if (badsecfd)
		rd_gbad(badsecfd);

#ifdef ADDBAD_DEBUG
	printf("\n main: Total bad sectors found= %d\n", gbadsl_chain_cnt);
	for (blc_p=gbadsl_chain; blc_p; blc_p=blc_p->bl_nxt) {
		for (i=0; i<blc_p->bl_cnt; i++)
			printf(" badsec=%d ", blc_p->bl_sec[i]);
	}
	printf("\n");
#endif
#ifdef PPP
	/*
	 * If init_flag is set, run to completion.
	 */
	if (gbadsl_chain_cnt == 0 && init_flag == 0)
		/*
		 * No defects and not initializing
		 */
		exit (0);
#endif
	if (gbadsl_chain_cnt != 0)
	{
		if (try_hw_remap () == SUCCESS)
			exit (0);
	}
	/*
	 * get ALTS slice
	 */
	for (i = 0; i < V_NUMPAR && alts_slice == -1; i++)
	{
		if (vtoc.v_part[i].p_tag == V_ALTSCTR)
		{
			alts_slice = i;
			part = &vtoc.v_part[i];
		}
	}
	if (alts_slice == -1)
	{
		fprintf(stderr, "%s: No alternates slice.\n", progname);
		exit(16);
	}
	l = strlen (devname);
	sprintf (numbuf, "%d", alts_slice);
	p = strlen (numbuf);
	alts_name = (char *)malloc (l + p);
	strcpy (alts_name, devname);
	alts_name[l - 2] = 's';
	strcpy (&alts_name[l - 1], numbuf);
	alts_name[l + p - 1] = '\0';
	if ((alts_fd=open(alts_name, O_RDWR)) == -1) {
		fprintf(stderr, "%s: open of %s failed\n", progname ,alts_name);
		perror("");
		exit(9);
	}
	if (print_flag)
	{
		print_altsec (part);
		exit (0);
	}
	updatebadsec(part, init_flag);
	wr_altsctr();

	if (ioctl(devfd, DKIOCADDBAD, NULL) == -1) {
		fprintf(stderr,  "Warning: DKIOCADDBAD io control failed. System must be re-booted\n");
		fprintf(stderr, "for alternate sectors to be usable.\n");
		exit(17);
	}
	sync();

	fclose(badsecfd);
	close (alts_fd);
	close (devfd);
	exit(0);
}

/*
 * Giveusage ()
 * Give a (not so) concise message on how to use this program.
 */
giveusage()
{
	fprintf(stderr, "%s [-p] [-a sector] [-f filename] raw-device\n", progname);
	fprintf(stderr, "	p - Print existing bad block map\n");
	fprintf(stderr, "	a - Add the given sectors to the bad block list\n");
	fprintf(stderr, "	f - Add the sectors from <filename> to the bad block list\n");
	if (devfd)
		close(devfd);
}


/*
 *	read in the additional growing bad sectors 
 */
rd_gbad(badsecfd)
FILE	*badsecfd;
{
	int	badsec_entry;
	int	status;

	status = fscanf(badsecfd, "%d", &badsec_entry);
	while (status!=EOF) {
		add_gbad(badsec_entry);
		status = fscanf(badsecfd, "%d", &badsec_entry);
	}
}

add_gbad(badsec_entry)
int	badsec_entry;
{
	struct badsec_lst *blc_p;

	if (!gbadsl_chain) {
		blc_p = (struct badsec_lst *)malloc(BADSLSZ);
		if (!blc_p) {
			fprintf(stderr, "Unable to allocate memory for additional bad sectors\n");
			exit(18);
		}
		gbadsl_chain = blc_p;
		blc_p->bl_cnt = 0;
		blc_p->bl_nxt = 0;
	}
	for (blc_p = gbadsl_chain; blc_p->bl_nxt; )
		blc_p = blc_p->bl_nxt;
				
	if (blc_p->bl_cnt == MAXBLENT) {
		blc_p->bl_nxt = (struct badsec_lst *)malloc(BADSLSZ);
		if (!blc_p->bl_nxt) {
			fprintf(stderr, "Unable to allocate memory for additional bad sectors\n");
			exit(19);
		}
		blc_p = blc_p->bl_nxt;
		blc_p->bl_cnt = 0;
		blc_p->bl_nxt = 0;
	}
	blc_p->bl_sec[blc_p->bl_cnt++] = badsec_entry;
	gbadsl_chain_cnt++;
}

/*
 * Map a block using hardware (SCSI) techniques.
 */
/*ARGSUSED*/
int
hardware_remap (bn)
int	bn;
{
	u_int	byte_swap_32 (u_int);
	u_short	byte_swap_16 (u_short);

	struct uscsi_cmd		ucmd;
	union scsi_cdb			cdb;
	struct scsi_reassign_blk	defect_list;

	/*
	 * Build and execute the uscsi ioctl
	 */
	(void) memset((char *)&ucmd, 0, sizeof (ucmd));
	(void) memset((char *)&cdb, 0, sizeof (union scsi_cdb));
	(void) memset((char *)&defect_list, 0,
		sizeof (struct scsi_reassign_blk));
	cdb.scc_cmd = SCMD_REASSIGN_BLOCK;
	ucmd.uscsi_cdb = (caddr_t) &cdb;
	ucmd.uscsi_cdblen = CDB_GROUP0;
	ucmd.uscsi_bufaddr = (caddr_t) &defect_list;
	ucmd.uscsi_buflen = sizeof (struct scsi_reassign_blk);
	defect_list.length = byte_swap_16 (sizeof (defect_list.defect));
	defect_list.defect = byte_swap_32 (bn);
	/*
	printf ("length - %x %x\n", sizeof (defect_list.defect), defect_list.length);
	printf ("defect - %x %x\n", bn, defect_list.defect);
	*/
	/*
	 * Set function flags for driver.
	 */
	ucmd.uscsi_flags = USCSI_ISOLATE | USCSI_DIAGNOSE | USCSI_SILENT;
	ucmd.uscsi_timeout = 30;		/* 30 seconds */

	/*
	 * Execute the ioctl
	 */
	if (ioctl(devfd, USCSICMD, &ucmd) == -1)
	{
		if (errno != ENOTTY)
		{
			perror ("SCSI hardware re-assign failed");
			/*
			 * It looks like a failure but by returning success
			 * the upper layer will not try to do
			 * software remapping.
			 */
			return (SUCCESS);
		}
		return (FAILURE);
	}
	return (SUCCESS);
}

u_int
byte_swap_32 (u_int nav)
{
	u_int	rc;
	rc = ((nav & 0xff000000) >> 24) | ((nav & 0x00ff0000) >> 8) |
	     ((nav & 0x0000ff00) << 8)  | ((nav & 0x000000ff) << 24);
	return (rc);
}

u_short
byte_swap_16 (u_short niv)
{
	u_short	rc;
	rc = (u_short)((int)(niv & 0xff00) >> 8) | ((niv & 0x00ff) << 8);
	return (rc);
}

try_hw_remap ()
{
	struct badsec_lst *blc_p;
	int	i;

	for (blc_p = gbadsl_chain; blc_p != 0; blc_p = blc_p->bl_nxt)
	{
				
		for (i = 0; i < blc_p->bl_cnt; i++)
		if (hardware_remap (blc_p->bl_sec[i]) == FAILURE)
			return (FAILURE);
	}
	return (SUCCESS);
}
