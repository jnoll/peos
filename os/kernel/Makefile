# compiler:
CC = gcc

CFLAGS = -g -I../.. -Wall

# keep track of dependencies automatically
.KEEP_STATE:

# lib
LIBRARY = libpeos.a
LIBS = $(LIBRARY) ../../pml/libpml.a 

CORE_LIBS = -ltcl
LDFLAGS = -g -L../../lib

# objects
OBJS = \
$(CORE_OBJS)

CORE_OBJS = \
tclinterp.o \
action.o \
events.o \
graph.o \
graph_engine.o \
process_table.o \
process.o \
resources.o \
predicate_evaluator.o \

# sources
SOURCES = $(OBJS:.o=.c)
HEADERS = $(wildcard *.h)

all: test shell lib peos

peos: peos.o $(CORE_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(CORE_LIBS) $(LIBS) 

daemon: enactment_daemon.o $(CORE_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(CORE_LIBS) $(LIBS) -lreadline -lhistory -ltermcap

# Make the kernel library.
lib: $(LIBRARY)

$(LIBRARY): $(OBJS)
	rm -f $@
	ar cru $@ $(OBJS)
	ranlib $@ || true

# Compile the program
shell: shell.o $(LIBRARY)
	$(CC) $(LDFLAGS) -o $@ $< $(CORE_LIBS) $(LIBS) -lreadline -lhistory -ltermcap

# Unit tests.
test: $(CORE_OBJS)
	$(MAKE) -C test 

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
	rm -f *.o core.*

reallyclean: clean
	rm -f proc_table.dat proc_table.dat.xml peos shell

dist: reallyclean
	tar cvf kernel.tar --exclude-from exclude -C .. kernel

# Dependencies.
include .depend

# GNU Make will automatically maintain .depend.  This rule allows explicit
# recreation of dependencies.
depend: .depend

.depend: $(SOURCES)
	$(CC) $(CFLAGS) -MM $^ > $@
.PHONY: test
