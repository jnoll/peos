HARNESS = ../kernel_harness.exp
RUNTESTS = -for f in *.tc ; do $(HARNESS) $$f 2> /dev/null | grep FAILURE; done 
test: inventory bounds

# Happy path tests of all control construct combinations.
inventory: clean
	../gen_tc.tcl -iterations 1 -num_act 2 -shell "../../shell"
	$(RUNTESTS)

# Some boundary tests.

bounds: full max_iterations max_actions

full: clean
	../gen_tc.tcl -iterations 1 -num_act 1 -fill_proc_table -shell "../../shell"
	$(RUNTESTS)

# 21 iterations causes failure tests with iterations.
max_iterations: clean
	../gen_tc.tcl -iterations 21 -num_act 2 -shell "../../shell"
	$(RUNTESTS)

max_actions: clean
	../gen_tc.tcl -iterations 1 -num_act 50 -shell "../../shell"


clean:
	-rm *.tc *.pml

reallyclean: clean
	-rm  core*

