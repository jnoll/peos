
COPTS= TRACE DEBUG

#Define the object files
OBJS= 

#Define the name of the executable
PROG= product

#Target "all" - to compile source and create executable.
all	: ${PROG}

#Target "clean" - delete any object files and the executable
clean	:
	rm *.o ${PROG}

#Target "run" - execute the program
run	:
	./${PROG}

#Additional targets. Define as needed.
${PROG} : ${OBJ}
	wish -o ${PROG} ${OBJS}

${OBJS} :
	pmlUI.o : pmlUI.tcl
		wish -c pmlUI.tcl


#
# makefile  - end of file
# 
