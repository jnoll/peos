images= ./images/
path= ./bin/peos
help= ./help/
pml= /tmp/pml #./testfiles/
XERCES_PATH=./include/
XERCES_INCLUDE=${XERCES_PATH}/xercesImpl.jar:${XERCES_PATH}/xmlParserAPIs.jar
JUNIT_PATH=./include/
JUNIT_INCLUDE=${JUNIT_PATH}/junit.jar
JAVAGUI_JAR=./include/java-gui.jar
java=-classpath ${CLASSPATH}:${XERCES_INCLUDE}:${JUNIT_INCLUDE}:${JAVAGUI_JAR}:.


all: PeosApp.class ProcessContent.class displayPO.class ActiveList.class DisabledPreferences.class DisabledPreferencesFactory.class Help.class

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
	cp runpeos peos_app
	cp Makefile peos_app
	mkdir peos_app/include
	mkdir peos_app/testfiles
	mkdir peos_app/images
	mkdir peos_app/help
	mkdir peos_app/help/images
	rm -rf include/CVS
	rm -rf testfiles/CVS
	rm -rf images/CVS
	rm -rf help/images/CVS
	rm -rf help/CVS
	cp include/* peos_app/include/
	cp testfiles/* peos_app/testfiles/
	cp images/* peos_app/images/
	cp -r help/* peos_app/help/
	tar cf peos_app.tar peos_app
	rm -r peos_app
clean:
	rm -rf *.class
	rm -f event.log
	rm -f proc_table*

script: 
	echo "xprop -root -remove _MOTIF_DEFAULT_BINDINGS" >runpeos
	echo "java ${java} -Dpeos.images=${images} -Dpeos.path=${path} -Dpeos.help=${help} -Dpeos.pml=${pml} PeosApp" >>runpeos
	chmod 700 runpeos

%.class : %.java
	  javac ${java} $<
