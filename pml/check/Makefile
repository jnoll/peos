CC	= gcc
CFLAGS	= -I.. -g -Wall
LIBS	= ../pml/libpml.a  ../expr/libexpr.a 
OBJS	= common.o local.o global.o qualifier.o main.o link/link.o link/lexer.o link/link_parser.o
SRCS	= 
PROG	= pmlcheck
YFLAGS	= -d

all:		$(PROG)

$(PROG):	$(OBJS) $(LIBS)
		$(CC) -o $(PROG) $(LDFLAGS) $(OBJS) $(LIBS)

link/lexer.o:		link/lexer.l
		cd link; make link_parser.c;

link/link_parser.o:	link/link_parser.y
		cd link; make lexer.c;

link/link.o:		link/link.c
		cd link; make;

clean:;		rm -f $(PROG) $(OBJS); cd link; make clean;

clobber:;	rm -f $(PROG) $(OBJS); cd link; make clobber;

