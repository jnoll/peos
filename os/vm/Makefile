####################################################################### 
# File :       Makefile
# Author:      Crystal Zhu
# Date:        11/17/99
# Description: Makefile for VM project in CSC5728, Fall 1999
#######################################################################

# compiler:
CC = gcc

## FLAGS for test
TFLAGS = -DTESTVM
##UNCOMMENT THE FOLLOWING WHEN DELIVER
#TFLAGS = 

CFLAGS = -g -I../repository $(TFLAGS)
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
all: pmlvm testvm dump_state query_state

pmlvm: $(OBJS)
	   $(CC) -o $@ $(OBJS) $(LIBS) -g

testvm: testvm.o
	$(CC) -o $@  $^ $(LIBS) -g

dump_state: dump_state.o
	$(CC) -o $@  $^ $(LIBS) -g

query_state: query_state.o
	$(CC) -o $@  $^ $(LIBS) -g

clean:
	rm *.o
