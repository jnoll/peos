# compiler:
PMLC = ../../../compiler/pmlc
CC = gcc

CFLAGS = -g -Wall -I. -I.. -I../../.. -I/home/jnoll/include
# keep track of dependencies automatically
.KEEP_STATE:

# lib
LIBS = ../../../pml/libpml.a  -ltcl 
TEST_LIBS = -lcheck $(LIBS)
LDFLAGS = -g -L/home/jnoll/lib

# objects
CORE_OBJS = action.o events.o graph.o graph_engine.o process.o process_table.o resources.o predicate_evaluator.o tclinterp.o
TEST_OBJS = test_util.o action.o graph.o tclinterp.o

# sources
SOURCES = $(wildcard *.c) $(addprefix ../, $(CORE_OBJS:.o=.c))

# These are in order of use.
TESTS =  test_resources test_graph test_action test_events test_process test_process_table test_graph_engine test_assertions test_predicate_evaluator test_tclinterp


all: test_list_actions_API test kernel_wrapper 
test: unit_tests test_list_actions
suite: unit_tests function accept

unit_tests:  $(TESTS) 
	@echo "running unit tests..."
	@-for i in $^ ; do ./$$i -f -q; done
	@echo "done."

test_list_actions: 
	./test_list_actions_API

function:
	$(MAKE) -C function_tests 

accept:
	$(MAKE) -C accept_tests 


test_list_actions_API: test_list_actions_API.o $(CORE_OBJS)
	$(CC) $(LDFLAGS) -g -o $@ $< $(CORE_OBJS) $(LIBS) 


test_action: test_action.o $(TEST_OBJS)
	$(CC) $(LDFLAGS) -g -o $@ $< $(filter-out action.o, $(TEST_OBJS)) $(TEST_LIBS) 

test_predicate_evaluator: test_predicate_evaluator.o $(TEST_OBJS)
	$(CC) $(LDFLAGS) -g -o $@ $< process.o graph_engine.o process_table.o $(filter-out predicate_evaluator.o, $(TEST_OBJS)) $(TEST_LIBS)

test_graph: test_graph.o $(TEST_OBJS)
	$(CC) $(LDFLAGS) -g -o $@ $< $(filter-out graph.o, $(TEST_OBJS)) $(TEST_LIBS)

test_tclinterp: test_tclinterp.o $(TEST_OBJS)
	$(CC) $(LDFLAGS) -g -o $@ $< $(filter-out tclinterp.o, $(TEST_OBJS)) $(TEST_LIBS)

test_resources: test_resources.o $(TEST_OBJS)
	$(CC) $(LDFLAGS) -g -o $@ $< $(TEST_OBJS) predicate_evaluator.o $(TEST_LIBS)

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
