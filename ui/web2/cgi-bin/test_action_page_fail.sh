#!/bin/sh
echo -n "$0..."
PROC_TABLE=dfTSHEvITlEm2.dat

#echo "CREATE PROCESS"
lynx -dump -mime_header "lynxcgi:create_process.cgi?model=commit_changes.pml&process_filename=${PROC_TABLE}" | grep "Location: action_list.cgi" > /dev/null || echo "create process FAILED"

#echo "RUN ACTION"
lynx -dump -mime_header "lynxcgi:action_page.cgi?pid=0&act_name=update_workspace&process_filename=${PROC_TABLE}" | grep '<h1>update_workspace</h1>' > /dev/null || echo "run action FAILED"

#echo "BIND RESOURCES"
lynx -mime_header "lynxcgi:bind_resources.cgi?pid=0&act_name=update_workspace&resource_type=requires&workspace=/usr/local/src/httpunit&process_filename=${PROC_TABLE}" | grep "Location: action_page.cgi" > /dev/null || echo "bind resources FAILED"

[ -f ${PROC_TABLE} ] || echo "FAILURE: process table ${PROC_TABLE} missing"

#echo "LIST ACTIONS"
lynx -mime_header "lynxcgi:action_page.cgi?process_filename=${PROC_TABLE}&pid=0&act_name=update_workspace" | grep '<h1>update_workspace</h1>' > /dev/null || echo "list actions FAILED"

[ -f ${PROC_TABLE} ] || echo "FAILURE: process table ${PROC_TABLE} missing"

[ -f ${PROC_TABLE} ] && rm ${PROC_TABLE}

echo "done"
