HARNESS = ../kernel_harness.exp
RUNTESTS = -for f in *.tc ; do $(HARNESS) $$f 2> /dev/null; done 
test: netbeans_req_release.pml
	$(RUNTESTS)

netbeans_req_release.pml:
	-ln -s ../../../../models/netbeans_req_release.pml .