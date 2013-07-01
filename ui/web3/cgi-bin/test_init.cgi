#!/bin/sh
# $Id: test_init.cgi,v 1.1 2007/04/29 05:08:21 jnoll Exp $ 
# Create test environment for end-to-end tests.

printf "Content-type: text/plain\r\n"
printf "\r\n"

echo "" > proc_table.dat 
rm -f proc_table.dat.xml
echo "UID=`id`"
./peos -c readme.pml 2>&1
./peos -c test_action.pml 2>&1
./peos -c test.pml 2>&1



