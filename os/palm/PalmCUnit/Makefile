
APPNAME = PalmCUnit
APPID = UntT

CC = m68k-palmos-gcc
PILRC = pilrc
BUILDPRC = build-prc
MULTIGEN = m68k-palmos-multigen

# assign a name for a target that will be built each time the test runner is 
# built.  This is to allow the makefile to do any kind of setup necessary in 
# the client code.  (Actually, the name doesn't matter since the rule uses 
# the TESTEDAPP macro anyway)
TESTEDAPP = tested_app
TESTEDDIR = ..

# if you want to store the framework files somewhere else, you can put that 
# path here
FRAMEWORKDIR = .

# add any path necessary to include headers for the code you're testing
INC_PATH = -I.. -I../../kernel -I/usr/m68k-palmos -I../../..

# Objects for the PalmCUnit app, the code being tested, and your testing code
OBJS = \
	PalmCUnit.o \
	TestParser.o
	

# These objects are for the CUnit framework itself.
FRAMEWORKOBJS = \
	TestRun.o \
	TestDB.o \
	AssertImpl.o \
	Util.o

# Pretty standard CFLAGS.  Be sure to define any symbols necessary to tell 
# your client code that it is being tested (for inclusion of mock objects, 
# etc.)
CFLAGS = -g -Wall -W -Wno-unused -Wno-switch -Wno-sign-compare -DPALM $(INC_PATH)
LIBS = -lPalmOSGlue -lgcc ../stdio.o \
	../../../pml/libpalmpml.a  \
	../../kernel/libpalmpeos.a

LDFLAGS = -g

.c.o:
	$(CC) $(CFLAGS) -c $<

# Include a dependency for your application
all: $(TESTEDAPP) $(APPNAME).prc

# I define a target on my main project's makefile called "unittestable".  It 
# does whatever is necessary to make the code ready for compilation into the 
# test runner.
$(TESTEDAPP):
	cd ..; make unittestable

clean: temp
	rm -f $(APPNAME).prc

# This is the target that makes the initial object code of the runner.  NOTE: 
# it outputs a .map file that will contain any error messages if the build 
# encounters a problem.  The map file is to help with debugging segmenting 
# complications that can arise from larger code bases.
$(APPNAME): $(OBJS) $(FRAMEWORKOBJS) PalmCUnit-sections.o PalmCUnit-sections.ld
	$(CC) -Xlinker -M -Xlinker --cref $(LDFLAGS) $(OBJS) $(FRAMEWORKOBJS) PalmCUnit-sections.o ${LIBS} -o $@ PalmCUnit-sections.ld \
2> PalmCUnit.map 
#> PalmCUnit.map 2>&1

$(APPNAME).prc: $(APPNAME)
	$(BUILDPRC) PalmCUnit.def -o $(APPNAME).prc -n $(APPNAME) -c $(APPID) PalmCUnit.ro $(APPNAME)

$(OBJS): PalmCUnitRsc.h

PalmCUnitRsc.h: PalmCUnit.rcp
	$(PILRC) -ro -allowEditID -q PalmCUnit.rcp PalmCUnit.ro

# Falch.net sends a "make temp" when cleaning up objects
temp:
	rm -f *.o $(APPNAME) PalmCUnit.map *.bin *.out PalmCUnit.ro *.grc PalmCUnitRsc.h PalmCUnit-sections.s PalmCUnit-sections.ld

PalmCUnit-sections.o: PalmCUnit-sections.s
	$(CC) -c PalmCUnit-sections.s

PalmCUnit-sections.s PalmCUnit-sections.ld: PalmCUnit.def
	$(MULTIGEN) PalmCUnit.def


## CUnit framework objs here
TestRun.o: $(FRAMEWORKDIR)/TestRun.c
	$(CC) $(CFLAGS) -c $<

TestDB.o: $(FRAMEWORKDIR)/TestDB.c
	$(CC) $(CFLAGS) -c $<

AssertImpl.o: $(FRAMEWORKDIR)/AssertImpl.c
	$(CC) $(CFLAGS) -c $<

Util.o: $(FRAMEWORKDIR)/Util.c
	$(CC) $(CFLAGS) -c $<


## application specific objs here
counter.o: $(TESTEDDIR)/counter.c
	$(CC) $(CFLAGS) -c $<

