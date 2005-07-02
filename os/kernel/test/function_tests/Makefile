HARNESS = ../kernel_harness.exp -- -verbosity 0
CASES = $(wildcard *.tc)
LOGS = $(CASES:tc=log)
NUM_ACTS = 1 3 31 53 73 113 187 213 271 301 513 769

all:
	@echo "type 'make gen' to generate test cases"
	@echo "type 'make run' to run generated test cases"
gen: clean test_cases
test:  run
test_cases: baseline inventory combinations bounds

# Run the tests.
run: $(LOGS)
#	@echo "tests:"
#	@(grep -l FAIL *.log && echo "FAILED") || echo "passed."
	@echo done.

#
# Baseline
# Basic sequential processes with varying numbers of actions.
#
baseline:
	for n in $(NUM_ACTS) ; do ../gen_tc.tcl -baseline -num_act $$n; done

#
# Inventory
# Happy path tests of all control constructs and combinations.
#
inventory:
	../gen_tc.tcl -inventory -num_act 2


#
# Combinations.
#
combo: combinations
combinations:
	../gen_tc.tcl -combinations -num_act 2

#
# Trends analysis.
# 
trends: base_trend inv_trend combo_trend
	echo "Now type 'make run'.  Caution: this takes a long time to execute!"

base_trend:
	i=1; while [ $$i -lt 1025 ] ; do \
	    ../gen_tc.tcl -baseline -num_act $$i; i=`expr $$i + 256`; \
	done

inv_trend:
	i=1; while [ $$i -lt 1025 ] ; do \
	../gen_tc.tcl -inventory -num_act $$i; i=`expr $$i + 256`; \
	done

combo_trend:
	i=1; while [ $$i -lt 1025 ] ; do \
	../gen_tc.tcl -combinations -num_act $$i; i=`expr $$i + 256`; \
	done


#
# Push the boundaries.
#
bounds: proc_table
# Process table has 10 entries.
proc_table:
	../gen_tc.tcl -baseline -num_act 2 -iterations 11 -mode concurrent
	../gen_tc.tcl -inventory -num_act 2 -iterations 11 -mode concurrent
	../gen_tc.tcl -combinations -num_act 2 -iterations 11 -mode concurrent


#
# How to run tests.
#
%.log: %.tc
	-@$(HARNESS) $<  2>&1 |tee $@ | grep -n FAILURE || true

clean:
	rm -f *.tc *.pml *.log

reallyclean: clean
	rm -f  core*

log_clean:
	rm -f *.log
