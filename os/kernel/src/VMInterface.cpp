// VMInterface.cpp: implementation of the VMInterface class.
//
//////////////////////////////////////////////////////////////////////

#include "VMInterface.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VMInterface::VMInterface( const string& path )
    : VMPath( path )
{

}

VMInterface::~VMInterface()
{

}

int VMInterface::StartNewAction( const string& proc, const string& action )
{
    int numberOfArgs = 5;
    char** arguments;
    char* application;
    int i;

    arguments = new char*[numberOfArgs + 1];
    for ( i = 0; i < numberOfArgs; i++ )
    {
        arguments[i] = new char[MAX_PATH_LEN];
        memset( arguments[i], '\0', MAX_PATH_LEN - 1 );
    }
    
    // Terminate the argument list.
    arguments[numberOfArgs] = new char;
    *arguments[numberOfArgs] = '\0';

    application = new char[MAX_PATH_LEN];
    memset( application, '\0', MAX_PATH_LEN );
    strncpy( application, VMPath.c_str(), MAX_PATH_LEN - 1 );
    if ( ( arguments[0] = strrchr( application, '/' ) ) != NULL )
    {
        arguments[0]++;
    }

    strcpy( arguments[1], "-p" );
    strncpy( arguments[2], proc.c_str(), MAX_PATH_LEN - 1 );
    strcpy( arguments[3], "-a" );
    strncpy( arguments[4], action.c_str(), MAX_PATH_LEN - 1 );

    int ret = StartVM( application, arguments );
    
    delete [] application;

    for ( i = 0; i < ( numberOfArgs + 1 ); i++ )
    {
        delete [] arguments[i];
    }

    delete [] arguments;

    return ret;
}

int VMInterface::AbortProcess( const string& proc )
{
    int numberOfArgs = 5;
    char** arguments;
    char* application;
    int i;

    arguments = new char*[numberOfArgs + 1];
    for ( i = 0; i < numberOfArgs; i++ )
    {
        arguments[i] = new char[MAX_PATH_LEN];
        memset( arguments[i], '\0', MAX_PATH_LEN - 1 );
    }
    
    // Terminate the argument list.
    arguments[numberOfArgs] = new char;
    *arguments[numberOfArgs] = '\0';

    application = new char[MAX_PATH_LEN];
    memset( application, '\0', MAX_PATH_LEN );
    strncpy( application, VMPath.c_str(), MAX_PATH_LEN - 1 );
    if ( ( arguments[0] = strrchr( application, '/' ) ) != NULL )
    {
        arguments[0]++;
    }

    strcpy( arguments[1], "-p" );
    strncpy( arguments[2], proc.c_str(), MAX_PATH_LEN - 1 );
    strcpy( arguments[3], "-e" );
    char temp[12];
    memset( temp, '\0', 12 );
    sprintf( temp, "%d", ABORT );
    strcpy( arguments[4], temp );
    int ret = StartVM( application, arguments );
    
    delete [] application;

    for ( i = 0; i < ( numberOfArgs + 1 ); i++ )
    {
        delete [] arguments[i];
    }

    delete [] arguments;

    return ret;
}

int VMInterface::ResumeFromWaitOnAction( const string& proc, const string& action, Event event )
{
    int numberOfArgs = 7;
    char** arguments;
    char* application;
    int i;

    arguments = new char*[numberOfArgs + 1];
    for ( i = 0; i < numberOfArgs; i++ )
    {
        arguments[i] = new char[MAX_PATH_LEN];
        memset( arguments[i], '\0', MAX_PATH_LEN - 1 );
    }
    
    // Terminate the argument list.
    arguments[numberOfArgs] = new char;
    *arguments[numberOfArgs] = '\0';

    application = new char[MAX_PATH_LEN];
    memset( application, '\0', MAX_PATH_LEN );
    strncpy( application, VMPath.c_str(), MAX_PATH_LEN - 1 );
    if ( ( arguments[0] = strrchr( application, '/' ) ) != NULL )
    {
        arguments[0]++;
    }

    strcpy( arguments[1], "-p" );
    strncpy( arguments[2], proc.c_str(), MAX_PATH_LEN - 1 );
    strcpy( arguments[3], "-e" );
    char temp[12];
    memset( temp, '\0', 12 );
    switch( event )
    {
    case NEW:
        sprintf( temp, "%d", NEW );
        break;
    case READY:
        sprintf( temp, "%d", READY );
        break;
    case DONE:
        sprintf( temp, "%d", DONE );
        break;
    case SUSPEND:
        sprintf( temp, "%d", SUSPEND );
        break;
    case ABORT:
        sprintf( temp, "%d", ABORT );
        break;
    case RUN:
        sprintf( temp, "%d", RUN );
        break;
    default:
        sprintf( temp, "%d", NONE );
        break;
    }
    strcpy( arguments[4], temp );
    strcpy( arguments[5], "-o" );
    strncpy( arguments[6], action.c_str(), MAX_PATH_LEN - 1 );

    int ret = StartVM( application, arguments );
    
    delete [] application;

    for ( i = 0; i < ( numberOfArgs + 1 ); i++ )
    {
        delete [] arguments[i];
    }

    delete [] arguments;

    return ret;
}

int VMInterface::ResumeFromWaitForChild( const string& proc, int childExitStatus )
{
    int numberOfArgs = 5;
    char** arguments;
    char* application;
    int i = 0;

    arguments = new char*[numberOfArgs + 1];
    for ( i = 0; i < numberOfArgs; i++ )
    {
        arguments[i] = new char[MAX_PATH_LEN];
        memset( arguments[i], '\0', MAX_PATH_LEN - 1 );
    }
    
    // Terminate the argument list.
    arguments[numberOfArgs] = new char;
    *arguments[numberOfArgs] = '\0';

    application = new char[MAX_PATH_LEN];
    memset( application, '\0', MAX_PATH_LEN );
    strncpy( application, VMPath.c_str(), MAX_PATH_LEN - 1 );
    if ( ( arguments[0] = strrchr( application, '/' ) ) != NULL )
    {
        arguments[0]++;
    }

    strcpy( arguments[1], "-p" );
    strncpy( arguments[2], proc.c_str(), MAX_PATH_LEN - 1 );
    strcpy( arguments[3], "-c" );
    char temp[12];
    memset( temp, '\0', 12 );
    sprintf( temp, "%d", childExitStatus );
    strcpy( arguments[4], temp );
    int ret = StartVM( application, arguments );
    delete [] application;

    for ( i = 0; i < ( numberOfArgs + 1 ); i++ )
    {
        delete [] arguments[i];
    }
    delete [] arguments;
    return ret;
}

int VMInterface::StartFromLine( const string& proc, int lineNumber )
{
    int numberOfArgs = 5;
    char** arguments;
    char* application;
    int i;

    arguments = new char*[numberOfArgs + 1];
    for ( i = 0; i < numberOfArgs; i++ )
    {
        arguments[i] = new char[MAX_PATH_LEN];
        memset( arguments[i], '\0', MAX_PATH_LEN - 1 );
    }
    
    // Terminate the argument list.
    arguments[numberOfArgs] = new char;
    *arguments[numberOfArgs] = '\0';

    application = new char[MAX_PATH_LEN];
    memset( application, '\0', MAX_PATH_LEN );
    strncpy( application, VMPath.c_str(), MAX_PATH_LEN - 1 );
    if ( ( arguments[0] = strrchr( application, '/' ) ) != NULL )
    {
        arguments[0]++;
    }

    strcpy( arguments[1], "-p" );
    strncpy( arguments[2], proc.c_str(), MAX_PATH_LEN - 1 );
    strcpy( arguments[3], "-l" );
    char temp[12];
    memset( temp, '\0', 12 );
    sprintf( temp, "%d", lineNumber );
    strcpy( arguments[4], temp );
    int ret = StartVM( application, arguments );
    delete [] application;

    for ( i = 0; i < ( numberOfArgs + 1 ); i++ )
    {
        delete [] arguments[i];
    }
    delete [] arguments;
    return ret;
}

int VMInterface::WaitOnSelectQuery( const string& proc, const string& query )
{
    int FDs[2] = { -1, -1 };
#if defined(_WIN32)

#else
    if ( pipe( FDs ) < 0 )
    {
        perror( "pipe call failed" );
        return -1;
    }

    int pid = fork();
    
    if ( pid < 0 )
    {
        perror( "pipe call failed" );
        close( FDs[0] );
        close( FDs[1] );
        return -1;
    }
    else if ( pid > 0 )      // parent
    {
        close( FDs[1] );     // we are not writing to pmlvm in this release.
        return FDs[0];
    }
    else                    // child, our pmlvm.
    {
        close( FDs[0] );  // we are not reading from OSServices in this release.

    	int numberOfArgs = 5;
    	char** arguments;
    	char* application;
    	int i;

    	arguments = new char*[numberOfArgs + 1];
  	for ( i = 0; i < numberOfArgs; i++ )
    	{
       		arguments[i] = new char[MAX_PATH_LEN];
        	memset( arguments[i], '\0', MAX_PATH_LEN - 1 );
    	}
    
    	// Terminate the argument list.
    	arguments[numberOfArgs] = new char;
    	*arguments[numberOfArgs] = '\0';

    	application = new char[MAX_PATH_LEN];
    	memset( application, '\0', MAX_PATH_LEN );
    	strncpy( application, VMPath.c_str(), MAX_PATH_LEN - 1 );
    	if ( ( arguments[0] = strrchr( application, '/' ) ) != NULL )
    	{
        	arguments[0]++;
    	}

    	strcpy( arguments[1], "-p" );
    	strncpy( arguments[2], proc.c_str(), MAX_PATH_LEN - 1 );
    	strcpy( arguments[3], "-q" );
    	char temp[12];
	strcpy(temp,"temp.c");
    	strcpy( arguments[4], temp );
	//////////////////////////////////////////////////
	//	Do Query and Wait for Success Here
	//////////////////////////////////////////////////
        // Redirect stdout to the pipe.
        if ( FDs[1] != STDOUT_FILENO )
        {
            if ( dup2( FDs[1], STDOUT_FILENO ) != STDOUT_FILENO )
            {
                perror( "dup2 call failed" );
                exit( 1 );
            }

            close( FDs[1] );

            if ( execv( application, arguments ) < 0 )
            {
                perror( "execv call failed" );
                exit( 1 );
            }
        }
    }
#endif
    return FDs[1];
}

int VMInterface::StartVM( char* path, char** commandLine )
{
    int FDs[2] = { -1, -1 };
#if defined(_WIN32)

#else
    if ( pipe( FDs ) < 0 )
    {
        perror( "pipe call failed" );
        return -1;
    }

    int pid = fork();
    
    if ( pid < 0 )
    {
        perror( "pipe call failed" );
        close( FDs[0] );
        close( FDs[1] );
        return -1;
    }
    else if ( pid > 0 )      // parent
    {
        close( FDs[1] );     // we are not writing to pmlvm in this release.
        return FDs[0];
    }
    else                    // child, our pmlvm.
    {
        close( FDs[0] );  // we are not reading from OSServices in this release.

        // Redirect stdout to the pipe.
        if ( FDs[1] != STDOUT_FILENO )
        {
            if ( dup2( FDs[1], STDOUT_FILENO ) != STDOUT_FILENO )
            {
                perror( "dup2 call failed" );
                exit( 1 );
            }

            close( FDs[1] );

            if ( execv( path, commandLine ) < 0 )
            {
                perror( "execv call failed" );
                exit( 1 );
            }
        }
    }
#endif
    return FDs[1];
}
