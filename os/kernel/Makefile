# compiler:
CC = gcc

CFLAGS = -g -I../.. -Wall

# keep track of dependencies automatically
.KEEP_STATE:

# lib
LIBS = ../../pml/libpml.a

CORE_LIBS = 
LDFLAGS = -g -L../../lib

# objects
OBJS = \
$(CORE_OBJS)

CORE_OBJS = \
action.o \
events.o \
graph_engine.o \
process_table.o \
process.o \



# sources
SOURCES = $(OBJS:.o=.c)
HEADERS = $(wildcard *.h)

all: test shell

# Compile the program
shell: shell.o $(CORE_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(CORE_LIBS) $(LIBS) -lreadline -lhistory -ltermcap

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
	-rm *.o core.*

reallyclean: clean

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