CC = gcc
CFLAGS = -g -c
OBJECTS = seeker.o vrepo.o simulator.o
simulator: $(OBJECTS)
	$(CC) -o simulator $(OBJECTS)
simulator.o: simulator.c vrepo.o seeker.o
	$(CC) $(CFLAGS) simulator.c
vrepo.o: vrepo.c vrepo.h seeker.o
	$(CC) $(CFLAGS) vrepo.c
seeker.o: seeker.c seeker.h
	$(CC) $(CFLAGS) seeker.c
