#!/bin/sh
echo -n "Running testscript $0... "

mkdir display_file_dir
echo "This is PEOS test." > display_file_dir/peos_test.txt

export QUERY_STRING="display_file_dir/peos_test.txt"
./display_file.cgi > output

if !(grep "Resource Content (File)" output > /dev/null)
then
  echo; echo "Rendering existing file resource failed"
fi

if !(grep "This is PEOS test." output > /dev/null)
then
  echo; echo "Rendering file content failed"
fi

export QUERY_STRING="display_file_dir"
./display_file.cgi > output
if !(grep "Resource Content (Directory)" output > /dev/null)
then
  echo; echo "Rendering existing directory resource failed"
fi

if !(grep "peos_test.txt" output > /dev/null)
then
  echo; echo "Rendering directory content failed"
fi

rm -R -f display_file_dir

export QUERY_STRING="display_file_dir/peos_test.txt"
./display_file.cgi > output

if !(grep "Such resource does not exist." output > /dev/null)
then
  echo; echo "Rendering non-existing file resource failed"
fi

export QUERY_STRING="display_file_dir"
./display_file.cgi > output
if !(grep "Such resource does not exist." output > /dev/null)
then
  echo; echo "Rendering non-existing directory resource failed"
fi

echo "done"
