####################################################################### 
# File :       Makefile
# Author:      Crystal Zhu
# Date:        11/17/99
# Description: Makefile for VM project in CSC5728, Fall 1999
#######################################################################

# compiler:
CC = gcc -g

## FLAGS for test
TFLAGS = -DTESTVM
##UNCOMMENT THE FOLLOWING WHEN DELIVER
#TFLAGS = 

CFLAGS = -I../repository $(TFLAGS)
# keep track of dependencies automatically
.KEEP_STATE:

# lib

LIBS =  -L../../lib -lpmlstate -lcrypt -L/usr/lib -lm 
# objects
OBJS =  pmlvm.o events.o vm.o 

# sources

HFILES = pmlevent.h vm.h
SRCS = pmlvm.c events.c vm.c

# Compile the program
pmlvm: $(OBJS)
	   $(CC) -o $@ $(OBJS) $(LIBS) -g

testvm: testvm.o
	$(CC) -o $@  testvm.o $(LIBS) -g
clean:
	rm *.o
