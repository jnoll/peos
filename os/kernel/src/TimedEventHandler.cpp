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
#include <unistd.h>
#include <time.h>


//Change this line to change the location or name of the config file. 
#define CONFIG_FILE_NAME "../share/TimedEvents" 


TimedEventList TimedEventHandler::Events;
DataAccessInterface* TimedEventHandler::dataAccessIF = NULL;

bool TimedEvent::operator==(const TimedEvent &rhs) const
{
	return (minute == rhs.minute && hour == rhs.hour
        && day_of_month == rhs.day_of_month
        && month_of_year == rhs.month_of_year
		&& day_of_week == rhs.day_of_week
        && process == rhs.process);
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
	//Turn off alarm if on
    alarm(0);
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
        //parse the config file for events
        while (!configFile.eof())
        {
            getline(configFile, s);
            if (s.size() > 0)
            {
                try
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
                    p = strtok(NULL, " \t");
                    if (!p || *p != '\"')
                        err = true;
                    else
                    {
                        ++p;
                        char *pe = p;
                        while (*pe && *pe!='\"')
                            ++pe;
                        *pe = '\0';
                        e.process = p;
                    }
                    if (err)
                        cerr << "Invalid TimedEvent entry: " + s;
                    else
                        Add(e);
                }
                catch (...)
                {
                    cerr << "Invalid TimedEvent entry: " + s;
                }
            }
        }
    }
    //Set an alarm for 1 minute.
	alarm(60);
	Start();
}


//This is the callback function for the SIGALRM signal
void TimedEventHandler::CheckEvents(int sig)
{
	if (sig != SIGALRM)
		return;
    string error;
    string procName;
    //get current time
    time_t currentTime = time(NULL);
    //break out time into minutes, hours etc.
    tm *theTime = localtime(&currentTime);
    //Check for expired events
	for (TimedEventList::const_iterator i = Events.begin(); i != Events.end(); ++i)
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
        //Use the DataAccessInterface to create and initialize a process
        if (!dataAccessIF->GetProcessName((*i).process, "", procName, error))
            cerr << "TimedEventHandler can't create process: " + (*i).process << endl;
        else if (!dataAccessIF->InitProcessState(procName, "", 0, error))
            cerr << "TimedEventHandler can't initialize process: " + (*i).process << endl;
	}
    //Register handler again since Linux goes back to
    //default handler after a signal is received
	signal(SIGALRM, CheckEvents);
    //check again in 1 minute
	alarm(60);
}

void TimedEventHandler::Add(const TimedEvent &e)
{
	Events.push_back(e);
}

void TimedEventHandler::Remove(const TimedEvent &e)
{
    Events.remove(e);
}

void TimedEventHandler::Main()
{
	bool terminated = false;
	while (!terminated)
	{
		terminated = exit;
		if (!terminated)
			SchedYield();
	}
}

