#!/bin/sh
echo -n "Running testscript $0... "

# First time the user logs in with no data
export QUERY_STRING="action=start"
export REQUEST_METHOD=GET
export REMOTE_USER=test
./active_processes.cgi > /dev/null
# Invoke create process
export QUERY_STRING="action=create&model=cvs_add_dir.pml&process_filename=dfZRuitU82fEY.dat"
./active_processes.cgi > /dev/null

# Create the action page
export QUERY_STRING="process_filename=dfZRuitU82fEY.dat&pid=0&action_name=create_directory"
./action_page.cgi > output

# Test the table on the left hand side
if !(grep '>Create Process</a>]' output > /dev/null)
then
  echo; echo "Create process link missing"
fi
if !(grep '>Active Process</a>]' output > /dev/null)
then
  echo; echo "Active process link missing"
fi
if !(grep '<li>cvs_add_dir.pml (Pid: 0)</li>' output > /dev/null)
then
  echo; echo "Model name & PID missing"
fi
if !(grep '<li>create_directory</li>' output > /dev/null)
then
  echo; echo "First action without link missing"
fi
ACTIONS="add_directory commit_directory change_permissions"
for i in $ACTIONS
do
  if !(grep ">$i</a></li>" output > /dev/null)
  then
    echo; echo "Action $i not found"
  fi
done

# Test the table on the right hand side
if !(grep '<h2>create_directory</h2>' output > /dev/null)
then
  echo; echo "Action heading missing"
fi
if !(grep '<b>State:</b>' output > /dev/null)
then
  echo; echo "State Row missing"
fi
if !(grep '<td>READY</td>' output > /dev/null)
then
  echo; echo "State value missing"
fi
if !(grep 'Required Resources' output > /dev/null)
then
  echo; echo "Required Resources Row missing"
fi
if !(grep '<td>No resources required</td>' output > /dev/null)
then
  echo; echo "Required Resources value missing"
fi
if !(grep '<b>Provided Resources:</b>' output > /dev/null)
then
  echo; echo "Provided Resources Row missing"
fi
if !(grep '<b>Script:</b>' output > /dev/null)
then
  echo; echo "Script Row missing"
fi
if !(grep 'In your workspace, create the new directory' output > /dev/null)
then
  echo; echo "Script value missing"
fi

rm output
rm dfZRuitU82fEY.dat*
echo "done"

