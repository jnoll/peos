.SUFFIXES: .o .h .cpp .c .C

# compiler
CC = g++

# executable name
NAME = peosuser

# keep track of dependencies automatically
.KEEP_STATE:

# path for executable when using make install
INSTALLPATH = ../../../../bin/

DEBUG = -g
INCLUDES = -I../include
DEFINES  = -D_REENTRANT

LIBPATH =
LIBS = -lcrypt

#C++ flags
CCFLAGS = $(DEBUG) $(INCLUDES) $(DEFINES)

#ld flags
LDFLAGS = $(LIBPATH) $(LIBS)


OBJS =  peosuser.o

# Compile the program
$(NAME): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

# Rules
.cpp.o:
	$(CC) $(CCFLAGS) -c $<

clean:
	rm -f *.o

bare:
	rm -f *.o core $(NAME)

install:
	cp $(NAME) $(INSTALLPATH)
