OS_DIR = ../../../os
KERNEL_DIR = $(OS_DIR)/kernel

CC = gcc

CFLAGS = -w -g -Wall -I$(OS_DIR) -I../../.. -I/usr/include/libxml2

LD = gcc
LIBS = -lpml -lcrypt -lxml2 -lz -lm
LDFLAGS = -g -L../../../pml -L/usr/lib


KERNEL_OBJECTS = $(KERNEL_DIR)/events.o \
	$(KERNEL_DIR)/graph_engine.o \
	$(KERNEL_DIR)/process_table.o \
	$(KERNEL_DIR)/process.o \
	$(KERNEL_DIR)/action.o \
	$(KERNEL_DIR)/graph.o \
	$(KERNEL_DIR)/resources.o \
	$(KERNEL_DIR)/predicate_evaluator.o\

OBJECTS = html.o \
	  xmlparse.o \
	  getcgi.o

TESTS = test_active_processes.sh \
	test_create_process.sh \
	test_action_page.sh \

CGI = active_processes.cgi create_process.cgi action_page.cgi
SOURCES = $(CGI:.cgi=.c) $(OBJECTS:.o=.c)

PML = test_action.pml readme.pml cvs_add_dir.pml

all: test

test: $(CGI) create_testtable
	@for t in $(TESTS) ; do sh $$t ; done
	rm event.log


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
	[ -d $(INSTALL_DIR)/cgi-bin ] && cp htaccess $(INSTALL_DIR)/cgi-bin/.htaccess
	chmod 744 $(INSTALL_DIR)/cgi-bin/.htaccess
	[ -d $(INSTALL_DIR)/cgi-bin/hidden ] || mkdir $(INSTALL_DIR)/cgi-bin/hidden
	# Don't overwrite existing htpasswd file.
	if [ ! -f $(INSTALL_DIR)/cgi-bin/hidden/.htpasswd ] ; then \
		cp hidden/htpasswd $(INSTALL_DIR)/cgi-bin/hidden/.htpasswd ;\
		chmod 744 $(INSTALL_DIR)/cgi-bin/hidden/.htpasswd ;\
	fi



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
