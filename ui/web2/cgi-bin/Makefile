OS_DIR = ../../../os
KERNEL_DIR = $(OS_DIR)/kernel

CC = gcc
CFLAGS = -g -I$(OS_DIR) -I../../..

LD = gcc
LIBS = -lpml
LDFLAGS = -g -L../../../pml


KERNEL_OBJECTS = $(KERNEL_DIR)/events.o \
	$(KERNEL_DIR)/graph_engine.o \
	$(KERNEL_DIR)/process_table.o \
	$(KERNEL_DIR)/process.o \
	$(KERNEL_DIR)/action.o 

OBJECTS = html.o

TESTS = test_main.sh \
	test_action_page.sh \
	test_create.sh \
	test_do.sh \
	test_docreate.sh \
	test_resource.sh

CGI = main.cgi action_page.cgi create.cgi do.cgi do_create.cgi resource.cgi
PML = test_action.pml ../readme.pml

all: test

test: $(CGI) create_testtable
	@for t in $(TESTS) ; do $$t ; done


install: 
	[ -d $(INSTALL_DIR)/cgi-bin ] && cp $(CGI) $(INSTALL_DIR)/cgi-bin
	for p in $(CGI); do chmod a+rx $(INSTALL_DIR)/cgi-bin/$$p; done
	[ -d $(INSTALL_DIR)/cgi-bin ] && cp $(PML) $(INSTALL_DIR)/cgi-bin
	chmod a+r $(INSTALL_DIR)/cgi-bin/*.pml

create_testtable: create_testtable.o
	$(LD) -o $@ $(LDFLAGS) $< $(KERNEL_OBJECTS) $(LIBS)

# How to create cgi programs from respective sources.
%.cgi: %.o $(OBJECTS)
	$(LD) -o $@ $(LDFLAGS) $< $(OBJECTS) $(KERNEL_OBJECTS) $(LIBS)

main.o action_page.o create.o do.o do_create.o resource.o: html.h
