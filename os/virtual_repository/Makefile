CC = gcc
CFLAGS = -g -c
OBJECTS = resultLinkedList.o seeker.o vrepo.o simulator.o
simulator: $(OBJECTS)
	$(CC) -o simulator $(OBJECTS)
simulator.o: simulator.c vrepo.o seeker.o
	$(CC) $(CFLAGS) simulator.c
vrepo.o: vrepo.c vrepo.h seeker.o resultLinkedList.o
	$(CC) $(CFLAGS) vrepo.c
seeker.o: seeker.c seeker.h resultLinkedList.o
	$(CC) $(CFLAGS) seeker.c
resultLinkedList.o: resultLinkedList.c resultLinkedList.h
	$(CC) $(CFLAGS) resultLinkedList.c
