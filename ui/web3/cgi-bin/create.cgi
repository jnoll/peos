#!/bin/sh
# $Id: create.cgi,v 1.1 2007/04/29 05:08:21 jnoll Exp $ 
# Create process.

printf "Content-type: text/html\r\n"
printf "\r\n"

# Get form variables.
eval "`./parsecgi $*`"
echo "model: $FORM_model"

# Construct a string to extract FORM_var value specifying (new) action state.
BINDING="EVENT=\$FORM_${ACTION}"
eval $BINDING


if [ ${FORM_model} ] ; then
   if [ ${FORM_model}.pml ] ; then
      ./peos -c ${FORM_model}.pml 1>&2
   fi
fi
