CC = gcc
CFLAGS = -g -c
OBJECTS = queryLinkedList.o resultLinkedList.o FSseeker.o setup_repositories.o vrepo.o search.o
search: $(OBJECTS)
	$(CC) -o search $(OBJECTS)
search.o: search.c vrepo.o FSseeker.o
	$(CC) $(CFLAGS) search.c
vrepo.o: vrepo.c vrepo.h FSseeker.o resultLinkedList.o queryLinkedList.o
	$(CC) $(CFLAGS) vrepo.c
setup_repositories.o: setup_repositories.c setup_repositories.h FSseeker.o FSseeker1.o
	$(CC) $(CFLAGS) setup_repositories.c
FSseeker.o: FSseeker.c FSseeker.h resultLinkedList.o queryLinkedList.o
	$(CC) $(CFLAGS) FSseeker.c 
resultLinkedList.o: resultLinkedList.c resultLinkedList.h
	$(CC) $(CFLAGS) resultLinkedList.c
queryLinkedList.o: queryLinkedList.c queryLinkedList.h resultLinkedList.o
	$(CC) $(CFLAGS) queryLinkedList.c
test_setup: FSseeker1.o setup_repositories.o test_setup.o 
	$(CC) -o test_setup FSseeker1.o setup_repositories.o test_setup.o
FSseeker1.o: FSseeker1.c FSseeker.h 
	$(CC) $(CFLAGS) FSseeker1.c
test_setup.o: test_setup.c setup_repositories.o FSseeker1.o
	$(CC) $(CFLAGS) test_setup.c
