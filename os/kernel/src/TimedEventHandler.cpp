///////////////////////////////////////////////////////////
// TimedEventHandler.cpp                                 //
// Implimentation for the TimedEventHandler class        //
// Developer: Brad Mcgarvey                              //
// Spring 2000                                           //
///////////////////////////////////////////////////////////

#include "TimedEventHandler.h"
#include "GDBMIF.h"
#include "RepositoryIF.h"
#include <signal.h>
#include <iostream.h>
#include <fstream.h>
#include <sys/time.h>
#include <time.h>

EventList TimedEventHandler::Events;
DataAccessInterface* TimedEventHandler::dataAccessIF = NULL;

bool TimedEvent::operator==(const TimedEvent &rhs)
{
	return (minute == rhs.minute && hour == rhs.hour 
		&& day_of_month == rhs.day_of_month && month_of_year ==
rhs.month_of_year
		&& day_of_week == rhs.day_of_week && process == rhs.process);
}


TimedEventHandler::TimedEventHandler(bool repUsed):EventHandler()
{
    if (repUsed)
        dataAccessIF = new RepositoryIF;
    else
        dataAccessIF = new GDBMIF;
    //register SIGALRM signal handler
    signal(SIGALRM, CheckEvents);
}

TimedEventHandler::~TimedEventHandler()
{
    //restore default signal handler
    signal(SIGALRM, SIG_DFL);
    delete dataAccessIF;
}

void TimedEventHandler::Init()
{
    Events.clear();
    ifstream configFile(CONFIG_FILE_NAME);
    if (configFile)
    {
        string s;
        char buff[512];
        TimedEvent e;
        bool err;
        while (!configFile.eof())
        {
            getline(configFile, s);
            if (s.size() > 0)
            {
                err = false;
                strcpy(buff, s.c_str());
                char *p;
                int n = 0;
                p = strtok(buff, " \t");
                if (p)
                    e.minute = (*p == '*' ? -1 : atoi(p));
                else
                    err = true;
                p = strtok(NULL, " \t");
                if (p)
                    e.hour = (*p == '*' ? -1 : atoi(p));
                else
                    err = true;
                p = strtok(NULL, " \t");
                if (p)
                    e.day_of_month = (*p == '*' ? -1 : atoi(p));
                else
                    err = true;
                p = strtok(NULL, " \t");
                if (p)
                    e.month_of_year = (*p == '*' ? -1 : atoi(p));
                else
                    err = true;
                p = strtok(NULL, " \t");
                if (p)
                    e.day_of_week = (*p == '*' ? -1 : atoi(p));
                else
                    err = true;
                p = strtok(NULL, "\"");
                if (p)
                    e.process = p;
                else
                    err = true;
                if (err)
                    cerr << "Invalid TimedEvent entry: " + s;
                else
                    Add(e);
            }
        }
    }
    EventHandler::Init();
	itimerval itval;
	itval.it_interval.tv_sec = 60;
	itval.it_interval.tv_usec = 0;
	setitimer(ITIMER_REAL, &itval, NULL);
}


void CheckEvents(int sig)
{
    string error;
    string procName;
    time_t currentTime = time(NULL);
    tm *theTime = localtime(&currentTime);
	for (EventList::const_iterator i = TimedEventHandler::Events.begin()
		; i != TimedEventHandler::Events.end(); ++i)
	{
		if ((*i).minute != -1 && (*i).minute != theTime->tm_min)
			continue;
		if ((*i).hour != -1 && (*i).hour != theTime->tm_hour)
			continue;
		if ((*i).day_of_month != -1 && (*i).day_of_month != theTime->tm_mday)
			continue;
		if ((*i).month_of_year != -1 && (*i).month_of_year - 1 != theTime->tm_mon)
			continue;
		//since tm.w_day is days from Sunday (range is 0 - 6) convert to
		//1 to 7 where 1 is Monday
		int dow = theTime->tm_wday == 0 ? 7 : theTime->tm_wday;
		if ((*i).day_of_week != -1 && (*i).day_of_week != dow)
			continue;
		//got a match
		//create process
	cout << "Creating process " + (*i).process << endl;
        if (!TimedEventHandler::dataAccessIF->GetProcessName((*i).process, "",
procName, error))             cerr << "TimedEventHandler can't create process
" + (*i).process + " : " + error << endl;         else if
(!TimedEventHandler::dataAccessIF->InitProcessState(procName, "", 0, error))   
         cerr << "TimedEventHandler can't create process " + (*i).process + "
: " + error << endl; 	} 
	signal(SIGALRM, CheckEvents);
}

void TimedEventHandler::Add(const TimedEvent &e)
{
	Events.push_back(e);
}

void TimedEventHandler::Remove(const TimedEvent &e)
{
    Events.remove(e);
}

