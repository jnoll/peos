PML_DIR = ../../../pml/
XPML_DIR = ../../../xpml/
OS_DIR = ../../../os
KERNEL_DIR = $(OS_DIR)/kernel
export COMPILER_DIR = $(KERNEL_DIR)

HOST = localhost
export INSTALL_DIR = /home/$(USER)/public_html/peos_test
WEB_SITE = $(HOST)/~$(USER)/peos_test

PEOS=$(KERNEL_DIR)/peos

CGI_CMD=

CC = gcc

CFLAGS = -g -I$(OS_DIR) -I$(PML_DIR)

LD = gcc
LIBS = -lpml
LDFLAGS = -g -L$(PML_DIR)/pml


KERNEL_OBJECTS = $(KERNEL_DIR)/events.o \
	$(KERNEL_DIR)/graph_engine.o \
	$(KERNEL_DIR)/process_table.o \
	$(KERNEL_DIR)/process.o \
	$(KERNEL_DIR)/action.o \
	$(KERNEL_DIR)/graph.o \
	$(KERNEL_DIR)/resources.o \
	$(KERNEL_DIR)/predicate_evaluator.o\

OBJECTS = 

TESTS = 

CGI = update.cgi list_procs.cgi test_init.cgi list_models.cgi create.cgi
XSL = get_model.xsl list_procs.xsl merge_instance.xsl pmldoc.xsl style.xsl html.xsl
XPML = test_action.xpml readme.xpml test.xpml
PML = $(XPML:xpml=pml)


all: test

parsecgi: parsecgi.c
	$(CC) -o $@ $<

test: test_xsl test_update

test_init: test_action.pml test.pml
	@rm -f proc_table.dat proc_table.dat.xml
	@-$(PEOS) -c readme.pml > /dev/null
	@-$(PEOS) -c test_action.pml > /dev/null
	@-$(PEOS) -c test.pml > /dev/null
	@[ -h pmldoc.xsl ] || ln -s $(XPML_DIR)/pmldoc.xsl .
	@[ -h html.xsl ] || ln -s $(XPML_DIR)/html.xsl .
	@[ -h style.xsl ] || ln -s $(XPML_DIR)/style.xsl .
	@echo "test_init done."

test_xsl: 
	xsltproc --stringparam PID 1 proc_table.xsl proc_table.dat.xml > readme.html
	lynx -dump readme.html
	xsltproc list_procs.xsl proc_table.dat.xml > list.html
	lynx -dump list.html
	xsltproc --stringparam PID 2 merge_instance.xsl $(XPML_DIR)/test.xml > merged.xml
	diff -w $(XPML_DIR)/test.xml merged.xml

test_list: test_init
	@-export REQUEST_METHOD="GET"; export QUERY_STRING='login=guest'; export CONTENT_LENGTH=$${#QUERY_STRING}; echo "$${QUERY_STRING}"|\
	./list_procs.cgi > list_procs.html
	lynx -dump list_procs.html

test_update: test_init
	@awk '$$1 == "actions:" && $$3 == "test_action" ' proc_table.dat
	@-export REQUEST_METHOD="POST"; export QUERY_STRING='PID=1&actions=test_action+test_script&test_action=start'; export CONTENT_LENGTH=$${#QUERY_STRING}; echo "$${QUERY_STRING}"|\
	./update.cgi > update.html
	@awk '$$1 == "actions:" && $$3 == "test_action" ' proc_table.dat
	@-export REQUEST_METHOD="POST"; export QUERY_STRING='PID=1&actions=test_action+test_script&test_action=finish&test_script=finish'; export CONTENT_LENGTH=$${#QUERY_STRING}; echo "$${QUERY_STRING}"|\
	./update.cgi>update.html
	@awk '$$1 == "actions:" && $$3 == "test_action" ' proc_table.dat


test_parsecgi:
	export REQUEST_METHOD="POST"; export QUERY_STRING='PID=1&actions=test_action+test_script&test_action=finish&test_script=finish'; export CONTENT_LENGTH=$${#QUERY_STRING}; echo "$${QUERY_STRING}"|./parsecgi

verify: install
	lynx -dump http://$(WEB_SITE)/test_init.cgi
	lynx -dump http://$(WEB_SITE)/list_procs.cgi
	lynx -dump http://$(WEB_SITE)/update.cgi?PID=0

install: $(CGI) $(XSL) $(PML) $(XPML) parsecgi
	if [ -d $(INSTALL_DIR) ] ; then \
	 cp $^ $(INSTALL_DIR) ; \
	 for p in $(CGI); do chmod a+rx,go-w $(INSTALL_DIR)/$$p; done ;\
	 cp $(XPML) $(INSTALL_DIR) ;\
	 cp $(PML) $(INSTALL_DIR) ;\
	 chmod a+r,go-w $(INSTALL_DIR)/*.*pml ;\
	 $(MAKE) -C ../../../os/kernel install ;\
	fi

test.xpml: 
	ln -s $(XPML_DIR)/test.xml test.xpml

%.pml: %.xpml
	xsltproc $(XPML_DIR)/xpml.xsl $< > $@

# How to create cgi programs from respective sources.
%.cgi: %.o $(OBJECTS)
	$(LD) -o $@ $(LDFLAGS) $< $(OBJECTS) $(KERNEL_OBJECTS) $(LIBS)

clean: 



.depend: $(SOURCES)
	$(CC) $(CFLAGS) -MM $^ > $@

.PHONY: test
