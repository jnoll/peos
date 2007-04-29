#!/bin/sh
# $Id: list_procs.cgi,v 1.1 2007/04/29 05:08:21 jnoll Exp $ 
# Generate clickable active process list.

printf "Content-type: text/html\r\n"
printf "\r\n"

# Get form variables.  
# XXX eventually these will include login and possible proc_table name.
eval "`./parsecgi $*`"

/usr/bin/xsltproc list_procs.xsl proc_table.dat.xml 2>&1

echo "<pre>"
echo "status: $? result: $result"
echo "</pre>"


