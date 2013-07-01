CFLAGS = -g -I../../../pml
DFLAGS = -g -DDOT_BIN=\"dot\"

LIBS = ../../../pml/pml/libpml.a

proc_graph:
	gcc $(CFLAGS) $(DFLAGS) -g -c -o  proc_graph.o proc_graph.c
	gcc -o proc_graph proc_graph.o $(LIBS)

