#!/bin/sh
echo -n "."

rm proc_table.dat
export QUERY_STRING="model_list=eggs"

do_create.cgi > /dev/null

if !(grep eggs.pml proc_table.dat > /dev/null)
then
  echo
  echo Failed create process.
  echo
fi

# Another test - overflow model_name buffer.
export QUERY_STRING="model_list=XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"

if !(do_create.cgi |grep -i "couldn't create" > /dev/null)
then
  echo
  echo Failed create non-existent process.
  echo
fi

