#!/bin/sh
echo -n "$0..."

export QUERY_STRING="process_filename=test"
export REQUEST_METHOD=GET

process_listing.cgi > output

for i in *.pml
do
  if !(grep $i output > /dev/null)
  then
    echo
    echo Failed process_listing.
    echo
  fi
done

if !(grep action_list.cgi?process_filename=test output > /dev/null)
then
    echo
    echo Failed action list link
    echo
fi

rm output
echo "done"
