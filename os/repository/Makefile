CC = gcc
# Install Path
PEOSLIBDIR = ../../lib


# Target to Build 
LIB = libpmlstate.a

# Object Files
OBJECTS = pml_state.o repository.o unixfs_repository.o debug.o

# Compiler Flags
CFLAGS = -g


all: install test
test: driver dumpDB 

# Build Dependencies
$(LIB): $(OBJECTS)
	ar -r $(LIB) $(OBJECTS)

# Test harness.
driver: state_driver.o
	$(CC) $(LDFLAGS) -o $@ $< $(LIB)

dumpDB: dumpDB.o $(LIB)
	$(CC) $(LDFLAGS) -o $@ $< $(LIB)

# Dependencies
$(OBJECTS): pml_state.h

# Clean Build 
clean:
	rm $(LIB) $(PEOSLIBDIR)/*.a *.o

clobber: clean

# Install Library
install: $(LIB)
	cp $(LIB) $(PEOSLIBDIR)

TAGS:
	etags *.[ch]

.PHONY: test
