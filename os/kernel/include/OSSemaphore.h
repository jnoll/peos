// OSSemaphore.h: interface for the OSSemaphore class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_OSSEMAPHORE_H)
#define _OSSEMAPHORE_H

#include "OSDefs.h"

class OSSemaphore  
{
public:
	OSSemaphore( int maximum, int initialValue );
	virtual ~OSSemaphore();

	void Release();
	void Lock();

private:
	SEMAPHORE_TYPE handle;
};

#endif // !defined(_OSSEMAPHORE_H)
