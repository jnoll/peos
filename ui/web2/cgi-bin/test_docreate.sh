#!/bin/sh
echo -n "."

rm proc_table.dat
export QUERY_STRING="model=test_action&test_resource=my_resource&test_provide=my_provide"

do_create.cgi > /dev/null

if !(grep test_action.pml proc_table.dat > /dev/null)
then
  echo
  echo Failed create process.
  echo
fi
if !(grep  my_resource proc_table.dat > /dev/null)
then
  echo
  echo Failed resource value input.
  echo
fi
if !(grep  my_provide proc_table.dat > /dev/null)
then
  echo
  echo Failed provide value input.
  echo
fi

# Another test - overflow model_name buffer.
export QUERY_STRING="model_list=XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"


if !(do_create.cgi 2>1 |grep -i "model file not found" > /dev/null)
then
  echo
  echo Failed create non-existent process.
  echo
fi

