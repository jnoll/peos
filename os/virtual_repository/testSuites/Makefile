CC = gcc
LDFLAGS = -g -L.. -L/home/jnoll/lib/
LIBS = -lvr -linn
CFLAGS = -g -c -I..
LIB = ../libvr.a

OBJECTS = \
../FSseeker.o \
../EMAILseeker.o \
../queryLinkedList.o \
../resultLinkedList.o \
../setup_repositories.o \
../vrepo.o \
../debug.o

TESTS = \
VRsetup \
testFSsearchEnd2End \
testFSsearchSeek \
testFSsearchInvalid \
testFSsearchEmpty \
testUNITandResult \
testUNITorResult \
testUNITsortResult \
testUNITquerywait \
testUpdateTestSite \
testEMAILsearchEnd2End \
testEMAILsearchInvalid \
testUNITquerywait \
testUNITgetPath 

FS_ACTUAL_RESULT_FILES = \
VRsetupActualResult.txt \
FSsearchEnd2EndActualResult.txt \
FSsearchSeekActualResult.txt \
FSsearchInvalidActualResult.txt \
FSsearchEmptyActualResult.txt\
UNITandResultActualResult.txt \
UNITorResultActualResult.txt \
UNITsortResultActualResult.txt \
UNITquerywaitActualResult.txt \
UNITgetPathActualResult.txt  \
EMAILsearchEnd2EndActualResult.txt \
EMAILsearchInvalidActualResult.txt

FS_EXPECTED_RESULT_FILES = \
FSsearchEnd2EndExpectedResult.txt \
FSsearchSeekExpectedResult.txt \
FSsearchInvalidExpectedResult.txt \
FSsearchEmptyExpectedResult.txt\
UNITandResultExpectedResult.txt \
UNITorResultExpectedResult.txt \
UNITsortResultExpectedResult.txt \
UNITquerywaitExpectedResult.txt \
EMAILsearchEnd2EndExpectedResult.txt \
EMAILsearchInvalidExpectedResult.txt 
	 
all: $(TESTS)

$(LIB): $(OBJECTS)
	$(MAKE) -C ..

# Create a test executable and run the test.
test%: test%.o $(LIB)
	$(CC) -o $@ $< $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $<

%.o: ../%.c
	$(CC) $(CFLAGS) $<

test:	$(TESTS)

	cp ../vr.rc .
	
	testUpdateTestSite
	
	testFSsearchEnd2End > FSsearchEnd2EndActualResult.txt
	diff FSsearchEnd2EndExpectedResult.txt FSsearchEnd2EndActualResult.txt
	
	testFSsearchSeek > FSsearchSeekActualResult.txt
	diff FSsearchSeekExpectedResult.txt FSsearchSeekActualResult.txt
	
	testFSsearchInvalid > FSsearchInvalidActualResult.txt
	diff FSsearchInvalidExpectedResult.txt FSsearchInvalidActualResult.txt
	
	testFSsearchEmpty > FSsearchEmptyActualResult.txt
	diff FSsearchEmptyExpectedResult.txt FSsearchEmptyActualResult.txt
	
	testEMAILsearchEnd2End > EMAILsearchEnd2EndActualResult.txt
	diff EMAILsearchEnd2EndExpectedResult.txt EMAILsearchEnd2EndActualResult.txt
	
	testEMAILsearchInvalid > EMAILsearchInvalidActualResult.txt
	diff EMAILsearchInvalidExpectedResult.txt EMAILsearchInvalidActualResult.txt
	
	testUNITandResult > UNITandResultActualResult.txt
	diff UNITandResultExpectedResult.txt UNITandResultActualResult.txt
	
	testUNITorResult > UNITorResultActualResult.txt
	diff UNITorResultExpectedResult.txt UNITorResultActualResult.txt
	
	testUNITsortResult > UNITsortResultActualResult.txt
	diff UNITsortResultExpectedResult.txt UNITsortResultActualResult.txt
	
	testUNITquerywait > UNITquerywaitActualResult.txt
	diff UNITquerywaitExpectedResult.txt UNITquerywaitActualResult.txt
		
	cp testvr.rc vr.rc
	testUNITgetPath > UNITgetPathActualResult.txt
	cp vr.rc $(HOME)
	testUNITgetPath >> UNITgetPathActualResult.txt
	diff UNITgetPathActualResult.txt UNITgetPathExpectedResult.txt

clean:
	-rm *.o
	-rm core*
	-rm $(FS_ACTUAL_RESULT_FILES)
	-rm $(FS_EXPECTED_RESULT_FILES)
	-rm $(TESTS)
