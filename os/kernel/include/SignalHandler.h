// SignalHandler.h: interface for the SignalHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_SIGNALHANDLER_H)
#define _SIGNALHANDLER_H

class ProcessStatusHandler;
 
class SignalHandler  
{
public:
	virtual ~SignalHandler();
	static void Init( ProcessStatusHandler* handler );

protected:
	SignalHandler();
	static void HandleSignal( int sig );

private:
	static SignalHandler*		instance;
	static ProcessStatusHandler*	statusHandler;
};

#endif // !defined(_SIGNALHANDLER_H)
