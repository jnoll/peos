#!/bin/sh
echo -n "Running testscript $0... "

check_for_notable() {
  ./active_processes.cgi > output

  # Testing the table does not appear
  if !(grep 'Active Processes' output > /dev/null)
  then
    echo; echo "Table header Active Processes not found"
  fi
  if (grep 'PID' output > /dev/null)
  then
    echo; echo "Table header PID not found"
  fi
  if (grep 'Process Name' output > /dev/null)
  then
    echo; echo "Table header Process name not found"
  fi
  if (grep 'Action Name' output > /dev/null)
  then
    echo; echo "Table header Action name not found"
  fi

  rm output
}

check_for_table() {
  ./active_processes.cgi > output

  # Testing the table headers
  if !(grep 'PID' output > /dev/null)
  then
    echo; echo "Table header PID not found"
  fi
  if !(grep 'Process Name' output > /dev/null)
  then
    echo; echo "Table header Process name not found"
  fi
  if !(grep 'Action Name' output > /dev/null)
  then
    echo; echo "Table header Action name not found"
  fi

  # Testing whether the process row is displayed
  if !(grep '<td>0</td>' output > /dev/null)
  then
    echo; echo "Cannot find the PID"
  fi
  if !(grep '>cvs_add_dir</a>' output > /dev/null)
  then
    echo; echo "Cannot find the Process name"
  fi
  if !(grep '>create_directory</a>' output > /dev/null)
  then
    echo; echo "Cannot find the active action name"
  fi

  rm output
}

# First time the user logs in with no data
export QUERY_STRING="action=start"
export REQUEST_METHOD=GET
export REMOTE_USER=test
check_for_notable

# Create process is invoked
export QUERY_STRING="action=create&model=cvs_add_dir.pml&process_filename=dfZRuitU82fEY.dat"
check_for_table

# Continue to view the page
export QUERY_STRING="action=continue&process_filename=dfZRuitU82fEY.dat"
check_for_table

# Delete the created process
export QUERY_STRING="action=delete&process_filename=dfZRuitU82fEY.dat&list=0,"
check_for_notable

rm dfZRuitU82fEY.dat*
echo "done"

