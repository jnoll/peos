#!/bin/sh

echo -n "$0..."

export QUERY_STRING="pid=0&process_filename=test.dat&act_name=test1"
export REQUEST_METHOD=GET

create_testtable
action_page.cgi > output

if !(grep '<h1>test1</h1>' output > /dev/null)
then
  echo
  echo Failed Page header.
  echo
fi
if !(grep 'test1' output > /dev/null)
then
  echo
  echo Failed action name.
  echo
fi
if !(grep 'READY' output > /dev/null)
then
  echo
  echo Failed action state.
  echo
fi
if !(grep '>test_script<' output > /dev/null)
then
  echo
  echo Failed script.
  echo
fi
if !(grep 'r1=\$\$' output > /dev/null)
then
  echo
  echo Failed Required Resources.
  echo
fi
if !(grep 'r2=\$\$' output > /dev/null)
then
  echo
  echo Failed Provided Resources.
  echo
fi


export QUERY_STRING="pid=0&act_name=test2&resource_type=requires&r3=/initial/value&process_filename=test.dat"

bind_resources.cgi | grep "Location: action_page.cgi" > /dev/null || (echo; echo "bind resources FAILED"; echo)

export QUERY_STRING="pid=0&process_filename=test.dat&act_name=test2"
action_page.cgi > output

if !(grep '<h1>test2</h1>' output > /dev/null)
then
  echo
  echo Failed Page header.
  echo
fi
if !(grep 'test2' output > /dev/null)
then
  echo
  echo Failed action name.
  echo
fi
# XXX Why is this RUN here?
if !(grep 'RUN' output > /dev/null)
then
  echo
  echo Failed action state.
  echo
fi
if !(grep '>test_script<' output > /dev/null)
then
  echo
  echo Failed script.
  echo
fi
if !(grep 'r3test.dat' output > /dev/null)
then
  echo
  echo Failed Required Resources.
  echo
fi
if !(grep 'r4=\$\$' output > /dev/null)
then
  echo
  echo Failed Provided Resources.
  echo
fi



# Test whether the symlink process really works.
RVAL=`ls -l r3test.dat | awk '{print $11}'`
[ "${RVAL}" = "/initial/value" ] || (echo; echo "Symlink failed."; echo)

[ -f test.dat ] || (echo; echo "Symlink failed: process table missing."; echo)


# Test whether unlink-symlink sequence (rebinding) works.
export QUERY_STRING="process_filename=test.dat&pid=0&act_name=test2&resource_type=requires&r3=/new/value"
export REQUEST_METHOD=GET

change_resource_bindings.cgi | grep "Location: action_page.cgi" > /dev/null || (echo; echo "rebind resources FAILED"; echo)

export QUERY_STRING="pid=0&process_filename=test.dat&act_name=test2"
export REQUEST_METHOD=GET
action_page.cgi > output

RVAL=`ls -l r3test.dat | awk '{print $11}'`
[ "${RVAL}" = "/new/value" ] || (echo; echo "Unlink-symlink failed."; echo)

# This test 

rm output
rm r3test.dat
rm test.dat

echo "done"
