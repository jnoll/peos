CC = gcc

OBJECTS = \
queryLinkedList.o \
resultLinkedList.o \
FSseeker.o \
EMAILseeker.o \
setup_repositories.o \
vrepo.o \
debug.o \
search.o

search: $(OBJECTS)
	$(CC) -o search $(OBJECTS)

clean:
	-rm $(OBJECTS)
	-rm core
	-rm search

