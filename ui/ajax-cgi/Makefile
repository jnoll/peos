# compiler:
CC = gcc 

PML_DIR=../../pml
KERNEL_DIR=../../os/kernel
CFLAGS = -g -I$(PML_DIR) -I$(KERNEL_DIR) -Wall

# keep track of dependencies automatically
.KEEP_STATE:

# lib
LIBS = $(KERNEL_DIR)/libpeos.a $(PML_DIR)/pml/libpml.a 

CORE_LIBS = 
LDFLAGS = -g

# objects
OBJS = peos.o getcgi.c


# sources
SOURCES = $(OBJS:.o=.c) 
HEADERS = $(wildcard *.h)

all: peos.cgi

peos.cgi: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS) -ltcl



# Unit tests.
test: 
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
	rm -f *.o core.* $(LIBRARY)
	$(MAKE) -C test clean

reallyclean: clean
	rm -f proc_table.dat proc_table.dat.xml peos shell

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
