// GDBMIF.cpp: implementation of the GDBMIF class.
//
//////////////////////////////////////////////////////////////////////

#include <gdbm.h>
#include "GDBMIF.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GDBMIF::GDBMIF()
{

}

GDBMIF::~GDBMIF()
{

}

bool GDBMIF::GetProcessName( const string& modelName,
		             const string& userName,
			     string& procName,
			     string& errorMsg )
{
    procName.erase();
    errorMsg.erase();

    GDBM_FILE dbfm, dbfp;
    int suf;
    string path;

    /* open model db */
    path  = "model";
    path += DIRECTORY_SEPARATOR;
    path += modelName;
    path += ".gdbm";
    dbfm = gdbm_open( (char*) path.c_str(), 0, GDBM_READER, 0777, 0 );
    
    if ( dbfm == NULL )
    {
        errorMsg = "500 Model '";
        errorMsg += modelName;
        errorMsg += "' does not exist. Use 'list' command\n";
        return false;
    }
    gdbm_close( dbfm );

    /* find an available suffix. max 999 procs per user*/
    procName = userName;
    procName += DIRECTORY_SEPARATOR;
    procName += modelName;
    char temp[] = ".000";
    suf = 1;
    dbfp = NULL;

    do
    {
        if ( dbfp ) gdbm_close( dbfp );
        sprintf( temp, ".%03d", suf );
        path = procName;
        path += temp;
        dbfp = gdbm_open( (char*) path.c_str(), 0, GDBM_READER, 0777, 0 ); 
        suf++;
    } while ( suf < 999 && dbfp != NULL ); 
    if ( dbfp ) gdbm_close( dbfp );

    if ( suf == 999 )
    {
	procName.erase();
        errorMsg = "500 Maximum number of processes exceeded.\n";
        return false;
    }
    procName = path;
    return true;
}

bool GDBMIF::InitProcessState( const string& procName,
			       const string& parentName,
			       int startPC,
		               string& errorMsg )
{
    struct context_t context;
    GDBM_FILE dbfm;
    GDBM_FILE dbfp;
    GDBM_FILE dbfs;
    int PC = 0;
    datum key;
    datum content;
    string path;
    char act_name[255];
    struct act_t act;

    errorMsg.erase();

    /* open model db - This code added on 4/16/00 by DA */
    /* Need path statement */
    dbfm = gdbm_open( (char*) path.c_str(), 0, GDBM_READER, 0777, 0);
    if ( dbfm == NULL )
    {
       errorMsg = "500 Open model failed\n";
       return false;
    }

    /* create proc db */
    path = procName;
    dbfp = gdbm_open( (char*) path.c_str(), 0, GDBM_WRCREAT, 0777, 0 ); 
    
    if ( dbfp == NULL )
    {
        gdbm_close( dbfm );
        errorMsg = "500 Create proc failed\n";
        return false;
    }

    /* create state db */    
    path += ".state";
    dbfs = gdbm_open( (char*) path.c_str(), 0, GDBM_WRCREAT, 0777, 0 ); 

    if ( dbfs == NULL )
    {
        gdbm_close( dbfm );
        gdbm_close( dbfp );
        errorMsg = "500 Create state failed\n";
        return false;
    }

    /* initialize state information and find first PC */
    PC = 0;
    int counter=0;
    while ( true )
    {
        key.dptr = (char *) &PC;
        key.dsize = sizeof( PC );
        content = gdbm_fetch( dbfm, key );
        return true;
        if ( content.dptr == NULL )
	{
            break;
	}

        if ( strcmp( content.dptr, "start" ) == 0 )
	{
            context.PC = PC;
            break;
        }

        memset( act_name, 0, sizeof( act_name ) );
        sscanf( content.dptr, "%s", act_name );
        free( content.dptr );
        key.dptr = (char *)act_name;
        key.dsize = strlen( act_name ) + 1; 
        act.PC = PC;
        act.state = NEW;
        act.wait = 0;
        content.dptr = (char *) &act;
        content.dsize = sizeof( act );
        gdbm_store( dbfs, key, content, GDBM_REPLACE );
        PC++;
    }
 
    gdbm_close( dbfm );
    gdbm_close( dbfs );
    
    strcpy( context.parent, "" );
    
    if ( startPC != 0 )
    {
        context.PC = startPC;
        strcpy( context.parent, parentName.c_str() );
    }

    /* store context to key 0 */
    context.SP = INT_MAX;
    PC = 0;
    key.dptr = (char *) &PC;
    key.dsize = sizeof( PC );
    content.dptr = (char *) &context;
    content.dsize = sizeof( struct context_t );
    gdbm_store( dbfp, key, content, GDBM_REPLACE );
    
    gdbm_close( dbfp );

    return true;
}

vector<string> GDBMIF::QueryActions( const string& userName, int state )
{
    vector<string> reply;
    DIR* dir;
    struct dirent* ent;
    string path;
    char* ext;
    GDBM_FILE dbfs;
    GDBM_FILE dbfm;
    datum keys;
    datum keym;
    datum content;
    char act_name[255];
    struct act_t act;
    string proc;

    path = userName;
    path += DIRECTORY_SEPARATOR;
    
    dir = opendir( userName.c_str() );
    if ( dir == NULL )
    {
	reply.clear();
        return reply;
    }

    ent = readdir( dir );
    string temp;

    while ( ent )
    { 
        ext = strrchr( ent->d_name, '.' );
        if (ext == NULL) {
            ent = readdir(dir);
            continue;
        }
        
	if ( strcmp( ext, ".state" ) !=0 )
	{
            ent = readdir(dir);
            continue;
        }

        /* strip .state extension */
        proc = ent->d_name;
        int pos =  proc.find_last_of( "." );
        if ( pos != string::npos )
	{
	    proc.erase( pos );
	}

        path = userName;
        path += DIRECTORY_SEPARATOR;
        path += ent->d_name;

        dbfs = gdbm_open( (char*) path.c_str(), 0, GDBM_READER, 0777, 0 );
        cout << (char*) path.c_str() << endl;
        
	if ( dbfs == NULL )
	{
            cerr << "GDBM::QueryActions: open state db error.\n" << endl;
            break;
        }

        /* strip .state extension */
        proc = ent->d_name;
        pos =  proc.find_last_of( "." );
        if ( pos != string::npos )
	{
	    proc.erase( pos );
	}

        path = "model";
        path += DIRECTORY_SEPARATOR;
        path += ent->d_name;
        pos =  path.find_last_of( "." );
        if ( pos != string::npos )
	{
	    path.erase( pos );
	}

	pos =  path.find_last_of( "." );
        if ( pos != string::npos )
	{
	    path.erase( pos );
	}

        path += ".gdbm";

        dbfm = gdbm_open( (char*) path.c_str(), 0, GDBM_READER, 0777, 0 );

        if ( dbfm == NULL )
	{
            cerr << "GDBM::QueryActions: open model db error.\n" << endl;
            break;
        }

        keys = gdbm_firstkey( dbfs );
	string msg;

        while ( keys.dptr )
	{
            content = gdbm_fetch( dbfs, keys );
        
	    if ( content.dptr == NULL )
	    {
                break;
	    }

            memcpy( &act, content.dptr, sizeof( struct act_t ) );
            free( content.dptr );
            
	    if ( act.state == state )
	    {
                keym.dptr = (char *) &act.PC;
                keym.dsize = sizeof( act.PC );
                content = gdbm_fetch( dbfm, keym );
                
		if ( content.dptr != NULL )
		{
                    msg = "100-";
                    msg += proc;
                    msg += " ";
                    msg += content.dptr;
                    msg += "\n";
                    reply.push_back( msg );
                    free(content.dptr);
                } 
		else
		{
                    msg = "100-";
                    msg += proc;
                    msg += " ";
		    msg.append( keys.dptr, keys.dsize );
		    msg += "\n";
                    reply.push_back( msg );
                }
            }

            keys = gdbm_nextkey(  dbfs, keys );
        };

        gdbm_close( dbfm );
        gdbm_close( dbfs );

        ent = readdir( dir );
    };

    closedir(dir);

    return reply;
}

vector<string> GDBMIF::GetCreatedProcs( const string& userName )
{
    vector<string> reply;
    DIR* dir;
    struct dirent* ent;
    string path;
    char* ext;
    string proc;
    
    path = userName;
    path += DIRECTORY_SEPARATOR;
    
    dir = opendir( userName.c_str() );
    if ( dir == NULL )
    {
	reply.clear();
        return reply;
    }

    ent = readdir( dir );
    string temp;

    while ( ent )
    { 
        ext = strrchr(ent->d_name, '.');
        if (ext == NULL) {
            ent = readdir(dir);
            continue;
        }
        
	if ( strcmp( ext, ".state" ) !=0 )
	{
            ent = readdir(dir);
            continue;
        }

        /* strip .state extension */
        proc = ent->d_name;
        int pos =  proc.find_last_of( "." );
        if ( pos != string::npos )
	{
	    proc.erase( pos );
	}

        /* strip .state extension */
        proc = ent->d_name;
        pos =  proc.find_last_of( "." );
        if ( pos != string::npos )
	{
	    proc.erase( pos );
	}

        reply.push_back( proc );

        ent = readdir( dir );
    };

    closedir(dir);
    return reply;
}

vector<string> GDBMIF::ListModels( string& error )
{
    vector<string> ret;
    error.erase();
    DIR*           dir;
    struct dirent* ent;
    char*          ext;
    char           msg[4096];
    dir = opendir( "model" );

    if ( dir == NULL )
    {
        error = "500 Model directory does not exist\n";
        return ret;
    }

    ent = readdir( dir );

    while ( ent )
    {
        ext = strrchr( ent->d_name, '.' );
        /* no extension */
        if (ext == NULL) 
	{
            ent = readdir( dir );
            continue;
        }
        if ( strcmp( ext, ".gdbm" ) != 0 )
	{
            ent = readdir( dir );
            continue;
        }

        ext[0] = 0;
        sprintf( msg, "100-%s\n", ent->d_name );
        ret.push_back( msg );
        ent = readdir( dir );
    };

    return ret;
}
