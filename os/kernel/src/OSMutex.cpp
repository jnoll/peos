// OSMutex.cpp: implementation of the OSMutex class.
//
//////////////////////////////////////////////////////////////////////

#include "OSMutex.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

OSMutex::OSMutex()
{
#if defined(_WIN32)
    mutex= CreateMutex(NULL,FALSE,NULL);
    if(mutex == NULL)
#else	
    if( pthread_mutex_init( &mutex, NULL ) )
#endif
    {
        cerr << "Error while initializing a mutex" << endl;
    }
}

OSMutex::~OSMutex()
{
#if !defined(_WIN32)
    pthread_mutex_destroy( &mutex );
#endif
}

void OSMutex::Lock()
{
#if defined(_WIN32)
    if( WaitForSingleObject( mutex, INFINITE ) == WAIT_FAILED ) 
#else
    if( pthread_mutex_lock( &mutex ) != 0 )
#endif
    {
        cerr << "Error while blocking on a mutex" << endl;
    }
}

void OSMutex::Release()
{
#if defined(_WIN32)
    ReleaseMutex(mutex); 
#else
    pthread_mutex_unlock( &mutex );
#endif
}
