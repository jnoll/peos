#these lines are for modification.
PEOS_HOME=${HOME}/.peos
INSTALL_PREFIX=${PEOS_HOME}
# Path to peos binary.  Only change this if peos is not in users' PATH or
# you want to use a specific instance of the binary.
path=peos  
images=${INSTALL_PREFIX}/images/
help=${INSTALL_PREFIX}/help/
pml=${INSTALL_PREFIX}/models/ 
runpeos=${HOME}/bin/runpeos
JAVAGUI_JAR=${INSTALL_PREFIX}/java-gui.jar

# these are obsolete.
XERCES_PATH= ${PWD}/include/
JUNIT_PATH= ${PWD}/include/


#these lines are used by the makefile. Do not modify.
XERCES_INCLUDE=${XERCES_PATH}/xercesImpl.jar:${XERCES_PATH}/xmlParserAPIs.jar
JUNIT_INCLUDE=${JUNIT_PATH}/junit.jar
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

# Note: doesn't uninstall models.
uninstall: 
	[ -d ${images} ] && rm -r ${images} 
	[ -d ${help} ] && rm -r ${help} 
	[ -f ${INSTALL_PREFIX}/runpeos ] && rm ${INSTALL_PREFIX}/runpeos
	[ -f ${INSTALL_PREFIX}/java-gui.jar ] && rm ${INSTALL_PREFIX}/java-gui.jar


install: script
	[ -d ${pml} ] || mkdir ${pml} 
	cp ./testfiles/*.pml ../../models/*.pml ${pml}
	cp ./runpeos ${runpeos}
	cp ./include/java-gui.jar ${INSTALL_PREFIX}
	[ -d $(images) ] || mkdir $(images)
	cp ./images/*.gif images/*.jpg ${images}
	[ -d ${help} ] || mkdir ${help} 
	cp ./help/*.* ${help}


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
	echo "cd ${PEOS_HOME}" >>runpeos 
	echo "	xprop -root -remove _MOTIF_DEFAULT_BINDINGS" >>runpeos
	echo "java -classpath $${CLASSPATH}:${JAVAGUI_JAR} -Dpeos.images=${images} -Dpeos.path=${path} -Dpeos.help=${help} -Dpeos.pml=${pml}  PeosApp" >>runpeos
	echo "fi" >>runpeos
	chmod +x runpeos

%.class : %.java
	  javac ${JFLAGS} $<
