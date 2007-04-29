#!/bin/sh
# $Id: update.cgi,v 1.1 2007/04/29 05:08:21 jnoll Exp $ 
# Update process state from stdin.

printf "Content-type: text/html\r\n"
printf "\r\n"

# Get form variables.
eval "`./parsecgi $*`"
echo "actions: $FORM_actions"
echo "list: $FORM_list"
#grep actions: proc_table.dat
for ACTION in $FORM_actions; do
    # Construct a string to extract FORM_var value specifying (new) action state.
    BINDING="EVENT=\$FORM_${ACTION}"
    eval $BINDING
    #printf "<i>action=\"${ACTION}\" event=\"${EVENT}\"</i>" 
    if [ ${EVENT} ] ; then
       ./peos -n ${FORM_PID} ${ACTION} ${EVENT} 1>&2
    fi
done
#grep actions: proc_table.dat
if [ $FORM_PID ] ; then
    MODEL_PML=`/usr/bin/xsltproc --stringparam PID $FORM_PID get_model.xsl proc_table.dat.xml`
    MODEL=${MODEL_PML%.pml}
    if [ -e ${MODEL}.xpml ] ; then 
	MODEL_XPML=${MODEL}.xpml
    elif [ -e ${MODEL}.xml ] ; then 
	MODEL_XPML=${MODEL}.xml
    else
	echo "<h1>ERROR: XPML model file for \"${MODEL}\" not found</h1>"
	exit 0
    fi
    /usr/bin/xsltproc --stringparam PID $FORM_PID merge_instance.xsl ${MODEL_XPML} |/usr/bin/xsltproc pmldoc.xsl - 2>&1
    echo "<em>status: $? result: $result</em>"
else
    echo  "<h1>ERROR: Process ID (PID) not specified. </h1>"
fi




