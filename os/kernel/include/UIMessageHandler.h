// UIMessageHandler.h: interface for the UIMessageHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_UIMESSAGEHANDLER_H)
#define _UIMESSAGEHANDLER_H

#include "OSDefs.h"
#include "OSThread.h"

class DataAccessInterface;

class UIMessageHandler : OSThread
{
public:
    UIMessageHandler( bool useRepositoryIF = false );
    UIMessageHandler( int soc, bool useRepositoryIF = false );
    virtual ~UIMessageHandler();
    virtual void Init();

    void SetSocket( int soc ) { socket = soc; }
    int  GetSocket() const { return socket; }

    bool IsActive();

    static int GetSessionCount() { return sessionCount; }

protected:
    bool DoneAction( const string& procName, const string& actionName );
    bool RunAction( const string& procName, const string& actionName );
    bool SuspendAction( const string& procName, const string& actionName );
    bool AbortProcess( const string& procName );
    void Main();

    bool CreateProcess( const string& modelName, const string& parentName, int startPC );
    bool SendHelp();
    bool SendMessage( const string& message );
    bool ListModels();
    bool Login();
    bool QueryActions( Event event );
    bool Authenticate( const string& uname, const string& passwd );
    bool CheckPrivilege( const string& model, const string& action );

private:
    int	                    socket;
    string                  userName;
    string                  password;
    set<string>	            processNames;
    list<string>            roles;
    static int              sessionCount;
    DataAccessInterface*    dataAccessIF;
	
};

#endif // !defined(_UIMESSAGEHANDLER_H)
