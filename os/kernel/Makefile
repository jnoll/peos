# compiler:
CC = gcc -g

CFLAGS = -g -I.
TCFLAGS = -g -I/usr/local/include

# keep track of dependencies automatically
.KEEP_STATE:

# lib
LIBS = #-lgdbm -lpmlstate
TLIBS = -lcheck $(LIBS)
CORE_LIBS = 
LDFLAGS = -g -L../../lib
TLDFLAGS = $(LDFLAGS) -L/usr/local/lib

# objects
OBJS = \
$(CORE_OBJS)

CORE_OBJS = \
events.o \
vm.o \
process_table.o \
process.o \
action.o

# sources
SOURCES = $(OBJS:.o=.c)

all: test shell

# Compile the program
PMLServer: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS) -g

shell: shell.o $(CORE_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(CORE_LIBS) -lreadline -lhistory -ltermcap

test: $(CORE_OBJS)
	$(MAKE) -C test 

xref:
	ctags -x $(SOURCES) > $@

tags:	TAGS ctags

TAGS:	$(SOURCES)
	etags $(SOURCES)

ctags:	$(SOURCES)
	ctags $(SOURCES)

clean:
	-rm *.o core

dist:
	tar cvf kernel.tar --exclude-from exclude -C .. kernel

# Dependencies.
include .depend

# GNU Make will automatically maintain .depend.  This rule allows explicit
# recreation of dependencies.
depend: .depend

.depend: $(SOURCES)
	$(CC) $(CFLAGS) -MM $^ > $@
