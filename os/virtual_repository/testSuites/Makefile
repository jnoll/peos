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
VRsetup \
testFSsearchEnd2End \
testFSsearchSeek \
testFSsearchInvalid \
testFSsearchEmpty

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

# Create a test executable and run the test.
test%: test%.o $(OBJECTS)
	$(CC) -o $@ $< $(OBJECTS) 

# Create a test executable and run the test.
VR%: VR%.o $(VROBJECTS)
	$(CC) -o $@ $< $(VROBJECTS) 

%.o: %.c
	$(CC) $(CFLAGS) $<

%.o: ../%.c
	$(CC) $(CFLAGS) $<

test:	$(TESTS)

	testFSsearchEnd2End > FSsearchEnd2EndActualResult.txt
	diff FSsearchEnd2EndExpectedResult.txt FSsearchEnd2EndActualResult.txt
	
	testFSsearchSeek > FSsearchSeekActualResult.txt
	diff FSsearchSeekExpectedResult.txt FSsearchSeekActualResult.txt
	
	testFSsearchInvalid > FSsearchInvalidActualResult.txt
	diff FSsearchInvalidExpectedResult.txt FSsearchInvalidActualResult.txt
	
	testFSsearchEmpty > FSsearchEmptyActualResult.txt
	diff FSsearchEmptyExpectedResult.txt FSsearchEmptyActualResult.txt
	
	VRsetup > VRsetupActualResult.txt
	diff VRsetupExpectedResult.txt VRsetupActualResult.txt	
	
clean:
	-rm *.o
	-rm core
	-rm $(FS_ACTUAL_RESULT_FILES)
	-rm $(FS_EXPECTED_RESULT_FILES)
	-rm $(FS_QUERY_FILES)
	-rm $(TESTS)
