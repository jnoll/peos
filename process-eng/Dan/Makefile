# compiler
CC = ../../bin/compiler

#PML flags
CCFLAGS = -fGDBM
DEBUGFLAGS = -fCPML

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
	$(CC) $(CCFLAGS) PostMortem.pml

debug: 
## This line work but doesn't, mitigation follows
##	$(CC) $(DEBUGFLAGS) $< $(OBJS)
	$(CC) $(DEBUGFLAGS) Checkout.pml; \
	$(CC) $(DEBUGFLAGS) Commit.pml; \
	$(CC) $(DEBUGFLAGS) Update.pml; \
	$(CC) $(DEBUGFLAGS) Edit.pml; \
	$(CC) $(DEBUGFLAGS) Architecture.pml; \
	$(CC) $(DEBUGFLAGS) PostMortem.pml


test:
	cp *.gdbm ~tools/workspace/summer99/Engine/src/model/.
	chmod 2664 ~tools/workspace/summer99/Engine/src/model/*.gdbm

install:
	cp *.gdbm ../../models/.
	chmod 2664 ../../models/*.gdbm

clean:
	rm -f core
	rm -f *.gdbm
	rm -f *.cpml
	rm -f *.pml.pp
