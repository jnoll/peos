/////////////////////////////////////////////////////
// ProductEventHandler.h                           //
// Definitions for the ProductEventHandler class   //
// Developer: Brad McGarvey                        //
// Spring 2000                                     //
/////////////////////////////////////////////////////

#ifndef __PRODUCT_EVENT_HANDLER_H
#define __PRODUCT_EVENT_HANDLER_H

#include "EventHandler.h"
#include "DataAccessInterface.h"
#include <list>
#include <string>

using namespace std;

//structure for events
enum ProductEvents {peCreated, peModified, peDeleted};

class ProductEvent
{
	public:
    ProductEvents action;
    string product;
	string process;
	bool operator==(const ProductEvent &rhs) const;
};

typedef list<ProductEvent> ProductEventList;

class ProductEventHandler:public EventHandler
{
	public:
		ProductEventHandler(bool repUsed);
		virtual ~ProductEventHandler();
		void Init();
		void Add(const ProductEvent &event);
		void Remove(const ProductEvent &event);
        void ReportProductEvent(ProductEvents event, const string &product);
	protected:
		void Main();
	private:
		ProductEventList Events;
        bool repositoryUsed;
        DataAccessInterface *dataAccessIF;
};


#endif
