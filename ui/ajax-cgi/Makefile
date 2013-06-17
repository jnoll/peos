HTML_DIR=$(HOME)/public_html
INSTALL_DIR=$(HTML_DIR)/PEOS
PML_DIR=../../pml
KERNEL_DIR=../../os/kernel

CGI=peos.cgi peos_init.tcl
HTML=
JS=

# compiler:
CC = gcc 
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

install:
	install -d $(INSTALL_DIR)
	install $(CGI) $(JS) $(HTML) $(INSTALL_DIR)
	install htaccess $(INSTALL_DIR)/.htaccess

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


# Dependencies.
include .depend

# GNU Make will automatically maintain .depend.  This rule allows explicit
# recreation of dependencies.
depend: .depend

.depend: $(SOURCES)
	$(CC) $(CFLAGS) -MM $^ > $@
.PHONY: test
