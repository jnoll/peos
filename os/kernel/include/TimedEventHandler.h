///////////////////////////////////////////////////////////
// TimedEventHandler.h                                   //
// Definitions for the TimedEventHandler class           //
// Developer: Brad McGarvey                              //
// Spring 2000                                           //              
///////////////////////////////////////////////////////////

#ifndef __TIMED_EVENT_HANDLER_H
#define __TIMED_EVENT_HANDLER_H

#include "EventHandler.h"
#include "DataAccessInterface.h"
#include <list>
#include <string>

using namespace std;

struct TimedEvent
{
	int minute;
	int hour;
	int day_of_month;
	int month_of_year;
	int day_of_week;
	string process;
	bool operator==(const TimedEvent &rhs);
};

typedef list<TimedEvent> EventList;

class TimedEventHandler:public EventHandler
{
	public:
		TimedEventHandler(bool repUsed);
		virtual ~TimedEventHandler();
		void Init();
		void Add(const TimedEvent &event);
		void Remove(const TimedEvent &event);
	protected:
	private:
		friend void CheckEvents(int sig);
		static EventList Events;
        	bool repositoryUsed;
        	static DataAccessInterface *dataAccessIF;
};

#define CONFIG_FILE_NAME "TimedEvents"

#endif

