CC = gcc
CFLAGS = -g -c -I..

OBJECTS = \
FSseeker.o \
queryLinkedList.o \
resultLinkedList.o \
setup_repositories.o \
vrepo.o

VROBJECTS = \
setup_repositories.o \
dummy_seekers.o

TESTS = \
testFSsearchEnd2End \
testFSsearchSeek \
testFSsearchInvalid \
testFSsearchEmpty

TEST1 = \
VRsetup

FS_ACTUAL_RESULT_FILES = \
VRsetupActualResult.txt \
FSsearchEnd2EndActualResult.txt \
FSsearchSeekActualResult.txt \
FSsearchInvalidActualResult.txt \
FSsearchEmptyActualResult.txt

FS_EXPECTED_RESULT_FILES = \
FSsearchEnd2EndExpectedResult.txt \
FSsearchSeekExpectedResult.txt \
FSsearchInvalidExpectedResult.txt \
FSsearchEmptyExpectedResult.txt
			 
FS_QUERY_FILES = \
FSsearchEnd2EndQuery.dat \
FSsearchSeekQuery.dat

#VRsetup: dummy_seekers.o setup_repositories.o VRsetup.o 
#	$(CC) -o VRsetup dummy_seekers.o setup_repositories.o VRsetup.o
#dummy_seekers.o: dummy_seekers.c FSseeker.h 
#	$(CC) $(CFLAGS) dummy_seekers.c
#VRsetup.o: VRsetup.c setup_repositories.o dummy_seekers.o
#	$(CC) $(CFLAGS) VRsetup.c

# Create a test executable and run the test.
test%: test%.o $(OBJECTS)
#	echo "my rules"
	$(CC) -o $@ $< $(OBJECTS) 
#	$@ $@.dat > $(@)ActualResult.txt
#	diff $(@)ExpectedResult.txt $(@)ActualResult.txt

# Create a test executable and run the test.
VR%:VR%.o $(VROBJECTS)
#	echo "my rules"
	$(CC) -o $@ $< $(VROBJECTS) 
#	$@ $@.dat > $(@)ActualResult.txt
#	diff $(@)ExpectedResult.txt $(@)ActualResult.txt

%.o: %.c
	$(CC) $(CFLAGS) $<

%.o: ../%.c
	$(CC) $(CFLAGS) $<

test:	$(TESTS) $(TEST1)

	testFSsearchEnd2End > FSsearchEnd2EndActualResult.txt
	diff FSsearchEnd2EndExpectedResult.txt FSsearchEnd2EndActualResult.txt
	
	testFSsearchSeek > FSsearchSeekActualResult.txt
	diff FSsearchSeekExpectedResult.txt FSsearchSeekActualResult.txt
	
	testFSsearchInvalid > FSsearchInvalidActualResult.txt
	diff FSsearchInvalidExpectedResult.txt FSsearchInvalidActualResult.txt
	
	testFSsearchEmpty > FSsearchEmptyActualResult.txt
	diff FSsearchEmptyExpectedResult.txt FSsearchEmptyActualResult.txt
	
	testVRsetup > VRsetupActualResult.txt
	diff VRsetupExpectedResult.txt VRsetupActualResult.txt	
	
clean:
	-rm *.o
	-rm core
	-rm $(FS_ACTUAL_RESULT_FILES)
	-rm $(FS_EXPECTED_RESULT_FILES)
	-rm $(FS_QUERY_FILES)
	-rm $(TESTS)
	-rm $(TEST1)


	
