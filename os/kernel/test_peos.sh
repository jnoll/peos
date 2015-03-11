#!/bin/sh

echo ..testing peos ...
#create the pml program
tc=test_peos_1
echo "process p {" > ${tc}.pml
echo "action a {}" >> ${tc}.pml
echo "action b {}" >> ${tc}.pml
echo "action c {}" >> ${tc}.pml
echo "}" >> ${tc}.pml


#create a process
./peos -l $tc -c ${tc}.pml > ${tc}.output

if !(grep '<process pid=\"0\"' ${tc}.dat.xml | grep "./${tc}.pml" > /dev/null)
then
  echo "${tc}: Failed process creation process tag."
fi

if !(grep '<action name=\"a\" state=\"READY\">' ${tc}.dat.xml > /dev/null)
then
  echo "${tc}: Failed process creation action a tag."
fi

if !(grep '<action name=\"b\" state=\"AVAILABLE\">' ${tc}.dat.xml > /dev/null)
then
  echo "${tc}: Failed process creation action b tag."
fi

if !(grep '<action name=\"c\" state=\"AVAILABLE\">' ${tc}.dat.xml > /dev/null)
then
  echo "${tc}: Failed process creation action c tag."
fi

#start an action
./peos -l ${tc} -n 0 a start > ${tc}.output


if !(grep '<process pid=\"0\"' ${tc}.dat.xml |  grep 'status=\"2\"'  > /dev/null)
then
  echo "${tc}: Failed start action process tag."
fi

if !(grep '<action name=\"a\" state=\"RUN\">' ${tc}.dat.xml > /dev/null)
then
  echo "${tc}: Failed start action action a tag."
fi

if !(grep '<action name=\"b\" state=\"AVAILABLE\">' ${tc}.dat.xml > /dev/null)
then
  echo "${tc}: Failed start action action b tag."
fi

if !(grep '<action name=\"c\" state=\"AVAILABLE\">' ${tc}.dat.xml > /dev/null)
then
  echo "${tc}: Failed start action action c tag."
fi

#suspend an action
./peos -l ${tc} -n 0 a suspend > ${tc}.output

if !(grep '<process pid=\"0\"' ${tc}.dat.xml |  grep 'status=\"2\"'  > /dev/null)
then
   echo "${tc}: Failed suspend action process tag."
fi

if !(grep '<action name=\"a\" state=\"SUSPEND\">' ${tc}.dat.xml > /dev/null)
then
  echo "${tc}: Failed suspend action action a tag."
fi

if !(grep '<action name=\"b\" state=\"AVAILABLE\">' ${tc}.dat.xml > /dev/null)
then
  echo "${tc}: Failed suspend action action b tag."
fi

if !(grep '<action name=\"c\" state=\"AVAILABLE\">' ${tc}.dat.xml > /dev/null)
then
  echo "${tc}: Failed suspend action action c tag."
fi

#abort action


./peos -l ${tc} -n 0 a start > ${tc}.output
./peos -l ${tc} -n 0 a abort > ${tc}.output


if !(grep '<process pid=\"0\"' ${tc}.dat.xml |  grep 'status=\"2\"' > /dev/null)
then
  echo "${tc}: Failed abort action process tag."
fi

if !(grep '<action name=\"a\" state=\"AVAILABLE\">' ${tc}.dat.xml > /dev/null)
then
  echo "${tc}: Failed abort action action a tag."
fi

if !(grep '<action name=\"b\" state=\"AVAILABLE\">' ${tc}.dat.xml > /dev/null)
then
  echo "${tc}: Failed abort action action b tag."
fi

if !(grep '<action name=\"c\" state=\"AVAILABLE\">' ${tc}.dat.xml > /dev/null)
then
  echo "${tc}: Failed abort action action c tag."
fi


#finish action
./peos -l ${tc} -n 0 a finish > ${tc}.output


if !(grep '<process pid=\"0\"' ${tc}.dat.xml |  grep 'status=\"2\"' > /dev/null)
then
  echo "${tc}: Failed start action process tag."
fi

if !(grep '<action name=\"a\" state=\"DONE\">' ${tc}.dat.xml > /dev/null)
then
  echo "${tc}: Failed finish action action a tag."
fi

if !(grep '<action name=\"b\" state=\"READY\">' ${tc}.dat.xml > /dev/null)
then
  echo "${tc}: Failed finish action action b tag."
fi

if !(grep '<action name=\"c\" state=\"AVAILABLE\">' ${tc}.dat.xml > /dev/null)
then
  echo "${tc}: Failed finish action action c tag."
fi

old_tc=${tc}
tc=test_peos_2
cp ${old_tc}.pml ${tc}.pml
[ -f ${old_tc}.res ] && cp ${old_tc}.res ${tc}.res

# Create two instances.
./peos -l ${tc} -c ${tc}.pml > ${tc}.output
./peos -l ${tc} -c ${tc}.pml > ${tc}.output

# Delete first instance.
./peos -l ${tc} -d 0 > ${tc}.output

if (grep '<process pid=\"0\"' ${tc}.dat.xml > /dev/null)
then
  echo "${tc}: Failed delete process instance."
fi

if !(grep '<process pid=\"1\"' ${tc}.dat.xml | grep 'status=\"2\"'  > /dev/null)
then
  echo "${tc}: Failed delete process pid 1 tag."
fi

if !(grep '<action name=\"a\" state=\"READY\">' ${tc}.dat.xml > /dev/null)
then
  echo "${tc}: Failed delete process pid 1 tag."
fi

if !(grep '<action name=\"b\" state=\"AVAILABLE\">' ${tc}.dat.xml > /dev/null)
then
  echo "${tc}: Failed delete process pid 1 tag."
fi

if !(grep '<action name=\"c\" state=\"AVAILABLE\">' ${tc}.dat.xml > /dev/null)
then
  echo "${tc}: Failed delete process pid 1 tag."
fi

#test update state
tc=test_peos_3

echo "process p {" > ${tc}.pml
echo "action a {}" >> ${tc}.pml
echo "action b {}" >> ${tc}.pml
echo "action c {}" >> ${tc}.pml
echo "}" >> ${tc}.pml

./peos -l ${tc} -c ${tc}.pml > ${tc}.output
./peos -l ${tc} -u

if !(grep '<action name=\"a\" state=\"READY\">' ${tc}.dat.xml > /dev/null)
then
  echo "${tc}: Failed update state action a tag."
fi

if !(grep '<action name=\"b\" state=\"AVAILABLE\">' ${tc}.dat.xml > /dev/null)
then
  echo "${tc}: Failed update state action b tag."
fi

if !(grep '<action name=\"c\" state=\"AVAILABLE\">' ${tc}.dat.xml > /dev/null)
then
  echo "${tc}: Failed update state action c tag."
fi

# test resource value binding
tc=test_peos_4

echo "process p {" > ${tc}.pml
echo "action a {" >> ${tc}.pml
echo "requires{r1}" >> ${tc}.pml
echo "}" >> ${tc}.pml
echo "}" >> ${tc}.pml

./peos -l ${tc} -c ${tc}.pml > ${tc}.output

./peos -l ${tc} -r 0 r1 r1val > ${tc}.output


if !(grep '<req_resource name=\"r1\" value=\"r1val\" qualifier=\"\">' ${tc}.dat.xml > stdout)
then
  echo "${tc}: Failed resource value binding."
fi


# test create process with resource file
tc=test_peos_5

echo "process p {" > ${tc}.pml
echo "  action a {" >> ${tc}.pml
echo "      requires{r1}" >> ${tc}.pml
echo "      provides{r2 && r3}" >> ${tc}.pml
echo "  }" >> ${tc}.pml
echo "}" >> ${tc}.pml

echo "r1: r1val" > ${tc}.res
echo "r2: \${r1}/r2val" >> ${tc}.res
echo "r3: \${r2}/r3val" >> ${tc}.res

./peos -l ${tc} -c ${tc}.pml > ${tc}.output

if !(grep '<req_resource name=\"r1\" value=\"r1val\" qualifier=\"\">' ${tc}.dat.xml > stdout)
then
  echo "${tc}: Failed resource file binding for 'r1'.

fi

if !(grep '<prov_resource name=\"r2\" value=\"r1val/r2val\" qualifier=\"\">' ${tc}.dat.xml > stdout)
then
  echo "${tc}: Failed resource file binding for 'r2'.
fi

if !(grep '<prov_resource name=\"r3\" value=\"r1val/r2val/r3val\" qualifier=\"\">' ${tc}.dat.xml > stdout)
then
  echo "${tc}: Failed resource file binding for 'r3'."
fi

old_tc=${tc}
tc=test_peos_6
cp ${old_tc}.pml ${tc}.pml
[ -f ${old_tc}.res ] && cp ${old_tc}.res ${tc}.res

echo "r2: \${r1}/r2val" > ${tc}.res
echo "r3: \${r2}/r3val" >> ${tc}.res


./peos -l ${tc} -c ${tc}.pml  > ${tc}.output

if !(grep '<prov_resource name=\"r2\" value=\"\${r1}/r2val\" qualifier=\"\">' ${tc}.dat.xml > stdout)
then
  echo "${tc}: Failed 2nd resource file binding for 'r2'.
fi

if !(grep '<prov_resource name=\"r3\" value=\"\${r1}/r2val/r3val\" qualifier=\"\">' ${tc}.dat.xml > stdout)
then
  echo "${tc}: Failed 2nd resource file binding for 'r3'.
fi

old_tc=${tc}
tc=test_peos_7
cp ${old_tc}.pml ${tc}.pml
[ -f ${old_tc}.res ] && cp ${old_tc}.res ${tc}.res

echo "r1:" > ${tc}.res
echo "r2: \${r1}/r2val" >> ${tc}.res
echo "r3: \${r2}/r3val" >> ${tc}.res


./peos -l ${tc} -c ${tc}.pml > ${tc}.output

if !(grep '<prov_resource name=\"r2\" value=\"\${r1}/r2val\" qualifier=\"\">' ${tc}.dat.xml > stdout)
then
  echo "${tc}: Failed 3rd resource file binding for 'r2'.
fi

if !(grep '<prov_resource name=\"r3\" value=\"\${r1}/r2val/r3val\" qualifier=\"\">' ${tc}.dat.xml > stdout)
then
  echo "${tc}: Failed 3rd resource file binding for 'r3'.
fi


#test resources operator
echo "process p {" > ${tc}.pml
echo "action act0 {}" >> ${tc}.pml
echo "action act1 {provides{r1}}" >> ${tc}.pml
echo "action act2 {provides{r2}}" >> ${tc}.pml
echo "action act3 {provides{r1.filesize}}" >> ${tc}.pml
echo "action act4 {provides{r1.filesize > r2.filesize}}" >> ${tc}.pml
echo "}" >> ${tc}.pml

old_tc=${tc}
tc=test_peos_8
cp ${old_tc}.pml ${tc}.pml
[ -f ${old_tc}.res ] && cp ${old_tc}.res ${tc}.res

echo "small file" > ${tc}.small

echo "r1: ${tc}.pml" > ${tc}.res
echo "r2: ${tc}.small" >> peos_rest.res

./peos -l ${tc} -c ${tc}.pml > ${tc}.output

if !(grep '<action name=\"act0\" state=\"READY\">' ${tc}.dat.xml > stdout)
then
  echo "${tc}: Failed evaluating resources"
fi

if !(grep '<action name=\"act1\" state=\"SATISFIED\">' ${tc}.dat.xml > stdout)
then
  echo "${tc}: Failed evaluating resources"
fi

if !(grep '<action name=\"act3\" state=\"SATISFIED\">' ${tc}.dat.xml > stdout)
then
  echo "${tc}: Failed evaluating resources"
fi

if !(grep '<action name=\"act4\" state=\"SATISFIED\">' ${tc}.dat.xml > stdout)
then
  echo "${tc}: Failed evaluating resources"
fi

#################################
rm test_peos_*.small
rm test_peos_*.res
rm test_peos_*.dat
rm test_peos_*.dat.xml
rm test_peos_*.pml
rm test_peos_*.output

echo .. done.
