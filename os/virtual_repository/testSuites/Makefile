CC = gcc
LDFLAGS = -g -L..
LIBS = -lvr /home/jnoll/lib/libinn.a
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
testUnitquerywait
#testEmail

FS_ACTUAL_RESULT_FILES = \
VRsetupActualResult.txt \
FSsearchEnd2EndActualResult.txt \
FSsearchSeekActualResult.txt \
FSsearchInvalidActualResult.txt \
FSsearchEmptyActualResult.txt\
UNITandResultActualResult.txt \
UNITorResultActualResult.txt \
UnitquerywaitInvalidActualResult.txt \
testEmailActualResult.txt

FS_EXPECTED_RESULT_FILES = \
FSsearchEnd2EndExpectedResult.txt \
FSsearchSeekExpectedResult.txt \
FSsearchInvalidExpectedResult.txt \
FSsearchEmptyExpectedResult.txt\
UNITandResultExpectedResult.txt \
UNITorResultExpectedResult.txt \
UnitquerywaitInvalidExpectedResult.txt \
testEmailExpectedResult.txt
			 
FS_QUERY_FILES = \
FSsearchEnd2EndQuery.dat \
FSsearchSeekQuery.dat

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

	testFSsearchEnd2End > FSsearchEnd2EndActualResult.txt
	diff FSsearchEnd2EndExpectedResult.txt FSsearchEnd2EndActualResult.txt
	
	testFSsearchSeek > FSsearchSeekActualResult.txt
	diff FSsearchSeekExpectedResult.txt FSsearchSeekActualResult.txt
	
	testFSsearchInvalid > FSsearchInvalidActualResult.txt
	diff FSsearchInvalidExpectedResult.txt FSsearchInvalidActualResult.txt
	
	testFSsearchEmpty > FSsearchEmptyActualResult.txt
	diff FSsearchEmptyExpectedResult.txt FSsearchEmptyActualResult.txt
	
	testUNITandResult > UNITandResultActualResult.txt
	diff UNITandResultExpectedResult.txt UNITandResultActualResult.txt
	
	testUNITorResult > UNITorResultActualResult.txt
	diff UNITorResultExpectedResult.txt UNITorResultActualResult.txt
	
	testUnitquerywait > UnitquerywaitInvalidActualResult.txt
	diff UnitquerywaitInvalidActualResult.txt UnitquerywaitInvalidActualResult.txt 

#	diff testEmailExpectedResult.txt testEmailActualResult.txt
#	testEmail > testEmailActualResult.txt

clean:
	-rm *.o
	-rm core*
	-rm $(FS_ACTUAL_RESULT_FILES)
	-rm $(FS_EXPECTED_RESULT_FILES)
	-rm $(FS_QUERY_FILES)
	-rm $(TESTS)
