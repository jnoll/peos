// GDBMIF.h: interface for the GDBMIF class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_GDBMIF_H)
#define _GDBMIF_H

#include "OSDefs.h"
#include "DataAccessInterface.h"

class GDBMIF : public DataAccessInterface  
{
public:
	GDBMIF();
	virtual ~GDBMIF();

	bool  GetProcessName( const string& modelName,
		              const string& userName,
			      string& procName,
			      string& errorMsg );

	bool  InitProcessState( const string& modelName,
				const string& procName,
			        const string& parentName,
			        int startPC,
		                string& errorMsg );

	vector<string> QueryActions( const string& userName, int state );

        vector<string> GetCreatedProcs( const string& userName );
        vector<string> ListModels( string& error );

};

#endif // !defined(_GDBMIF_H)
