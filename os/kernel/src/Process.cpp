// Process.cpp: implementation of the Process class.
//
//////////////////////////////////////////////////////////////////////

#include "Process.h"

const int maxAtempt = 10;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Process::Process( const string& proc, Process* p )
	: name( proc ),
	  parent( p ),
	  children(),
	  state( PS_NEW ),
          event( NONE ),
          lastChildStatus( 0 ),
          myExitStatus( 0 )
{

}

Process::~Process()
{
    set<Process*>::iterator itr;

    for( itr = children.begin(); itr != children.end(); itr++ )
    {
        (*itr)->ParentExit( this );
    }

    children.clear();

    if ( parent )
    {
        parent->ChildExit( this );
    }
}


bool Process::AddChild( Process* c )
{
    childrenLock.BeginWriting();
    if ( children.find( c ) != children.end() )
    {
        children.insert( c );
        childrenLock.StopWriting();
        return true;
    }
    else
    {
        childrenLock.StopWriting();
        return false;
    }
}

bool Process::RemoveChild( Process* c )
{
    childrenLock.BeginWriting();
    set<Process*>::iterator itr =  children.find( c );
    if ( itr != children.end() )
    {
        children.erase( itr );
        childrenLock.StopWriting();
	return true;
    }
    else
    {
        childrenLock.StopWriting();
        return false;
    }
}

void Process::ParentExit( Process* p )
{
    if ( parent != NULL )
    {
        if ( parent == p )
        {
            parent = NULL;
        }
        else
        {
            cerr << "Process::ParentExit(): call for wrong child!!!" << endl;
        }
    }
    else
    {
        cerr << "Process::ParentExit(): call for the process without parent!!!" << endl;
    }
}

void Process::ChildExit( Process* c )
{
    set<Process*>::iterator itr;

    if ( RemoveChild( c ) )
    {
        if ( state == PS_WAIT )
        {
            SetChildStatusState( c->GetExitStatus() );
        }
    }
    else
    {
        cerr << "Process::ChildExit(): can't find child process!!!" << endl;
    }

}

string Process::GetMsgFromBuffer()
{
    string ret( "" );
    bufLock.BeginReading();
    int pos = buffer.find_first_of( MESSAGE_DELIMETER );
    if ( pos != string::npos )
    {
        ret = buffer.substr( 0, pos );
        bufLock.StopReading();
        bufLock.BeginWriting();
        buffer.erase( 0, pos );
        bufLock.StopWriting();
    }
    else
    {
        bufLock.StopReading();
    }

    return ret;
}

void Process::AppendBuffer( const string& buf )
{
    bufLock.BeginWriting();
    buffer.append( buf );
    bufLock.StopWriting();
}

void Process::ClearBuffer()
{
    bufLock.BeginWriting();
    buffer.erase();
    bufLock.StopWriting();
}