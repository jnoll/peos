CC = gcc
# Build Path
BUILD = .
 
# Install Path
PEOSLIBDIR = ../../lib

# Target to Build 
LIB = libpmlstate.so
TARGET = $(BUILD)/pmlstate

# Static Library to Build 
STATIC = $(PEOSLIBDIR)/libpmlstate.a

# Object Files
OBJECTS = pml_state.o repository.o unixfs_repository.o debug.o

# Compiler Flags
CFLAGS = -g

# Linker Flags
SLIBS = -L$(PEOSLIBDIR) -lpmlstate 
DLIBS =  -lm

all:	$(LIB) 
test: $(TARGET)

# Build Dependencies
$(LIB): $(OBJECTS)
	ld -G -o $(LIB) $(OBJECTS)
$(TARGET): $(OBJECTS)
	cc $(CFLAGS) $(IFLAGS) -o $@ $(OBJECTS) $(SLIBS) $(DLIBS)

# Suffixes
.SUFFIXES	:	.o .c 

.c.o :
	$(CC) $(CFLAGS) $(IFLAGS) -c $< 

# Clean Build 
clean:
	rm $(LIB) $(PEOSLIBDIR)/*.a *.o

# Make Static Library
static: $(OBJECTS)
	ar -r $(STATIC) $(OBJECTS)

# Install Library
install:
	cp $(LIB) $(PEOSLIBDIR)

