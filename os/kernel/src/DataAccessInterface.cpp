// DataAccessInterface.cpp: implementation of the DataAccessInterface class.
//
//////////////////////////////////////////////////////////////////////

#include "DataAccessInterface.h"
#include "OSDefs.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DataAccessInterface::DataAccessInterface()
{

}

DataAccessInterface::~DataAccessInterface()
{

}

bool DataAccessInterface::InitProcessState( const string& procName,
                                            const string& parentName,
                                            int startPC,
                                            string& errorMsg )
{
    int startpos = procName.find_last_of(DIRECTORY_SEPARATOR);
    if (startpos == string::npos) startpos = 0;
    int endpos = procName.find_first_of(".", startpos);
    string model = procName.substr(startpos + 1, endpos - startpos - 1);
    return(InitProcessState(model, procName, parentName, startPC, errorMsg));
}
