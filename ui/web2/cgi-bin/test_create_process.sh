#!/bin/sh
echo -n "Running testscript $0... "

# First time the user logs in with no data
export QUERY_STRING="action=start"
export REQUEST_METHOD=GET
export REMOTE_USER=test
active_processes.cgi > /dev/null
# Create model list
export QUERY_STRING="process_filename=dfZRuitU82fEY.dat"
create_process.cgi > output

# Check the title
if !(grep '>Create Process</h2>' output > /dev/null)
then
  echo; echo "Create process heading missing"
fi
if !(grep '>Active Process</a>]' output > /dev/null)
then
  echo; echo "Active process link missing"
fi

# Check the table
if !(grep '>cvs_add_dir</a></li>' output > /dev/null)
then
  echo; echo "List missing"
fi

rm output
rm dfZRuitU82fEY.dat*
echo "done"

