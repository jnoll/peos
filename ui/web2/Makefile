# Where the cgi programs get installed.
INSTALL_DIR=$(HTML_DIR)/PEOS
KERNEL_DIR = ../../../os/kernel

all: build
build:
	$(MAKE) -C cgi-bin

install: 
	@@[ -n "$(HTML_DIR)" ] || (echo "You must set your HTML_DIR env. variable to the directory where the PEOS web UI is to be installed."; exit 1)
	[ -d $(INSTALL_DIR) ] || mkdir $(INSTALL_DIR)
	[ -d $(INSTALL_DIR)/cgi-bin ] || mkdir $(INSTALL_DIR)/cgi-bin
	[ -d $(INSTALL_DIR)/cgi-bin/hidden ] || mkdir $(INSTALL_DIR)/cgi-bin/hidden
	[ -d $(INSTALL_DIR)/images ] || mkdir $(INSTALL_DIR)/images
	chmod a+rX $(INSTALL_DIR) 
	chmod a+rX $(INSTALL_DIR)/images
	chmod a+rX $(INSTALL_DIR)/cgi-bin
	chmod a+rX $(INSTALL_DIR)/cgi-bin/hidden

	$(MAKE) -C cgi-bin  install INSTALL_DIR=$(INSTALL_DIR)
	$(MAKE) -C images install INSTALL_DIR=$(INSTALL_DIR)

