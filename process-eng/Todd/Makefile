# compiler
CC = ../../bin/compiler

#PML flags
CCFLAGS =

OBJS = Analysis.pml \
	OperationalConcept.pml \
	ProjectLog.pml \
	RepositoryCheckIn.pml \
	RepositoryCheckOut.pml \
	RepositorySynchronize.pml \
	RiskIdentification.pml \
	SourceCodeEdit.pml \
	FunctionalRequirements.pml \
	NonFunctionalRequirements.pml \
	Milestone2.pml \
	Milestone3.pml

# Compile the program

PML: $(OBJS)
	$(CC) $(CCFLAGS) $(OBJS)

.pml:
	$(CC) $(CCFLAGS) $< $(OBJS)
	
install:
	cp *.gdbm ~tools/workspace/summer99/Engine/src/model/.

clean:
	rm -f *.gdbm
