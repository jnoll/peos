#
# This makefile generates the java-gui-2 engineInterface classes
#
 
#
# homes. 
#
APP_HOME=..
JAVA_HOME=/usr/local/java/jdk1.2.2

# The java compiler
JAVAC=$(JAVA_HOME)/bin/javac

# The java class files
APP_CLASS_PATH=${APP_HOME}/classes
JAVA_CLASS_PATH=.:${JAVA_HOME}/lib/classes.zip:$(APP_CLASS_PATH)

all:
	$(JAVAC) -d ${APP_CLASS_PATH} -classpath ${JAVA_CLASS_PATH} *.java

clean:
	rm ${APP_CLASS_PATH}/*.class
	
