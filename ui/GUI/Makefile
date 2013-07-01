# required symbols
#
SHELL= /bin/sh

# tools used
#
CC= gcc 
GNOME_CONFIG = gnome-config
PKG_CONFIG= pkg-config
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
#CC_WARN= -Wall -W -Werror
CC_WARN= -Wall
#MISC_CC= -M
MISC_CC=

GNOME_CONFIG_CFLAGS= ${GNOME_CONFIG} --cflags xml2
PKG_CONFIG_CFLAGS= ${PKG_CONFIG} --cflags gtkhtml-1.1
GNOME_CONFIG_LDFLAGS= ${GNOME_CONFIG}  --libs xml2
PKG_CONFIG_LDFLAGS= ${PKG_CONFIG} --libs gtkhtml-1.1
CFLAGS=	`${GNOME_CONFIG_CFLAGS}` `${PKG_CONFIG_CFLAGS}` \
	${OPT_DEBUG} ${CC_WARN} ${MISC_CC}
LDFLAGS= `${GNOME_CONFIG_LDFLAGS}` `${PKG_CONFIG_LDFLAGS}` \
 	 ${OPT_DEBUG}

# our source
#
H_SRC= buttonimages.h callbacks.h ctrlimages.h deletemenu.h \
	engine.h glo.h html.h htmllinkage.h images.h interface.h \
	menu.h menuimages.h notebook.h parse.h support.h text.h \
	global_error.htree.h treeimages.h viewmenu.h
	
C_SRC=	main.c interface.c callbacks.c engine.c \
	glo.c parse.c tree.c html.c viewmenu.c  \
	global_error.c deletemenu.c htmllinkage.c notebook.c
	
C_OBJ=	main.o interface.o callbacks.o engine.o \
	glo.o parse.o tree.o html.o viewmenu.o  \
	global_error.o deletemenu.o htmllinkage.o notebook.o

# build location
#
TREETOP= peosdir

# default rule
#
all: gtkpeos

# compile gtkpeos using system installed libs
#
gtkpeos: ${C_OBJ}
	${CC} ${LDFLAGS} ${C_OBJ} -o gtkpeos

.c.o:
	${CC} ${CFLAGS} -c $<

gtkpeos_lib_dirs:
	@(${GNOME_CONFIG_LDFLAGS}; ${PKG_CONFIG_LDFLAGS}) | \
	    ${AWK} '{ for (i=1; i <= NF; ++i) { print $$i; } }' | \
	    ${GREP} '^-L' | ${SED} -e 's/^-L//' | ${SORT} -u

# static libraries 
gtkpeos_lib_names_a:
	@(${GNOME_CONFIG_LDFLAGS}; ${PKG_CONFIG_LDFLAGS}) | \
	    ${AWK} '{ for (i=1; i <= NF; ++i) { print $$i; } }' | \
	    ${GREP} '^-l' | ${SED} -e 's/^-l/lib/' -e 's/$$/.a/'

# dynamic libraries
gtkpeos_lib_names_so:
	@(${GNOME_CONFIG_LDFLAGS}; ${PKG_CONFIG_LDFLAGS}) | \
	    ${AWK} '{ for (i=1; i <= NF; ++i) { print $$i; } }' | \
	    ${GREP} '^-l' | ${SED} -e 's/^-l/lib/' -e 's/$$/.so/'

gtkpeos_lib_locations_a:
	@rm -f tmp_dir_list
	@${MAKE} -s gtkpeos_lib_dirs > tmp_dir_list
	@${MAKE} -s  gtkpeos_lib_names_a | while read i; do \
	    ${FIND} `< tmp_dir_list` -name $$i; \
	done
	@rm -f tmp_dir_list

gtkpeos_lib_locations_so:
	@rm -f tmp_dir_list
	@${MAKE} -s gtkpeos_lib_dirs > tmp_dir_list
	@${MAKE} -s  gtkpeos_lib_names_so | while read i; do \
	    ${FIND} `< tmp_dir_list` -name $$i; \
	done
	@rm -f tmp_dir_list

# header files
gtkpeos_header_dirs: ${C_SRC}
	@(${GNOME_CONFIG_CFLAGS}; ${PKG_CONFIG_CFLAGS}) | \
	    ${AWK} '{ for (i=1; i <= NF; ++i) { print $$i; } }' | \
	    ${GREP} '^-I' | ${SED} -e 's/^-I//' | ${SORT} -u

gtkpeos_headers:
	${FIND} `${MAKE} -s gtkpeos_header_dirs` -name '*.h'

collect_headers:
	@-if [ ! -d "${TREETOP}" ]; then \
		echo ${MKDIR} -p ${TREETOP}; \
		${MKDIR} -p "${TREETOP}"; \
	fi
	@echo staring header copy
	@${MAKE} -s gtkpeos_headers | ${CPIO} -dumpL ${TREETOP}
	@echo ending header copy


collect_libs: exclude_libs gtkpeos
	@-if [ ! -d "${TREETOP}" ]; then \
		echo ${MKDIR} -p ${TREETOP}; \
		${MKDIR} -p ${TREETOP}; \
	fi 
	@echo starting lib copy
	@${MAKE} -s gtkpeos_lib_locations_so | ${CPIO} -dumpL ${TREETOP}
	@echo ending lib copy

gtkpeos_lib_dirs_sed:
#	@echo -n 's:@@@LIB_LIST@@@:'
#	@${MAKE} -s gtkpeos_lib_locations_a | \
#	 ${SED} -e 's/^/./' | \
#	 ${XARGS} echo -n
#	@echo ':'
	@echo -n 's:@@@L_LIST@@@:'
	@${MAKE} -s gtkpeos_lib_dirs | \
	 ${SED} -e 's/^/-L./' | \
	 ${XARGS} echo -n
	@echo ':'
	@echo -n 's:@@@l_list@@@:'
	@${MAKE} -s gtkpeos_lib_names_so | \
	 ${SED} -e 's/^lib/-l/' -e 's/\.so$$//' | \
	 ${XARGS} echo -n
	@echo ':'
	@echo -n 's:@@@I_LIST@@@:'
	@${MAKE} -s gtkpeos_header_dirs | \
	 ${SED} -e 's/^/-I./' | \
	 ${XARGS} echo -n
	@echo ':'

Makefile-release: Makefile-release.in
	rm -f Makefile-release tmp_sed
	${MAKE} -s gtkpeos_lib_dirs_sed > tmp_sed
	sed -f tmp_sed $< > $@
	rm -f tmp_sed

# utility rules
#
gdb: gtkpeos
	gdb ./gtkpeos

splint: ${C_SRC}
	${SPLINT} ${SPLINT_ARGS} ${C_SRC} ${CFLAGS}

dist: all Makefile-release
	rm -rf ${TREETOP}
	mkdir -p ${TREETOP}
	${MAKE} -s collect_headers
	${MAKE} -s collect_libs
	cp ${C_SRC} ${H_SRC} *.pml ${TREETOP}
	#cp megahack.c ${TREETOP}
	cp Makefile-release ${TREETOP}/Makefile
	cp -r ./image ./gtk README peos exclude_libs ${TREETOP}
	@echo start of peos.tar.gz build
	tar -zcf peos.tar.gz ${TREETOP}
	@echo end of peos.tar.gz build

clean:
	rm -f ${C_OBJ} tmp_dir_list tmp_sed Makefile-release

clobber: clean
	rm -f gtkpeos proc* core* pelog
	rm -rf ${TREETOP} treetop peos.tar.gz message event.log
