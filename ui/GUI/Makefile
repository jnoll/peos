# required symbols
#
SHELL= /bin/sh

# tools used
#
CC= gcc 
GNOME_CONFIG = gnome-config
LDD= ldd
GREP= grep
AWK= awk 
XARGS=	xargs
LIBTOOL= libtool 
SORT= sort
SPLINT= splint
SED= sed
CPIO= cpio
MKDIR= mkdir
MAKE= make
EGREP= egrep
FGREP= fgrep
FIND= find

# C compiler flags
#
SPLINT_ARGS=
#OPT_DEBUG= -g3 -O3
OPT_DEBUG= -g3
#CC_WARN=
#CC_WARN= -Wall
#CC_WARN= -Wall -W
CC_WARN= -Wall -Werror
#MISC_CC= -M
MISC_CC= 

XML2_CONFIG_CFLAGS= ${XML2_CONFIG} --cflags
XML2_CONFIG_LDFLAGS= ${XML2_CONFIG} --libs

#System Make
XML2_CONFIG= xml2-config
GTK_CONFIG= gtk-config
PKG_CONFIG= pkg-config
GNOME_CONFIG_CFLAGS= ${GTK_CONFIG} --cflags gtk
GNOME_CONFIG_LDFLAGS= ${GTK_CONFIG} --libs gtk

#Manual Make
#XML2_CONFIG= xml2new-config
#GTK_CONFIG= gtknew-config
#PKG_CONFIG= pkgnew-config
#GNOME_CONFIG_CFLAGS= ${GTK_CONFIG} --cflags
#GNOME_CONFIG_LDFLAGS= ${GTK_CONFIG} --libs

PKG_CONFIG_CFLAGS= ${PKG_CONFIG} --cflags gtkhtml-1.1
PKG_CONFIG_LDFLAGS= ${PKG_CONFIG} --libs gtkhtml-1.1

CFLAGS=	`${XML2_CONFIG_CFLAGS}` `${GNOME_CONFIG_CFLAGS}` `${PKG_CONFIG_CFLAGS}` \
	${OPT_DEBUG} ${CC_WARN} ${MISC_CC}
LDFLAGS= `${XML2_CONFIG_LDFLAGS}` `${GNOME_CONFIG_LDFLAGS}` `${PKG_CONFIG_LDFLAGS}` \
 	 ${OPT_DEBUG}

# our source
#
H_SRC= buttonimages.h callbacks.h ctrlimages.h deletemenu.h \
	engine.h glo.h html.h htmllinkage.h images.h interface.h \
	menu.h menuimages.h notebook.h parse.h support.h text.h \
	tree.h treeimages.h viewmenu.h
C_SRC=	main.c interface.c callbacks.c engine.c \
	glo.c parse.c tree.c html.c viewmenu.c  \
	deletemenu.c htmllinkage.c notebook.c
C_OBJ=	main.o interface.o callbacks.o engine.o \
	glo.o parse.o tree.o html.o viewmenu.o  \
	deletemenu.o htmllinkage.o notebook.o

# build location
#
TREETOP= peosdir

# default rule
#
all: runpeos

# compile runpeos using system installed libs
#
runpeos: ${C_OBJ}
	${CC} ${LDFLAGS} ${C_OBJ} -o runpeos

.c.o:
	${CC} ${CFLAGS} -c $<

runpeos_lib_dirs:
	@(${GNOME_CONFIG_LDFLAGS}; ${PKG_CONFIG_LDFLAGS}) | \
	    ${AWK} '{ for (i=1; i <= NF; ++i) { print $$i; } }' | \
	    ${GREP} '^-L' | ${SED} -e 's/^-L//' | ${SORT} -u

# static libraries 
runpeos_lib_names_a:
	@(${GNOME_CONFIG_LDFLAGS}; ${PKG_CONFIG_LDFLAGS}) | \
	    ${AWK} '{ for (i=1; i <= NF; ++i) { print $$i; } }' | \
	    ${GREP} '^-l' | ${SED} -e 's/^-l/lib/' -e 's/$$/.a/'

# dynamic libraries
runpeos_lib_names_so:
	@(${GNOME_CONFIG_LDFLAGS}; ${PKG_CONFIG_LDFLAGS}) | \
	    ${AWK} '{ for (i=1; i <= NF; ++i) { print $$i; } }' | \
	    ${GREP} '^-l' | ${SED} -e 's/^-l/lib/' -e 's/$$/.so/'

runpeos_lib_locations_a:
	@rm -f tmp_dir_list
	@${MAKE} -s runpeos_lib_dirs > tmp_dir_list
	@${MAKE} -s  runpeos_lib_names_a | while read i; do \
	    ${FIND} `< tmp_dir_list` -name $$i; \
	done
	@rm -f tmp_dir_list

runpeos_lib_locations_so:
	@rm -f tmp_dir_list
	@${MAKE} -s runpeos_lib_dirs > tmp_dir_list
	@${MAKE} -s  runpeos_lib_names_so | while read i; do \
	    ${FIND} `< tmp_dir_list` -name $$i; \
	done
	@rm -f tmp_dir_list

# header files
runpeos_header_dirs: ${C_SRC}
	@(${GNOME_CONFIG_CFLAGS}; ${PKG_CONFIG_CFLAGS}) | \
	    ${AWK} '{ for (i=1; i <= NF; ++i) { print $$i; } }' | \
	    ${GREP} '^-I' | ${SED} -e 's/^-I//' | ${SORT} -u

runpeos_headers:
	${FIND} `${MAKE} -s runpeos_header_dirs` -name '*.h'

collect_headers:
	@-if [ ! -d "${TREETOP}" ]; then \
		echo ${MKDIR} -p ${TREETOP}; \
		${MKDIR} -p "${TREETOP}"; \
	fi
	@echo starting header copy
	@${MAKE} -s runpeos_headers | ${CPIO} -dumpL ${TREETOP}
	@echo ending header copy


collect_libs: exclude_libs runpeos
	@-if [ ! -d "${TREETOP}" ]; then \
		echo ${MKDIR} -p ${TREETOP}; \
		${MKDIR} -p ${TREETOP}; \
	fi 
	@echo starting lib copy
	@${MAKE} -s runpeos_lib_locations_so | ${CPIO} -dumpL ${TREETOP}
	@echo ending lib copy

runpeos_lib_dirs_sed:
#	@echo -n 's:@@@LIB_LIST@@@:'
#	@${MAKE} -s runpeos_lib_locations_a | \
#	 ${SED} -e 's/^/./' | \
#	 ${XARGS} echo -n
#	@echo ':'
	@echo -n 's:@@@L_LIST@@@:'
	@${MAKE} -s runpeos_lib_dirs | \
	 ${SED} -e 's/^/-L./' | \
	 ${XARGS} echo -n
	@echo ':'
	@echo -n 's:@@@l_list@@@:'
	@${MAKE} -s runpeos_lib_names_so | \
	 ${SED} -e 's/^lib/-l/' -e 's/\.so$$//' | \
	 ${XARGS} echo -n
	@echo ':'
	@echo -n 's:@@@I_LIST@@@:'
	@${MAKE} -s runpeos_header_dirs | \
	 ${SED} -e 's/^/-I./' | \
	 ${XARGS} echo -n
	@echo ':'

Makefile-release: Makefile-release.in
	rm -f Makefile-release tmp_sed
	${MAKE} -s runpeos_lib_dirs_sed > tmp_sed
	sed -f tmp_sed $< > $@
	rm -f tmp_sed

# utility rules
#
gdb: runpeos
	gdb ./runpeos

splint: ${C_SRC}
	${SPLINT} ${SPLINT_ARGS} ${C_SRC} ${CFLAGS}

dist: all Makefile-release
	rm -rf ${TREETOP}
	mkdir -p ${TREETOP}
	${MAKE} -s collect_headers
	${MAKE} -s collect_libs
	cp ${C_SRC} ${H_SRC} *.pml ${TREETOP}
	cp Makefile-release ${TREETOP}/Makefile
	cp -r ./image ./gtk ./calc-rpm README peos exclude_libs ${TREETOP}
	@echo start of peos.tar.gz build
	tar -zcf peos.tar.gz ${TREETOP}
	@echo end of peos.tar.gz build

clean:
	rm -f ${C_OBJ} tmp_dir_list tmp_sed Makefile-release

clobber: clean
	rm -f runpeos proc* core*
	rm -rf ${TREETOP} treetop peos.tar.gz
