OS_DIR = ../../../os
KERNEL_DIR = $(OS_DIR)/kernel

CC = gcc

CFLAGS = -g -Wall -I$(OS_DIR) -I../../..

LD = gcc
LIBS = -lpml
LDFLAGS = -g -L../../../pml


KERNEL_OBJECTS = $(KERNEL_DIR)/events.o \
	$(KERNEL_DIR)/graph_engine.o \
	$(KERNEL_DIR)/process_table.o \
	$(KERNEL_DIR)/process.o \
	$(KERNEL_DIR)/action.o \
	$(KERNEL_DIR)/graph.o \
	$(KERNEL_DIR)/resources.o \
	$(KERNEL_DIR)/predicate_evaluator.o\

OBJECTS = html.o \
	  getcgi.o

TESTS = test_login.sh \
	test_action_list.sh \
	test_action_page.sh \
	test_create_process.sh \
	test_process_listing.sh \
	

CGI = login.cgi action_list.cgi action_page.cgi create_process.cgi process_listing.cgi action_event.cgi handle_run.cgi bind_resources.cgi multiple_dones.cgi multiple_done_bindings.cgi
SOURCES = $(CGI:.cgi=.c) $(OBJECTS:.o=.c)

PML = test_action.pml readme.pml

all: $(CGI)
#all: test

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

clean: 
	rm *.cgi

#main.o action_page.o create.o do.o do_create.o resource.o: html.h
.depend: $(SOURCES)
	$(CC) $(CFLAGS) -MM $^ > $@
.PHONY: test
