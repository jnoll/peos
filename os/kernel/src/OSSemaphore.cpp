// OSSemaphore.cpp: implementation of the OSSemaphore class.
//
//////////////////////////////////////////////////////////////////////
#include "OSSemaphore.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

OSSemaphore::OSSemaphore( int maximum, int initialValue)
{
#if defined(_WIN32)
	handle= CreateSemaphore( NULL, (long)maximum, (long)maximum, NULL);
	if( handle == NULL )
#else
        if( sem_init( &handle, 0, ( unsigned int ) maximum ) )
#endif
	{
		cerr << "Error while initializing a semaphore" << endl;
	}

	for( int i = maximum; i > initialValue; i-- )
	{
		Lock();
	}
}

OSSemaphore::~OSSemaphore()
{
#if !defined(_WIN32)
	sem_destroy( &handle );
#endif
}

void OSSemaphore::Lock()
{
#if defined(_WIN32)
    if( WaitForSingleObject( handle, INFINITE ) == WAIT_FAILED ) 
#else
    if( sem_wait( &handle ) != 0 )
#endif
    {
        cerr << "Error while blocking on a semaphore" << endl;
    }
}

void OSSemaphore::Release()
{
#if defined(_WIN32)
    ReleaseSemaphore( handle, 1, NULL ); 
#else
    sem_post( &handle );
#endif
}
