// UIMessageHandler.cpp: implementation of the UIMessageHandler class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <gdbm.h>
#include <sys/stat.h>
#include <sys/types.h>
#define __USE_XOPEN
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <dirent.h>
#include <pwd.h>
#include <iostream.h>
#include <fstream.h>
#include <vector>

#include "UIMessageHandler.h"
#include "DataAccessInterface.h"
#include "RepositoryIF.h"
#include "ProcessStatusHandler.h"
#include "GDBMIF.h"
#include "IPCHandler.h"

//#define strcasecmp strcmp


const int invalidSocket = -1;
int UIMessageHandler::sessionCount = 0;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

UIMessageHandler::UIMessageHandler( bool useRepositoryIF )
	: socket( invalidSocket ),
	  dataAccessIF( NULL )
{
    sessionCount++;

    if ( useRepositoryIF )
    {
        dataAccessIF = new RepositoryIF;
    }
    else
    {
	dataAccessIF = new GDBMIF;
    }
}

UIMessageHandler::UIMessageHandler( int soc, bool useRepositoryIF )
	: socket( soc )
{
    sessionCount++;

    if ( useRepositoryIF )
    {
        dataAccessIF = new RepositoryIF;
    }
    else
    {
	dataAccessIF = new GDBMIF;
    }
}

UIMessageHandler::~UIMessageHandler()
{
    shutdown( socket, SHUT_RDWR );
    this->Stop();
    sessionCount--;
}

bool UIMessageHandler::CreateProcess( const string& modelName, const string& parentName, int startPC )
{
    string error;
    string process;

    /* Make sure that user is allowed to create the process */
    if (!CheckPrivilege(modelName, "create")) {
        SendMessage("500 - Insufficient privileges to create this process\n");
        return false;
    }

    /* Retrieve the next available process name */
    if (!dataAccessIF->GetProcessName( modelName, userName, process, error)) {
	SendMessage( error );
	return false;
    }

    /* Make sure that the process does not already exist */
    if (processNames.find( process ) != processNames.end()) {
        error = "500 Process with name '";
        error += process;
        error += "' already exists\n";
        SendMessage( error );
        return false;
    }

    /* Initialize process */
    if ( !dataAccessIF->InitProcessState( modelName, 
                                          process, 
                                          parentName,
                                          startPC, 
                                          error)) {
	SendMessage( error );
	return false;
    }
    processNames.insert( process );
    SendMessage( "100 Create successful.\n" );
    return true;
}

bool UIMessageHandler::SendHelp()
{
    string msg = "100 \n\r";
    msg += "100 - login <username> <password>\n\r";
    msg += "100 - list\n\r";
    msg += "100 - create <modelname>\n\r";
    msg += "100 - available\n\r";
    msg += "100 - run <proc> <act>\n\r";
    msg += "100 - running\n\r";
    msg += "100 - done <proc> <act>\n\r";
    msg += "100 - suspend <proc> <act>\n\r";
    msg += "100 - abort <proc>\n\r";
    msg += "100 - logout\n\r";
    msg += "100 - exit\n\r";
    msg += "100 - help\n";

    return SendMessage( msg );
}

bool UIMessageHandler::SendMessage( const string& message )
{
    string msg( message );
    msg += "\r";
    if ( send( socket, msg.c_str(), strlen( msg.c_str() ), 0 ) != -1 ) {
        return true;
    }
    else {
        return false;
    }
}

bool UIMessageHandler::ListModels()
{
    string err;
    vector<string> models = dataAccessIF->ListModels( err );

    /* Check if there is an error */
    if ( models.empty() == true && err.empty() == false ) {
        SendMessage( err );
        return false;
    }

    /* Create model list and send to the user.
       Include only models that the user is authorized to see. */
    string reply( "100 \n\r" );
    for ( int i = 0; i < models.size(); i++ ) {
        if (CheckPrivilege(models[i], "list")) {
            reply += "100 - ";
            reply += models[i];
            reply += "\n";
            if (i < (models.size() - 1)) {
                reply += "\r";
            }
        }
    }
    SendMessage( reply );
    return true;
}

bool UIMessageHandler::Login()
{
    int    bufferLen;
    char   buffer[4096];
    char   rcvBuf;
    int    rcvLen;
    string receivedString;
    bool   ret = false;

    /* login */
    do 
    {
        memset( buffer, '\0', sizeof( buffer ) );
        bufferLen = 0;

        do
        {
            rcvLen = recv( socket, &rcvBuf, 1, 0 );

            if ( rcvLen == -1 && errno != EINTR )
            {
                perror( "recv call failed" );
                return false;
            }
            else if ( rcvLen > 0 )
            {
                buffer[bufferLen] = rcvBuf;
                bufferLen++;
            }
        }
        while ( ( rcvBuf != '\n' && rcvLen > 0 ) || errno == EINTR );

        buffer[bufferLen] = '\0'; /* append string terminater */

        /* peer died ? */
        if ( bufferLen == 0 )
        {
            continue;
        }

        receivedString = buffer;

        if ( strcasecmp( receivedString.substr( 0, 5 ).c_str(), "LOGIN" ) ==
0 )         {             char uname[9];     /* username */
            char passwd[14];   /* password */

            if ( sscanf( buffer + 5, "%9s%14s", uname, passwd ) != 2 )
	    {
                SendMessage( "500 Invalid format!\n" );
            }
            else
	    {
                ret = Authenticate( uname, passwd );
            
	        if ( ret == false )
		{
                    SendMessage("500 login failed, try again\n");
		}
                else
		{
                    SendMessage( "100 Login successful\n" );
		}
	    }
        }
        else if ( strcasecmp( receivedString.substr( 0, 4 ).c_str(), "EXIT" ) == 0 
               || strcasecmp( receivedString.substr( 0, 4 ).c_str(), "LOGOUT" ) == 0 )
        {
            return false;
        }
        else if (strcasecmp( receivedString.substr( 0, 4 ).c_str(), "HELP" ) == 0 )
        {
            SendHelp();
        }
        else 
	{
            SendMessage( "500 login first!\n" );
        }

    } while ( ret == false );

    return ret;
}

void UIMessageHandler::Init()
{
    Start();
}

void UIMessageHandler::Main()
{   
    int bufferLen = 0;
    char buffer[4096];
    char rcvBuf;
    int rcvLen;
    string receivedString;
    bool condition = true;
   
    /* Login() always return with success */ 
    if ( Login() == false )
    {
        myLock.BeginWriting();
        shutdown( socket, SHUT_RDWR );
        socket = -1;
        myLock.StopWriting();
        Exit();
    }

    vector<string> procs = dataAccessIF->GetCreatedProcs( userName );
    myLock.BeginWriting();
    for ( int i = 0; i < procs.size(); i++ )
    {
        processNames.insert( procs[i] );
    }
    myLock.StopWriting();

    while( condition )
    {
        memset( buffer, '\0', sizeof( buffer ) );
        bufferLen = 0;

        do
        {
            rcvLen = recv( socket, &rcvBuf, 1, 0 );

            if ( rcvLen == -1 && errno != EINTR )
            {
                perror( "recv call failed" );
                myLock.BeginWriting();
                shutdown( socket, SHUT_RDWR );
                socket = -1;
                myLock.StopWriting();
                Exit();
            }
            else if ( rcvLen > 0 && rcvBuf != '\r' )
            {
                buffer[bufferLen] = rcvBuf;
                bufferLen++;
            }
        }
        while ( ( rcvBuf != '\n' && rcvLen > 0 ) || errno == EINTR );

        buffer[bufferLen] = '\0'; /* append string terminater */

        /* peer died ? */
        if ( bufferLen == 0 )
        {
            continue;
        }

        receivedString = buffer;

        if ( strcasecmp( receivedString.substr( 0, 4 ).c_str(), "LIST" ) == 0 )
        {
            ListModels();
        }
        else if ( strcasecmp( receivedString.substr( 0, 6 ).c_str(), "CREATE" ) == 0 )
        {
            char model[256];

            if ( sscanf( buffer + 6, "%255s", model ) != 1)
            {
                SendMessage("500 Geeze, use correct format!\n");
            }
            else if ( CreateProcess( model, "", 0 ) == false )
            {
                SendMessage("500 Cannot create process. Check format!\n");
            }
        }   
        else if ( strcasecmp( receivedString.substr( 0, 9 ).c_str(), "AVAILABLE" ) == 0 )
        {
            QueryActions( READY );
        }
        else if ( strcasecmp( receivedString.substr( 0, 7 ).c_str(), "RUNNING" ) == 0 )
        {
            QueryActions( RUN );
        }
        else if ( strcasecmp( receivedString.substr( 0, 3 ).c_str(), "RUN" ) == 0 )
        {
            char proc[256];
            char act[256];

            if ( sscanf( buffer + 4, "%255s%255s", proc, act ) != 2 )
            {
                SendMessage("500 Geeze, use correct format!\n");
            }
            else
            {
                RunAction( proc, act );
            }
        } 
        else if ( strcasecmp( receivedString.substr( 0, 7 ).c_str(), "SUSPEND" ) == 0 )
        {
            char proc[256];
            char act[256];

            if ( sscanf( buffer + 7, "%255s%255s", proc, act ) != 2 )
            {
                SendMessage("500 Geeze, use correct format!\n");
            }
            else
            {
                SuspendAction( proc, act );
            }
        }
        else if ( strcasecmp( receivedString.substr( 0, 5 ).c_str(), "ABORT" ) == 0 )
        {
            char proc[256];

            if ( sscanf( buffer + 5, "%255s", proc ) != 1)
            {
                SendMessage("500 Geeze, use correct format!\n");
            }
            else
            {
                AbortProcess( proc );
            }
        } 
        else if ( strcasecmp( receivedString.substr( 0, 4 ).c_str(), "DONE" ) == 0 )
        {
            char proc[256];
            char act[256];

            if ( sscanf( buffer + 5, "%s%s", proc, act) != 2 )
            {
                 SendMessage("500 Geeze, use correct format!\n");
            }
            else
            {
                DoneAction( proc, act );
            }
        }
        else if ( strcasecmp( receivedString.substr( 0, 6 ).c_str(), "LOGOUT" ) == 0 )
        {
            myLock.BeginWriting();
            shutdown( socket, SHUT_RDWR );
            socket = -1;
            myLock.StopWriting();
            Exit();
        }
        else if ( strcasecmp( receivedString.substr( 0, 4 ).c_str(), "EXIT" ) == 0 )
        {
            myLock.BeginWriting();
            shutdown( socket, SHUT_RDWR );
            socket = -1;
            myLock.StopWriting();
            Exit();
        }
        else if (strcasecmp( receivedString.substr( 0, 4 ).c_str(), "HELP" ) == 0 )
        {
            SendHelp();
        }
        else
        {
            SendMessage("500 Hi, what are you mumbling?\n");
        }

        myLock.BeginReading();
        condition = ( exit == false ) ? true : false;
        myLock.StopReading();
    }
}

bool UIMessageHandler::IsActive()
{
    bool retVal;

    myLock.BeginReading();
    retVal = exit;
    myLock.StopReading();

    return retVal;
}


bool UIMessageHandler::QueryActions( Event event )
{
    vector<string> msgs = dataAccessIF->QueryActions( userName, (int) event );

    if ( msgs.empty() )
    {
        SendMessage( "500 No available processes\n" );
        return false;
    }

    string reply( "100 \n\r" );

    for ( int i = 0; i < msgs.size(); i++ )
    {
        reply += msgs[i];

        if ( i < ( msgs.size() - 1 ) )
        {
            reply += "\r";
        }
    }

    SendMessage( reply );

    return true;
}

bool UIMessageHandler::RunAction( const string& procName, const string& actionName )
{

    string error;

    if ( processNames.find( procName ) == processNames.end() )
    {
        error = "500 Process '";
        error += procName;
        error += "' has not been created yet\n";
        SendMessage( error );
        return false;
    }

    ProcessStatusHandler* handler = ProcessStatusHandler::Instance();
    map<UIMessageHandler*, IPCHandler*>::iterator itr;
    handler->BeginReadingMap();
    itr = handler->GetMapRef().find( this );
    if ( itr != handler->GetMapRef().end() )
    {
        if ( (*itr).second->StartNewAction( procName, actionName, error ) == false )
        {
            handler->StopReadingMap();
            SendMessage( error );
            return false;
        }
    }
    else
    {
        handler->StopReadingMap();
        error = "500 Cannot obtain IPC handler for process '";
        error += procName;
        error += "'\n";
        SendMessage( error );
        return false;
    }

    handler->StopReadingMap();

    return true;
}

bool UIMessageHandler::SuspendAction( const string& procName, const string& actionName )
{

    string error;

    if ( processNames.find( procName ) == processNames.end() )
    {
        error = "500 Process '";
        error += procName;
        error += "' has not been created yet\n";
        SendMessage( error );
        return false;
    }

    ProcessStatusHandler* handler = ProcessStatusHandler::Instance();
    map<UIMessageHandler*, IPCHandler*>::iterator itr;
    handler->BeginReadingMap();
    itr = handler->GetMapRef().find( this );
    if ( itr != handler->GetMapRef().end() )
    {
        if ( (*itr).second->SuspendAction( procName, actionName, error ) == false )
        {
            handler->StopReadingMap();
            SendMessage( error );
            return false;
        }
    }
    else
    {
        handler->StopReadingMap();
        error = "500 Cannot obtain IPC handler for process '";
        error += procName;
        error += "'\n";
        SendMessage( error );
        return false;
    }

    handler->StopReadingMap();

    return true;
}

bool UIMessageHandler::AbortProcess( const string& procName )
{

    string error;
    set<string>::iterator setItr;

    if ( ( setItr = processNames.find( procName ) ) == processNames.end() )
    {
        error = "500 Process '";
        error += procName;
        error += "' has not been created yet\n";
        SendMessage( error );
        return false;
    }

    ProcessStatusHandler* handler = ProcessStatusHandler::Instance();
    map<UIMessageHandler*, IPCHandler*>::iterator itr;
    handler->BeginReadingMap();
    itr = handler->GetMapRef().find( this );
    if ( itr != handler->GetMapRef().end() )
    {
        if ( (*itr).second->AbortProcess( procName, error ) == false )
        {
            handler->StopReadingMap();
            SendMessage( error );
            return false;
        }
    }
    else
    {
        handler->StopReadingMap();
        error = "500 Cannot obtain IPC handler for process '";
        error += procName;
        error += "'\n";
        SendMessage( error );
        return false;
    }

    handler->StopReadingMap();

    processNames.erase( setItr );

    return true;
}

bool UIMessageHandler::DoneAction( const string& procName, const string& actionName )
{
    return true;
}

bool UIMessageHandler::Authenticate( const string& uname, const string& passwd )
{
#if !defined(_WIN32)
    FILE *password_file;
    struct passwd * uinfo;
    string roleuser;
    string rolename;
    char pwd[13];
    char salt[2];

    /* Open the password file */
    if((password_file = fopen("./peos_passwd", "r")) == NULL) {
        perror("Could not find password file\n");
        SendMessage("500 User could not be verified");
        return false;
    }

    /* Find the user in the password file */
    do {
        if((uinfo = fgetpwent(password_file)) == NULL) {
            SendMessage("500 User could not be verified\n");
            fclose(password_file);
            return false;
        }
    } while(strncmp(uinfo->pw_name, uname.c_str(), sizeof(uname.c_str())) != 0);
    fclose(password_file);    

    /* Check the user's password */
    salt[0] = uinfo->pw_passwd[0];
    salt[1] = uinfo->pw_passwd[1];
    if ( strncmp( uinfo->pw_passwd, crypt( passwd.c_str(), salt ), 13) != 0 ) {
        SendMessage("500 User authorization failed\n");
        return false;
    }

    /* Get the user's roles */
    roles.clear();
    rolename = "guest";  /* Everyone gets this role */
    roles.push_back(rolename);
    ifstream role_stream("./peos_roles");
    while (role_stream) {
        role_stream >> roleuser >> rolename;
        if (roleuser == uname) {
            roles.push_back(rolename);
        }
    }
    role_stream.close();

    /* Check for user's directory - if it's not there then make it */
    DIR* dir = opendir( uname.c_str() );
    if ( dir == NULL ) {
        int success = mkdir( uname.c_str(), 0777 );
        if ( success < 0 && errno != EEXIST ) {
            perror( "mkdir call failed" );
            SendMessage( "500 Making user's working directory error\n" );
	      return false;
        }
    }
    else {
        closedir( dir );
    }

    userName = uname;
    password = passwd;

#endif
    return true;
}

bool UIMessageHandler::CheckPrivilege(const string& model, const string& action)
{
    string role;
    string privilege;
    string acl_file = "./model/" + model + ".acl";

    ifstream acl_stream(acl_file.c_str());
    while (acl_stream) {
        acl_stream >> role >> privilege;
        for(list<string>::const_iterator i = roles.begin(); i != roles.end(); i++) {
            if ( role == *i && privilege == action) {
                acl_stream.close();
                return true;
            }
        }
    }
    acl_stream.close();
    return false;
}
