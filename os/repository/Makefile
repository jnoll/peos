CC = gcc
# Install Path
PEOSLIBDIR = ../../lib

# Target to Build 
LIB = libpmlstate.a

# Object Files
OBJECTS = pml_state.o repository.o unixfs_repository.o debug.o

# Compiler Flags
CFLAGS = -g


all:	install
test: $(TARGET)

# Build Dependencies
$(LIB): $(OBJECTS)
	ar -r $(LIB) $(OBJECTS)


# Dependencies
$(OBJECTS): pml_state.h

# Clean Build 
clean:
	rm $(LIB) $(PEOSLIBDIR)/*.a *.o


# Install Library
install: $(LIB)
	cp $(LIB) $(PEOSLIBDIR)

