# compiler:
PMLC = ../../../compiler/pmlc
CC = gcc

CFLAGS = -g -Wall -I. -I.. -I../../.. -I/home/jnoll/include
# keep track of dependencies automatically
.KEEP_STATE:

# lib
LIBS = ../../../pml/libpml.a
TEST_LIBS = -lcheck 
LDFLAGS = -g -L/home/jnoll/lib

# objects
CORE_OBJS =  events.o process.o process_table.o action.o graph_engine.o
TEST_OBJS = test_util.o action.o

# sources
SOURCES = $(wildcard *.c) $(addprefix ../, $(CORE_OBJS:.o=.c))

# These are in order of use.
TESTS =  test_action test_events test_process test_process_table test_graph_engine


all: test #kernel_wrapper
test: unit_tests 
suite: unit_tests function accept

unit_tests:  $(TESTS) test_assertions
	@echo -n "running unit tests"
	@-for i in $^ ; do $$i -f -q; done
	@echo "The following test should report an Error due to assertion failure:"
	@-test_assertions -f
	@-rm core*
	@echo "done."

function:
	$(MAKE) -C function_tests 

accept:
	$(MAKE) -C accept_tests 


test_action: test_action.o $(TEST_OBJS)
	$(CC) $(LDFLAGS) -g -o $@ $< $(filter-out action.o, $(TEST_OBJS)) $(TEST_LIBS) 

test_%: test_%.o $(TEST_OBJS)
	$(CC) $(LDFLAGS) -g -o $@ $< $(TEST_OBJS) $(TEST_LIBS) 

kernel_wrapper: kernel_wrapper.o $(CORE_OBJS)
	$(CC) $(LDFLAGS) -g -o $@ $< $(CORE_OBJS) $(LIBS) -lreadline -lhistory -ltermcap

%.o: ../%.c test_%.c
	$(CC) -c $(CFLAGS) $< 

test_%.o: ../%.c test_%.c test_util.o test_util.h
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
	-rm *.o core.*

reallyclean: clean
	-rm $(TESTS) test_assertions

# Dependencies.
include .depend

# GNU Make will automatically maintain .depend.  This rule allows explicit
# recreation of dependencies.
depend: .depend

.depend: $(SOURCES)
	$(CC) $(CFLAGS) -MM $^ > $@ 
