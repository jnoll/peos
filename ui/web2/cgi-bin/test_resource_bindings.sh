#!/bin/sh

echo -n "$0 .."


export QUERY_STRING="process_filename=test.dat&act_name=test2&pid=0&resource_type=requires&r3=r3val"
export REQUEST_METHOD=GET

create_testtable
bind_resources.cgi > /dev/null

export QUERY_STRING="process_filename=test.dat&act_name=test2&resource_type=requires&pid=0"
export REQUEST_METHOD=GET

resource_bindings.cgi > output

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


if !(grep 'r3val' output > /dev/null)
then
  echo
  echo Failed required resource r3 value.
  echo
fi

rm output
rm test.dat


export QUERY_STRING="process_filename=test.dat&act_name=test1&pid=0&resource_type=provides&r2=r2val"
export REQUEST_METHOD=GET

create_testtable
bind_resources.cgi > /dev/null

export QUERY_STRING="process_filename=test.dat&act_name=test1&resource_type=provides&pid=0"
export REQUEST_METHOD=GET

resource_bindings.cgi > output

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


if !(grep 'r2val' output > /dev/null)
then
  echo
  echo Failed required resource r2 value.
  echo
fi

rm output
rm test.dat


echo "done"
