// OSLock.h: interface for the OSLock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_OSLOCK_H)
#define _OSLOCK_H

#include "OSMutex.h"
#include "OSSemaphore.h"

class OSLock  
{
public:
	OSLock();
	virtual ~OSLock();

	void BeginReading();		// To be call by a reader before reading
	void StopReading();		// To be call by a reader when done reading
	
	void BeginWriting();		// To be call by a writer before writing
	void StopWriting();		// To be call by a writer when done writing
	
protected:
	OSSemaphore writerLine;		// The line of writer waiting to write
	OSSemaphore readerLine;		// The line of reader waiting to read
	int writersInLine;		// Number of writer waiting
	int readersInLine;		// Number of readers waiting
	int readers;			// how many readers are reading ?
	bool writer;			// is there a writer in action ?

	OSMutex mutex;
};

#endif // !defined(_OSLOCK_H)
