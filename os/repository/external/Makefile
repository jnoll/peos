CC = gcc
DEFS = -D`uname -s
BINDEST = /peos/src/bin/
LIBDEST = /peos/src/lib/
DATADEST = /peos/src/share/

all:	debug.o repository.o unixfs_repository.o rep_test 

clean:
	rm -f *.o rep_test

install: debug.o repository.o unixfs_repository.o rep_test 
	cp rep_test $(BINDEST)
	cp test.rep $(BINDEST)

debug.o: debug.c
	$(CC) -c debug.c

repository.o: repository.c
	$(CC) -c repository.c

unixfs_repository.o: unixfs_repository.c
	$(CC) -c unixfs_repository.c

rep_test: rep_test.c unixfs_repository.o debug.o repository.o
	$(CC) -o rep_test rep_test.c unixfs_repository.o repository.o\
		debug.o



