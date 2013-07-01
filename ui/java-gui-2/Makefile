#
# This makefile generates the java-gui-2 jar
#
 
#
# The java 
#
JAVA_HOME=/usr/local/java/jdk1.2.2
JAVAC=${JAVA_HOME}/bin/javac
JAVAJAR=${JAVA_HOME}/bin/jar

#
# The application
#
APP_HOME=.
APP_LIB_SOURCE=${APP_HOME}/engineInterface
APP_LIB=../../share/java/engineInterface

all:
#	rm PEOS_perform.jar ?? if exits

	${JAVAC} ${APP_LIB_SOURCE}/*.java
	${JAVAC} *.java
	${JAVAJAR} -cmf manifest.my  PEOS_perform.jar ${APP_HOME}/*.class ${APP_LIB_SOURCE}/*.class

# do I need to make path? if exists rm $(APP_LIB)/*.class ??
	[ -d $(APP_LIB) ] ||  mkdir $(APP_LIB) 
#	rm $(APP_LIB)/*.class
	mv ${APP_LIB_SOURCE}/*.class $(APP_LIB)
	rm ${APP_HOME}/*.class
