CC = gcc
DEFS = -D`uname -s

all:	unixfs_repository.o repository.o

install:


clean:
	rm -f *.o

unixfs_repository.o: unixfs_repository.c unixfs_repository.h
	$(CC) -c unixfs_repository.c

repository.o: repository.c repository.h
	$(CC) -c repository.c 



