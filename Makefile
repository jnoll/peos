# Where the PEOS web UI will be installed.  You must set HTML_DIR as an 
# environment variable pointing to the parent of the PEOS dir, 
# i.e. $(HOME)/public_html or /web/username/.

INSTALL_DIR=$(HTML_DIR)/PEOS
TCLF_DIR=peos/src/os/kernel

SUBDIRS = pml os/kernel ui/GUI ui/java-gui ui/web2 

all:  build install

build: dummy
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done

clean:
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i clean; done
	rm  core *~

update:
	cvs update $(SUBDIRS) Makefile

install: 
	$(MAKE) -C ui/web2 install INSTALL_DIR=$(HTML_DIR)/PEOS

dummy:

