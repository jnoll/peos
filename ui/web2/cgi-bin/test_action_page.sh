#!/bin/sh

export QUERY_STRING="0+test_script=ready"
create_testtable
action_page.cgi > output

if !(grep "Action: test_script   (ready)" output)
then
  echo
  echo Failed Action header.
  echo
fi
if !(grep Resources output)
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
if !(grep "this is a script" output)
then
  echo
  echo Failed script.
  echo
fi
if !(grep perform_button.jpg output)
then
  echo
  echo Failed Perform button.
  echo
fi
if !(grep back_button.jpg output)
then
  echo
  echo Failed Back button.
  echo
fi

export QUERY_STRING="0+test_script=active"
action_page.cgi > output

if !(grep done_button.jpg output)
then
  echo
  echo Failed Done button.
  echo
fi
if !(grep suspend_button.jpg output)
then
  echo
  echo Failed Suspend button.
  echo
fi
if !(grep back_button.jpg output)
then
  echo
  echo Failed Back button.
  echo
fi

rm output
