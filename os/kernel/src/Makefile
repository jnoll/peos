####################################################################### 
# File :       Makefile
# Author:      Nikolai Kiachtchenko (Modified by Brad McGarvey)
# Date:        11/1/1999 (Amended 4/3/00)
# Description: Makefile for OSServices project in CSC5728,  Fall 1999
#######################################################################
.SUFFIXES: .o .h .cpp .c .C

# compiler
CC = g++

# executable name
NAME = OSServices

# keep track of dependencies automatically
.KEEP_STATE:

# path for executable when using make install
INSTALLPATH = ../../../bin/

DEBUG = -g
INCLUDES = -I../include
DEFINES  = -D_REENTRANT

LIBPATH =
LIBS = -lgdbm -lcrypt -lpthread

#C++ flags
CCFLAGS = $(DEBUG) $(INCLUDES) $(DEFINES)

#ld flags
LDFLAGS = $(LIBPATH) $(LIBS)


OBJS = 	DataAccessInterface.o \
	EventHandler.o \
	GDBMIF.o \
	IPCHandler.o \
	OSLock.o \
	OSMutex.o \
	OSThread.o \
	Process.o \
	ProcessStatusHandler.o \
	RepositoryIF.o \
	SignalHandler.o \
	UIMessageHandler.o \
	VMInterface.o \
	main.o \
        OSSemaphore.o\
	TimedEventHandler.o\
	ProductEventHandler.o

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
