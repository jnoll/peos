# Build Path
BUILD = ../build

# Install Path
PEOSLIBDIR = /home/pml/lib

# Target to Build 
TARGET = $(BUILD)/libpmlstate.so

# Static Library to Build 
STATIC = $(BUILD)/libpmlstate.a

# Object Files
OBJECTS = pml_state.o

# Compiler Flags
CFLAGS = -g

# Build Dependencies
$(TARGET): $(OBJECTS)
	ld -G -o $(TARGET) $(OBJECTS)

# Suffixes
.SUFFIXES	:	.o .c 

.c.o :
	cc $(CFLAGS) $(IFLAGS) -c $< 

# Clean Build 
clean:
	rm $(TARGET) $(BUILD)/*.a *.o

# Make Static Library
static:
	ar -r $(STATIC) $(OBJECTS)

# Install Library
install:
	cp $(TARGET) $(PEOSLIBDIR)

