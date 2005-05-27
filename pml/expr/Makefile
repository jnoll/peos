CC	= gcc
CFLAGS	= -I.. -g -Wall
LIB	= libexpr.a
OBJS	= expr.o
SRCS	= expr.h expr.c
YFLAGS	= -d


all:		$(LIB)

$(LIB):		$(SRCS) $(OBJS)
		rm -f $(LIB)
		ar cru $(LIB) $(OBJS)
		ranlib $(LIB) || true

clean:;		rm -f $(LIB) $(OBJS)

clobber:;	rm -f $(LIB) $(OBJS)



