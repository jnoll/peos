CC	= gcc
CFLAGS	= -I..  -g -Wall
LIB	= ../pml/libpml.a
OBJS	= traverse.o
SRCS	= traverse.c
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

clean:
	rm -f $(OBJS)