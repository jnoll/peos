#!/bin/sh
echo -n "Running testscript $0... "

# Check for peos_init.tcl
if [ ! -f peos_init.tcl ]
then
  echo; echo "Cannot find peos_init.tcl"
fi

# Clean environment; otherwise, pid may be offset by previous test.
rm dfZRuitU82fEY.dat*

# First time the user logs in with no data
export QUERY_STRING="action=start"
export REQUEST_METHOD=GET
export REMOTE_USER=test
./active_processes.cgi > /dev/null
# Invoke create process
export QUERY_STRING="action=create&model=test_action.pml&process_filename=dfZRuitU82fEY.dat"
./active_processes.cgi > /dev/null

# Create the action page
export QUERY_STRING="process_filename=dfZRuitU82fEY.dat&pid=0&action_name=test_script"
./action_page.cgi > output

# make sure all $$ has been removed
if (grep '\$\$' output > /dev/null)
then
    echo; echo '\$\$ exists'
fi

# Click the Start button
export QUERY_STRING="action_event=Run&pid=0&act_name=test_script&process_filename=dfZRuitU82fEY.dat"
./action_event.cgi > /dev/null

# Check the modified action page
export QUERY_STRING="resource_type=requires&pid=0&action_name=test_script&process_filename=dfZRuitU82fEY.dat"
./action_page.cgi > output

# make sure all $$ has been removed
if (grep '\$\$' output > /dev/null)
then
    echo; echo '\$\$ exists'
fi

# Test the table on the left hand side
if !(grep '>Create Process</a>]' output > /dev/null)
then
  echo; echo "Create process link missing"
fi
if !(grep '>Active Process</a>]' output > /dev/null)
then
  echo; echo "Active process link missing"
fi
if !(grep '<li>test_action.pml (Pid: 0)</li>' output > /dev/null)
then
  echo; echo "Model name & PID missing"
fi
if !(grep '<li>test script <small>(BLOCKED)</small></li>' output > /dev/null)
then
  echo; echo "First action without link missing"
fi

# Test the table on the right hand side
if !(grep '<h2>test script</h2>' output > /dev/null)
then
  echo; echo "Action heading missing"
fi
if !(grep '<h4>Supply Resource Bindings</h4>' output > /dev/null)
then
  echo; echo "Title \"Supply Resource Bindings\" missing"
fi
if !(grep '>test_resource<br></td>' output > /dev/null)
then
  echo; echo "Binding name missing"
fi
if !(grep '<input type="text" size="100" name="resource0" value="" maxlength="256">' output > /dev/null)
then
  echo; echo "Text field missing"
fi
if !(grep '<input type="Submit" name="Submit" value="Submit"' output > /dev/null)
then
  echo; echo "Submit button missing"
fi
if !(grep '<input type="Reset" name="Reset" value="Reset">' output > /dev/null)
then
  echo; echo "Reset button missing"
fi

rm output

# Submit a value for resource binding
export QUERY_STRING="pid=0&process_filename=dfZRuitU82fEY.dat&act_name=test_script&resource_type=requires&resource0=test1&action=new"
./bind_resources.cgi > /dev/null

# Load the new action page to check the behavior
export QUERY_STRING="pid=0&action_name=test_script&process_filename=dfZRuitU82fEY.dat"
./action_page.cgi > output
if !(grep '<td>RUN</td>' output > /dev/null)
then
  echo; echo "State incorrect after clicking Start"
fi

rm output

# Click the Suspend button
export QUERY_STRING="action_event=Suspend&pid=0&act_name=test_script&process_filename=dfZRuitU82fEY.dat"
./action_event.cgi > /dev/null

# Check the modified action page
export QUERY_STRING="pid=0&action_name=test_script&process_filename=dfZRuitU82fEY.dat"
./action_page.cgi > output
if !(grep '<td>SUSPEND</td>' output > /dev/null)
then
  echo; echo "State incorrect after clicking Suspend"
fi

# Click the Finish button
export QUERY_STRING="action_event=Finish&pid=0&act_name=test_script&process_filename=dfZRuitU82fEY.dat"
./action_event.cgi > /dev/null

# Check the modified action page
export QUERY_STRING="resource_type=provides&pid=0&action_name=test_script&process_filename=dfZRuitU82fEY.dat"
./action_page.cgi > output

# Test the table on the right hand side
if !(grep '<h2>test script</h2>' output > /dev/null)
then
  echo; echo "Action heading missing"
fi
if !(grep '<h4>Supply Resource Bindings</h4>' output > /dev/null)
then
  echo; echo "Title \"Supply Resource Bindings\" missing"
fi
if !(grep '>test_provide<br></td>' output > /dev/null)
then
  echo; echo "Binding name missing"
fi
if !(grep '<input type="text" size="100" name="resource0" value="" maxlength="256">' output > /dev/null)
then
  echo; echo "Text field missing"
fi
if !(grep '<input type="Submit" name="Submit" value="Submit"' output > /dev/null)
then
  echo; echo "Submit button missing"
fi
if !(grep '<input type="Reset" name="Reset" value="Reset">' output > /dev/null)
then
  echo; echo "Reset button missing"
fi

rm output

# Submit a value for resource binding
export QUERY_STRING="pid=0&process_filename=dfZRuitU82fEY.dat&act_name=test_script&resource_type=provides&resource0=test2&action=new"
./bind_resources.cgi > /dev/null

# Load the new action page to check the behavior
export QUERY_STRING="pid=0&action_name=test_script&process_filename=dfZRuitU82fEY.dat"
./action_page.cgi > output
if !(grep '<td>PENDING</td>' output > /dev/null)
then
  echo; echo "State incorrect after clicking Finish"
fi

rm output

# Click the Start button
export QUERY_STRING="action_event=Run&pid=0&act_name=test_script&process_filename=dfZRuitU82fEY.dat"
./action_event.cgi > /dev/null

# Submit to the action page
export QUERY_STRING="pid=0&process_filename=dfZRuitU82fEY.dat&action_name=test_script&resource_type=requires"
./action_page.cgi > /dev/null

# Check the modified action page
export QUERY_STRING="pid=0&action_name=test_script&process_filename=dfZRuitU82fEY.dat"
./action_page.cgi > output
if !(grep '<td>RUN</td>' output > /dev/null)
then
  echo; echo "State incorrect after clicking Start again"
fi

rm output

# Click the Abort button
export QUERY_STRING="action_event=Abort&pid=0&act_name=test_script&process_filename=dfZRuitU82fEY.dat"
./action_event.cgi > /dev/null

# Check the modified action page
export QUERY_STRING="pid=0&action_name=test_script&process_filename=dfZRuitU82fEY.dat"
./action_page.cgi > output
if !(grep '<td>NONE</td>' output > /dev/null)
then
  echo; echo "State incorrect after clicking Abort"
fi

#rm output
#rm dfZRuitU82fEY.dat*
echo "done"

