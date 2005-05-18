CC	= gcc
CFLAGS	= -I..  -g -Wall
LIB	= $(HOME)/tmp/peos/src/pml/libpml.a
OBJS	= graph.o list.o parser.o scanner.o tree.o
SRCS	= parser.c scanner.c y.tab.h
YFLAGS	= -d
INSTALL_DIR = $(HOME)

all:		traverse

traverse:	traverse.o $(LIB)
	$(CC) -o $@ $< $(LIB)

install: traverse 
	[ -d "${INSTALL_DIR}" ] || mkdir $INSTALL_DIR
	[ -d "${INSTALL_DIR}/bin" ] || mkdir ${INSTALL_DIR}/bin
	[ -d "${INSTALL_DIR}/lib" ] || mkdir ${INSTALL_DIR}/lib
	cp traverse ${INSTALL_DIR}/bin 
	cp graphit  ${INSTALL_DIR}/bin 
	cp color-pml.awk ${INSTALL_DIR}/lib
