SUBDIRS = pml os/kernel ui/web2/cgi-bin

all: dummy
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done

clean:
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i clean; done
	rm  core *~
html:
install_html:
dummy:
