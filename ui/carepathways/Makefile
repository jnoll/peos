<<<<<<< HEAD
#HTML_DIR=$(HOME)/public_html
HTML_DIR=C:/xampp/htdocs
=======
HTML_DIR=$(HOME)/public_html
#HTML_DIR=C:/xampp/htdocs
>>>>>>> 71f6a61cb06573d2dbf317e9ee585b978950aef8
INSTALL_DIR=$(HTML_DIR)/carepathways
PML_DIR=../../pml
KERNEL_DIR=../../os/kernel

<<<<<<< HEAD
CGI=peos.cgi check.cgi 
=======
CGI=peos.cgi check.cgi demo.php demo.cgi
>>>>>>> 71f6a61cb06573d2dbf317e9ee585b978950aef8
HTML=$(wildcard *.html)
PMLCHECK=$(PML_DIR)/check/pmlcheck 
JS=$(wildcard scripts/*.*)
COOKIE_JS=$(wildcard scripts/jquery_cookie/*.*)
PML=$(wildcard *.pml)
<<<<<<< HEAD
RES=$(wildcard *._res)
HL7=$(wildcard *._hl7)
PHP=$(wildcard *.php)
SUPPORT=$(wildcard *.tcl)
=======
TCL=$(wildcard *.tcl) 
RES=$(wildcard *._res)
HL7=$(wildcard *._hl7)
SLIDES=$(wildcard slides/slide*.jpg)
SUPPORT=event.log proc_table.dat proc_table.dat.xml log.txt
>>>>>>> 71f6a61cb06573d2dbf317e9ee585b978950aef8
CSS=$(wildcard css/*.css)
IMAGES=$(wildcard images/*.*)

# compiler:
CC = gcc 
CFLAGS = -g -I$(PML_DIR) -I$(KERNEL_DIR) -Wall

# keep track of dependencies automatically
.KEEP_STATE:

# lib
<<<<<<< HEAD
LIBS = $(KERNEL_DIR)/libpeos.a $(PML_DIR)/pml/libpml.a C:/cygwin/lib/libxml2.dll.a
=======
LIBS = $(KERNEL_DIR)/libpeos.a $(PML_DIR)/pml/libpml.a #C:/cygwin/lib/libxml2.dll.a
>>>>>>> 71f6a61cb06573d2dbf317e9ee585b978950aef8

CORE_LIBS = 
LDFLAGS = -g

# objects
OBJS = getcgi.o util.o

# sources
SOURCES = $(OBJS:.o=.c) 
HEADERS = $(wildcard *.h)

<<<<<<< HEAD
DIR_MODE  = ug+rwX,o+rX
SCRIPT_MODE  = ug+rwx,o+rX
FILE_MODE = ug+rw,o+r

all: build
build: $(PML_DIR)/check/pmlcheck peos.cgi check.cgi
$(PMLCHECK):
	$(MAKE) -c $(PML_DIR)/check pmlcheck

=======

# These permissions are for per-user public_html installation.
# For other situations it might be better not to give "others" 
# write permission. 
DIR_MODE  = ug+rwX,o+rwX
SCRIPT_MODE  = ug+rwx,o+rwX
FILE_MODE = ug+rw,o+rw
WRITE_MODE = $(FILE_MODE)

all: build
build: $(PML_DIR)/check/pmlcheck peos.cgi check.cgi $(SUPPORT)
$(PMLCHECK):
	$(MAKE) -c $(PML_DIR)/check pmlcheck

event.log: 
	echo "" > $@

proc_table.dat: proc_table.dat.in
	cp $< $@
proc_table.dat.xml: proc_table.dat.xml.in
	cp $< $@
log.txt:
	echo "" > $@

>>>>>>> 71f6a61cb06573d2dbf317e9ee585b978950aef8
%.cgi: %.o $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS) -ltcl

install: build
	install -m $(DIR_MODE) -d $(INSTALL_DIR)
	install -m $(DIR_MODE) -d $(INSTALL_DIR)/scripts
	install -m $(DIR_MODE) -d $(INSTALL_DIR)/scripts/jquery_cookie
	install -m $(DIR_MODE) -d $(INSTALL_DIR)/images
	install -m $(DIR_MODE) -d $(INSTALL_DIR)/css
	install -m $(SCRIPT_MODE) $(CGI) $(INSTALL_DIR)
	install -m $(SCRIPT_MODE) $(PMLCHECK) $(INSTALL_DIR)
<<<<<<< HEAD
	install -m $(FILE_MODE) $(HTML) $(PML) $(RES) $(HL7) $(PHP) $(SUPPORT) $(INSTALL_DIR)
	install -m $(FILE_MODE) $(JS) $(INSTALL_DIR)/scripts
	install -m $(FILE_MODE) $(COOKIE_JS) $(INSTALL_DIR)/scripts/jquery_cookie
	install -m $(FILE_MODE) $(IMAGES) $(INSTALL_DIR)/images
	install -m $(FILE_MODE) $(CSS) $(INSTALL_DIR)/css
	install -m $(FILE_MODE) ${KERNEL_DIR}/peos_init.tcl $(INSTALL_DIR)/kernel_init.tcl
	install htaccess $(INSTALL_DIR)/.htaccess
=======
	install -m $(FILE_MODE) $(HTML) $(PML) $(TCL) $(RES) $(HL7) $(INSTALL_DIR)
	install -m $(FILE_MODE) $(JS) $(INSTALL_DIR)/scripts
	install -m $(FILE_MODE) $(COOKIE_JS) $(INSTALL_DIR)/scripts/jquery_cookie
	install -m $(FILE_MODE) $(IMAGES) $(SLIDES) $(INSTALL_DIR)/images
	install -m $(FILE_MODE) $(CSS) $(INSTALL_DIR)/css
	install -m $(FILE_MODE) ${KERNEL_DIR}/peos_init.tcl $(INSTALL_DIR)/kernel_init.tcl
	install htaccess $(INSTALL_DIR)/.htaccess
	install -m $(WRITE_MODE)  $(SUPPORT) $(INSTALL_DIR)
>>>>>>> 71f6a61cb06573d2dbf317e9ee585b978950aef8

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
