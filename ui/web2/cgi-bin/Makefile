# Where the cgi programs get installed.
# XXX Should be inherited from parent make.
HTML_DIR=$(HOME)/public_html/
INSTALL_DIR=$(HTML_DIR)/PEOS
KERNEL_DIR = ../../../os/kernel

CC = gcc
CFLAGS = -g -I$(KERNEL_DIR)

LD = gcc
LIBS = -lpml
LDFLAGS = -g -L../../../pml


OBJECTS = $(KERNEL_DIR)/events.o \
	$(KERNEL_DIR)/graph_engine.o \
	$(KERNEL_DIR)/process_table.o \
	$(KERNEL_DIR)/process.o \
	$(KERNEL_DIR)/action.o 

TESTS = test_main.sh \
	test_action_page.sh \
	test_create.sh \
	test_do.sh \
	test_docreate.sh

CGI = main.cgi action_page.cgi create.cgi do.cgi do_create.cgi
PML = test_action.pml

all: test

test: $(CGI) create_testtable
	@ echo -n "running unit tests"
	@for t in $(TESTS) ; do $$t ; done
	@ echo "done."

install: 
	cp $(CGI) $(INSTALL_DIR)/cgi-bin
	for p in $(CGI); do chmod a+rx $(INSTALL_DIR)/cgi-bin/$$p; done
	cp $(PML) $(INSTALL_DIR)/cgi-bin

create_testtable: create_testtable.o
	$(LD) -o $@ $(LDFLAGS) $< $(OBJECTS) $(LIBS)

# How to create cgi programs from respective sources.
%.cgi: %.o 
	$(LD) -o $@ $(LDFLAGS) $< $(OBJECTS) $(LIBS)

