/*****************************************************************
File:        main.cpp
Author:      Nikolai Kiachtchenko
Date:        11/1/1999
Description: main function of OS services application.
             Listen on TCP port. Create a thread to handle incoming
             commands when accepting a connection, create a child
             and exec a pmlvm process, create an IPC thread to 
             handle a communication to the pmlvm.
*****************************************************************/
#include "OSDefs.h"
#include "SignalHandler.h"
#include "ProcessStatusHandler.h"
#include "EventHandler.h"

const char* args = "p:m:n:RG";
const int kMaxNumberOfSessions = 1000;
const char* usage = "USAGE: OSServices -p listenPort [-m maxNumberOfSessions(1000)] [-n PMLVMExecutableName] [-R | -G]";

const string kVMName = "pmlvm";

void main( int argc, char** argv )
{
    int listenPort = -1;
    int maxNumberOfSessions = kMaxNumberOfSessions;
    string VMName = kVMName;
    int c;
    bool UseRepositoryIF = false;
#if defined(_WIN32)
    listenPort = 14523;
#else

    while ( ( c = getopt( argc, argv, args ) ) != -1 )
    {
	    switch ( c )
            {
	    case 'p':
	        listenPort = atoi( optarg );
	        break;
	    case 'm':
	        maxNumberOfSessions = atoi( optarg );
                break;
            case 'n':
	        VMName = optarg;
                break;
            case 'G':
                UseRepositoryIF = false;
                break;
            case 'R':
                UseRepositoryIF = true;
                break;
            default:
                cerr << usage << endl;
                exit( 1 );      
	    }
    }
#endif
    if ( listenPort == -1 )
    {
        cerr << "Missing or wrong listener port number, must be greater then 0." << endl;
        cerr << usage << endl;
        exit( 1 );
    }


	ProcessStatusHandler* statusHandler = NULL;
	ProcessStatusHandler::Init( listenPort, maxNumberOfSessions, VMName, UseRepositoryIF );

	statusHandler = ProcessStatusHandler::Instance();

	if ( statusHandler == NULL )
	{
        cerr << "Could not create a Process Status Handler object. Exitting..." << endl;
		exit( 1 );
	}

	SignalHandler::Init( statusHandler );

    EventHandler* eventHand = new EventHandler();

    statusHandler->SetEventHandler( eventHand );

	statusHandler->Run();
}
