#!/bin/sh
echo -n "."
echo TEST_CREATE.SH......

create.cgi > output

for i in *.pml
do
  if !(grep $i output > /dev/null)
  then
    echo
    echo Failed List of Processes.
    echo
  fi
done

