# compiler:
CC = gcc 

PML_DIR=../../pml
CFLAGS = -g -I$(PML_DIR) -Wall

# keep track of dependencies automatically
.KEEP_STATE:

# lib
LIBRARY = libpeos.a
LIBS = $(LIBRARY) $(PML_DIR)/pml/libpml.a 

CORE_LIBS = 
LDFLAGS = -g -L../../lib

# objects
OBJS = \
$(CORE_OBJS)

PALM_CORE_OBJS = \
action.o \
error.o \
events.o \
graph.o \
graph_engine.o \
process_table.o \
process.o \
resources.o \
peos_util.o \

CORE_OBJS = \
predicate_evaluator.o \
${PALM_CORE_OBJS}

SHELL_TESTS = \
test_peos_with_login.sh \
test_peos.sh

# sources
SOURCES = $(OBJS:.o=.c)
HEADERS = $(wildcard *.h)

all: test shell lib peos shell_test

peos: peos.o $(CORE_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(CORE_LIBS) $(LIBS) -ltcl -lmcheck

palmpeos: peos.o $(PALM_CORE_OBJS)
	$(CC) $(LDFLAGS) -dPALM -o $@ $^ $(LIBS) 

daemon: enactment_daemon.o $(CORE_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(CORE_LIBS) $(LIBS) -lreadline -lhistory -lncurses

# Make the kernel library.
lib: $(LIBRARY)

$(LIBRARY): $(OBJS)
	rm -f $@
	ar cru $@ $(OBJS)
	ranlib $@ || true

# Compile the program
shell: shell.o $(LIBRARY)
	$(CC) $(LDFLAGS) -o $@ $< $(CORE_LIBS) $(LIBS) -lreadline -lhistory -lncurses -ltcl	

# Unit tests.
test: $(CORE_OBJS)
	$(MAKE) -C test 

shell_test:
	@for t in $(SHELL_TESTS) ; do sh $$t ; done

# Programming aids.
xref:
	ctags -x $(SOURCES) > $@

tags:	TAGS ctags

TAGS:	$(SOURCES) $(HEADERS)
	etags $^

ctags:	$(SOURCES) $(HEADERS)
	ctags $^

cflow:
	cflow -I../.. $(SOURCES) > cflow

clean:
	rm -f *.o core.* $(LIBRARY)
	$(MAKE) -C test clean

reallyclean: clean
	rm -f proc_table.dat proc_table.dat.xml peos shell

clobber: reallyclean

dist: reallyclean
	tar cvf kernel.tar --exclude-from exclude -C .. kernel

unittestable:

# Dependencies.
include .depend

# GNU Make will automatically maintain .depend.  This rule allows explicit
# recreation of dependencies.
depend: .depend

.depend: $(SOURCES)
	$(CC) $(CFLAGS) -MM $^ > $@
.PHONY: test
