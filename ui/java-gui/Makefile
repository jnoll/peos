java=	-classpath ./include/xercesImpl.jar:./include/xmlParserAPIs.jar:./include/junit.jar:.
j=	-classpath ./include/xercesImpl.jar:./include/xmlParserAPIs.jar:.

all: PeosApp.class ProcessContent.class displayPO.class ActiveList.class DisabledPreferences.class DisabledPreferencesFactory.class Help.class

test: testSFAS.class testPO.class testAC.class testLoad.class displayPO.class
	rm -f proc_table.dat
	rm -f proc_table.dat.xml
	java ${j} testPO
	rm -f proc_table.dat
	rm -f proc_table.dat.xml
	java ${j} testLoad
	java ${j} testSFAS
	java ${j} testAC
jtest: testLoadJU.class LoadProcess.class testACJU.class testSFASJU.class \
	testPOJU.class testCRJU.class

	rm -f proc_table.dat
	rm -f proc_table.dat.xml
	java ${java} testPOJU
	rm -f proc_table.dat
	rm -f proc_table.dat.xml
	java ${java} testLoadJU
	java ${java} testSFASJU
	java ${java} testCRJU

setup: SetupPath.class
	java ${j} SetupPath
	xprop -root -remove _MOTIF_DEFAULT_BINDINGS
disp: displayPO.class
#	java -classpath ./include/xercesImpl.jar:./include/xmlParserAPIs.jar:. displayPO
#mt: MyTest.class
#	java -classpath ./include/xercesImpl.jar:./include/xmlParserAPIs.jar:. MyTest
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
#	rm -f PeosApp.cfg
%.class : %.java
	  javac ${java} $<
