CC      = gcc
CFLAGS  = -I../.. -g -Wall
LIBS    = ../pml/libpml.a
OBJS    = link_parser.o lexer.o link.o
SRCS    = link_parser.c lexer.c y.tab.h
YFLAGS  = -d


all:		$(SRCS) $(OBJS)		

link_parser.c:	link_parser.y
		yacc -d link_parser.y
		mv y.tab.h y.tab.h.temp
		cat y.tab.h.temp | sed -f link.sed > y.tab.h
		cat y.tab.c | sed -f link.sed > link_parser.c
		rm -f y.tab.c y.tab.h.temp
		
lexer.c:	lexer.l
		lex -t lexer.l | sed -f link.sed > lexer.c
		rm -f lexer.h.temp


clean:;		rm -f  $(OBJS)

clobber:;	rm -f  $(OBJS) $(SRCS)


