#!/bin/sh

echo ..testing peos with login ...
#create the pml program

echo "process p {" > peos_test.pml
echo "action a {}" >> peos_test.pml
echo "action b {}" >> peos_test.pml
echo "action c {}" >> peos_test.pml
echo "}" >> peos_test.pml

if test -s test_user.dat.xml 
then
mv test_user.dat.xml old_test_user.dat.xml 
fi

if test -s test_user.dat 
then 
mv test_user.dat old_test_user.dat
fi

#create a process
peos -l test_user -c peos_test.pml > output


if !(grep '<process pid=\"0\" model=\"./peos_test.pml\" status=\"2\">' test_user.dat.xml > /dev/null)
then
  echo
  echo Failed process creation process tag.
  echo
fi

if !(grep '<action name=\"a\" state=\"READY\">' test_user.dat.xml > /dev/null)
then
  echo
  echo Failed process creation action a tag.
  echo
fi

if !(grep '<action name=\"b\" state=\"AVAILABLE\">' test_user.dat.xml > /dev/null)
then
  echo
  echo Failed process creation action b tag.
  echo
fi

if !(grep '<action name=\"c\" state=\"AVAILABLE\">' test_user.dat.xml > /dev/null)
then
  echo
  echo Failed process creation action c tag.
  echo
fi

#start an action
peos -l test_user -n 0 a start > output


if !(grep '<process pid=\"0\" model=\"./peos_test.pml\" status=\"2\">' test_user.dat.xml > /dev/null)
then
  echo
  echo Failed start action process tag.
  echo
fi

if !(grep '<action name=\"a\" state=\"RUN\">' test_user.dat.xml > /dev/null)
then
  echo
  echo Failed start action action a tag.
  echo
fi

if !(grep '<action name=\"b\" state=\"AVAILABLE\">' test_user.dat.xml > /dev/null)
then
  echo
  echo Failed start action action b tag.
  echo
fi

if !(grep '<action name=\"c\" state=\"AVAILABLE\">' test_user.dat.xml > /dev/null)
then
  echo
  echo Failed start action action c tag.
  echo
fi

#suspend an action
peos -l test_user -n 0 a suspend > output


if !(grep '<process pid=\"0\" model=\"./peos_test.pml\" status=\"2\">' test_user.dat.xml > /dev/null)
then
  echo
  echo Failed suspend action process tag.
  echo
fi

if !(grep '<action name=\"a\" state=\"SUSPEND\">' test_user.dat.xml > /dev/null)
then
  echo
  echo Failed suspend action action a tag.
  echo
fi

if !(grep '<action name=\"b\" state=\"AVAILABLE\">' test_user.dat.xml > /dev/null)
then
  echo
  echo Failed suspend action action b tag.
  echo
fi

if !(grep '<action name=\"c\" state=\"AVAILABLE\">' test_user.dat.xml > /dev/null)
then
  echo
  echo Failed suspend action action c tag.
  echo
fi

#abort action

peos -l test_user -n 0 a start > output
peos -l test_user -n 0 a abort > output


if !(grep '<process pid=\"0\" model=\"./peos_test.pml\" status=\"2\">' test_user.dat.xml > /dev/null)
then
  echo
  echo Failed abort action process tag.
  echo
fi

if !(grep '<action name=\"a\" state=\"AVAILABLE\">' test_user.dat.xml > /dev/null)
then
  echo
  echo Failed abort action action a tag.
  echo
fi

if !(grep '<action name=\"b\" state=\"AVAILABLE\">' test_user.dat.xml > /dev/null)
then
  echo
  echo Failed abort action action b tag.
  echo
fi

if !(grep '<action name=\"c\" state=\"AVAILABLE\">' test_user.dat.xml > /dev/null)
then
  echo
  echo Failed abort action action c tag.
  echo
fi


#finish action
peos -l test_user -n 0 a finish > output


if !(grep '<process pid=\"0\" model=\"./peos_test.pml\" status=\"2\">' test_user.dat.xml > /dev/null)
then
  echo
  echo Failed start action process tag.
  echo
fi

if !(grep '<action name=\"a\" state=\"DONE\">' test_user.dat.xml > /dev/null)
then
  echo
  echo Failed finish action action a tag.
  echo
fi

if !(grep '<action name=\"b\" state=\"READY\">' test_user.dat.xml > /dev/null)
then
  echo
  echo Failed finish action action b tag.
  echo
fi

if !(grep '<action name=\"c\" state=\"AVAILABLE\">' test_user.dat.xml > /dev/null)
then
  echo
  echo Failed finish action action c tag.
  echo
fi

peos -l test_user -c peos_test.pml > output

peos -l test_user -d 0 > output

if (grep '<process pid=\"0\"' test_user.dat.xml > /dev/null)
then
  echo
  echo Failed delete process instance.
  echo
fi

if !(grep '<process pid=\"1\" model=\"./peos_test.pml\" status=\"2\">' test_user.dat.xml > /dev/null)
then
  echo
  echo Failed delete process pid 1 process tag.
  echo
fi

if !(grep '<action name=\"a\" state=\"READY\">' test_user.dat.xml > /dev/null)
then
  echo
  echo Failed process delete pid 1 action a tag.
  echo
fi

if !(grep '<action name=\"b\" state=\"AVAILABLE\">' test_user.dat.xml > /dev/null)
then
  echo
  echo Failed process delete pid 1 action b tag.
  echo
fi

if !(grep '<action name=\"c\" state=\"AVAILABLE\">' test_user.dat.xml > /dev/null)
then
  echo
  echo Failed process delete pid 1 action c tag.
  echo
fi

rm test_user.dat.xml
rm test_user.dat
rm peos_test.pml 
rm output

#test update state

echo "process p {" > peos_test.pml
echo "action a {}" >> peos_test.pml
echo "action b {}" >> peos_test.pml
echo "action c {}" >> peos_test.pml
echo "}" >> peos_test.pml

peos -l test_user -c peos_test.pml > output
peos -l test_user -u

if !(grep '<action name=\"a\" state=\"READY\">' test_user.dat.xml > /dev/null)
then
  echo
  echo Failed update state action a tag.
  echo
fi

if !(grep '<action name=\"b\" state=\"AVAILABLE\">' test_user.dat.xml > /dev/null)
then
  echo
  echo Failed update state action b tag.
  echo
fi

if !(grep '<action name=\"c\" state=\"AVAILABLE\">' test_user.dat.xml > /dev/null)
then
  echo
  echo Failed update state action c tag.
  echo
fi


rm test_user.dat.xml
rm test_user.dat
rm peos_test.pml 
rm output



# test resource value binding

echo "process p {" > peos_test.pml
echo "action a {" >> peos_test.pml
echo "requires{r1}" >> peos_test.pml
echo "}" >> peos_test.pml
echo "}" >> peos_test.pml

peos -l test_user -c peos_test.pml > output

peos -l test_user -r 0 r1 r1val > output


if !(grep '<req_resource name=\"r1\" value=\"r1val\" qualifier=\"\">' test_user.dat.xml > stdout)
then
  echo
  echo Failed resource value binding.
  echo
fi

rm test_user.dat
rm test_user.dat.xml
rm peos_test.pml
rm output

if test -s old_test_user.dat.xml 
then
mv old_test_user.dat.xml test_user.dat.xml 
fi

if test -s old_test_user.dat 
then 
mv old_test_user.dat test_user.dat
fi

echo .. done.
