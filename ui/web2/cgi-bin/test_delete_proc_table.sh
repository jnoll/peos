#!/bin/sh

echo -n "$0..."

export QUERY_STRING="process_filename=test.dat"
export REQUEST_METHOD=GET

create_testtable
delete_proc_table.cgi > output

if !(grep "Process table 'test.dat' deleted." output > /dev/null)
then
  echo
  echo Failed delete.
  echo
fi

delete_proc_table.cgi > output
if !(grep "failed to delete process table." output > /dev/null)
then
  echo
  echo Failed to detect missing process table.
  echo
fi

rm output

echo "done"
