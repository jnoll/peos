/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : FSseeker.c						*
 ************************************************************************/

#include "form.h"
#include "debug.h"
#include "variables.h"
#include "resultLinkedList.h"
#include "queryLinkedList.h"
#include <ftw.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

query *ftwQuery ;			// will contain the Query data for ftw traversing in getFile function
char searchFile[100] ;			// the value of the query request
static char searchPath[100] ;		// the value of SEARCHDIR in the configuration file
char directoryPath[100] ;		// the path of the local user
int doneDirectoryPath = 0 ;		// boolean-like sentinel for the extracting of directoryPath

/************************************************************************
 * Function:	FSqueryTool						*
 *									*
 * Description:	Taking queries with file: request and searching for 	*
 *		matching filenames in the unix file system.		*
 ************************************************************************/

queryList* FSqueryTool( queryList *listpointer )
{
	int getFile( const char *filename, const struct stat *statptr, int flag ) ;
	void getSearchPath( ) ;
	void getDirectoryPath( ) ;
	int isFileQuery( char * ) ;
	
   	char oneLine[100] ;		// one line from the an opened file
   	char *word ;			// a token during string tokenization
   	struct stat statBuffer ;	// a required arguement in unix stat() function
   	queryList *tempQueries ;	// a pointer to the query list arguement
   	
   	getSearchPath( ) ;
   	getDirectoryPath( ) ;
  	
	tempQueries = listpointer ;

	while( tempQueries != NULL )
	{
		if( isFileQuery( tempQueries -> oneQuery -> myClauses[0].value ) )
		{
			char tempPath[100] = {'\0'} ;
			int numslash = 1 ;
   	
			strcpy( searchFile, ( tempQueries -> oneQuery -> myClauses[0].value ) + 5 ) ;
			
			while( strncmp( searchFile + numslash, "/", 1 ) == 0 )
				numslash++ ;
		
			switch( numslash )
			{
				case 2: 	strcpy( tempPath, directoryPath ) ;
						strcat( tempPath, searchFile + 1 ) ;
						if( strlen( tempPath ) > ( strlen( directoryPath ) + 1 ) )
						{
							if( stat( tempPath, &statBuffer ) == 0 )
							{
								tempQueries -> oneQuery -> results = addResultItem ( tempQueries -> oneQuery -> results, 
														     tempQueries -> oneQuery -> myClauses[0].value ) ;
								tempQueries -> oneQuery -> numFound++ ;
							}
						}
						break ;

		  		case 3:		if( strlen( searchFile ) > ( strlen( directoryPath ) + 3 ) )
						{
							if( stat( searchFile + 2, &statBuffer ) == 0 )
							{
								tempQueries -> oneQuery -> results = addResultItem ( tempQueries -> oneQuery -> results, 
													     	     tempQueries -> oneQuery -> myClauses[0].value ) ;
								tempQueries -> oneQuery -> numFound++ ;
		    					}
		    				}
		    				break ;
		    				
		    		default: 	puts("invalid query...") ;
		    	}
		}
		tempQueries = ( queryList* ) tempQueries -> link ;
	}
	return listpointer ;	
}

/************************************************************************
 * Function:	isFileQuery						*
 *									*
 * Description:	Determines if the query is a file query			*
 *									*
 *		Returns 1 if a file query, otherwise returns 0		*
 ************************************************************************/
 
int isFileQuery( char *value )
{
	char *repository ;		// a token that defines the repository type of the query request
	
	strcpy( searchFile, value ) ;
	repository = strtok( searchFile , ":" ) ;
					
	if( ( strcmp( "file", repository ) == 0 ) )
		return 1 ;
	else
		return 0 ;
}
		
/************************************************************************
 * Function:	getSearchPath						*
 *									*
 * Description:	Gets the pre-configured search path from the		*
 * 		configuration file					*
 ************************************************************************/
 
void getSearchPath( )
{
	int doneSearchPath ;		// boolean-like sentinel for the extracting of searchPath
	FILE *configFile ;		// file pointer to the configuration file
   	char oneLine[100] ;		// one line from the an opened file
   	char *word ;			// a token during string tokenization
   	
   	doneSearchPath = 0 ;
	
   	if( !strlen( searchPath ) )
   	{
      		if( ( configFile = fopen( "config.txt", "r" ) ) != NULL )
   		{
      			while( fgets( oneLine, 100, configFile ) != NULL )
   			{
       				word = strtok( oneLine, "=" ) ;
       				_assert( __FILE__, __LINE__, word ) ;
       				while( !doneSearchPath )
    				{
       					if( strncmp( "SEARCHDIR", word, 9 ) == 0 )
    					{
       						word = strtok( NULL, "\n" ) ;
		       				_assert( __FILE__, __LINE__, word ) ;
   						strcpy( searchPath, word ) ;
						doneSearchPath = 1 ;
					}
				}
    			}
        		fclose( configFile ) ;
  		}
  	}
}

/************************************************************************
 * Function:	getDirectoryPath					*
 *									*
 * Description:	Gets the directory path of the local user		*
 ************************************************************************/
 
void getDirectoryPath( )
{
  	if( !doneDirectoryPath )
  	{
  		if ( getcwd( directoryPath, 100 ) == NULL )
		{
			printf( "error getting directory path...\n" ) ;
			exit( 0 ) ;
		}
		else
			doneDirectoryPath = 1 ;
  	}
}

/************************************************************************
 * Function:	getFile							*
 *									*
 * Description:	Unix function that traverses from the current directory *
 *		recursively while searching for matching filename	*
 ************************************************************************/
     
int getFile( const char *filename, const struct stat *statptr, int flag )
{
	char* position ;		// pointer to the requested filename
	char target[100] ;		// value of the requested filename
	int index = 0 ;			// used in determining the postion of the filename

	switch ( flag )
	{
		case FTW_F : 	position = strrchr(filename, '/' ) ; 
       				_assert( __FILE__, __LINE__, position ) ;
				index = filename - position; 
				
				if( index < 0 )
					index *= -1;

    				strcpy( target, filename + index + 1 ) ;
	    											
				if( strncmp( searchFile, target, strlen( searchFile) ) == 0 )
				{
					ftwQuery -> results = addResultItem ( ftwQuery -> results, filename );
				    	ftwQuery -> numFound++ ;
				}
				break ;
	}
	
	return 0 ;
}

// NAME attribute facility
		/*if( strchr( searchFile, ':' ) == NULL )	
		{
			ftwQuery = tempQueries -> oneQuery ;
			ftw( searchPath, getFile, 5 ) ;
		}*/

	

