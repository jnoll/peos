#these lines are for modification.
images= ./images/
path= ./bin/peos 
help= ./help/
pml= ./testfiles/ 
XERCES_PATH=./include/
JUNIT_PATH=./include/

#these lines are used by the makefile. Do not modify.
XERCES_INCLUDE=${XERCES_PATH}/xercesImpl.jar:${XERCES_PATH}/xmlParserAPIs.jar
JAVAGUI_JAR=./include/java-gui.jar
JUNIT_INCLUDE=${JUNIT_PATH}/junit.jar
java=-classpath ${CLASSPATH}:${XERCES_INCLUDE}:${JUNIT_INCLUDE}:${JAVAGUI_JAR}:.


all: PeosApp.class ProcessContent.class displayPO.class ActiveList.class DisabledPreferences.class \
	DisabledPreferencesFactory.class Help.class ActionMap.class LinkedList.class LinkNode.class \
	SetupPath.class MyFileFilter.class ResourceMap.class	
	jar cf java-gui.jar *.class
	mv java-gui.jar ./include

jtest: testLoadJU.class testActionMapJU.class testResourceMapJU.class testACJU.class testSFASJU.class \
	testPOJU.class testCRJU.class

	rm -f proc_table.dat
	rm -f proc_table.dat.xml
	java ${java} -Dpeos.path=${path} testPOJU
	rm -f proc_table.dat
	rm -f proc_table.dat.xml
	java ${java} -Dpeos.path=${path} testLoadJU
	java ${java} -Dpeos.path=${path} testSFASJU
	java ${java} -Dpeos.path=${path} testACJU
	java ${java} -Dpeos.path=${path} testCRJU
	java ${java} -Dpeos.path=${path} testActionMapJU
	java ${java} -Dpeos.path=${path} testResourceMapJU

setup: SetupPath.class
	java ${java} SetupPath
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
	echo "test -x ${path}" >>runpeos
	echo 'if test $$? -eq 1' >>runpeos
	echo "then" >>runpeos
	echo "	echo PEOS does not have execute permissions or does not exist." >>runpeos
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
	echo "java ${java} -Dpeos.images=${images} -Dpeos.path=${path} -Dpeos.help=${help} -Dpeos.pml=${pml}  PeosApp" >>runpeos
	echo "fi" >>runpeos
	chmod 700 runpeos

%.class : %.java
	  javac ${java} $<
