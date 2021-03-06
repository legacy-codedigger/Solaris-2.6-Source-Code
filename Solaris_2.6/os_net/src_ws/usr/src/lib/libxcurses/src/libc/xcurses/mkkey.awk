#
# Copyright (c) 1995, by Sun Microsystems, Inc.
# All rights reserved.
#
# ident	"@(#)mkkey.awk 1.2	96/01/17 SMI"
#
# mkkey.awk
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
# USAGE:
# 	awk -f mkkey.awk caps >keyindex.c
#
#  rcsid = $Header: /rd/src/libc/xcurses/rcs/mkkey.awk 1.1 1995/05/11 14:39:17 ant Exp $
#	

BEGIN {
print "/*"
print " * keyindex.c"
print " *"
print " * XCurses Library"
print " *"
print " * **** THIS FILE IS MACHINE GENERATED."
print " * **** DO NOT EDIT THIS FILE."
print " *"
print " * Copyright 1990, 1995 by Mortice Kern Systems Inc.  All rights reserved."
print " *"
print " * This Software is unpublished, valuable, confidential property of"
print " * Mortice Kern Systems Inc.  Use is authorized only in accordance"
print " * with the terms and conditions of the source licence agreement"
print " * protecting this Software.  Any unauthorized use or disclosure of"
print " * this Software is strictly prohibited and will result in the"
print " * termination of the licence agreement."
print " *"
print " * If you have any questions, please consult your supervisor."
print " */"
print
print "#if M_RCSID"
print "#ifndef lint"
printf "static char rcsID[] = \"$Header%s\";\n", "$"
print "#endif"
print "#endif"
print
print "#include <private.h>"
print
print "/*"
print " * This table is used to map a KEY_ constant to an"
print " * index for cur_term->_str[] for use by keyname()."
print " */"
print "short __m_keyindex[][2]= {"
	strcount = 0;
}

$4 == "str" {
	if ($1 ~ /^key_f[0-9][0-9]*/)
		printf("\t{ %d, KEY_F(%d) },\n", strcount, substr($1, 6, 2));
	else if ($1 ~ /^key_/)
		printf "\t{ %d, %s },\n", strcount, toupper($1);
	++strcount;
}

END {
	print "\t{ -1, -1 }"
	print "};"
}
