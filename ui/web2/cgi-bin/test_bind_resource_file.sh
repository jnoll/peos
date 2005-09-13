#!/bin/sh
echo -n "Running testscript $0... "

# Check for peos_init.tcl
if [ ! -f peos_init.tcl ]
then
  echo; echo "Cannot find peos_init.tcl"
fi

# Clean environment; otherwise, pid may be offset by previous test.
rm dfZRuitU82fEY.dat*

# create bind_resource_file.res
echo "r0: v0" > bind_resource_file.res
echo "r1: v1" >> bind_resource_file.res
echo "r2: v2" >> bind_resource_file.res
echo "r3: v3" >> bind_resource_file.res
echo "r4: v4" >> bind_resource_file.res
echo "r5: v5" >> bind_resource_file.res

# First time the user logs in with no data
export QUERY_STRING="action=start"
export REQUEST_METHOD=GET
export REMOTE_USER=test
./active_processes.cgi > /dev/null
# Invoke create process
export QUERY_STRING="action=create&model=bind_resource_file.pml&process_filename=dfZRuitU82fEY.dat"
./active_processes.cgi > /dev/null

# Test r0
# Go to a0
export QUERY_STRING="process_filename=dfZRuitU82fEY.dat&pid=0&action_name=a0"
./action_page.cgi > output
# Test resource evaluated value
if !(grep '<td>r0 = v0' output > /dev/null)
then
    echo; echo "r0 evaluated value failed"
fi
# Test resource actual value
# Go to supply resource binding page
export QUERY_STRING="resource_type=requires&action=change&process_filename=dfZRuitU82fEY.dat&pid=0&action_name=a0"
./action_page.cgi > output

if !(grep 'name="resource0" value="v0"' output > /dev/null)
then
    echo; echo "r0 actual value failed"
fi

# Test r1
# Go to a0
export QUERY_STRING="process_filename=dfZRuitU82fEY.dat&pid=0&action_name=a0"
./action_page.cgi > output
#Test resource evaluated value
if !(grep '<td>r1 = v1' output > /dev/null)
then
    echo; echo "r1 evaluated value failed"
fi
# Test resource actual value
# Go to supply resource binding page
export QUERY_STRING="resource_type=provides&action=change&process_filename=dfZRuitU82fEY.dat&pid=0&action_name=a0"
./action_page.cgi > output

if !(grep 'name="resource0" value="v1"' output > /dev/null)
then
    echo; echo "r1 actual value failed"
fi

# Test r2 && r3
# Go to a1
export QUERY_STRING="process_filename=dfZRuitU82fEY.dat&pid=0&action_name=a1"
./action_page.cgi > output
# Test resource evaluated value
if !(grep '<td>r2 = v2, r3 = v3' output > /dev/null)
then
    echo; echo "r2 && r3 evaluated value failed"
fi
# Test resource actual value
# Go to supply resource binding page
export QUERY_STRING="resource_type=requires&action=change&process_filename=dfZRuitU82fEY.dat&pid=0&action_name=a1"
./action_page.cgi > output

if !(grep 'name="resource0" value="v2"' output > /dev/null)
then
    echo; echo "r2 actual value failed"
fi

if !(grep 'name="resource1" value="v3"' output > /dev/null)
then
    echo; echo "r3 actual value failed"
fi

# Test r4 && r5
# Go to a1
export QUERY_STRING="process_filename=dfZRuitU82fEY.dat&pid=0&action_name=a1"
./action_page.cgi > output
# Test resource evaluated value
if !(grep '<td>r4 = v4, r5 = v5' output > /dev/null)
then
    echo; echo "r4 && r5 evaluated value failed"
fi
# Test resource actual value
# Go to supply resource binding page
export QUERY_STRING="resource_type=provides&action=change&process_filename=dfZRuitU82fEY.dat&pid=0&action_name=a1"
./action_page.cgi > output

if !(grep 'name="resource0" value="v4"' output > /dev/null)
then
    echo; echo "r4 actual value failed"
fi

if !(grep 'name="resource1" value="v5"' output > /dev/null)
then
    echo; echo "r5 actual value failed"
fi

# create bind_resource_file.res
echo "r1: \${r0}/v1" > bind_resource_file.res
echo "r2: \${r1}/v2" >> bind_resource_file.res
echo "r3: \${r2}/v3" >> bind_resource_file.res
echo "r4: \${r3}/v4" >> bind_resource_file.res
echo "r5: \${r4}/v5" >> bind_resource_file.res

# Invoke create process
export QUERY_STRING="action=create&model=bind_resource_file.pml&process_filename=dfZRuitU82fEY.dat"
./active_processes.cgi > /dev/null

# Test r0
# Go to a0
export QUERY_STRING="process_filename=dfZRuitU82fEY.dat&pid=1&action_name=a0"
./action_page.cgi > output
#Test resource evaluated value
if !(grep '<td>r0 = ${r0}' output > /dev/null)
then
    echo; echo "r0 evaluated value failed"
fi
# Test resource actual value
# Go to supply resource binding page
export QUERY_STRING="resource_type=requires&action=change&process_filename=dfZRuitU82fEY.dat&pid=1&action_name=a0"
./action_page.cgi > output

if !(grep 'name="resource0" value=""' output > /dev/null)
then
    echo; echo "r0 actual value failed"
fi

# Test r1
# Go to a0
export QUERY_STRING="process_filename=dfZRuitU82fEY.dat&pid=1&action_name=a0"
./action_page.cgi > output
#Test resource evaluated value
if !(grep '<td>r1 = \${r0}/v1' output > /dev/null)
then
    echo; echo "r1 evaluated value failed"
fi
# Test resource actual value
# Go to supply resource binding page
export QUERY_STRING="resource_type=provides&action=change&process_filename=dfZRuitU82fEY.dat&pid=1&action_name=a0"
./action_page.cgi > output

if !(grep 'name="resource0" value="\${r0}/v1"' output > /dev/null)
then
    echo; echo "r1 actual value failed"
fi

# Test r2 && r3
# Go to a1
export QUERY_STRING="process_filename=dfZRuitU82fEY.dat&pid=1&action_name=a1"
./action_page.cgi > output
#Test resource evaluated value
if !(grep '<td>r2 = \${r0}/v1/v2, r3 = \${r0}/v1/v2/v3' output > /dev/null)
then
    echo; echo "r2 && r3 evaluated value failed"
fi
# Test resource actual value
# Go to supply resource binding page
export QUERY_STRING="resource_type=requires&action=change&process_filename=dfZRuitU82fEY.dat&pid=1&action_name=a1"
./action_page.cgi > output

if !(grep 'name="resource0" value="\${r1}/v2"' output > /dev/null)
then
    echo; echo "r2 actual value failed"
fi

if !(grep 'name="resource1" value="\${r2}/v3"' output > /dev/null)
then
    echo; echo "r3 actual value failed"
fi

# Test r4 && r5
# Go to a1
export QUERY_STRING="process_filename=dfZRuitU82fEY.dat&pid=1&action_name=a1"
./action_page.cgi > output
# Test resource evaluated value
if !(grep '<td>r4 = \${r0}/v1/v2/v3/v4, r5 = \${r0}/v1/v2/v3/v4/v5' output > /dev/null)
then
    echo; echo "r4 && r5 evaluated value failed"
fi
# Test resource actual value
# Go to supply resource binding page
export QUERY_STRING="resource_type=provides&action=change&process_filename=dfZRuitU82fEY.dat&pid=1&action_name=a1"
./action_page.cgi > output

if !(grep 'name="resource0" value="\${r3}/v4"' output > /dev/null)
then
    echo; echo "r4 actual value failed"
fi

if !(grep 'name="resource1" value="\${r4}/v5"' output > /dev/null)
then
    echo; echo "r5 actual value failed"
fi

# Test supply resource value to r0
# Click the Start button
export QUERY_STRING="action_event=Run&pid=1&act_name=a0&process_filename=dfZRuitU82fEY.dat"
./action_event.cgi > /dev/null
# Click Submmit with v0 as value for r0
export QUERY_STRING="pid=1&process_filename=dfZRuitU82fEY.dat&act_name=a0&resource_type=requires&resource0=v0"
./bind_resources.cgi > /dev/null
# Go to a0
export QUERY_STRING="process_filename=dfZRuitU82fEY.dat&pid=1&action_name=a0"
./action_page.cgi > output

#Test resource evaluated value
if !(grep '<td>r0 = v0' output > /dev/null)
then
    echo; echo "r0 evaluated value failed"
fi
# Test resource actual value
# Go to supply resource binding page
export QUERY_STRING="resource_type=requires&action=change&process_filename=dfZRuitU82fEY.dat&pid=1&action_name=a0"
./action_page.cgi > output

if !(grep 'name="resource0" value="v0"' output > /dev/null)
then
    echo; echo "r0 actual value failed"
fi

# Test r1
# Go to a0
export QUERY_STRING="process_filename=dfZRuitU82fEY.dat&pid=1&action_name=a0"
./action_page.cgi > output
#Test resource evaluated value
if !(grep '<td>r1 = v0/v1' output > /dev/null)
then
    echo; echo "r1 evaluated value failed"
fi
# Test resource actual value
# Go to supply resource binding page
export QUERY_STRING="resource_type=provides&action=change&process_filename=dfZRuitU82fEY.dat&pid=1&action_name=a0"
./action_page.cgi > output

if !(grep 'name="resource0" value="\${r0}/v1"' output > /dev/null)
then
    echo; echo "r1 actual value failed"
fi

# Test r2 && r3
# Go to a1
export QUERY_STRING="process_filename=dfZRuitU82fEY.dat&pid=1&action_name=a1"
./action_page.cgi > output
#Test resource evaluated value
if !(grep '<td>r2 = v0/v1/v2, r3 = v0/v1/v2/v3' output > /dev/null)
then
    echo; echo "r2 && r3 evaluated value failed"
fi
# Test resource actual value
# Go to supply resource binding page
export QUERY_STRING="resource_type=requires&action=change&process_filename=dfZRuitU82fEY.dat&pid=1&action_name=a1"
./action_page.cgi > output

if !(grep 'name="resource0" value="\${r1}/v2"' output > /dev/null)
then
    echo; echo "r2 actual value failed"
fi

if !(grep 'name="resource1" value="\${r2}/v3"' output > /dev/null)
then
    echo; echo "r3 actual value failed"
fi

# Test r4 && r5
# Go to a1
export QUERY_STRING="process_filename=dfZRuitU82fEY.dat&pid=1&action_name=a1"
./action_page.cgi > output
# Test resource evaluated value
if !(grep '<td>r4 = v0/v1/v2/v3/v4, r5 = v0/v1/v2/v3/v4/v5' output > /dev/null)
then
    echo; echo "r4 && r5 evaluated value failed"
fi
# Test resource actual value
# Go to supply resource binding page
export QUERY_STRING="resource_type=provides&action=change&process_filename=dfZRuitU82fEY.dat&pid=1&action_name=a1"
./action_page.cgi > output

if !(grep 'name="resource0" value="\${r3}/v4"' output > /dev/null)
then
    echo; echo "r4 actual value failed"
fi

if !(grep 'name="resource1" value="\${r4}/v5"' output > /dev/null)
then
    echo; echo "r5 actual value failed"
fi

echo "done"

