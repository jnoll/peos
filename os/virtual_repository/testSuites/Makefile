CC = gcc
CFLAGS = -g -c -I..

OBJECTS = \
FSseeker.o \
queryLinkedList.o \
resultLinkedList.o \
setup_repositories.o \
vrepo.o

TESTS = \
FSsearchEnd2End \
FSsearchSeek \
FSsearchInvalid \
FSsearchEmpty

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
FS%: FS%.o $(OBJECTS)
	echo "my rules"
	$(CC) -o $@ $< $(OBJECTS) 
#	$@ $@.dat > $(@)ActualResult.txt
#	diff $(@)ExpectedResult.txt $(@)ActualResult.txt

%.o: %.c
	$(CC) $(CFLAGS) $<

%.o: ../%.c
	$(CC) $(CFLAGS) $<

test:	$(TESTS)

	FSsearchEnd2End FSsearchEnd2EndQuery.dat > FSsearchEnd2EndActualResult.txt
	diff FSsearchEnd2EndExpectedResult.txt FSsearchEnd2EndActualResult.txt
	
	FSsearchSeek FSsearchSeekQuery.dat > FSsearchSeekActualResult.txt
	diff FSsearchSeekExpectedResult.txt FSsearchSeekActualResult.txt
	
	FSsearchInvalid FSsearchInvalid.dat > FSsearchInvalidActualResult.txt
	diff FSsearchInvalidExpectedResult.txt FSsearchInvalidActualResult.txt
	
	FSsearchEmpty FSsearchEmpty.dat > FSsearchEmptyActualResult.txt
	diff FSsearchEmptyExpectedResult.txt FSsearchEmptyActualResult.txt	
	
#	make VRsetup
#	VRsetup > VRsetupActualResult.txt
#	diff VRsetupExpectedResult.txt VRsetupActualResult.txt

clean:
	-rm *.o
	-rm core
	-rm $(FS_ACTUAL_RESULT_FILES)
	-rm $(FS_EXPECTED_RESULT_FILES)
	-rm $(FS_QUERY_FILES)
	-rm $(TESTS)

	
