CC	= gcc 
CFLAGS	= -I.. -g -Wall
LIB	= libpml.a

OBJS	= graph.o list.o parser.o tree.o scanner.o
SRCS	= parser.c scanner.c y.tab.h
YFLAGS	= -d


all:		$(LIB)

$(LIB):		$(SRCS) $(OBJS)		
		rm -f $(LIB)
		ar cru $(LIB) $(OBJS)
		ranlib $(LIB) || true

traverse:	traverse.o $(LIB)
	$(CC) -o $@ $< $(LIB)

ee: 		ee.o $(LIB)
	$(CC) -o $@ $< $(LIB)

t:              t.o $(LIB)
	$(CC) -o $@ $< $(LIB)



clean:;		rm -f $(LIB) $(OBJS)

clobber:;	rm -f $(LIB) $(OBJS) $(SRCS) y.tab.h libpml.a

unittestable:
