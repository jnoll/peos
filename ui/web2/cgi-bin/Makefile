OS_DIR = ../../../os
KERNEL_DIR = $(OS_DIR)/kernel

CC = gcc

CFLAGS = -g -Wall -I$(OS_DIR) -I../../..

LD = gcc
LIBS = -lpml -lcrypt
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

TESTS = test_delete_proc_table.sh \
	test_process_listing.sh \
	test_action_list.sh \
	test_action_page.sh \
	test_action_page_fail.sh \
        test_multiple_dones.sh \
        test_handle_run.sh \
	test_bind_resources.sh \
	test_multiple_done_bindings.sh \
	test_resource_bindings.sh \
	test_change_resource_bindings.sh \

CGI = action_list.cgi action_page.cgi create_process.cgi delete_proc_table.cgi process_listing.cgi action_event.cgi handle_run.cgi bind_resources.cgi multiple_dones.cgi multiple_done_bindings.cgi error_page.cgi resource_bindings.cgi change_resource_bindings.cgi
SOURCES = $(CGI:.cgi=.c) $(OBJECTS:.o=.c)

PML = test_action.pml readme.pml

all: test

test: $(CGI) create_testtable
	@for t in $(TESTS) ; do $$t ; done


install: 
	[ -d $(INSTALL_DIR)/cgi-bin ] && cp $(CGI) $(INSTALL_DIR)/cgi-bin
	for p in $(CGI); do chmod a+rx $(INSTALL_DIR)/cgi-bin/$$p; done
	[ -d $(INSTALL_DIR)/cgi-bin ] && cp $(PML) $(INSTALL_DIR)/cgi-bin
	chmod a+r $(INSTALL_DIR)/cgi-bin/*.pml
	echo "order deny,allow" > htaccess
	echo " " >> htaccess
	echo "AuthName \"PEOS Enactment Demonstration\" " >> htaccess
	echo "Authtype Basic" >> htaccess
	echo "AuthUserFile $(INSTALL_DIR)/cgi-bin/hidden/.htpasswd" >> htaccess
	echo " " >> htaccess
	echo "require valid-user" >> htaccess
	[ -d $(INSTALL_DIR)/cgi-bin ] && cp htaccess $(INSTALL_DIR)/cgi-bin
	mv $(INSTALL_DIR)/cgi-bin/htaccess $(INSTALL_DIR)/cgi-bin/.htaccess
	chmod 744 $(INSTALL_DIR)/cgi-bin/.htaccess
	[ -d $(INSTALL_DIR)/cgi-bin/hidden ] && cp hidden/htpasswd $(INSTALL_DIR)/cgi-bin/hidden
	mv $(INSTALL_DIR)/cgi-bin/hidden/htpasswd $(INSTALL_DIR)/cgi-bin/hidden/.htpasswd
	chmod 744 $(INSTALL_DIR)/cgi-bin/hidden/.htpasswd



create_testtable: create_testtable.o
	$(LD) -o $@ $(LDFLAGS) $< $(KERNEL_OBJECTS) $(LIBS)

# How to create cgi programs from respective sources.
%.cgi: %.o $(OBJECTS)
	$(LD) -o $@ $(LDFLAGS) $< $(OBJECTS) $(KERNEL_OBJECTS) $(LIBS)

clean: 
	rm *.cgi

.depend: $(SOURCES)
	$(CC) $(CFLAGS) -MM $^ > $@
.PHONY: test
