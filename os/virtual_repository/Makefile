CC = gcc
CFLAGS = -g -c
OBJECTS = queryLinkedList.o resultLinkedList.o FSseeker.o \
	  setup_repositories.o vrepo.o debug.o search.o

search: $(OBJECTS)
	$(CC) -o search $(OBJECTS)
search.o: search.c vrepo.o FSseeker.o
	$(CC) $(CFLAGS) search.c
vrepo.o: vrepo.c vrepo.h FSseeker.o resultLinkedList.o queryLinkedList.o
	$(CC) $(CFLAGS) vrepo.c
setup_repositories.o: setup_repositories.c setup_repositories.h FSseeker.o
	$(CC) $(CFLAGS) setup_repositories.c
FSseeker.o: FSseeker.c FSseeker.h resultLinkedList.o queryLinkedList.o
	$(CC) $(CFLAGS) FSseeker.c 
resultLinkedList.o: resultLinkedList.c resultLinkedList.h
	$(CC) $(CFLAGS) resultLinkedList.c
queryLinkedList.o: queryLinkedList.c queryLinkedList.h resultLinkedList.o
	$(CC) $(CFLAGS) queryLinkedList.c
debug.o: debug.c debug.h 
	$(CC) $(CFLAGS) debug.c
clean:
	touch dummy.o
	rm *.o
