#!/bin/sh

rm proc_table.dat
create_testtable
main.cgi > output

if !(grep Status output)
then
  echo
  echo Failed Status header.
  echo
fi
if !(grep Task output)
then
  echo
  echo Failed Task header.
  echo
fi
if !(grep Resource\(s\) output)
then
  echo
  echo Failed Resource header.
  echo
fi
if !(grep Description output)
then
  echo
  echo Failed Description header.
  echo
fi
if !(grep ready output)
then
  echo
  echo Failed ready state.
  echo
fi
if !(grep test_script output)
then
  echo
  echo Failed action name.
  echo
fi
if !(grep "this is a script" output)
then
  echo
  echo Failed script. 
  echo
fi

rm output
