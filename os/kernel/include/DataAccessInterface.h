// DataAccessInterface.h: interface for the DataAccessInterface class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_DATAACCESSINTERFACE_H)
#define _DATAACCESSINTERFACE_H

#include "OSDefs.h"

class DataAccessInterface  
{
public:
	virtual bool  GetProcessName( const string& modelName,
		                      const string& userName,
				      string& procName,
				      string& errorMsg ) = 0;
	virtual bool  InitProcessState( const string& procName,
			                const string& parentName,
			                int startPC,
		                        string& errorMsg ) = 0;
	virtual vector<string> QueryActions( const string& userName, int state ) = 0;

        virtual vector<string> GetCreatedProcs( const string& userName ) = 0;
        virtual vector<string> ListModels(  string& error ) = 0;

	DataAccessInterface();
	virtual ~DataAccessInterface();

};

#endif // !defined(_DATAACCESSINTERFACE_H)
