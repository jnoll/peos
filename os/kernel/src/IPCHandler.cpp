// IPCHandler.cpp: implementation of the IPCHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "IPCHandler.h"
#include "VMInterface.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IPCHandler::IPCHandler( const string& vm )
    : pmlvmIF( NULL ),
      processMap()
{
    pmlvmIF = new VMInterface( vm );
}

IPCHandler::~IPCHandler()
{
    this->Stop();
    map<int, Process* >::iterator itr = processMap.begin();
    processLock.BeginWriting();
    while ( itr != processMap.end() )
    {
        delete (*itr).second;
        close( (*itr).first );
    }
    processMap.clear();
    processLock.StopWriting();
    if ( pmlvmIF )
    {
        delete pmlvmIF;
        pmlvmIF = NULL;
    }
}

void IPCHandler::Init()
{
        map<int, Process* >::iterator itr = processMap.begin();
        processLock.BeginWriting();
        while ( itr != processMap.end() )
        {
            delete (*itr).second;
        }
        processMap.clear();
        processLock.StopWriting();
	Start();
}

void IPCHandler::Main()
{       
    bool condition = true;
    map<int, Process*> newProcs;

    while( condition )
    {
        map<int, Process*>::iterator itr;
        string newMsg;
        processLock.BeginWriting();
        for ( itr = processMap.begin(); itr != processMap.end(); itr++ )
        {
            if ( Read( (*itr).first, newMsg ) )
            {
                (*itr).second->AppendBuffer( newMsg );
            }
            else
            {
                (*itr).second->SetState( PS_ABORT );
            }
        }

        processLock.StopWriting();
        myLock.BeginReading();
        condition = ( exit == false ) ? true : false;
        myLock.StopReading();
        if ( condition == false )
        {
            Exit();
        }
        processLock.BeginWriting();

        for ( itr = processMap.begin(); itr != processMap.end(); itr++ )
        {
            newMsg = (*itr).second->GetMsgFromBuffer();

            if ( newMsg.empty() == false )
            {
                char msgID = newMsg[0];
                int pos = newMsg.find_first_not_of( " \t" );
                newMsg.erase( 0, pos );

                if ( msgID == SC_WAIT )
                {
                    (*itr).second->SetState( PS_WAIT );
                }
                else if ( msgID == SC_SELECT )
                {
			Process* temp = (*itr).second;
                	processMap.erase( itr );
			string query("");
			query = newMsg.substr( 2, newMsg.length());
			int FD = pmlvmIF->WaitOnSelectQuery( temp->GetName(), query);
			if ( FD > 0 )
	                {
        	            if ( processMap.insert( make_pair( FD, temp ) ).second == false )
                	    {
                        	close( FD );
	                        delete temp;
        	            }
                	    else
	                    {
        	                temp->SetState( PS_RUN );
                	    }
 	               }
        	        else
                	{
	                    delete temp;
        	        }

                	//itr = processMap.begin();
		}
                else if ( msgID == SC_FORK )
                {
                    string childName( "" );
                    int lineNumber = 0;
                    pos = newMsg.find_first_of( " \t" );
                    childName = newMsg.substr( 0 , pos );
                    newMsg.erase( 0, pos );
                    pos = newMsg.find_first_of( " \t" );
                    newMsg.erase( pos );
                    if ( newMsg.empty() == false )
                    {
                        lineNumber = atoi( newMsg.c_str() );
                    }
                    
                    int FD = pmlvmIF->StartFromLine( childName, lineNumber );

                    if ( FD > 0 )
                    {
                        Process* temp = new Process( childName, (*itr).second );
                        if ( newProcs.insert( make_pair( FD, temp ) ).second == false )
                        {
                            delete temp;
                            close( FD );
                        }
                        else
                        {
                            temp->SetState( PS_RUN );
                        }
                    }
                }
                else if ( msgID == SC_JOIN )
                {
                    string childName( "" );
                    pos = newMsg.find_first_of( " \t" );
                    childName = newMsg.substr( 0 , pos );
                    if ( newMsg.empty() == false )
                    {
                        (*itr).second->SetState( PS_WAIT );
                    }
                }
                else if ( msgID == SC_EXIT )
                {
                    string status( "" );
                    pos = newMsg.find_first_of( " \t" );
                    status = newMsg.substr( 0 , pos );
                    if ( newMsg.empty() == false )
                    {
                        (*itr).second->SetExitState( atoi( newMsg.c_str() ) );
                    }
                }
            }
        }

        processLock.StopWriting();
        myLock.BeginReading();
        condition = ( exit == false ) ? true : false;
        myLock.StopReading();
        if ( condition == false )
        {
            Exit();
        }
        processLock.BeginWriting();

        for ( itr = newProcs.begin(); itr != newProcs.end(); itr++ )
        {
            if ( processMap.insert( make_pair( (*itr ).first, (*itr).second ) ).second == false )
            {
                delete (*itr).second;
                close( (*itr).first );
            }
        }

        itr = processMap.begin();

        while( itr != processMap.end() )
        {
            if ( (*itr).second->GetState() == PS_NEW )
            {
                // Do nothing.
            }
            else if ( (*itr).second->GetState() == PS_READY_CHILD )
            {
                Process* temp = (*itr).second;
                processMap.erase( itr );
                int FD = pmlvmIF->ResumeFromWaitForChild( temp->GetName(), temp->GetChildStatus() );
                if ( FD > 0 )
                {
                    if ( processMap.insert( make_pair( FD, temp ) ).second == false )
                    {
                        close( FD );
                        delete temp;
                    }
                    else
                    {
                        temp->SetState( PS_RUN );
                    }
                }
                else
                {
                    delete temp;
                }

                itr = processMap.begin();
            }
            else if ( (*itr).second->GetState() == PS_READY_ACT )
            {
                Process* temp = (*itr).second;
                processMap.erase( itr );
                int FD = pmlvmIF->ResumeFromWaitOnAction( temp->GetName(), temp->GetAction(), temp->GetEvent() );
                if ( FD > 0 )
                {
                    if ( processMap.insert( make_pair( FD, temp ) ).second == false )
                    {
                        close( FD );
                        delete temp;
                    }
                    else
                    {
                        temp->SetState( PS_RUN );
                    }
                }
                else
                {
                    delete temp;
                }

                itr = processMap.begin();
            }
            else if ( (*itr).second->GetState() == PS_DONE 
                   || (*itr).second->GetState() == PS_ABORT )
            {
                Process* temp = (*itr).second;
                processMap.erase( itr );
                delete temp;
                itr = processMap.begin();
            }
            else
            {
                itr++;
            }
        }

        processLock.StopWriting();

        myLock.BeginReading();
        condition = ( exit == false ) ? true : false;
        myLock.StopReading();

        if ( condition )
        {
            SchedYield();
        }
    }
}

bool IPCHandler::Read( int readFD, string& msg )
{
    char buffer;
    int  readBytes;
    msg.erase();
    do
    {
        readBytes = read( readFD, &buffer, 1 );
        if ( readBytes == -1 && errno == EAGAIN )
        {
            break;
        }
        else if ( ( readBytes == 0 ) || 
                  ( readBytes == -1 && errno != EINTR ) )
        {
            msg.erase();
            return false;
        }
        else if ( readBytes > 0 )
        {
            msg += buffer;
        }
    }
    while ( readBytes == -1 && errno == EINTR );

    return true;
}

bool IPCHandler::Write( int writeFD, const string& msg )
{
    // No implementation yet.
    return false;
}

bool IPCHandler::StartNewAction( const string& parent, const string& action, string& error )
{
    error.erase();
    int FD = pmlvmIF->StartNewAction( parent, action );
    if ( FD < 0 )
    {
        error = "500 Failed to start the action '";
        error += action; 
        error += "' on process '"; 
        error += parent;
        error += "'\n";
        return false;
    }
    else
    {
        Process* temp = new Process( parent );
        temp->SetState( PS_RUN );
        processLock.BeginWriting();
        if ( processMap.insert( make_pair( FD, temp ) ).second == false )
        {
            processLock.StopWriting();
            error = "500 Failed to insert the process '";
            error += parent; 
            error += "' in to the Process Map\n";
            return false;
        }
        else
        {
            processLock.StopWriting();
        }
    }

    return true;
}

bool IPCHandler::AbortProcess( const string& parent, string& error )
{
    error.erase();
    int FD = pmlvmIF->AbortProcess( parent );
    if ( FD < 0 )
    {
        error = "500 Failed to abort the process '";
        error += parent;
        error += "'\n";
        return false;
    }
    else
    {
        Process* temp = new Process( parent );
        temp->SetState( PS_RUN );
        processLock.BeginWriting();
        if ( processMap.insert( make_pair( FD, temp ) ).second == false )
        {
            processLock.StopWriting();
            error = "500 Failed to insert the process '";
            error += parent; 
            error += "' in to the Process Map\n";
            return false;
        }
        else
        {
            processLock.StopWriting();
        }
    }

    return true;
}

bool IPCHandler::SuspendAction( const string& parent, const string& action, string& error )
{
    error.erase();
    error = "500 Failed to abort the action '";
    error += action; 
    error += "' on process '"; 
    error += parent;
    error += "'. Not implemented\n";
    return false;

}

bool IPCHandler::IsActive()
{
    bool retVal;

    myLock.BeginReading();
    retVal = exit;
    myLock.StopReading();

    return retVal;
}

