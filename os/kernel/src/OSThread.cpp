// OSThread.cpp: implementation of the OSThread class.
//
//////////////////////////////////////////////////////////////////////

#include "OSThread.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

OSThread::OSThread()
{
    myLock.BeginWriting();
	exit = true;
    myLock.StopWriting();
}

OSThread::~OSThread()
{
    myLock.BeginReading();

	if( !exit )
	{
        myLock.StopReading();
		Stop();
	}
    else
    {
        myLock.StopReading();
    }
}

void OSThread::Start()
{
        myLock.BeginWriting();
	exit= false;

#if defined(_WIN32)
	DWORD threadID;

	threadHandle = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE) StartingPoint, this, 0, (LPDWORD) &threadID );
	
        if(!threadHandle)
#else
	if( pthread_create( &threadHandle, NULL, StartingPoint, this) != 0 )
#endif
        {
	    cerr << "Cannot create thread" << endl;
            exit = true;
	}
    myLock.StopWriting();
}

THREAD_FUNCTION_RETURN_TYPE OSThread::StartingPoint( void * arguments )
{
	( (OSThread*) arguments )->Main();

	return THREAD_FUNCTION_RETURN_VALUE;
}

void OSThread::Stop()
{
    myLock.BeginReading();

    if( exit == false )
    {
        myLock.StopReading();
        myLock.BeginWriting();
        exit = true;
        myLock.StopWriting();
#if defined(_WIN32)
	WaitForSingleObject( threadHandle, INFINITE ); 
#else
        pthread_join( threadHandle, NULL );
#endif
    }
    else
    {
        myLock.StopReading();
    }
}

void OSThread::SchedYield()
{
#if defined(_WIN32)
    Sleep( 100 );
#else
    sched_yield();
#endif
}

void OSThread::Exit()
{
    myLock.BeginWriting();
    exit = true;
    myLock.StopWriting();
#if defined(_WIN32)
    ExitThread( 0 );
#else
    pthread_exit( NULL );
#endif
}
