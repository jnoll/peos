# compiler:
PMLC = ../../../compiler/pmlc
CC = gcc -g

CFLAGS = -g -I. -I.. -I/usr/local/include -Wall
# keep track of dependencies automatically
.KEEP_STATE:

# lib
TEST_LIBS = -lcheck
LDFLAGS = -g -L/usr/local/lib

# objects
PMLFILES = $(wildcard *.pml)
MODELS = $(PMLFILES:.pml=.txt)

CORE_OBJS = vm.o events.o process_table.o process.o action.o

TEST_OBJS = test_util.o action.o

# sources
SOURCES = $(wildcard *.c) $(addprefix ../, $(CORE_OBJS:.o=.c))

# These are in order of use.
TESTS = test_vm test_action test_process test_events test_process_table
#test_assertions

all: test driver vm_harness

test:  $(TESTS) 
	-for i in $^ ; do $$i -f; done

driver: driver.o $(CORE_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(CORE_LIBS)

vm_harness: vm_harness.o $(CORE_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(CORE_LIBS)

models: $(MODELS)

%.txt: %.pml
	-$(PMLC) -f TEXT $< 

test_action: test_action.o $(TEST_OBJS)
	$(CC) $(LDFLAGS) -g -o $@ $< $(filter-out action.o, $(TEST_OBJS)) $(TEST_LIBS)

test_%: test_%.o $(TEST_OBJS)
	$(CC) $(LDFLAGS) -g -o $@ $< $(TEST_OBJS) $(TEST_LIBS)

%.o: ../%.c test_%.c
	$(CC) -c $(CFLAGS) $<

test_%.o: ../%.c test_%.c test_util.h
	$(CC) -o $@ -c $(CFLAGS) -DUNIT_TEST $<

%.o: %.c
	$(CC) -c $(CFLAGS) -DUNIT_TEST $<

xref:
	ctags -x $(SOURCES) > $@

TAGS:	$(SOURCES)
	etags $(SOURCES)

tags:	$(SOURCES)
	ctags $(SOURCES)

clean:
	-rm *.o core

# Dependencies.
include .depend

# GNU Make will automatically maintain .depend.  This rule allows explicit
# recreation of dependencies.
depend: .depend

.depend: $(SOURCES)
	$(CC) $(CFLAGS) -MM $^ > $@
