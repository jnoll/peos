// RepositoryIF.h: interface for the RepositoryIF class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_REPOSITORYIF_H)
#define _REPOSITORYIF_H

#include "DataAccessInterface.h"

class RepositoryIF : public DataAccessInterface  
{
public:
	RepositoryIF();
	virtual ~RepositoryIF();

	bool  GetProcessName( const string& modelName,
		                      const string& userName,
				      string& procName,
				      string& errorMsg );

	bool  InitProcessState( const string& procName,
			                const string& parentName,
			                int startPC,
		                        string& errorMsg );

	vector<string> QueryActions( const string& userName, int state );

        vector<string> GetCreatedProcs( const string& userName );
        vector<string> ListModels( string& error );
};

#endif // !defined(_REPOSITORYIF_H)
