PML_HOME=../..
JAVA_HOME=/usr/local/java/jdk

# The java tools:
JAVAC=$(JAVA_HOME)/bin/javac
JAVA=$(JAVA_HOME)/bin/java
JAVAH=$(JAVA_HOME)/bin/javah

# The java class files
PML_CLASS_PATH=${PML_HOME}/out
JAVA_CLASSPATH=.:${JAVA_HOME}/lib/classes.zip:${PML_CLASS_PATH}


all: 
	echo ${JAVA_CLASS_PATH}
	${JAVA_HOME}/bin/javac -d ${PML_CLASS_PATH} -classpath ${JAVA_CLASSPATH} \
		*.java

clean:
	$(RM) ${PML_CLASS_PATH}/Utils/*.class
