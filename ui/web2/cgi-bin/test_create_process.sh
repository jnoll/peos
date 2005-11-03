#!/bin/sh
echo -n "Running testscript $0... "

# Check for peos_init.tcl
if [ ! -f peos_init.tcl ]
then
  echo; echo "Cannot find peos_init.tcl"
fi

# First time the user logs in with no data
export QUERY_STRING="action=start"
export REQUEST_METHOD=GET
export REMOTE_USER=test
./active_processes.cgi > /dev/null
# Create model list
./create_process.cgi > output

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
if !(grep '>CVS ADD DIR</a></li>' output > /dev/null)
then
  echo; echo "List missing"
fi

rm output
rm dfZRuitU82fEY.dat*
echo "done"

