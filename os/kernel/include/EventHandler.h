// EventHandler.h: interface for the EventHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_EVENTHANDLER_H)
#define _EVENTHANDLER_H

#include "OSThread.h"

class EventHandler : OSThread
{
public:
	bool IsRunning();
	void Init();
	EventHandler();
	virtual ~EventHandler();

protected:
        void Main();

private:

};

#endif // !defined(_EVENTHANDLER_H)
