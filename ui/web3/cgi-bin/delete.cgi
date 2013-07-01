#!/bin/sh
# $Id: delete.cgi,v 1.1 2007/04/29 07:14:45 jnoll Exp $ -*-perl-*-
# Create process instance.

printf "Content-type: text/html\r\n"
printf "\r\n"

# Get form variables.
eval "`./parsecgi $*`"
echo "pid: $FORM_PID"


echo "<pre>"
result=`./peos -d ${FORM_PID} 2>&1`
echo "</pre>"

/usr/bin/xsltproc list_procs.xsl proc_table.dat.xml 2>&1
echo "<pre>"
echo "status: $? pid: ${FORM_PID} result: $result"
echo "</pre>"
