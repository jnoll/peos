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
	[ -f PEOS_perform.jar ] && rm PEOS_perform.jar
	${JAVAC} ${APP_LIB_SOURCE}/*.java
	${JAVAC} *.java
	${JAVAJAR} -cmf manifest.my  PEOS_perform.jar ${APP_HOME}/*.class ${APP_LIB_SOURCE}/*.class
	[ -d $(APP_LIB) ] ||  mkdir $(APP_LIB) 
	#[ -f $(APP_LIB)/*.class ] && rm $(APP_LIB)/*.class
	mv ${APP_LIB_SOURCE}/*.class $(APP_LIB)
	rm ${APP_HOME}/*.class
