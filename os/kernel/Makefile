# compiler:
CC = gcc -g


CFLAGS = -I../repository -I../vm
# keep track of dependencies automatically
.KEEP_STATE:

# lib

LIBS = -lgdbm -lpmlstate

LDFLAGS = -L../../lib

# objects
# modified 7/18/02 - gets events.o, vm.o from os/vm/
OBJS = \
dispatch.o \
server.o \
events.o \
../vm/events.o \
../vm/vm.o \
../repository/repository.o \
../repository/unixfs_repository.o \
../repository/debug.o

# sources

# Compile the program
PMLServer: $(OBJS)
	   $(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS) -g

clean:
	rm *.o
