#!/bin/sh
echo -n "$0..."

export QUERY_STRING="process_filename=test.dat&actions=0\$test1&actions=0\$test2"
export REQUEST_METHOD=GET

create_testtable
multiple_dones.cgi > output

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
if !(grep 'r4' output > /dev/null)
then
  echo
  echo Failed required resource r4.
  echo
fi

rm output
rm test.dat

echo "done"
