#!/bin/sh

echo ..testing peos ...
#create the pml program

echo "process p {" > peos_test.pml
echo "action a {}" >> peos_test.pml
echo "action b {}" >> peos_test.pml
echo "action c {}" >> peos_test.pml
echo "}" >> peos_test.pml

if test -s proc_table.dat.xml 
then
mv proc_table.dat.xml old_proc_table.dat.xml 
fi

if test -s proc_table.dat 
then 
mv proc_table.dat old_proc_table.dat
fi

#create a process
./peos -c peos_test.pml > output

if !(grep '<process pid=\"0\" model=\"./peos_test.pml\" status=\"2\">' proc_table.dat.xml > /dev/null)
then
  echo
  echo Failed process creation process tag.
  echo
fi

if !(grep '<action name=\"a\" state=\"READY\">' proc_table.dat.xml > /dev/null)
then
  echo
  echo Failed process creation action a tag.
  echo
fi

if !(grep '<action name=\"b\" state=\"AVAILABLE\">' proc_table.dat.xml > /dev/null)
then
  echo
  echo Failed process creation action b tag.
  echo
fi

if !(grep '<action name=\"c\" state=\"AVAILABLE\">' proc_table.dat.xml > /dev/null)
then
  echo
  echo Failed process creation action c tag.
  echo
fi

#start an action
./peos -n 0 a start > output


if !(grep '<process pid=\"0\" model=\"./peos_test.pml\" status=\"2\">' proc_table.dat.xml > /dev/null)
then
  echo
  echo Failed start action process tag.
  echo
fi

if !(grep '<action name=\"a\" state=\"RUN\">' proc_table.dat.xml > /dev/null)
then
  echo
  echo Failed start action action a tag.
  echo
fi

if !(grep '<action name=\"b\" state=\"AVAILABLE\">' proc_table.dat.xml > /dev/null)
then
  echo
  echo Failed start action action b tag.
  echo
fi

if !(grep '<action name=\"c\" state=\"AVAILABLE\">' proc_table.dat.xml > /dev/null)
then
  echo
  echo Failed start action action c tag.
  echo
fi

#suspend an action
./peos -n 0 a suspend > output


if !(grep '<process pid=\"0\" model=\"./peos_test.pml\" status=\"2\">' proc_table.dat.xml > /dev/null)
then
  echo
  echo Failed suspend action process tag.
  echo
fi

if !(grep '<action name=\"a\" state=\"SUSPEND\">' proc_table.dat.xml > /dev/null)
then
  echo
  echo Failed suspend action action a tag.
  echo
fi

if !(grep '<action name=\"b\" state=\"AVAILABLE\">' proc_table.dat.xml > /dev/null)
then
  echo
  echo Failed suspend action action b tag.
  echo
fi

if !(grep '<action name=\"c\" state=\"AVAILABLE\">' proc_table.dat.xml > /dev/null)
then
  echo
  echo Failed suspend action action c tag.
  echo
fi

#abort action

./peos -n 0 a start > output
./peos -n 0 a abort > output


if !(grep '<process pid=\"0\" model=\"./peos_test.pml\" status=\"2\">' proc_table.dat.xml > /dev/null)
then
  echo
  echo Failed abort action process tag.
  echo
fi

if !(grep '<action name=\"a\" state=\"AVAILABLE\">' proc_table.dat.xml > /dev/null)
then
  echo
  echo Failed abort action action a tag.
  echo
fi

if !(grep '<action name=\"b\" state=\"AVAILABLE\">' proc_table.dat.xml > /dev/null)
then
  echo
  echo Failed abort action action b tag.
  echo
fi

if !(grep '<action name=\"c\" state=\"AVAILABLE\">' proc_table.dat.xml > /dev/null)
then
  echo
  echo Failed abort action action c tag.
  echo
fi


#finish action
./peos -n 0 a finish > output


if !(grep '<process pid=\"0\" model=\"./peos_test.pml\" status=\"2\">' proc_table.dat.xml > /dev/null)
then
  echo
  echo Failed start action process tag.
  echo
fi

if !(grep '<action name=\"a\" state=\"DONE\">' proc_table.dat.xml > /dev/null)
then
  echo
  echo Failed finish action action a tag.
  echo
fi

if !(grep '<action name=\"b\" state=\"READY\">' proc_table.dat.xml > /dev/null)
then
  echo
  echo Failed finish action action b tag.
  echo
fi

if !(grep '<action name=\"c\" state=\"AVAILABLE\">' proc_table.dat.xml > /dev/null)
then
  echo
  echo Failed finish action action c tag.
  echo
fi

./peos -c peos_test.pml > output

./peos -d 0 > output

if (grep '<process pid=\"0\"' proc_table.dat.xml > /dev/null)
then
  echo
  echo Failed delete process instance.
  echo
fi

if !(grep '<process pid=\"1\" model=\"./peos_test.pml\" status=\"2\">' proc_table.dat.xml > /dev/null)
then
  echo
  echo Failed delete process pid 1 tag.
  echo
fi

if !(grep '<action name=\"a\" state=\"READY\">' proc_table.dat.xml > /dev/null)
then
  echo
  echo Failed delete process pid 1 tag.
  echo
fi

if !(grep '<action name=\"b\" state=\"AVAILABLE\">' proc_table.dat.xml > /dev/null)
then
  echo
  echo Failed delete process pid 1 tag.
  echo
fi

if !(grep '<action name=\"c\" state=\"AVAILABLE\">' proc_table.dat.xml > /dev/null)
then
  echo
  echo Failed delete process pid 1 tag.
  echo
fi

rm proc_table.dat.xml
rm proc_table.dat
rm peos_test.pml 
rm output

#test update state

echo "process p {" > peos_test.pml
echo "action a {}" >> peos_test.pml
echo "action b {}" >> peos_test.pml
echo "action c {}" >> peos_test.pml
echo "}" >> peos_test.pml

./peos -c peos_test.pml > output
./peos -u

if !(grep '<action name=\"a\" state=\"READY\">' proc_table.dat.xml > /dev/null)
then
  echo
  echo Failed update state action a tag.
  echo
fi

if !(grep '<action name=\"b\" state=\"AVAILABLE\">' proc_table.dat.xml > /dev/null)
then
  echo
  echo Failed update state action b tag.
  echo
fi

if !(grep '<action name=\"c\" state=\"AVAILABLE\">' proc_table.dat.xml > /dev/null)
then
  echo
  echo Failed update state action c tag.
  echo
fi


rm proc_table.dat.xml
rm proc_table.dat
rm peos_test.pml 
rm output



# test resource value binding

echo "process p {" > peos_test.pml
echo "action a {" >> peos_test.pml
echo "requires{r1}" >> peos_test.pml
echo "}" >> peos_test.pml
echo "}" >> peos_test.pml

./peos -c peos_test.pml > output

./peos -r 0 r1 r1val > output


if !(grep '<req_resource name=\"r1\" value=\"r1val\" qualifier=\"\">' proc_table.dat.xml > stdout)
then
  echo
  echo Failed resource value binding.
  echo
fi


rm proc_table.dat
rm proc_table.dat.xml
rm peos_test.pml
rm output

# test create process with resource file
echo "process p {" > peos_test.pml
echo "  action a {" >> peos_test.pml
echo "      requires{r1}" >> peos_test.pml
echo "      provides{r2 && r3}" >> peos_test.pml
echo "  }" >> peos_test.pml
echo "}" >> peos_test.pml

echo "r1: r1val" > peos_test.res
echo "r2: \${r1}/r2val" >> peos_test.res
echo "r3: \${r2}/r3val" >> peos_test.res

./peos -c peos_test.pml > output
<<<<<<< HEAD
cp proc_table.dat.xml ./a

if !(grep '<req_resource name=\"r1\" value=\"r1val\" qualifier=\"\">' proc_table.dat.xml > stdout)
then
  echo
  echo Failed resource 'file' binding.
  echo
=======
cp proc_table.dat.xml ./a.dat.xml

if !(grep '<req_resource name=\"r1\" value=\"r1val\" qualifier=\"\">' proc_table.dat.xml > stdout)
then
  echo Failed resource file binding for 'r1'.
>>>>>>> 71f6a61cb06573d2dbf317e9ee585b978950aef8
fi

if !(grep '<prov_resource name=\"r2\" value=\"r1val/r2val\" qualifier=\"\">' proc_table.dat.xml > stdout)
then
<<<<<<< HEAD
  echo
  echo Failed resource 'file' binding.
  echo
=======
  echo Failed resource file binding for 'r2'.
>>>>>>> 71f6a61cb06573d2dbf317e9ee585b978950aef8
fi

if !(grep '<prov_resource name=\"r3\" value=\"r1val/r2val/r3val\" qualifier=\"\">' proc_table.dat.xml > stdout)
then
<<<<<<< HEAD
  echo
  echo Failed resource 'file' binding.
  echo
=======
  echo Failed resource file binding for 'r3'.
>>>>>>> 71f6a61cb06573d2dbf317e9ee585b978950aef8
fi

echo "r2: \${r1}/r2val" > peos_test.res
echo "r3: \${r2}/r3val" >> peos_test.res

./peos -c peos_test.pml > output
<<<<<<< HEAD

if !(grep '<prov_resource name=\"r2\" value=\"\${r1}/r2val\" qualifier=\"\">' proc_table.dat.xml > stdout)
then
  echo
  echo Failed resource 'file' binding.
  echo
=======
cp proc_table.dat.xml ./b.dat.xml

if !(grep '<prov_resource name=\"r2\" value=\"\${r1}/r2val\" qualifier=\"\">' proc_table.dat.xml > stdout)
then
  echo Failed 2nd resource file binding for 'r2'.
>>>>>>> 71f6a61cb06573d2dbf317e9ee585b978950aef8
fi

if !(grep '<prov_resource name=\"r3\" value=\"\${r1}/r2val/r3val\" qualifier=\"\">' proc_table.dat.xml > stdout)
then
<<<<<<< HEAD
  echo
  echo Failed resource 'file' binding.
  echo
=======
  echo Failed 2nd resource file binding for 'r3'.
>>>>>>> 71f6a61cb06573d2dbf317e9ee585b978950aef8
fi

echo "r1:" > peos_test.res
echo "r2: \${r1}/r2val" >> peos_test.res
echo "r3: \${r2}/r3val" >> peos_test.res

<<<<<<< HEAD
if !(grep '<prov_resource name=\"r2\" value=\"\${r1}/r2val\" qualifier=\"\">' proc_table.dat.xml > stdout)
then
  echo
  echo Failed resource 'file' binding.
  echo
=======
./peos -c peos_test.pml > output
cp proc_table.dat.xml ./c.dat.xml

if !(grep '<prov_resource name=\"r2\" value=\"\${r1}/r2val\" qualifier=\"\">' proc_table.dat.xml > stdout)
then
  echo Failed 3rd resource file binding for 'r2'.
>>>>>>> 71f6a61cb06573d2dbf317e9ee585b978950aef8
fi

if !(grep '<prov_resource name=\"r3\" value=\"\${r1}/r2val/r3val\" qualifier=\"\">' proc_table.dat.xml > stdout)
then
<<<<<<< HEAD
  echo
  echo Failed resource 'file' binding.
  echo
=======
  echo Failed 3rd resource file binding for 'r3'.
>>>>>>> 71f6a61cb06573d2dbf317e9ee585b978950aef8
fi

rm peos_test.res
rm proc_table.dat
rm proc_table.dat.xml
rm peos_test.pml
rm output

#test resources operator
echo "process p {" > peos_test.pml
echo "action act0 {}" >> peos_test.pml
echo "action act1 {provides{r1}}" >> peos_test.pml
echo "action act2 {provides{r2}}" >> peos_test.pml
echo "action act3 {provides{r1.filesize}}" >> peos_test.pml
echo "action act4 {provides{r1.filesize > r2.filesize}}" >> peos_test.pml
echo "}" >> peos_test.pml

echo "small file" > small

echo "r1: peos_test.pml" > peos_test.res
echo "r2: small" >> peos_rest.res

./peos -c peos_test.pml > output

if !(grep '<action name=\"act0\" state=\"READY\">' proc_table.dat.xml > stdout)
then
  echo
  echo Failed evaluating resources
  echo
fi

if !(grep '<action name=\"act1\" state=\"SATISFIED\">' proc_table.dat.xml > stdout)
then
  echo
  echo Failed evaluating resources
  echo
fi

if !(grep '<action name=\"act3\" state=\"SATISFIED\">' proc_table.dat.xml > stdout)
then
  echo
  echo Failed evaluating resources
  echo
fi

if !(grep '<action name=\"act4\" state=\"SATISFIED\">' proc_table.dat.xml > stdout)
then
  echo
  echo Failed evaluating resources
  echo
fi
#################################
rm small
rm peos_test.res
rm proc_table.dat
rm proc_table.dat.xml
rm peos_test.pml
rm output

if test -s old_proc_table.dat.xml 
then
mv old_proc_table.dat.xml proc_table.dat.xml 
fi

if test -s old_proc_table.dat 
then 
mv old_proc_table.dat proc_table.dat
fi

echo .. done.
