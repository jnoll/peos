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
# For some reason this line doesn't work
#	$(PML) $(PMLFLAGS) $(OBJS)
	$(PML) $(PMLFLAGS) OperationalConcept.pml; \
	$(PML) $(PMLFLAGS) ProjectLog.pml; \
	$(PML) $(PMLFLAGS) RepositoryCheckIn.pml; \
	$(PML) $(PMLFLAGS) RepositoryCheckOut.pml; \
	$(PML) $(PMLFLAGS) RepositorySynchronize.pml; \
	$(PML) $(PMLFLAGS) RiskIdentification.pml; \
	$(PML) $(PMLFLAGS) SourceCodeEdit.pml; \
	$(PML) $(PMLFLAGS) FunctionalRequirements.pml; \
	$(PML) $(PMLFLAGS) NonFunctionalRequirements.pml; \
	$(PML) $(PMLFLAGS) Milestone2.pml; \
	$(PML) $(PMLFLAGS) Milestone3.pml

.pml:
	$(PML) $(PMLFLAGS) $< $(OBJS)
	
install:
	chmod 2664 *.gdbm
	cp *.gdbm ~peos/src/models/.

clean:
	rm -f *.gdbm
