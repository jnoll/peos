// OSLock.cpp: implementation of the OSLock class.
//
//////////////////////////////////////////////////////////////////////

#include "OSLock.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

OSLock::OSLock()
    : writerLine( 1000, 0 ),
      readerLine( 1000, 0 )
{
	writersInLine= 0;
	readersInLine= 0;
	readers= 0;			
	writer= false;			
}

OSLock::~OSLock()
{

}

void OSLock::BeginReading()
{
	mutex.Lock();

	if( writer || writersInLine > 0 )
	{
		readersInLine++;
		mutex.Release();
		readerLine.Lock();
	}
	else
	{
		readers++;
		mutex.Release();
	}
}

void OSLock::StopReading()
{
	mutex.Lock();
	readers--;
	if( readers == 0 && writersInLine > 0 )
	{
		writer = true;
		writersInLine--;
		writerLine.Release();
	}
	mutex.Release();
}

void OSLock::BeginWriting()
{
	mutex.Lock();
	if( !writer && readers == 0 )
	{
		writer= true;
		mutex.Release();
	}
	else
	{
		writersInLine++;
		mutex.Release();
		writerLine.Lock();
	}
}

void OSLock::StopWriting()
{
	mutex.Lock();
	writer= false;
	if( readersInLine > 0 )
	{
		while( readersInLine > 0 )
		{
			readersInLine--;
			readers++;
			readerLine.Release();
		}
	}
	else if( writersInLine > 0 )
	{
		writersInLine--;
		writer= true;
		writerLine.Release();
	}
	mutex.Release();
}
