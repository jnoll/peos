#!/bin/sh

echo -n "$0 .."
export QUERY_STRING="process_filename=test.dat&act_name=test2&resource_type=requires&pid=0"

export REQUEST_METHOD=GET

create_testtable
handle_run.cgi > output

if !(grep '<h1>Please Supply Resource Bindings</h1>' output > /dev/null)
then
  echo
  echo Failed Page header.
  echo
fi
if !(grep 'r3' output > /dev/null)
then
  echo
  echo Failed required resource r3.
  echo
fi

rm output
rm test.dat

export QUERY_STRING="process_filename=test.dat&act_name=test1&resource_type=provides&pid=0"

create_testtable
handle_run.cgi > output

if !(grep '<h1>Please Supply Resource Bindings</h1>' output > /dev/null)
then
  echo
  echo Failed Page header.
  echo
fi
if !(grep 'r2' output > /dev/null)
then
  echo
  echo Failed required resource r2.
  echo
fi

rm output
rm test.dat

echo "done"
