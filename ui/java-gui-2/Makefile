#
# This makefile generates the java-gui-2 jar
#
 
#
# homes 
#
APP_HOME=.
JAVA_HOME=/usr/local/java/jdk1.2.2

#
# The java tools
#
JAVAC=${JAVA_HOME}/bin/javac
JAVAJAR=${JAVA_HOME}/bin/jar

# The java class files
APP_CLASS_PATH=${APP_HOME}/classes
MYLIB_CLASS_PATH=${APP_HOME}/classes/engineInterface
JAVA_CLASS_PATH=${APP_CLASS_PATH}:${JAVA_HOME}/lib/classes.zip

all:
	[ -d $(APP_CLASS_PATH) ] ||  mkdir $(APP_CLASS_PATH) 
	cd engineInterface; make all
	${JAVAC} -d ${APP_CLASS_PATH} -classpath ${JAVA_CLASS_PATH} *.java
	${JAVAJAR} -cmf manifest.my  PEOS_perform.jar \
		${APP_CLASS_PATH}/*.class ${MYLIB_CLASS_PATH}/*.class 

clean:
	rm $(MYLIB_CLASS_PATH)/*.class
	rmdir $(MYLIB_CLASS_PATH)
	rm ${APP_CLASS_PATH}/*.class
	rmdir $(APP_CLASS_PATH)	
