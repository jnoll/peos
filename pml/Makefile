CC	= gcc
CFLAGS	= -I.. -g -Wall
LIB	= libpml.a
OBJS	= graph.o list.o parser.o scanner.o tree.o
SRCS	= parser.c scanner.c y.tab.h
YFLAGS	= -d


all:		$(LIB)

$(LIB):		$(SRCS) $(OBJS)
		rm -f $(LIB)
		ar cru $(LIB) $(OBJS)
		ranlib $(LIB) || true

clean:;		rm -f $(LIB) $(OBJS)

clobber:;	rm -f $(LIB) $(OBJS) $(SRCS)
