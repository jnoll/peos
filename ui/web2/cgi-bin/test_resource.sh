#!/bin/sh

echo -n "$0..."
test -e proc_table.dat && rm proc_table.dat
create_testtable
export QUERY_STRING="model_list=test_action"
resource.cgi > output

if !(grep "<b>t</b><b>e</b><b>s</b><b>t</b><b>_</b><b>p</b><b>r</b><b>o</b><b>v</b><b>i</b><b>d</b><b>e</b><b>:" output > /dev/null)
then
echo
echo Provide resource failed.
echo
fi
if !(grep "<b>t</b><b>e</b><b>s</b><b>t</b><b>_</b><b>r</b><b>e</b><b>s</b><b>o</b><b>u</b><b>r</b><b>c</b><b>e</b><b>:</b>" output > /dev/null)
then
echo
echo Require resource failed.
echo
fi

rm output
rm proc_table.dat

echo "done"
