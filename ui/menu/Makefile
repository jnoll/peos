CFLAGS = -g -I../.. -I/usr/local/include
LDFLAGS = -g 
LIBS = ../../os/kernel/libpeos.a  ../../pml/libpml.a -lcdk -lncurses  -lmenu
LD = gcc

OBJECTS = main.o process.o action.o

menu: $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)
