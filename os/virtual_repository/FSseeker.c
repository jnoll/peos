/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : FSseeker.c						*
 ************************************************************************/

#include "form.h"
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
	
	int doneSearchPath ;		// boolean-like sentinel for the extracting of searchPath
	FILE *configFile ;		// file pointer to the configuration file
   	char oneLine[100] ;		// one line from the an opened file
   	char *word ;			// a token during string tokenization
   	char *repository ;		// a token that defines the repository type of the query request
   	struct stat statBuffer ;	// a required arguement in unix stat() function
   	queryList *tempQueries ;	// a pointer to the query list arguement
   	
   	// get the current search path from the configuration file
   	doneSearchPath = 0 ;
   	if( !strlen( searchPath ) )
   	{
      		if( (configFile = fopen( "config.txt", "r" ) ) != NULL )
   		{
      			while( fgets( oneLine, 100, configFile ) != NULL)
   			{
       				word = strtok( oneLine, "=" ) ;
       				while( !doneSearchPath )
    				{
       					if( strncmp( "SEARCHDIR", word, 9 ) == 0 )
    					{
       						word = strtok( NULL, "\n" ) ;
   						strcpy( searchPath, word ) ;
						doneSearchPath = 1 ;
					}
				}
    			}
        		fclose( configFile ) ;
  		}
  	}
  	
  	// get the current path of the local user
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

	tempQueries = listpointer ;

	while( tempQueries != NULL )
	{
		
		strcpy( searchFile, tempQueries -> oneQuery -> myClauses[0].value ) ;
		
		// NAME attribute facility
		/*if( strchr( searchFile, ':' ) == NULL )	
		{
			ftwQuery = tempQueries -> oneQuery ;
			ftw( searchPath, getFile, 5 ) ;
		}*/

		repository = strtok( searchFile , ":" ) ;
					
		if( strncmp( "file", repository, 4 ) == 0 )
		{
			strcpy( searchFile, strpbrk( tempQueries -> oneQuery -> myClauses[0].value, "/" ) ) ;
			if( strchr( searchFile + 2, '/' ) != NULL )
			{
				// executing file:/// query request
				if( strlen( searchFile ) > ( strlen( directoryPath ) + 3 ) )
				{
					if( stat( searchFile + 1, &statBuffer ) == 0 )
					{
						tempQueries -> oneQuery -> results = addResultItem ( tempQueries -> oneQuery -> results, 
												     tempQueries -> oneQuery -> myClauses[0].value ) ;
				    		tempQueries -> oneQuery -> numFound++ ;
		    			}
		    		}
		    	}
			else
			{
				char tempPath[100] ;
				
				// executing file:// query request			
				if( strlen( searchFile ) > 2 )
				{
					strcpy( tempPath, directoryPath ) ;
					strcat( tempPath, searchFile + 1 ) ;
					if( stat( tempPath, &statBuffer ) == 0 )
					{
						tempQueries -> oneQuery -> results = addResultItem ( tempQueries -> oneQuery -> results, 
												     tempQueries -> oneQuery -> myClauses[0].value ) ;
						tempQueries -> oneQuery -> numFound++ ;
					}
				}
			}
		}

		tempQueries = ( queryList* ) tempQueries -> link ;
	}
	return listpointer ;	
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

	

