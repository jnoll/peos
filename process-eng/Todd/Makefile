# compiler
PML = ../../bin/compiler

#PML flags
PMLFLAGS =

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

all: $(OBJS)
	$(PML) $(PMLFLAGS) $(OBJS)

.pml:
	$(PML) $(PMLFLAGS) $< $(OBJS)
	
install:
	cp *.gdbm ~tools/workspace/summer99/Engine/src/model/.

clean:
	rm -f *.gdbm
