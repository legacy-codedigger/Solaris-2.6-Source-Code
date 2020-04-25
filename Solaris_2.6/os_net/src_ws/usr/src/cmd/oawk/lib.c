/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*
 * Copyright (c) 1996, by Sun Microsystems, Inc.
 * All rights reserved.
 */

#ident	"@(#)lib.c	1.10	96/04/18 SMI"

#include "stdio.h"
#include "awk.def"
#include "awk.h"
#include "ctype.h"
#include "wctype.h"
#include "awktype.h"


FILE	*infile	= NULL;
wchar_t *file;
#define	RECSIZE (5 * 512)
wchar_t record[RECSIZE];
wchar_t fields[RECSIZE];
wchar_t L_NULL[] = L"";


#define	MAXFLD	100
int	donefld;	/* 1 = implies rec broken into fields */
int	donerec;	/* 1 = record is valid (no flds have changed) */
int	mustfld;	/* 1 = NF seen, so always break */
static wchar_t L_record[] = L"$record";


#define	FINIT	{ OCELL, CFLD, 0, L_NULL, 0.0, FLD|STR }
CELL fldtab[MAXFLD] = {		/* room for fields */
	{ OCELL, CFLD, L_record, record, 0.0, STR|FLD},
	       FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT,
	FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT,
	FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT,
	FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT,
	FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT,
	FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT,
	FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT,
	FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT,
	FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT,
	FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT, FINIT
};
int	maxfld	= 0;	/* last used field */
CELL	*maxmfld = &fldtab[0];	/* pointer to CELL for maximum field assigned to */




getrec()
{
	register wchar_t *rr, *er;
	register c, sep;
	register FILE *inf;
	extern int svargc;
	extern wchar_t **svargv;


	dprintf("**RS=%o, **FS=%o\n", **RS, **FS, NULL);
	donefld = 0;
	donerec = 1;
	record[0] = 0;
	er = record + RECSIZE;
	while (svargc > 0) {
		dprintf("svargc=%d, *svargv=%ws\n", svargc, *svargv, NULL);
		if (infile == NULL) {	/* have to open a new file */
			if (member('=', *svargv)) {
				/* it's a var=value argument */
				setclvar(*svargv);
				if (svargc > 1) {
					svargv++;
					svargc--;
					continue;
				}
				*svargv = L"-";
			}
			*FILENAME = file = *svargv;
			dprintf("opening file %ws\n", file, NULL, NULL);
			if (*file == (wchar_t) L'-')
				infile = stdin;
			else if ((infile = fopen(toeuccode(file), "r")) == NULL)
				error(FATAL, "can't open %ws", file);
		}
		if ((sep = **RS) == 0)
			sep = '\n';
		inf = infile;
		for (rr = record; /* dummy */; /* dummy */) {
			for (; (c = getwc(inf)) != sep && c != EOF && rr < er;
			    *rr++ = c)
				;
			if (rr >= er)
				error(FATAL, "record `%.20ws...' too long",
				    record);
			if (**RS == sep || c == EOF)
				break;
			if ((c = getwc(inf)) == '\n' || c == EOF)
			/* 2 in a row */
				break;
			*rr++ = '\n';
			*rr++ = c;
		}
		if (rr >= er)
			error(FATAL, "record `%.20ws...' too long", record);
		*rr = 0;
		if (mustfld)
			fldbld();
		if (c != EOF || rr > record) {	/* normal record */
			recloc->tval &= ~NUM;
			recloc->tval |= STR;
			++nrloc->fval;
			nrloc->tval &= ~STR;
			nrloc->tval |= NUM;
			return (1);
		}
		/* EOF arrived on this file; set up next */
		if (infile != stdin)
			fclose(infile);
		infile = NULL;
		svargc--;
		svargv++;
	}
	return (0);	/* true end of file */
}


setclvar(s)	/* set var=value from s */
wchar_t *s;
{
	wchar_t *p;
	CELL *q;


	for (p = s; *p != '='; p++)
		;
	*p++ = 0;
	q = setsymtab(s, tostring(p), 0.0, STR, symtab);
	setsval(q, p);
	dprintf("command line set %ws to |%ws|\n", s, p, NULL);
}


fldbld()
{
	register wchar_t *r, *fr, sep, c;
	static wchar_t L_NF[] = L"NF";
	CELL *p, *q;
	int i, j;


	r = record;
	fr = fields;
	i = 0;	/* number of fields accumulated here */
	if ((sep = **FS) == ' ')
		for (i = 0; /* dummy */; /* dummy */) {
			c = *r;
			while (iswblank(c) || c == '\t' || c == '\n')
				c = *(++r);
			if (*r == 0)
				break;
			i++;
			if (i >= MAXFLD)
				error(FATAL,
			"record `%.20ws...' has too many fields", record);
			if (!(fldtab[i].tval&FLD))
				xfree(fldtab[i].sval);
			fldtab[i].sval = fr;
			fldtab[i].tval = FLD | STR;
			do {
				*fr++ = *r++;
				c = *r;
			} while (! iswblank(c) && c != '\t' &&
				c != '\n' && c != '\0');


			*fr++ = 0;

	} else if (*r != 0)	/* if 0, it's a null field */
		for (;;) {
			i++;
			if (i >= MAXFLD)
				error(FATAL,
			"record `%.20ws...' has too many fields", record);
			if (!(fldtab[i].tval&FLD))
				xfree(fldtab[i].sval);
			fldtab[i].sval = fr;
			fldtab[i].tval = FLD | STR;
			while ((c = *r) != sep && c != '\n' && c != '\0')
				/* \n always a separator */
				*fr++ = *r++;
			*fr++ = 0;
			if (*r++ == 0)
				break;
		}
	*fr = 0;
	/* clean out junk from previous record */
	for (p = maxmfld, q = &fldtab[i]; p > q; p--) {
		if (!(p->tval&FLD))
			xfree(p->sval);
		p->tval = STR | FLD;
		p->sval = L_NULL;
	}
	maxfld = i;
	maxmfld = &fldtab[i];
	donefld = 1;
	for (i = 1; i <= maxfld; i++)
		if (isanumber(fldtab[i].sval)) {
			fldtab[i].fval = watof(fldtab[i].sval);
			fldtab[i].tval |= NUM;
		}
	setfval(lookup(L_NF, symtab, 0), (awkfloat) maxfld);
	if (dbg)
		for (i = 0; i <= maxfld; i++)
			printf("field %d: |%ws|\n", i, fldtab[i].sval);
}


recbld()
{
	int i;
	register wchar_t *r, *p;


	if (donefld == 0 || donerec == 1)
		return;
	r = record;
	for (i = 1; i <= *NF; i++) {
		p = getsval(&fldtab[i]);
		while (*r++ = *p++)
			;
		*(r-1) = **OFS;
	}
	*(r-1) = '\0';
	dprintf("in recbld FS=%o, recloc=%o\n", **FS, recloc, NULL);
	recloc->tval = STR | FLD;
	dprintf("in recbld FS=%o, recloc=%o\n", **FS, recloc, NULL);
	if (r > record+RECSIZE)
		error(FATAL, "built giant record `%.20ws...'", record);
	dprintf("recbld = |%ws|\n", record, NULL, NULL);
}


CELL *fieldadr(n)
{
	if (n < 0 || n >= MAXFLD)
		error(FATAL, "trying to access field %d", n);
	return (&fldtab[n]);
}


int	errorflag	= 0;


yyerror(s) char *s; {
	fprintf(stderr, gettext("awk: %s near line %lld\n"), gettext(s), lineno);
	errorflag = 2;
}


error(f, s, a1, a2, a3, a4, a5, a6, a7)


{
	fprintf(stderr, "awk: ");
	fprintf(stderr, gettext((char *) s), a1, a2, a3, a4, a5, a6, a7);
	fprintf(stderr, "\n");
	if (NR && *NR > 0)
		fprintf(stderr, gettext(" record number %g\n"), *NR);
	if (f)
		exit(2);
}


PUTS(s) char *s; {
	dprintf("%s\n", s, NULL, NULL);
}


#define	MAXEXPON	38	/* maximum exponenet for fp number */


isanumber(s)
register wchar_t *s;
{
	register d1, d2;
	int point;
	wchar_t *es;


	d1 = d2 = point = 0;
	while (*s == ' ' || *s == '\t' || *s == '\n')
		s++;
	if (*s == '\0')
		return (0);	/* empty stuff isn't number */
	if (*s == '+' || *s == '-')
		s++;
	/*
	* Since, iswdigit() will include digit from other than code set 0,
		* we have to check it from code set 0 or not.
		*/
	if (!(iswdigit(*s) && iswascii(*s)) && *s != '.')
		return (0);
	if (iswdigit(*s) && iswascii(*s)) {
		do {
			d1++;
			s++;
		} while (iswdigit(*s) && iswascii(*s));
	}
	if (d1 >= MAXEXPON)
		return (0);	/* too many digits to convert */
	if (*s == '.') {
		point++;
		s++;
	}
	if (iswdigit(*s) && iswascii(*s)) {
		d2++;
		do {
			s++;
		} while (iswdigit(*s) && iswascii(*s));
	}


	if (!(d1 || point && d2))
		return (0);
	if (*s == 'e' || *s == 'E') {
		s++;
		if (*s == '+' || *s == '-')
			s++;
		if (!(iswdigit(*s) && iswascii(*s)))
			return (0);
		es = s;
		do {
			s++;
		} while (iswdigit(*s) && iswascii(*s));


		if (s - es > 2)
			return (0);
		else if (s - es == 2 &&
			10 * (*es-'0') + *(es+1)-'0' >= MAXEXPON)
			return (0);
	}
	while (*s == ' ' || *s == '\t' || *s == '\n')
		s++;
	if (*s == '\0')
		return (1);
	else
		return (0);
}
char *
toeuccode(str)
wchar_t *str;
{
	static char euccode[RECSIZE];


		wcstombs(euccode, str, RECSIZE);
	return (euccode);
}
