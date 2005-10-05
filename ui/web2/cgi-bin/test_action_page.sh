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
if !(grep '<li>CVS ADD DIR (PID: 0)</li>' output > /dev/null)
then
  echo; echo "Model name & PID missing"
fi
if !(grep '<li>Create Directory <small>(READY)</small></li>' output > /dev/null)
then
  echo; echo "First action without link missing"
fi
if !(grep '>Add Directory</a> <small>(AVAILABLE)</small></li>' output > /dev/null)
  then
    echo; echo "Action node 'add directory' failed"
fi
if !(grep '>Commit Directory</a> <small>(AVAILABLE)</small></li>' output > /dev/null)
  then
    echo; echo "Action node 'commit directory' failed"
fi
if !(grep '>Change Permissions</a> <small>(AVAILABLE)</small></li>' output > /dev/null)
  then
    echo; echo "Action node 'change permissions' failed"
fi

# Test the table on the right hand side
if !(grep '<h2>Create Directory (READY)</h2>' output > /dev/null)
then
  echo; echo "Action heading missing"
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
if !(grep 'In your workspace, create the new directory' output > /dev/null)
then
  echo; echo "Script value missing"
fi

#test Previous Next
if !(grep "&lt;&lt;Prev&nbsp;&nbsp;<a" output > /dev/null)
then
  echo; echo "Previous failed"
fi

if !(grep "<a href=\"action_page.cgi?process_filename=dfZRuitU82fEY.dat&pid=0&action_name=add_directory\">Next&gt;
&gt;</a>" output > /dev/null)
then
  echo; echo "Next failed"
fi

# go to next action
export QUERY_STRING="process_filename=dfZRuitU82fEY.dat&pid=0&action_name=add_directory"
./action_page.cgi > output

if !(grep "<a href=\"action_page.cgi?process_filename=dfZRuitU82fEY.dat&pid=0&action_name=create_directory\">&lt;&lt;Prev</a>" output > /dev/null)
then
  echo; echo "Previous failed"
fi

if !(grep "<
a href=\"action_page.cgi?process_filename=dfZRuitU82fEY.dat&pid=0&action_name=commit_directory\">Next&gt;&gt;</a>" output > /dev/null)
then
  echo; echo "Next failed"
fi

# go to next action
export QUERY_STRING="process_filename=dfZRuitU82fEY.dat&pid=0&action_name=commit_directory"
./action_page.cgi > output

if !(grep "<a href=\"action_page.cgi?process_filename=dfZRuitU82fEY.dat&pid=0&action_name=add_directory\">&lt;&lt;Prev</a>" output > /dev/null)
then
  echo; echo "Previous failed"
fi

if !(grep "<
a href=\"action_page.cgi?process_filename=dfZRuitU82fEY.dat&pid=0&action_name=change_permissions\">Next&gt;&gt;</a>" output > /dev/null)
then
  echo; echo "Next failed"
fi

# go to next action
export QUERY_STRING="process_filename=dfZRuitU82fEY.dat&pid=0&action_name=change_permissions"
./action_page.cgi > output

if !(grep "<a href=\"action_page.cgi?process_filename=dfZRuitU82fEY.dat&pid=0&action_name=commit_directory\">&lt;&lt;Prev</a>" output > /dev/null)
then
  echo; echo "Previous failed"
fi

if !(grep "/a>&nbsp;&nbsp;Next&gt;&gt;" output > /dev/null)
then
  echo; echo "Next failed"
fi

# test action state
echo "process p {" > state_test.pml
echo "  action a0 { }" >> state_test.pml
echo "  action a1 { requires {r1} }" >> state_test.pml
echo "  action a2 { provides{r2} }" >> state_test.pml
echo "}" >> state_test.pml

export QUERY_STRING="action=create&model=state_test.pml&process_filename=dfZRuitU82fEY.dat"
./active_processes.cgi > /dev/null

export QUERY_STRING="process_filename=dfZRuitU82fEY.dat&pid=1&action_name=a0"
./action_page.cgi > output

if !(grep ">A0 <small>(READY)" output > /dev/null)
then
  echo; echo "Action 'a0' failed"
fi

if !(grep ">A1</a>" output > /dev/null)
then
  echo; echo "Action 'a1' failed"
fi

if !(grep ">A2</a> <small>(AVAILABLE)" output > /dev/null)
then
  echo; echo "Action 'a2' failed"
fi

# Click the Start button for a0
export QUERY_STRING="action_event=Run&pid=1&act_name=a0&process_filename=dfZRuitU82fEY.dat"
./action_event.cgi > output

if !(grep "Location: action_page.cgi?resource_type=requires&process_filename=dfZRuitU82fEY.dat&pid=1&action_name=a0" output > /dev/null)
then
  echo; echo "Action page redirect failed"
fi

export QUERY_STRING="resource_type=requires&process_filename=dfZRuitU82fEY.dat&pid=1&action_name=a0"
./action_page.cgi > output

if !(grep "action_page.cgi?process_filename=dfZRuitU82fEY.dat&pid=1&action_name=a0" output > /dev/null)
then
  echo; echo "Action page redirect failed"
fi

export QUERY_STRING="process_filename=dfZRuitU82fEY.dat&pid=1&action_name=a0"
./action_page.cgi > output

if !(grep ">A0 <small>(RUN)" output > /dev/null)
then
  echo; echo "Action 'a0' failed"
fi

if !(grep ">A1</a>" output > /dev/null)
then
  echo; echo "Action 'a1' failed"
fi

if !(grep ">A2</a> <small>(AVAILABLE)" output > /dev/null)
then
  echo; echo "Action 'a2' failed"
fi

# Click the Finish button for a0
export QUERY_STRING="action_event=Finish&pid=1&act_name=a0&process_filename=dfZRuitU82fEY.dat"
./action_event.cgi > output

if !(grep "Location: action_page.cgi?resource_type=provides&process_filename=dfZRuitU82fEY.dat&pid=1&action_name=a0" output > /dev/null)
then
  echo; echo "Action page redirect failed"
fi

export QUERY_STRING="resource_type=provides&process_filename=dfZRuitU82fEY.dat&pid=1&action_name=a0"
./action_page.cgi > output

# Redirect to a1
if !(grep "Location: action_page.cgi?process_filename=dfZRuitU82fEY.dat&pid=1&action_name=a1" output > /dev/null)
then
  echo; echo "Action page redirect to 'a1' failed"
fi

export QUERY_STRING="process_filename=dfZRuitU82fEY.dat&pid=1&action_name=a1"
./action_page.cgi > output

if !(grep ">A0</a> <small>(DONE)" output > /dev/null)
then
  echo; echo "Action 'a0' failed"
fi

if !(grep ">A1 <small>(BLOCKED)" output > /dev/null)
then
  echo; echo "Action 'a1' failed"
fi

if !(grep ">A2</a> <small>(AVAILABLE)" output > /dev/null)
then
  echo; echo "Action 'a2' failed"
fi

#rm output
#rm dfZRuitU82fEY.dat*
echo "done"

