# compiler
CC = ../../bin/compiler

#PML flags
CCFLAGS = -fGDBM

OBJS:=$(wildcard *.pml)

# Compile the program

PML: $(OBJS)
## This line work but doesn't, mitigation follows
##	$(CC) $(CCFLAGS) $< $(OBJS)
	$(CC) $(CCFLAGS) Checkout.pml; \
	$(CC) $(CCFLAGS) Commit.pml; \
	$(CC) $(CCFLAGS) Update.pml; \
	$(CC) $(CCFLAGS) Edit.pml; \
	$(CC) $(CCFLAGS) Architecture.pml; \
	$(CC) $(CCFLAGS) PostMortem.pml;

	
test:
	cp *.gdbm ~tools/workspace/summer99/Engine/src/model/.
	chmod 2664 ~tools/workspace/summer99/Engine/src/model/*.gdbm

install:
	cp *.gdbm ~tools/workspace/summer99/Engine/src/model/.
	chmod 2664 ~tools/workspace/summer99/Engine/src/model/*.gdbm

clean:
	rm -f *.gdbm
	rm -f *.pml.pp
