// EventHandler.cpp: implementation of the EventHandler class.
//
//////////////////////////////////////////////////////////////////////
#include "OSDefs.h"
#include "EventHandler.h"
#include "ProcessStatusHandler.h"
#include "UIMessageHandler.h"
#include "IPCHandler.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

EventHandler::EventHandler()
  : OSThread()
{

}

EventHandler::~EventHandler()
{
    this->Stop();
}

void EventHandler::Main()
{
    ProcessStatusHandler* handler = ProcessStatusHandler::Instance();
    bool condition = true;

    while ( condition )
    {
        handler->BeginWritingMap();
        map<UIMessageHandler*, IPCHandler*>::iterator itr;
        for ( itr = handler->GetMapRef().end(); itr != handler->GetMapRef().begin(); --itr )
        {
            if ( (*itr).first->IsActive() == false
              || (*itr).second->IsActive() == false )
            {
                delete (*itr).first;
                delete (*itr).second;
                handler->GetMapRef().erase( itr );
            }
        }
        handler->StopWritingMap();

        myLock.BeginReading();
        condition = ( exit == false ) ? true : false;
        myLock.StopReading();

        if ( condition )
        {
            SchedYield();
        }
    }
}

void EventHandler::Init()
{
    this->Start();
}

bool EventHandler::IsRunning()
{
    bool ret;
    myLock.BeginReading();
    ret = exit;
    myLock.StopReading();
    return ret;
}
