#HTML_DIR=$(HOME)/public_html
HTML_DIR=C:/xampp/htdocs
INSTALL_DIR=$(HTML_DIR)/carepathways
PML_DIR=../../pml
KERNEL_DIR=../../os/kernel

CGI=peos.cgi 
HTML=index.html
JS=$(wildcard scripts/*.*)
COOKIE_JS=$(wildcard scripts/jquery_cookie/*.*)
PML=$(wildcard *.pml)
SUPPORT=peos_init.tcl
CSS=$(wildcard css/*.css)
IMAGES=$(wildcard images/*.*)

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

DIR_MODE  = ug+rwX,o+rX
SCRIPT_MODE  = ug+rwx,o+rX
FILE_MODE = ug+rw,o+r

all: build
build: peos.cgi
peos.cgi: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS) -ltcl

install: build
	install -m $(DIR_MODE) -d $(INSTALL_DIR)
	install -m $(DIR_MODE) -d $(INSTALL_DIR)/scripts
	install -m $(DIR_MODE) -d $(INSTALL_DIR)/scripts/jquery_cookie
	install -m $(DIR_MODE) -d $(INSTALL_DIR)/images
	install -m $(DIR_MODE) -d $(INSTALL_DIR)/css
	install -m $(SCRIPT_MODE) $(CGI) $(INSTALL_DIR)
	install -m $(FILE_MODE) $(HTML) $(PML) $(SUPPORT) $(INSTALL_DIR)
	install -m $(FILE_MODE) $(JS) $(INSTALL_DIR)/scripts
	install -m $(FILE_MODE) $(COOKIE_JS) $(INSTALL_DIR)/scripts/jquery_cookie
	install -m $(FILE_MODE) $(IMAGES) $(INSTALL_DIR)/images
	install -m $(FILE_MODE) $(CSS) $(INSTALL_DIR)/css
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
