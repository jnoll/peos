// OSMutex.h: interface for the OSMutex class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_OSMUTEX_H)
#define _OSMUTEX_H

#include "OSDefs.h"

class OSMutex  
{
public:
	OSMutex();
	virtual ~OSMutex();

	void Lock();
	void Release();
	
protected:
	MUTEX_TYPE mutex;

};

#endif // !defined(_OSMUTEX_H)
