###################################################
#                                                 #
# Process Engineering - Product Modeling Makefile #
#                                                 #
###################################################

# MAKE flags
MAKEFLAGS = -k

# PML file extensions
SUFFIXES = pml
.SUFFIXES = $(SUFFIXES)

# PML compiler
PML = ../../compiler/compiler

# PML gdbm flags
PMLFLAGS = -f GDBM

# PML raw source location
SRCLOC = .

# PML compiled source location
OBJLOC = .

# PML compiled source 
OBJECTS = $(OBJLOC)/checkin.gdbm \
	$(OBJLOC)/checkout.gdbm \
	$(OBJLOC)/make.gdbm \
	$(OBJLOC)/milestone1.gdbm \
	$(OBJLOC)/milestone5.gdbm \
	$(OBJLOC)/updateANDresolve.gdbm 

all: $(OBJECTS)

exec: $(OBJECTS)

# PML compilation
$(OBJLOC)/%.gdbm : $(SRCLOC)/%.pml
	$(PML) $(PMLFLAGS) $(SRCLOC)/$*.pml

install:
	cp *.gdbm ../../compiler/models
	cp *.gdbm ~tools/workspace/summer99/Engine/src/model

clean: 
	rm -f *.gdbm
