HTML_DIR=$(HOME)/public_html
<<<<<<< HEAD
INSTALL_DIR=$(HTML_DIR)/PEOS
=======
#HTML_DIR=C:/xampp/htdocs
INSTALL_DIR=$(HTML_DIR)/PEOS1
>>>>>>> 71f6a61cb06573d2dbf317e9ee585b978950aef8
PML_DIR=../../pml
KERNEL_DIR=../../os/kernel

CGI=peos.cgi check.cgi
HTML=index.html pmlcheck.html
PMLCHECK=$(PML_DIR)/check/pmlcheck
JS=$(wildcard *.js) 
PML=$(wildcard *.pml) 
SUPPORT=$(wildcard *.tcl) proc_table.dat proc_table.dat.xml

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
OBJS = getcgi.o util.o

# sources
SOURCES = $(OBJS:.o=.c) 
HEADERS = $(wildcard *.h)

DIR_MODE  = ug+rwX,o+rX
SCRIPT_MODE  = ug+rwx,o+rX
FILE_MODE = ug+rw,o+r

all: build
build: $(PML_DIR)/check/pmlcheck peos.cgi check.cgi
$(PMLCHECK):
	$(MAKE) -c $(PML_DIR)/check pmlcheck

%.cgi: %.o $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS) -ltcl

install: build
	install -m $(DIR_MODE) -d $(INSTALL_DIR)
	install -m $(SCRIPT_MODE) $(CGI) $(INSTALL_DIR)
	install -m $(SCRIPT_MODE) $(PMLCHECK) $(INSTALL_DIR)
<<<<<<< HEAD
	install -m $(FILE_MODE) $(JS) $(HTML) $(PML) $(SUPPORT) $(INSTALL_DIR)
	install -m $(FILE_MODE) ${KERNEL_DIR}/peos_init.tcl $(INSTALL_DIR)/kernel_init.tcl
	install htaccess $(INSTALL_DIR)/.htaccess

=======
	install -m $(FILE_MODE) $(JS) $(HTML) $(PML) $(INSTALL_DIR)
	install -m $(FILE_MODE) ${KERNEL_DIR}/peos_init.tcl $(INSTALL_DIR)/kernel_init.tcl
	install htaccess $(INSTALL_DIR)/.htaccess
	install -m $(FILE_MODE)  $(SUPPORT) $(INSTALL_DIR)
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
<<<<<<< HEAD
	rm -f proc_table.dat proc_table.dat.xml peos shell

dist: reallyclean
	tar cvf kernel.tar --exclude-from exclude -C .. kernel
=======
	rm -f proc_table.dat proc_table.dat.xml peos.cgi check.cgi
>>>>>>> 71f6a61cb06573d2dbf317e9ee585b978950aef8


# Dependencies.
include .depend

# GNU Make will automatically maintain .depend.  This rule allows explicit
# recreation of dependencies.
depend: .depend

.depend: $(SOURCES)
	$(CC) $(CFLAGS) -MM $^ > $@
.PHONY: test
