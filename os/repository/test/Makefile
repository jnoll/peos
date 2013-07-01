# Object Files
OBJECTS = 

# Library Paths
LPATH =-L..

# Include Paths 
IPATH = -I..

# Compiler Flags
CFLAGS = $(IPATH) -g


# Linker Flags
LDFLAGS = $(LPATH) -g
SLIBS = -lpmlstate 
DLIBS = -lm

all: drive dump

# Build Dependencies
drive: state_driver.o
	$(CC) $(LDFLAGS) -o $@ $< $(SLIBS) $(DLIBS)

dump: dump.o
	$(CC) $(LDFLAGS) -o $@ $< $(SLIBS) $(DLIBS)

# Suffixes
.SUFFIXES	:	.o .c 

# Clean Build 
clean:
	rm $(TARGET) *.o 

