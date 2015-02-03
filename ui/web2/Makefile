# Where the cgi programs get installed.
HTML_DIR=$(HOME)/public_html
INSTALL_DIR=$(HTML_DIR)/PEOS
KERNEL_DIR = ../../../os/kernel



all: build
build:
	$(MAKE) -C cgi-bin

install: 
	@# Must have a test in order to preserve the build.
	@if test -n "${HTML_DIR}"; then \
		[ -d $(INSTALL_DIR) ] || mkdir $(INSTALL_DIR); \
		cp index.html $(INSTALL_DIR); \
		chmod a+r $(INSTALL_DIR)/index.html; \
		chmod go-w $(INSTALL_DIR)/index.html; \
		[ -d $(INSTALL_DIR)/cgi-bin ] || mkdir $(INSTALL_DIR)/cgi-bin; \
		[ -d $(INSTALL_DIR)/cgi-bin/hidden ] \ \
		|| mkdir $(INSTALL_DIR)/cgi-bin/hidden; \
		[ -d $(INSTALL_DIR)/images ] || mkdir $(INSTALL_DIR)/images; \
		chmod a+rX $(INSTALL_DIR); \
		chmod a+rX $(INSTALL_DIR)/images; \
		chmod a+rX $(INSTALL_DIR)/cgi-bin; \
		chmod a+rX $(INSTALL_DIR)/cgi-bin/hidden; \
                \
		$(MAKE) -C cgi-bin  install INSTALL_DIR=$(INSTALL_DIR); \
		$(MAKE) -C images install INSTALL_DIR=$(INSTALL_DIR); \
	else \
		echo "You must set your HTML_DIR env. variable to the directory where the PEOS web UI is to be installed."; \
	fi;	


clean: 
	$(MAKE) -C cgi-bin clean