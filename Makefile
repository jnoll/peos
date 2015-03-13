# Where the PEOS web UI will be installed.  You must set HTML_DIR as an 
# environment variable pointing to the parent of the PEOS dir, 
# i.e. $(HOME)/public_html or /web/username/.

TREEBUILD= /scratch

TREETOP= peos

INSTALL_DIR=$(HTML_DIR)/PEOS
TCLF_DIR=peos/src/os/kernel

SUBDIRS = pml os/kernel #  ui/web2  ui/ajax-cgi  ui/carepathways

V = @ 

all:  build #install

build: dummy
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done

clean:
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i clean; done
	rm  -f core *~

clobber: clean
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i clobber; done

update:
	cvs update $(SUBDIRS) Makefile

# DO NOT USE THIS UNLESS YOUR ENVIRONMENT WILL SPAWN YOUR EDITOR FOR THE LOG MESSAGE!!!
commit: 
	cvs commit $(SUBDIRS) Makefile

# Dist depends on all but may not be ready for it yet!
#dist:   all
dist: 
	${V}rm -rf ${TREEBUILD}/${TREETOP}
	${V}cd ${TREEBUILD} && mkdir -p ./${TREETOP} ./${TREETOP}/os \
	        ./${TREETOP}/ui/gtk-gui/gtk \
		./${TREETOP}/os/kernel ./${TREETOP}/pml ./${TREETOP}/ui \
		./${TREETOP}/ui/java-gui ./${TREETOP}/ui/java-gui/include \
		./${TREETOP}/ui/gtk-gui && cd - 
	${V}cp -rf ./os/kernel/*.c ./os/kernel/*.h \
		${TREEBUILD}/${TREETOP}/os/kernel
	${V}cp -rf ./pml/*.c ./pml/*.h ./pml/*.l ${TREEBUILD}/${TREETOP}/pml
	${V}cp -rf ./ui/java-gui/*.java ${TREEBUILD}/${TREETOP}/ui/java-gui/
	${V}cp -f ./ui/java-gui/include/java-gui.jar ${TREEBUILD}/${TREETOP}/ui/java-gui/include
	${V}cp -rf ./ui/java-gui/images ${TREEBUILD}/${TREETOP}/ui/java-gui/images
	${V}rm -rf ${TREEBUILD}/${TREETOP}/ui/java-gui/images/CVS	
	${V}cp -rf ./ui/java-gui/help ${TREEBUILD}/${TREETOP}/ui/java-gui/help
	${V}rm -rf ${TREEBUILD}/${TREETOP}/ui/java-gui/help/CVS \
		${TREEBUILD}/${TREETOP}/ui/java-gui/help/images/CVS	
	${V}cp -rf ./models ${TREEBUILD}/${TREETOP}/models
	${V}rm -rf ${TREEBUILD}/${TREETOP}/models/CVS \
		${TREEBUILD}/${TREETOP}/models/Makefile
	${V}cp -rf ./ui/java-gui/bin ${TREEBUILD}/${TREETOP}/ui/java-gui/bin
	${V}rm -rf ${TREEBUILD}/${TREETOP}/ui/java-gui/bin/*
	${V}cp -rf ./ui/GUI/*.c ./ui/GUI/*.h ./ui/GUI/image \
		./ui/GUI/gtk ./ui/GUI/README ${TREEBUILD}/${TREETOP}/ui/gtk-gui
	${V}rm -rf ${TREEBUILD}/${TREETOP}/ui/gtk-gui/gtk/CVS \
	        ${TREEBUILD}/${TREETOP}/ui/gtk-gui/image/CVS \
		${TREEBUILD}/${TREETOP}/ui/gtk-gui/image/NEWS \
		${TREEBUILD}/${TREETOP}/ui/gtk-gui/image/README \
		${TREEBUILD}/${TREETOP}/ui/gtk-gui/gtk/NEWS \
		${TREEBUILD}/${TREETOP}/ui/gtk-gui/gtk/README 
	${V}rm -rf ${TREEBUILD}/${TREETOP}/ui/java-gui/test*
	${V}cp -f ./README.dist ${TREEBUILD}/${TREETOP}/README
	${V}cp -f ./INSTALL.dist ${TREEBUILD}/${TREETOP}/INSTALL
	${V}cp -f ./configure.in ${TREEBUILD}/${TREETOP}/configure
	${V}cp -f ./install-sh.in ${TREEBUILD}/${TREETOP}/install-sh
	${V}cp -f ./config.h.in ${TREEBUILD}/${TREETOP}/
	${V}cp -f ./config.sub.in ${TREEBUILD}/${TREETOP}/config.sub
	${V}cp -f ./config.guess.in ${TREEBUILD}/${TREETOP}/config.guess
	${V}cp -f ./Makefile.in ${TREEBUILD}/${TREETOP}/Makefile.in
	${V}cp -f ./pml/Makefile.in ${TREEBUILD}/${TREETOP}/pml
	${V}cp -f ./os/Makefile.in ${TREEBUILD}/${TREETOP}/os
	${V}cp -f ./os/kernel/Makefile.in ${TREEBUILD}/${TREETOP}/os/kernel
	${V}cp -f ./ui/Makefile.in ${TREEBUILD}/${TREETOP}/ui
	${V}cp -f ./ui/java-gui/Makefile.in \
		${TREEBUILD}/${TREETOP}/ui/java-gui/Makefile.in
	${V}cp -f ./ui/GUI/Makefile.in ${TREEBUILD}/${TREETOP}/ui/gtk-gui

	${V}chmod 0755 ${TREEBUILD}/${TREETOP} ${TREEBUILD}/${TREETOP}/ui \
		${TREEBUILD}/${TREETOP}/ui/java-gui ${TREEBUILD}/${TREETOP}/ui/gtk-gui \
	${TREEBUILD}/${TREETOP}/os \
	${TREEBUILD}/${TREETOP}/os/kernel \
	${TREEBUILD}/${TREETOP}/models ${TREEBUILD}/${TREETOP}/os \
	${TREEBUILD}/${TREETOP}/pml ${TREEBUILD}/${TREETOP}/configure \
	${TREEBUILD}/${TREETOP}/install-sh \
	${TREEBUILD}/${TREETOP}/config.sub \
	${TREEBUILD}/${TREETOP}/config.guess

	${V}chmod 0644 ${TREEBUILD}/${TREETOP}/ui/java-gui/*.java \
		${TREEBUILD}/${TREETOP}/ui/java-gui/*.in \
		${TREEBUILD}/${TREETOP}/Makefile.in

	${V}chmod 0755 ${TREEBUILD}/${TREETOP}/ui/java-gui/bin \
		${TREEBUILD}/${TREETOP}/ui/java-gui/help \
		${TREEBUILD}/${TREETOP}/ui/java-gui/images \
		${TREEBUILD}/${TREETOP}/ui/java-gui/include

	${V}chmod 0644 ${TREEBUILD}/${TREETOP}/ui/java-gui/help/*.htm\
		${TREEBUILD}/${TREETOP}/ui/java-gui/help/*.jpg\
		${TREEBUILD}/${TREETOP}/ui/java-gui/help/*.doc\
		${TREEBUILD}/${TREETOP}/ui/java-gui/help/*.db \
		${TREEBUILD}/${TREETOP}/config.h.in


	${V}chmod 0644 ${TREEBUILD}/${TREETOP}/ui/java-gui/help/images/*.jpg \
		${TREEBUILD}/${TREETOP}/ui/java-gui/help/images/*.gif \
		${TREEBUILD}/${TREETOP}/ui/java-gui/help/images/*.GIF \
		${TREEBUILD}/${TREETOP}/ui/java-gui/help/images/*.png \
		${TREEBUILD}/${TREETOP}/ui/java-gui/include/java-gui.jar

	${V}chmod 0644 ${TREEBUILD}/${TREETOP}/ui/java-gui/images/*.jpg \
		${TREEBUILD}/${TREETOP}/ui/java-gui/images/*.gif 
	${V}chmod 0644 ${TREEBUILD}/${TREETOP}/ui/gtk-gui/*.c \
		${TREEBUILD}/${TREETOP}/ui/gtk-gui/*.h ${TREEBUILD}/${TREETOP}/ui/gtk-gui/*.in \
		${TREEBUILD}/${TREETOP}/ui/gtk-gui/README 
	${V}chmod 0755 ${TREEBUILD}/${TREETOP}/ui/gtk-gui/gtk \
		${TREEBUILD}/${TREETOP}/ui/gtk-gui/gtk/gtkrc \
		${TREEBUILD}/${TREETOP}/ui/gtk-gui/image \
		${TREEBUILD}/${TREETOP}/ui/java-gui/images 
	${V}chmod 0644 ${TREEBUILD}/${TREETOP}/ui/gtk-gui/gtk/*.png
	${V}chmod 0644 ${TREEBUILD}/${TREETOP}/ui/gtk-gui/image/*.xpm
	${V}chmod 0644 ${TREEBUILD}/${TREETOP}/README ${TREEBUILD}/${TREETOP}/INSTALL 
	${V}chmod 0644 ${TREEBUILD}/${TREETOP}/models/*.pml \
		${TREEBUILD}/${TREETOP}/pml/*.h ${TREEBUILD}/${TREETOP}/pml/*.c \
		${TREEBUILD}/${TREETOP}/pml/*.l ${TREEBUILD}/${TREETOP}/pml/*.in \
		${TREEBUILD}/${TREETOP}/os/*.in ${TREEBUILD}/${TREETOP}/os/kernel/*.c \
		${TREEBUILD}/${TREETOP}/os/kernel/*.h ${TREEBUILD}/${TREETOP}/os/kernel/*.in \
		${TREEBUILD}/${TREETOP}/ui/*.in

	${V}echo ...start of peos.tar.gz build...
	${V}tar -C ${TREEBUILD} -zcf peos.tar.gz ${TREETOP}
	${V}echo .....end of peos.tar.gz build...
	${V}rm -rf ${TREEBUILD}/${TREETOP}
	${V}chmod -f 0644 ./peos.tar.gz

install: 
	$(MAKE) -C ui/web2 install INSTALL_DIR=$(HTML_DIR)/PEOS

dummy:
