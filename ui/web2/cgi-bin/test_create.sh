#!/bin/sh


create.cgi > output

for i in *.txt
do
  if !(grep $i output)
  then
    echo
    echo Failed List of Processes.
    echo
  fi
done

