#!/bin/sh
echo -n "Running testscript $0... "

export QUERY_STRING="process_filename=test.dat&act_name=test2&pid=0&resource_type=requires&resource0=r3val"
#export REMOTE_USER="test.dat"
export REQUEST_METHOD=GET

./create_testtable
./bind_resources.cgi > output

if !(grep 'Location' output > /dev/null)
then
  echo
  echo Failed Location header.
  echo
fi
if !(grep 'action_page\.cgi' output > /dev/null)
then
  echo
  echo Failed action_page_redirection.
  echo
fi
#if !(grep 'test\.dat' output > /dev/null)
#then
#  echo
#  echo Failed process_filename.
#  echo
#fi

if !(grep 'test2' output > /dev/null)
then
  echo
  echo Failed act_name.
  echo
fi

if !(grep '0' output > /dev/null)
then
  echo
  echo Failed pid.
  echo
fi


if !(grep 'r1 "" r2 "" r3 "r3val" r4 ""' test.dat > /dev/null)
then
  echo
  echo Failed resource bindings.
  echo
fi

rm output
rm test.dat

export QUERY_STRING="process_filename=test.dat&act_name=test1&pid=0&resource_type=provides&resource0=r2val"
#export REMOTE_USER="test.dat"
export REQUEST_METHOD=GET

./create_testtable
./bind_resources.cgi > output

if !(grep 'Location' output > /dev/null)
then
  echo
  echo Failed Location header.
  echo
fi
if !(grep 'action_page\.cgi' output > /dev/null)
then
  echo
  echo Failed action_page_redirection.
  echo
fi
#if !(grep 'test\.dat' output > /dev/null)
#then
#  echo
#  echo Failed process_filename.
#  echo
#fi


if !(grep 'r1 "" r2 "r2val" r3 "" r4 ""' test.dat > /dev/null)
then
  echo
  echo Failed resource bindings.
  echo
fi

rm output
rm test.dat

echo "done"
