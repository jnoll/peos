#!/bin/sh


export QUERY_STRING="pid=0&process_filename=test.dat&act_name=test1"
export REQUEST_METHOD=GET

create_testtable
action_page.cgi > output

if !(grep '<h1>Action Details</h1>' output > /dev/null)
then
  echo
  echo Failed Page header.
  echo
fi
if !(grep 'test1' output > /dev/null)
then
  echo
  echo Failed action name.
  echo
fi
if !(grep 'READY' output > /dev/null)
then
  echo
  echo Failed action state.
  echo
fi
if !(grep '"test_script"' output > /dev/null)
then
  echo
  echo Failed script.
  echo
fi
if !(grep 'r1=\$\$' output > /dev/null)
then
  echo
  echo Failed Required Resources.
  echo
fi
if !(grep 'r2=\$\$' output > /dev/null)
then
  echo
  echo Failed Provided Resources.
  echo
fi

rm output
rm test.dat

echo "done"
