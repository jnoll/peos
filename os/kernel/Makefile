# compiler:
CC = gcc -g


CFLAGS = 
# keep track of dependencies automatically
.KEEP_STATE:

# lib

LIBS = -lgdbm

# objects
OBJS = \
dispatch.o \
events.o \
server.o \
vm.o \

# sources

# Compile the program
PMLServer: $(OBJS)
	   $(CC) -o $@ $(OBJS) $(LIBS) -g

clean:
	rm *.o
