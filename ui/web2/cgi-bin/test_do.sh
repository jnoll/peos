#!/bin/sh
echo -n "."
echo TEST_DO.SH......

rm proc_table.dat
create_testtable

export QUERY_STRING="0+test_script=run"
do.cgi > output

if !(grep "test_script 2" proc_table.dat > /dev/null)
then
  echo
  echo Failed running action.
  echo
fi

export QUERY_STRING="0+test_script=suspend"
do.cgi > output
if !(grep "test_script 4" proc_table.dat > /dev/null)
then
  echo
  echo Failed suspending process.
  echo
fi

export QUERY_STRING="0+test_script=resume"
do.cgi > output
if !(grep "test_script 2" proc_table.dat > /dev/null)
then
  echo
  echo Failed creating process.
  echo
fi
export QUERY_STRING="0+test_script=done"
do.cgi > output
if !(grep "test_script 3" proc_table.dat > /dev/null)
then
  echo
  echo Failed finishing process.
  echo
fi

rm output
rm proc_table.dat
