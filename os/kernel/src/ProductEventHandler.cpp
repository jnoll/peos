///////////////////////////////////////////////////////////
// ProductEventHandler.cpp                               //
// Implimentation for the ProductEventHandler class      //
// Developer: Brad Mcgarvey                              //
// Spring 2000                                           //
///////////////////////////////////////////////////////////

#include "ProductEventHandler.h"
#include "GDBMIF.h"
#include "RepositoryIF.h"
#include <iostream.h>
#include <fstream.h>


//Change this line to change the location or name of the config file. 
#define CONFIG_FILE_NAME "../share/ProductEvents" 
 

bool ProductEvent::operator==(const ProductEvent &rhs) const 
{
	return (action == rhs.action && product == rhs.product
		&& process == rhs.process);
}


ProductEventHandler::ProductEventHandler(bool repUsed):EventHandler()
{
    if (repUsed)
        dataAccessIF = new RepositoryIF;
    else
        dataAccessIF = new GDBMIF;
}

ProductEventHandler::~ProductEventHandler()
{
    delete dataAccessIF;
}

void ProductEventHandler::Init()
{
    Events.clear();
    ifstream configFile(CONFIG_FILE_NAME);
    if (configFile)
    {
        string s;
        ProductEvent event;
        //parse the config file for events
        while (!configFile.eof())
        {
            getline(configFile, s);
            if (s.size() > 0)
            {
                try
                {
                    int b = 0, e;
                    while (b < s.size() && isspace(s[b]))
                        ++b;
                    if (s.find("created", b) == b)
			event.action = peCreated;
			else if (s.find("modified", b) == b)
				event.action = peModified;
			else if (s.find("deleted", b) == b)
				event.action = peDeleted;
			else
				throw (0);
			b = s.find('\"');
			e = s.find('\"', b + 1);
			if (b < 0 || e < 0)
				throw (0);
			event.product = s.substr(b + 1, e - b - 1);
			b = s.find('\"', e + 1);
			e = s.find('\"', b + 1);
			if (b < 0 || e < 0)
				throw (0);
			event.process = s.substr(b + 1, e - b - 1);
			Add(event);
                }
                catch(...)
                {
                    cerr << "Invalid ProductEvent entry: " + s;
                }
            }
        }
    }
}


void ProductEventHandler::Add(const ProductEvent &e) 
{ 
	Events.push_back(e); 
} 
 
void ProductEventHandler::Remove(const ProductEvent &e) 
{ 
    Events.remove(e); 
} 
 
void ProductEventHandler::Main() 
{ 
	bool terminated = false; 
	while (!terminated) 
	{ 
		terminated = exit; 
		if (!terminated) 
			SchedYield(); 
	} 
}


void ProductEventHandler::ReportProductEvent(ProductEvents event, const string &product)
{
	string procName;
	string error;
	for (ProductEventList::const_iterator i = Events.begin()
		; i!= Events.end(); ++i)
	{
		if ((*i).action == event && (*i).product == product)
		{
			//Use the DataAccessInterface to create and initialize a process 
        		if (!dataAccessIF->GetProcessName((*i).process, "", procName, error))
              			cerr << "ProductEventHandler can't create process: " + (*i).process << endl; 
        		else if (!dataAccessIF->InitProcessState(procName, "", 0, error)) 
            			cerr << "ProductEventHandler can't initialize process: " + (*i).process << endl; 		
		}
	}
}
