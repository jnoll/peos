// ProcessStatusHandler.h: interface for the ProcessStatusHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_PROCESSSTATUSHANDLER_H)
#define _PROCESSSTATUSHANDLER_H

#include "OSLock.h"

class IPCHandler;
class UIMessageHandler;
class EventHandler;
class TimedEventHandler;

class ProcessStatusHandler
{
public:
	void SetEventHandler( EventHandler* handler );
	static void Init( int port, int maxSes, const string& vm, bool repUsed );
	static ProcessStatusHandler* Instance();
	void Run();
	void Shutdown();
	virtual ~ProcessStatusHandler();
	void BeginWritingMap();
	void StopWritingMap();
	void BeginReadingMap();
	void StopReadingMap();

        map<UIMessageHandler*, IPCHandler*>& GetMapRef() { return processMap; }

private:
    map<UIMessageHandler*, IPCHandler*>	        processMap;
    static ProcessStatusHandler*                instance;
    OSLock                                      mapLock;
    EventHandler*                               eventHandler;
    static int                                  myPort;
    static int                                  maxSession;
    static string                               pmlvm;
    int                                         sessionCount;
    int                                         sockfd;
    bool                                        repositoryUsed;
    TimedEventHandler*                          timedEventHandler;
	 protected:
	ProcessStatusHandler( bool = false );	
};

#endif // !defined(_PROCESSSTATUSHANDLER_H)
