// ProcessStatusHandler.cpp: implementation of the ProcessStatusHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "ProcessStatusHandler.h"
#include "IPCHandler.h"
#include "UIMessageHandler.h"
#include "EventHandler.h"
#include "TimedEventHandler.h"
#include "ProductEventHandler.h"

const int kBacklog = 5;

ProcessStatusHandler* ProcessStatusHandler::instance = NULL;
int ProcessStatusHandler::myPort = -1;
int ProcessStatusHandler::maxSession = 0;
string ProcessStatusHandler::pmlvm = "pmlvm";
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ProcessStatusHandler::ProcessStatusHandler( bool repUsed )
	: processMap(),
          eventHandler( NULL ),
          sessionCount( 0 ),
          sockfd( -1 ),
          repositoryUsed( repUsed )
{

}

ProcessStatusHandler::~ProcessStatusHandler()
{
    delete timedEventHandler;
    delete productEventHandler;
    map<UIMessageHandler*, IPCHandler*>::iterator itr;

    for ( itr = processMap.end(); itr != processMap.begin(); --itr )
    {
        delete (*itr).first;
        delete (*itr).second;
        processMap.erase( itr );
    }
}

void ProcessStatusHandler::Shutdown()
{
    shutdown( sockfd, SHUT_RDWR );
}

void ProcessStatusHandler::Run()
{
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = ntohs( myPort );
#if defined(_WIN32)
    server.sin_addr.S_un.S_addr =  INADDR_ANY;
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD( 2, 2 );
    err = WSAStartup( wVersionRequested, &wsaData );
    if ( err != 0 )
    {
        //Tell the user that we could not find a usable WinSock DLL.
        perror( "WSAStartup call failed" );
        exit( 1 );
    }
    // set up the transport end point
    if ( ( sockfd = socket( AF_INET, SOCK_STREAM, PROTOCOL ) ) == -1 )
    {
        err = WSAGetLastError();
        string errMsg = "socket call failed ";
        switch( err )
        {
        case WSANOTINITIALISED:
            errMsg += "WSANOTINITIALISED";
            break;
        case WSAENETDOWN:
            errMsg += "WSAENETDOWN";
            break;
        case WSAEAFNOSUPPORT:
            errMsg += "WSAEAFNOSUPPORT";
            break;
        case WSAEINPROGRESS:
            errMsg += "WSAEINPROGRESS";
            break;
        case WSAEMFILE:
            errMsg += "WSAEMFILE";
            break;
        case WSAENOBUFS:
            errMsg += "WSAENOBUFS";
            break;
        case WSAEPROTONOSUPPORT:
            errMsg += "WSAEPROTONOSUPPORT";
            break;
        case WSAEPROTOTYPE:
            errMsg += "WSAEPROTOTYPE";
            break;
        case WSAESOCKTNOSUPPORT:
            errMsg += "WSAESOCKTNOSUPPORT";
            break;
        case WSAEINVAL:
            errMsg += "WSAEINVAL";
            break;
        case WSAEFAULT:
            errMsg += "WSAEFAULT";
            break;
        default:
            errMsg += "UNKNOWN";
        }
        perror( errMsg.c_str() );
        exit( 1 );
    }
#else
    server.sin_addr.s_addr = INADDR_ANY;
    if ( ( sockfd = socket( AF_INET, SOCK_STREAM, PROTOCOL ) ) == -1 )
    {
        perror( "socket call failed" );
        exit( 1 );
    }
#endif
    // bind an address to the end point
    if ( bind( sockfd, (struct sockaddr*) &server, sizeof(struct sockaddr_in) ) == -1 )
    {
        perror( "bind call failed" );
        exit( 1 );
    }

    // start listening for incoming connections
    if ( listen( sockfd, kBacklog ) == -1 )
    {
        perror( "listen call failed" );
        exit( 1 );
    }
    timedEventHandler = new TimedEventHandler(repositoryUsed);
    timedEventHandler->Init();
    productEventHandler = new ProductEventHandler(repositoryUsed);
    productEventHandler->Init();
    while( true )
    {
        int newsockfd;

        // accept a connection 
        if ( ( newsockfd = accept( sockfd, NULL, NULL ) ) == -1 )
        {
            //perror("accept call failed");
            continue;
        }

        if ( sessionCount < maxSession )
        {
            IPCHandler*       IPCHand =  new IPCHandler( pmlvm );
            UIMessageHandler* msgHand = new UIMessageHandler( newsockfd );
            BeginWritingMap();
            processMap.insert( make_pair( msgHand, IPCHand ) );
            IPCHand->Init();
            msgHand->Init();
            StopWritingMap();
            if ( eventHandler )
            {
                if ( eventHandler->IsRunning() == false )
                {
                    eventHandler->Init();
                }
            }
            sessionCount++;
        }
        else
        {
            char* msg = "500 Cannot create a session. Maximun number of sessions exists. Closing the connection.\n";
            send( newsockfd, msg, sizeof( msg ), 0 );
            close( newsockfd );
        }
    }
}

void ProcessStatusHandler::Init( int port, int maxSes, const string& vm, bool repUsed )
{
        myPort = port;
        maxSession = maxSes;
        pmlvm      = vm;

	if ( instance == NULL )
	{
		instance = new ProcessStatusHandler( repUsed );
	}
}

ProcessStatusHandler* ProcessStatusHandler::Instance()
{
	return instance;
}

void ProcessStatusHandler::SetEventHandler( EventHandler* handler )
{
    eventHandler = handler;
}

void ProcessStatusHandler::BeginWritingMap()
{
    mapLock.BeginWriting();
}

void ProcessStatusHandler::StopWritingMap()
{
    mapLock.StopWriting();
}

void ProcessStatusHandler::BeginReadingMap()
{
    mapLock.BeginReading();
}

void ProcessStatusHandler::StopReadingMap()
{
    mapLock.StopReading();
}
