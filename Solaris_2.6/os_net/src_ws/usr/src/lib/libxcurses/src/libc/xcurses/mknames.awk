#
# Copyright (c) 1995, by Sun Microsystems, Inc.
# All rights reserved.
#
# ident	"@(#)mknames.awk 1.2	96/01/17 SMI"
#
# mknames.awk	
#
# XCurses Library
#
# Copyright 1990, 1995 by Mortice Kern Systems Inc.  All rights reserved.
#
# This Software is unpublished, valuable, confidential property of
# Mortice Kern Systems Inc.  Use is authorized only in accordance
# with the terms and conditions of the source licence agreement
# protecting this Software.  Any unauthorized use or disclosure of
# this Software is strictly prohibited and will result in the
# termination of the licence agreement.
#
# If you have any questions, please consult your supervisor.
#
#  rcsid = $Header: /rd/src/libc/xcurses/rcs/mknames.awk 1.1 1995/05/11 15:12:57 ant Exp $
#

function header(file, array) {
print "/*" > file
printf " * %s\n", file > file
print " *" > file
print " * XCurses Library" > file
print " *" > file
print " * **** THIS FILE IS MACHINE GENERATED." > file
print " * **** DO NOT EDIT THIS FILE." > file
print " *" > file
print " * Copyright 1990, 1995 by Mortice Kern Systems Inc.  All rights reserved." > file
print " *" > file
print " * This Software is unpublished, valuable, confidential property of" > file
print " * Mortice Kern Systems Inc.  Use is authorized only in accordance" > file
print " * with the terms and conditions of the source licence agreement" > file
print " * protecting this Software.  Any unauthorized use or disclosure of" > file
print " * this Software is strictly prohibited and will result in the" > file
print " * termination of the licence agreement." > file
print " *" > file
print " * If you have any questions, please consult your supervisor." > file
print " *" > file
print " */" > file
print > file
print "#if M_RCSID" > file
print "#ifndef lint" > file
printf "static char rcsID[] = \"$Header%s\"\n", "$" > file
print "#endif" > file
print "#endif" > file
print > file
printf "char *__m_%s[] = {\n", array > file
}

function tail(file) {
	printf("\t0\n};\n\n") >file;
	close(file);
}

BEGIN {
	header("boolname.c", "boolnames");
	header("boolcode.c", "boolcodes");
	header("boolfnam.c", "boolfnames");
	header("numname.c", "numnames");
	header("numcode.c", "numcodes");
	header("numfnam.c", "numfnames");
	header("strname.c", "strnames");
	header("strcode.c", "strcodes");
	header("strfnam.c", "strfnames");
}

$4 == "bool" {
	printf "\t\"%s\",\n", $1 > "boolfnam.c"
	printf "\t\"%s\",\n", $2 > "boolname.c"
	printf "\t\"%s\",\n", $3 > "boolcode.c"
}

$4 == "number" {
	printf "\t\"%s\",\n", $1 > "numfnam.c"
	printf "\t\"%s\",\n", $2 > "numname.c"
	printf "\t\"%s\",\n", $3 > "numcode.c"
}

$4 == "str" {
	printf "\t\"%s\",\n", $1 > "strfnam.c"
	printf "\t\"%s\",\n", $2 > "strname.c"
	printf "\t\"%s\",\n", $3 > "strcode.c"
}

END {
	tail("boolname.c");
	tail("boolcode.c");
	tail("boolfnam.c");
	tail("numname.c");
	tail("numcode.c");
	tail("numfnam.c");
	tail("strname.c");
	tail("strcode.c");
	tail("strfnam.c");
}
