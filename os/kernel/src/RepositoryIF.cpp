// RepositoryIF.cpp: implementation of the RepositoryIF class.
//
//////////////////////////////////////////////////////////////////////

#include "RepositoryIF.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RepositoryIF::RepositoryIF()
{

}

RepositoryIF::~RepositoryIF()
{

}

bool RepositoryIF::GetProcessName( const string& modelName,
		                   const string& userName,
				   string& procName,
				   string& errorMsg )
{
    return false;
}

bool RepositoryIF::InitProcessState( const string& procName,
			             const string& parentName,
			             int startPC,
		                     string& errorMsg )
{
    return false;
}

vector<string> RepositoryIF::QueryActions( const string& userName, int state )
{
    vector<string> ret;
    return ret;
}

vector<string> RepositoryIF::GetCreatedProcs( const string& userName )
{
    vector<string> ret;
    return ret;
}

vector<string> RepositoryIF::ListModels( string& error )
{
    vector<string> ret;
    return ret;
}