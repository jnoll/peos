# Build Path
BUILD = ../build

# Executable to Build
TARGET = $(BUILD)/pmlstate

# Object Files
OBJECTS = state_driver.o

# Library Paths
LPATH =-L../build 

# Include Paths 
IPATH = -I../lib/

# Compiler Flags
CFLAGS = $(IPATH) -g

# Linker Flags
SLIBS = -L../build -lpmlstate 
DLIBS = -L/usr/lib -lm

# Build Dependencies
$(TARGET): $(OBJECTS)
	cc $(CFLAGS) $(IFLAGS) -o $@ $(OBJECTS) $(SLIBS) $(DLIBS)

# Suffixes
.SUFFIXES	:	.o .c 

.c.o :
	cc $(CFLAGS) $(IFLAGS) -c $< 

# Clean Build 
clean:
	rm $(TARGET) *.o 

