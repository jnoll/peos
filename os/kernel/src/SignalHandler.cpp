// SignalHandler.cpp: implementation of the SignalHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "SignalHandler.h"
#include "ProcessStatusHandler.h"

SignalHandler* SignalHandler::instance = NULL;
ProcessStatusHandler*	SignalHandler::statusHandler = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SignalHandler::SignalHandler()
{
#if !defined(_WIN32)
	signal(SIGCHLD, SIG_IGN);
        signal(SIGPIPE, HandleSignal);
#endif
	signal(SIGINT, HandleSignal);
        signal(SIGTERM, HandleSignal);
}

SignalHandler::~SignalHandler()
{

}

void SignalHandler::Init( ProcessStatusHandler* handler )
{
	statusHandler = handler;

	if ( instance )
	{
		delete instance;
		instance = NULL;
	}

	instance = new SignalHandler();
}

void SignalHandler::HandleSignal( int sig )
{
	switch( sig )
	{
		case SIGINT:
			cerr << "Process interrupted by user. Exitting..." << endl;
			if ( statusHandler )
			{
				statusHandler->Shutdown();
			}
			exit( 0 );
#if !defined(_WIN32)
		case SIGPIPE:
			cerr << "Broken connection detected. Continue..." << endl;
			break;
#endif
		case SIGTERM:
			cerr << "Somebody shutdown OS services. Terminating..." << endl;
			if ( statusHandler )
			{
				statusHandler->Shutdown();
			}
			exit( 0 );
	}

}

