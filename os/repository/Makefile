CC = gcc
DEFS = -D`uname -s
BINDEST = /peos/src/bin/
LIBDEST = /peos/src/lib/
DATADEST = /peos/src/share/
OBJECTS = debug.o pml_state.o repository.o unixfs_repository.o

all:	rep_test 


clean:
	rm -f *.o rep_test

install: rep_test 
	cp rep_test $(BINDEST)
	cp test.rep $(BINDEST)

debug.o: debug.c
	$(CC) -c debug.c

repository.o: repository.c
	$(CC) -c repository.c

unixfs_repository.o: unixfs_repository.c
	$(CC) -c unixfs_repository.c

rep_test: $(OBJECTS)
	$(CC) -o rep_test $(OBJECTS)




