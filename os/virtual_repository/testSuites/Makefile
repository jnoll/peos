CC = gcc
CFLAGS = -g -c

OBJECTS = queryLinkedList.o resultLinkedList.o FSseeker.o \
	  setup_repositories.o vrepo.o debug.o search.o
	  
FS_SEARCH_OBJECTS = FSsearchEnd2End.o
FS_SEARCH_SEEK_OBJECTS = FSsearchSeek.o
FS_SEARCH_INVALID_OBJECTS = FSsearchInvalid.o
FS_SEARCH_EMPTY_OBJECTS = FSsearchEmpty.o

FS_actualResultFiles = 	VRsetupActualResult.txt FSsearchEnd2EndActualResult.txt \
			FSsearchSeekActualResult.txt FSsearchInvalidActualResult.txt \
			FSsearchEmptyActualResultEmpty.txt
		
FS_expectedResultFiles = FSsearchEnd2EndExpectedResult.txt FSsearchSeekExpectedResult.txt \
			 FSsearchInvalidExpectedResult.txt FSsearchEmptyExpectedResult.txt
			 
FS_queryFiles = FSsearchEnd2EndQuery.dat FSsearchSeekQuery.dat

#VRsetup: dummy_seekers.o setup_repositories.o VRsetup.o 
#	$(CC) -o VRsetup dummy_seekers.o setup_repositories.o VRsetup.o
#dummy_seekers.o: dummy_seekers.c FSseeker.h 
#	$(CC) $(CFLAGS) dummy_seekers.c
#VRsetup.o: VRsetup.c setup_repositories.o dummy_seekers.o
#	$(CC) $(CFLAGS) VRsetup.c

FSsearchEnd2End: $(FS_SEARCH_OBJECTS) 
	$(CC) -o FSsearchEnd2End $(FS_SEARCH_OBJECTS) 
FSsearchEnd2End.o: FSsearchEnd2End.c vrepo.o FSseeker.o
	$(CC) $(CFLAGS) FSsearchEnd2End.c
FSsearchSeek: $(FS_SEARCH_SEEK_OBJECTS)
	$(CC) -o FSsearchSeek $(FS_SEARCH_SEEK_OBJECTS)
FSsearchSeek.o: FSsearchSeek.c vrepo.o FSseeker.o
	$(CC) $(CFLAGS) FSsearchSeek.c
FSsearchInvalid: $(FS_SEARCH_INVALID_OBJECTS)
	$(CC) -o FSsearchInvalid $(FS_SEARCH_INVALID_OBJECTS)
FSsearchInvalid.o: FSsearchInvalid.c vrepo.o FSseeker.o
	$(CC) $(CFLAGS) FSsearchInvalid.c
FSsearchEmpty: $(FS_SEARCH_EMPTY_OBJECTS)
	$(CC) -o FSsearchEmpty $(FS_SEARCH_EMPTY_OBJECTS)
FSsearchEmpty.o: FSsearchEmpty.c vrepo.o FSseeker.o
	$(CC) $(CFLAGS) FSsearchEmpty.c
debug.o: debug.c debug.h 
	$(CC) $(CFLAGS) debug.c

test:	
	make clean
	
	../make
	pwd
	mv *.o ./testSuites
	cd testSuites	
	
#	make VRsetup
#	VRsetup > VRsetupActualResult.txt
#	diff VRsetupExpectedResult.txt VRsetupActualResult.txt
	
	make FSsearchEnd2End 
	FSsearchEnd2End FSsearchEnd2EndQuery.dat > FSsearchEnd2EndActualResult.txt
	diff FSsearchEnd2EndExpectedResult.txt FSsearchEnd2EndActualResult.txt
	
	make FSsearchSeek
	FSsearchSeek FSsearchSeekQuery.dat > FSsearchSeekActualResult.txt
	diff FSsearchSeekExpectedResult.txt FSsearchSeekActualResult.txt
	
	make FSsearchInvalid
	FSsearchInvalid FSsearchInvalid.dat > FSsearchInvalidActualResult.txt
	diff FSsearchInvalidExpectedResult.txt FSsearchInvalidActualResult.txt
	
	make FSsearchEmpty
	FSsearchEmpty FSsearchEmpty.dat > FSsearchEmptyActualResult.txt
	diff FSsearchEmptyExpectedResult.txt FSsearchEmptyActualResultEmpty.txt	
	
clean:
	touch dummy.o
	
	touch FS_actualResultFiles
	touch FS_expectedResultFiles			 
	touch FS_queryFiles
		
	rm *.o
	
	rm FS_actualResultFiles
	rm FS_expectedResultFiles			 
	rm FS_queryFiles
