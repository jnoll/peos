SUBDIRS=pml expr check graph

all: dummy
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done

clean:
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i clean; done
	rm -f  core *~

dummy:
