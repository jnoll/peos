// Process.h: interface for the Process class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_PROCESS_H)
#define _PROCESS_H

#include "OSDefs.h"
#include "OSLock.h"

class Process  
{
public:
	Process( const string& proc, Process* p = NULL );
	virtual ~Process();

	void   SetState( ProcessState newState ) 
           { state = newState; action.erase(); event = NONE; lastChildStatus = 0; }
	bool   AddChild( Process* c );
	bool   RemoveChild( Process* c );
        void   ParentExit( Process* p );
        void   ChildExit(  Process* c );

	string		    GetName() const { return name; }
	Process*            GetParent() { return parent; }
	ProcessState        GetState() const { return state; }
	set<Process*>&      GetChildren() { return children; }
        string              GetMsgFromBuffer();
        void                AppendBuffer( const string& buf );
        void                ClearBuffer();
        void                SetExitState( int st ) 
           { myExitStatus = st; state = ( st == 0 ) ? PS_DONE : PS_ABORT; }
        void                SetActEventState( const string& act, Event ev )
           { action = act; event = ev; state = PS_READY_ACT; lastChildStatus = 0; }
        void                SetChildStatusState( int stat )
                            { lastChildStatus = stat; state = PS_READY_CHILD; }
        string              GetAction() const { return action; }
        Event               GetEvent() const { return event; }
        int                 GetChildStatus() const { return lastChildStatus; }
        int                 GetExitStatus() const { return myExitStatus; }
private:
	string             name;
        ProcessState       state;
	Process*           parent;
	set<Process*>      children;
        string             buffer;
        OSLock             bufLock;
        OSLock             childrenLock;
        string             action;
        Event              event;
        int                lastChildStatus;
        int                myExitStatus;
};

#endif // !defined(_PROCESS_H)
