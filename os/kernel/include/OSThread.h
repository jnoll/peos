// OSThread.h: interface for the OSThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_OSTHREAD_H)
#define _OSTHREAD_H

#include "OSDefs.h"
#include "OSLock.h"

class OSThread  
{
public:
    OSThread();
    virtual ~OSThread();

    void Start();
    void Stop();
    void SchedYield();
    void Exit();
protected:
    virtual void    Main() = 0;
    bool            exit;
    THREAD_TYPE     threadHandle;
    OSLock          myLock;

private:
    static THREAD_FUNCTION_RETURN_TYPE StartingPoint( void* arguments );

};

#endif // !defined(_OSTHREAD_H)
