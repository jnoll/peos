# compiler
CC = ../../compiler/compiler

#PML flags
CCFLAGS = -fGDBM

OBJS:=$(wildcard *.pml)

# Compile the program

PML: $(OBJS)
	$(CC) $(CCFLAGS) $(OBJS)

.pml:
	$(CC) $(CCFLAGS) $< $(OBJS)
	
install:
	cp *.gdbm ~tools/workspace/summer99/Engine/src/model/.
	chmod 2664 ~tools/workspace/summer99/Engine/src/model/*.gdbm

clean:
	rm -f *.gdbm
