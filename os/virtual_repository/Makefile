CC = gcc
CFLAGS = -g -c
OBJECTS = queryLinkedList.o resultLinkedList.o FSseeker.o setup_repositories.o vrepo.o search.o
TEST_OBJECTS = queryLinkedList.o resultLinkedList.o FSseeker.o setup_repositories.o vrepo.o test_search.o
TEST_INVALID_OBJECTS = queryLinkedList.o resultLinkedList.o FSseeker.o setup_repositories.o vrepo.o test_searchInvalid.o
TEST_EMPTY_OBJECTS = queryLinkedList.o resultLinkedList.o FSseeker.o setup_repositories.o vrepo.o test_searchEmpty.o
search: $(OBJECTS)
	$(CC) -o search $(OBJECTS)
search.o: search.c vrepo.o FSseeker.o
	$(CC) $(CFLAGS) search.c
vrepo.o: vrepo.c vrepo.h FSseeker.o resultLinkedList.o queryLinkedList.o
	$(CC) $(CFLAGS) vrepo.c
setup_repositories.o: setup_repositories.c setup_repositories.h FSseeker.o dummy_seekers.o
	$(CC) $(CFLAGS) setup_repositories.c
FSseeker.o: FSseeker.c FSseeker.h resultLinkedList.o queryLinkedList.o
	$(CC) $(CFLAGS) FSseeker.c 
resultLinkedList.o: resultLinkedList.c resultLinkedList.h
	$(CC) $(CFLAGS) resultLinkedList.c
queryLinkedList.o: queryLinkedList.c queryLinkedList.h resultLinkedList.o
	$(CC) $(CFLAGS) queryLinkedList.c
test_setup: dummy_seekers.o setup_repositories.o test_setup.o 
	$(CC) -o test_setup dummy_seekers.o setup_repositories.o test_setup.o
dummy_seekers.o: dummy_seekers.c FSseeker.h 
	$(CC) $(CFLAGS) dummy_seekers.c
test_setup.o: test_setup.c setup_repositories.o dummy_seekers.o
	$(CC) $(CFLAGS) test_setup.c
test_search: $(TEST_OBJECTS)
	$(CC) -o test_search $(TEST_OBJECTS)
test_search.o: test_search.c vrepo.o FSseeker.o
	$(CC) $(CFLAGS) test_search.c
test_searchInvalid: $(TEST_INVALID_OBJECTS)
	$(CC) -o test_searchInvalid $(TEST_INVALID_OBJECTS)
test_searchInvalid.o: test_searchInvalid.c vrepo.o FSseeker.o
	$(CC) $(CFLAGS) test_searchInvalid.c
test_searchEmpty: $(TEST_EMPTY_OBJECTS)
	$(CC) -o test_searchEmpty $(TEST_EMPTY_OBJECTS)
test_searchEmpty.o: test_searchEmpty.c vrepo.o FSseeker.o
	$(CC) $(CFLAGS) test_searchEmpty.c
test:	
	make test_search 
	make test_searchInvalid
	make test_searchEmpty
	make test_setup
	test_setup > actualSetup.txt
	diff expectedSetup.txt actualSetup.txt
	test_search testQuery.dat > actualResult.txt
	diff expectedResult.txt actualResult.txt
	test_searchInvalid testInvalid.dat > actualResultInvalid.txt
	diff expectedResultInvalid.txt actualResultInvalid.txt
	test_searchEmpty testEmpty.dat > actualResultEmpty.txt
	diff expectedResultEmpty.txt actualResultEmpty.txt
clean:
	rm *.o
	rm actualResult.txt
	rm expectedResult.txt
	rm actualResultInvalid.txt
	rm expectedResultInvalid.txt
	rm actualResultEmpty.txt
	rm expectedResultEmpty.txt
