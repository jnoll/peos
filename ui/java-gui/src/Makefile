JAVA_HOME=/usr/local/java/jdk

# The java tools:
JAVAR=$(JAVA_HOME)/bin/jar

# The java class files
JAVA_CLASS_PATH=../out

all:
	cd PML; make all
	cd Utils; make all
	cd Kernel; make all
	cd Commands; make all
	cd GUIComponents; make all
	cd ${JAVA_CLASS_PATH}; ${JAVAR} -cmf0 ../src/manifest.tmp  pml_gui.jar \
		PMLCommands/*.class \
		GUIComponents/*.class \
		Kernel/*.class \
		PML/*.class \
		Utils/*.class

clean:
	cd PML; make clean
	cd Utils; make clean
	cd Kernel; make clean
	cd Commands; make clean
	cd GUIComponents; make clean
	rm -f ../out/pml_gui.jar
