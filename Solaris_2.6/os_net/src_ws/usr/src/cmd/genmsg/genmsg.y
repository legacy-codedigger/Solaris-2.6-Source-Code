%{
/*
 * Copyright (c) 1995 by Sun Microsystems, Inc.
 */
 
#pragma ident "@(#)genmsg.y	5.4 96/05/19"

#include <stdio.h>
#include <libintl.h>
#include "genmsg.h"
extern int is_cat_found;		/* from main.c */
extern int lineno;			/* genmsg.l */
extern int msg_line;			/* genmsg.l */
extern int end_of_cat;			/* from genmsg.l */
extern void set_linemsgid(int, int);	/* from genmsg.l */
extern void add_msg(int, int, char *, char*, int, int);	/* from util.c */

static void do_catgets(int, int, char*);
static char *add_qstring(char *, char*);
%}

%union {
	char *str;
	int id;
}

%token CATGETS
%token CONST
%token CATD
%token INT, CHAR, INC
%token <str> STR
%token <id> SETID, MSGID, DIGIT
%token <str> QSTR

%type <id> cast_setid, setid, cast_msgid, msgid, cast_digit, digit
%type <str> catd, arg_list, arg_def, arg_func, arg_exp, str, 
	cast_qstr, paren_qstr, qstr_list

%left '-' '+'
%left '*' '/'
%nonassoc UMINUS

%%

genmsg_list:	/* empty */
		{
			if (!IsActiveMode(ReplaceMode)) {
				src_err(srcfile, (lineno - 1),
					gettext("catgets not found"));
			}
		}
	|	genmsg			{ is_cat_found = TRUE; }
	;

genmsg:		catgets			{ end_of_cat = TRUE; }
	|	genmsg catgets		{ end_of_cat = TRUE; }
	;

catgets:       CATGETS '(' catd ',' cast_setid ',' cast_msgid ',' cast_qstr ')'
		{ 
			do_catgets($5, $7, $9); free($9);
		}
	|	error
	;

catd:		'(' CATD ')' arg_list		{ $$ = $4; }
	|	'(' CONST CATD ')' arg_list	{ $$ = $5; }
	|	arg_list
	;

arg_list:	arg_def
	|	arg_list '-' '>' arg_def
	|	'(' arg_list '-' '>' arg_def ')'{ $$ = $2; }
	;

arg_def:	arg_func
	|	arg_exp
	|	str
	;

arg_func:	'(' arg_func ')'	{ $$ = $2; }
	|	str '(' ')'
	|	str '(' str ')'
	|	str '(' cast_digit ')'
	|	str '(' cast_qstr ')'	{ free($3); }
	;

arg_exp:	'(' arg_exp ')'		{ $$ = $2; }
	|	str INC
	|	INC str			{ $$ = $2; }
	;

str:		'(' str ')'		{ $$ = $2; }
	|	'*' str			{ $$ = $2; }
	|	STR
	;

cast_setid:	'(' INT ')' setid	{ $$ = $4; }
	|	'(' CONST INT ')' setid	{ $$ = $5; }
	|	setid
	;

setid:		setid '+' setid		{ $$ = $1 + $3; }
	|	setid '-' setid		{ $$ = $1 - $3; }
	|	setid '*' setid		{ $$ = $1 * $3; }
	|	setid '/' setid
		{ 
			if ($3 == 0) {
				yyerror(gettext("zero divide"));
			} else {
				$$ = $1 / $3; 
			}
		}
	|	'-' setid %prec UMINUS	{ $$ = -$2; }		
	|	'(' setid ')'		{ $$ = $2; }
	|	SETID
	;

cast_msgid:	'(' INT ')' msgid	{ $$ = $4; }
	|	'(' CONST INT ')' msgid	{ $$ = $5; }
	|	msgid
	;

msgid:		msgid '+' msgid		{ $$ = $1 + $3; }
	|	msgid '-' msgid		{ $$ = $1 - $3; }
	|	msgid '*' msgid		{ $$ = $1 * $3; }
	|	msgid '/' msgid		
		{ 
			if ($3 == 0) {
				yyerror(gettext("zero devide"));
			} else {
				$$ = $1 / $3; 
			}
		}
	|	'-' msgid %prec UMINUS	{ $$ = -$2; }		
	|	'(' msgid ')'		{ $$ = $2; }
	|	MSGID
	;

cast_digit:	'(' INT ')' digit	{ $$ = $4; }
	|	'(' CONST INT ')' digit	{ $$ = $5; }
	|	digit
	;

digit:		digit '+' digit		{ $$ = $1 + $3; }
	|	digit '-' digit		{ $$ = $1 - $3; }
	|	digit '*' digit		{ $$ = $1 * $3; }
	|	digit '/' digit
		{ 
			if ($3 == 0) {
				yyerror(gettext("zero divide"));
			} else {
				$$ = $1 / $3; 
			}
		}
	|	'-' digit %prec UMINUS	{ $$ = -$2; }		
	|	'(' digit ')'		{ $$ = $2; }
	|	DIGIT
	;

cast_qstr:	'(' CHAR '*' ')' paren_qstr		{ $$ = $5; }
	|	'(' CONST CHAR '*' ')' paren_qstr	{ $$ = $6; }
	|	paren_qstr
	;

paren_qstr:	'(' qstr_list ')'	{ $$ = $2; }
	|	qstr_list
	;

qstr_list:	QSTR
	|	qstr_list QSTR		{ $$ = add_qstring($1, $2); }
	;

%%

static void
do_catgets(int setid, int msgid, char *str)
{
	int id = msgid;
	if (IsActiveMode(ReplaceMode)) {
		return;
	}
	if (setid == 0 || setid > NL_SETMAX) {
		src_err(srcfile, lineno, 
			gettext("improper set number: %d"), setid);
		return;
	}
	if (IsActiveMode(ProjectMode)) {
		set_msgid(setid, id);
		add_msg(setid, id, str, srcfile, lineno, TRUE);
	} else if (IsActiveMode(ReverseMode)) {
		set_linemsgid(msg_line, NOMSGID);
	} else if (IsActiveMode(AutoNumMode)) {
		if (id == NOMSGID) {
			id = get_msgid(srcfile, msg_line, setid, str);
			set_linemsgid(msg_line, id);
		} 
		if (id != NOMSGID) {
			set_msgid(setid, id);
			add_msg(setid, id, str, srcfile, lineno, FALSE);
		}
	} else if (id == NOMSGID) {
		warning(gettext("improper message number: -1"));
	} else {
		add_msg(setid, id, str, srcfile, lineno, FALSE);
	}
}

static char *
add_qstring(char *str, char *add)
{
	int len1 = strlen(str);
	int len2 = strlen(add);
	/* 3 includes '\', '\n' and '\0' */
	char *tmp = malloc(len1 + len2 + 3);
	if (!tmp) {
		prg_err(gettext("fatal: out of memory"));
		exit(EXIT_FAILURE);
	}
	sprintf(tmp, "%s\\\n%s", str, add);
	free(str);
	free(add);
	return tmp;
}
