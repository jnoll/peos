// ProcessState.h: interface for the ProcessState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_PROCESSSTATE_H)
#define _PROCESSSTATE_H


enum ProcessState  
{
	NEW = 0,
	READY,
	RUN,
	DONE,
	SUSPENDED,
	ABORTED,
	NONE   
};

#endif // !defined(_PROCESSSTATE_H)
