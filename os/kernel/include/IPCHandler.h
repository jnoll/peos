// IPCHandler.h: interface for the IPCHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_IPCHANDLER_H)
#define _IPCHANDLER_H

#include "OSDefs.h"
#include "OSThread.h"
#include "Process.h"

class VMInterface;

class IPCHandler : OSThread
{
public:
	IPCHandler( const string& vm );
	virtual ~IPCHandler();
	virtual void Init();

	bool StartNewAction( const string& parent,
		             const string& action,
                             string& error );
        bool AbortProcess( const string& parent,
                             string& error );
        bool SuspendAction( const string& parent,
		            const string& action,
                            string& error );
        bool IsActive();
protected:
	void Main();
        bool Read( int readFD, string& msg );
        bool Write( int writeFD, const string& msg );

private:
	VMInterface*			pmlvmIF;
	map<int, Process*>              processMap;
	OSLock				processLock;
};

#endif // !defined(_IPCHANDLER_H)
