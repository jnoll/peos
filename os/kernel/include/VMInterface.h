// VMInterface.h: interface for the VMInterface class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_VMINTERFACE_H)
#define _VMINTERFACE_H

#include "OSDefs.h"

class VMInterface  
{
public:
	VMInterface( const string& path );
	virtual ~VMInterface();

	int StartNewAction( const string& proc, const string& action );
        int AbortProcess( const string& proc );
	int ResumeFromWaitOnAction( const string& proc, const string& action, Event event );
	int ResumeFromWaitForChild( const string& proc, int childExitStatus );
	int StartFromLine( const string& proc, int lineNumber );
	int WaitOnSelectQuery( const string& proc, const string& query );
        int StartVM( char* path, char** commandLine );
private:
        string VMPath;

};

#endif // !defined(_VMINTERFACE_H)
