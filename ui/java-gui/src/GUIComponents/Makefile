PML_HOME=../..
JAVA_HOME=/usr/local/java/jdk

# The java tools:
JAVAC=$(JAVA_HOME)/bin/javac
JAVA=$(JAVA_HOME)/bin/java
JAVAH=$(JAVA_HOME)/bin/javah

# The java class files
JAVA_CLASS_PATH=${PML_HOME}/out

all: 
	${JAVA_HOME}/bin/javac -d ${JAVA_CLASS_PATH} *.java

clean:
	$(RM) ${JAVA_CLASS_PATH}/GUIComponents/*.class
