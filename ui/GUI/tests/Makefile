SHELL=/bin/sh


CC=gcc
GNOME_CONFIG=gnome-config
PKG_CONFIG=pkg-config
OPT_DEBUG=-g3
CC_WARN=-Wall

GNOME_CONFIG_CFLAGS=$(GNOME_CONFIG) --cflags xml2
PKG_CONFIG_CFLAGS=$(PKG_CONFIG) --cflags gtkhtml-1.1
GNOME_CONFIG_LDFLAGS=$(GNOME_CONFIG) --libs xml2
PKG_CONFIG_LDFLAGS=$(PKG_CONFIG) --libs gtkhtml-1.1

CFLAGS_DIR=-I/home/jnoll/include -I./ -I../
LDFLAGS_DIR=-L/home/jnoll/lib

CFLAGS=	$(CFLAGS_DIR) `$(GNOME_CONFIG_CFLAGS)` `$(PKG_CONFIG_CFLAGS)` \

# Keeps track of dependencies automatically.
.KEEP_STATE:

# libraries	
LDFLAGS= `$(GNOME_CONFIG_LDFLAGS)` `$(PKG_CONFIG_LDFLAGS)` \
		$(LDFLAGS_DIR)

LIBS=
TEST_LIBS=$(LIBS) -lcheck

# source files
H_SRC=interface.h callbacks.h engine.h glo.h parse.h \
      tree.h html.h viewmenu.h global_error.h htmllinkage.h \
      notebook.h deletemenu.h text.h buttonimages.h ctrlimages.h \
      menu.h menuimages.h images.h treeimages.h

C_SRC=interface.c callbacks.c engine.c glo.c parse.c \
      tree.c html.c viewmenu.c global_error.c htmllinkage.c \
      notebook.c deletemenu.c

# objects
CORE_OBJS=interface.o callbacks.o engine.o glo.o  \
	  parse.o tree.o html.o viewmenu.o        \
	  global_error.o htmllinkage.o notebook.o \
	  deletemenu.o

TEST_OBJS= $(CORE_OBJS) test_util.o set_test.o \
	  set_globals.o 
# sources
SOURCES=$(wildcard *.c) $(addprefix ../, $(CORE_OBJS:.o=.c))

# unit tests
TESTS=test_engine test_htmllinkage test_parse

## Make Rules
all:	test 

test:	 $(CORE_OBJS) unit_tests

suite:	unit_tests

unit_tests:	$(TESTS)
	@echo "running unit tests..."
	@-for i in $^ ; do ./$$i -f -q; done
	@echo "done."

# This make rule needs to be reworked.
test_engine:	test_engine.o $(TEST_OBJS)
		$(CC) $(LDFLAGS) $(OPT_DEBUG) -o \
		$@ $< $(TEST_OBJS) \
	        $(TEST_LIBS)	


test_htmllinkage:	test_htmllinkage.o $(TEST_OBJS)
			$(CC) $(LDFLAGS) $(OPT_DEBUG) -o $@ \
			$< $(TEST_OBJS) \
			$(TEST_LIBS)

test_parse:      	test_parse.o $(TEST_OBJS)
			$(CC) $(LDFLAGS) $(OPT_DEBUG) -o $@ \
			$< $(TEST_OBJS) \
			$(TEST_LIBS)
			
test_%: test_%.o $(TEST_OBJS)
	$(CC) $(LDFLAGS) -g -o $@ $< $(TEST_OBJS) $(TEST_LIBS) 

%.o: ../%.c
	$(CC) -c $(CFLAGS) $< 

xref:	ctags -x $(SOURCES) > $@

TAGS:	$(SOURCES)
	etags $(SOURCES)

tags:	$(SOURCES)
	ctags $(SOURCES)

clean:	
	rm *.o


clobber:	clean
	rm $(TESTS)
	rm proc_table* message event.log .depend

# Dependencies
include .depend

# GNU will automaticall maintain .depend.  This rule allows explicit
# recreation of dependencies.
depend:	.depend

.depend:	$(SOURCES)
		$(CC) $(CFLAGS) -MM $^ > $@

	  
		    
	  
