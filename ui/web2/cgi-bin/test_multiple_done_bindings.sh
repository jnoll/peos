#!/bin/sh
echo -n "$0..."

export QUERY_STRING="process_filename=test.dat&actions=test1&actions=test2&pid=0&r2=r2val&r4=r4val"
export REQUEST_METHOD=GET

create_testtable
multiple_done_bindings.cgi > output

if !(grep 'Location' output > /dev/null)
then
  echo
  echo Failed Location header.
  echo
fi
if !(grep 'action_list\.cgi' output > /dev/null)
then
  echo
  echo Failed action_page_redirection.
  echo
fi
if !(grep 'test\.dat' output > /dev/null)
then
  echo
  echo Failed process_filename.
  echo
fi

if !(grep 'start=false' output > /dev/null)
then
  echo
  echo Failed start.
  echo
fi


if !(grep 'r1 \$\$ r2 r2val r3 \$\$ r4 r4val' test.dat > /dev/null)
then
  echo
  echo Failed resource bindings.
  echo
fi



rm output
rm test.dat

echo "done"
