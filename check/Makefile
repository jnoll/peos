CC	= gcc
CFLAGS	= -I.. -g -Wall
LIBS	= ../pml/libpml.a
OBJS	= global.o local.o main.o
PROG	= pmlcheck


all:		$(PROG)

$(PROG):	$(OBJS) $(LIBS)
		$(CC) -o $(PROG) $(LDFLAGS) $(OBJS) $(LIBS)

clean:;		rm -f $(PROG) $(OBJS)

clobber:;	rm -f $(PROG) $(OBJS)
