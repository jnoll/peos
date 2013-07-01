# Path to peos binary.  Only change this if peos is not in users' PATH or
# you want to use a specific instance of the binary.
# If you want to run the runpeos script in another directory, then you must specify the 
# correct variables below and then type `make`.  It's better to install the whole project because
# the install prefix is set for you.

path=../../os/kernel/peos  
images=./images/
help=./help/
pml=../../models/ 
JAVAGUI_JAR=./include/java-gui.jar

JFLAGS=-classpath ${CLASSPATH}:include/java-gui.jar:.


all: test jar script

jar: PeosApp.class ProcessContent.class displayPO.class ActiveList.class DisabledPreferences.class \
	DisabledPreferencesFactory.class Help.class ActionMap.class LinkedList.class LinkNode.class \
	SetupPath.class MyFileFilter.class ResourceMap.class SpecLoader.class \
	ActionViewer.class ActionList.class
	jar cf java-gui.jar *.class
	mv java-gui.jar ./include

disp:
	java ${JFLAGS} displayPO

# Run the java gui from working directory.
run:
	java -classpath include:${CLASSPATH} -Dpeos.images=./images -Dpeos.path=../../os/kernel/peos -Dpeos.help=./help -Dpeos.pml=./testfiles  PeosApp

test: testLoadJU.class testActionMapJU.class testResourceMapJU.class testACJU.class testSFASJU.class \
	testPOJU.class testCRJU.class testSpec.class

	rm -f proc_table.dat
	rm -f proc_table.dat.xml
	java ${JFLAGS} -Dpeos.path=${path} testPOJU
	rm -f proc_table.dat
	rm -f proc_table.dat.xml
	java ${JFLAGS} -Dpeos.path=${path} testLoadJU
	java ${JFLAGS} -Dpeos.path=${path} testSFASJU
	java ${JFLAGS} -Dpeos.path=${path} testACJU
	java ${JFLAGS} -Dpeos.path=${path} testCRJU
	java ${JFLAGS} -Dpeos.path=${path} testActionMapJU
	java ${JFLAGS} -Dpeos.path=${path} testResourceMapJU
	java ${JFLAGS} -Dpeos.path=${path} testSpec
	rm -f proc_table.dat
	rm -f proc_table.dat.xml

setup: SetupPath.class
	java ${JFLAGS} SetupPath
	xprop -root -remove _MOTIF_DEFAULT_BINDINGS

dist:
	rm -rf peos_app
	mkdir peos_app
	cp *.txt peos_app
	cp *.java peos_app
	rm -r peos_app/test*.java
	cp Makefile peos_app
	mkdir peos_app/include
	mkdir peos_app/testfiles
	mkdir peos_app/images
	mkdir peos_app/help
	mkdir peos_app/help/images
	cp -r include/* peos_app/include/
	cp -r testfiles/* peos_app/testfiles/
	cp -r images/* peos_app/images/
	cp -r help/* peos_app/help/
	rm -rf peos_app/include/CVS
	rm -rf peos_app/testfiles/CVS
	rm -rf peos_app/images/CVS
	rm -rf peos_app/help/images/CVS
	rm -rf peos_app/help/CVS
	tar cf peos_app.tar peos_app
	rm -r peos_app
clean:
	rm -rf *.class
	rm -f event.log
	rm -f proc_table*
	rm -rf include/java-gui.jar

script:
	echo "runApp=0" >runpeos
	echo "test -w ." >>runpeos
	echo 'if test $$? -eq 1'>>runpeos
	echo "then" >>runpeos
	echo "	echo Working directory must have write-permission.">>runpeos
	echo "	runApp=1;" >>runpeos
	echo "fi" >>runpeos
	echo "which ${path} > /dev/null" >>runpeos
	echo "if test $$? -eq 1" >>runpeos
	echo "then" >>runpeos
	echo "	echo 'peos' binary does not have execute permissions or does not exist." >>runpeos
	echo "	runApp=1" >>runpeos
	echo "fi" >>runpeos
	echo "test -r ${pml}">>runpeos
	echo 'if test $$? -eq 1' >>runpeos
	echo "then" >>runpeos
	echo "echo '<Warning>Default PML directory is not readable or does not exist.'" >>runpeos
	echo "fi" >>runpeos
	echo 'if test $$runApp -eq 0' >>runpeos
	echo "then" >>runpeos
	echo "	xprop -root -remove _MOTIF_DEFAULT_BINDINGS" >>runpeos
	echo "java -classpath $${CLASSPATH}:${JAVAGUI_JAR} -Dpeos.images=${images} -Dpeos.path=${path} -Dpeos.help=${help} -Dpeos.pml=${pml}  PeosApp" >>runpeos
	echo "fi" >>runpeos
	chmod +x runpeos

%.class : %.java
	  javac ${JFLAGS} $<
