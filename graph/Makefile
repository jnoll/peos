CC	= gcc
CFLAGS	= -I..  -g -Wall
LIB	= ../pml/libpml.a
OBJS	= graph.o list.o parser.o scanner.o tree.o
SRCS	= parser.c scanner.c y.tab.h
YFLAGS	= -d


all:		traverse

traverse:	traverse.o $(LIB)
	$(CC) -o $@ $< $(LIB)




