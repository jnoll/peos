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
#include "seekerTools.h"
#include "FSseeker.h"
#include <ftw.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

#define BUFFER_SIZE 1000

int doneDirectoryPath = 0 ;		// boolean-like sentinel for the extracting of directoryPath
char operatorType[3] ;
char searchFile[BUFFER_SIZE] ;		// the value of the query request
char queryName[BUFFER_SIZE] ;
time_t fileQueryTime;
char directoryPath[BUFFER_SIZE] ;	 //the path of the local user
resultList *dateResults ;
resultList *nameResults ;

/************************************************************************
 * Function:	FSqueryTool						*
 *									*
 * Description:	Taking queries with file: request and searching for 	*
 *		matching filenames in the unix file system.		*
 ************************************************************************/

queryList* FSqueryTool( queryList *listpointer )
{
	void getDirectoryPath( ) ;
	void attributeDATE( ) ;
	void attributeNAME( ) ;
	resultList* attributeID( query *, int ) ;
	
   	char oneLine[BUFFER_SIZE] ;	// one line from the an opened file
   	char *word ;			// a token during string tokenization
   	int numClauses ;
   	queryList *tempQueries ;	// a pointer to the query list arguement
   	resultList *tempResults ;
   	   	
   	getDirectoryPath( ) ;
  	
	tempQueries = listpointer ;
	tempResults = NULL ;
	
	_debug( __FILE__, __LINE__, 5, "entering FSqueryTool" ) ;
	
	while( tempQueries != NULL )
	{
	   	
	   	_debug( __FILE__, __LINE__, 5, "tempQueries -> oneQuery -> numClauses is %d", tempQueries -> oneQuery -> numClauses ) ;	
		for( numClauses = 0 ; numClauses <= tempQueries -> oneQuery -> numClauses ; numClauses++ )
		{
			_debug( __FILE__, __LINE__, 5, "numClauses count is %d", numClauses ) ;
			if( strcmp( tempQueries -> oneQuery -> myClauses[numClauses].attribute, "ID" ) == 0 )
			{
				_debug( __FILE__, __LINE__, 5, "attribute is ID") ;

				if( numClauses == 0 )
					tempResults = attributeID( tempQueries -> oneQuery, numClauses ) ;
				else
				{
					if( strcmp( tempQueries -> oneQuery -> myClauses[numClauses-1].conjecture, "AND" ) == 0 )
					{
						_debug( __FILE__, __LINE__, 5, "conjecture is AND") ;
						tempResults = andResult( tempResults, attributeID( tempQueries -> oneQuery, numClauses ) ) ;
					}

					if( strcmp( tempQueries -> oneQuery -> myClauses[numClauses-1].conjecture, "OR" ) == 0 )
					{
						_debug( __FILE__, __LINE__, 5, "conjecture is OR") ;
						tempResults = orResult(tempResults, attributeID( tempQueries -> oneQuery, numClauses ) ) ;
					}
				}
			}
	
			if( strcmp( tempQueries -> oneQuery -> myClauses[numClauses].attribute, "DATE" ) == 0 )
			{
				char fileTime[22] = { '\0' } ;
				_debug( __FILE__, __LINE__, 5, "attribute is DATE" ) ;
				dateResults = NULL;
				
				_debug( __FILE__, __LINE__, 5, "oldFileTime is %s", tempQueries -> oneQuery -> myClauses[numClauses].value ) ;
				formatTimeStamp( fileTime, tempQueries -> oneQuery -> myClauses[numClauses].value ) ;
				_debug( __FILE__, __LINE__, 5, "newFileTime is %s", fileTime ) ;
				fileQueryTime = parsedate( fileTime, NULL) ;	
				
				strcpy( operatorType, tempQueries -> oneQuery -> myClauses[numClauses].operator ) ;
				attributeDATE( ) ;
				if( numClauses == 0 )
					tempResults = dateResults ;
				else
				{
					if( strcmp( tempQueries -> oneQuery -> myClauses[numClauses-1].conjecture, "AND" ) == 0 )
					{
						_debug( __FILE__, __LINE__, 5, "conjecture is AND") ;
						tempResults = andResult( tempResults, dateResults ) ;
					}
					if( strcmp( tempQueries -> oneQuery -> myClauses[numClauses-1].conjecture, "OR" ) == 0 )
					{
						_debug( __FILE__, __LINE__, 5, "conjecture is OR") ;
						tempResults = orResult( tempResults, dateResults ) ;
					}
				}
			}

			if( strcmp( tempQueries -> oneQuery -> myClauses[numClauses].attribute, "NAME" ) == 0 )
			{
				_debug( __FILE__, __LINE__, 5, "attribute is NAME") ;
				nameResults = NULL;
				strcpy( operatorType, tempQueries -> oneQuery -> myClauses[numClauses].operator ) ;
				strcpy( queryName, tempQueries -> oneQuery -> myClauses[numClauses].value ) ;
				attributeNAME( ) ;
				if( numClauses == 0 )
					tempResults = nameResults ;
				else
				{
					if( strcmp( tempQueries -> oneQuery -> myClauses[numClauses-1].conjecture, "AND" ) == 0 )
						tempResults = andResult( tempResults, nameResults ) ;
					if( strcmp( tempQueries -> oneQuery -> myClauses[numClauses-1].conjecture, "OR" ) == 0 )
						tempResults = orResult( tempResults, nameResults ) ;
				}
			}
			
		}
	
		while ( tempResults != NULL )
		{
			tempQueries -> oneQuery -> results = addResultItem( tempQueries -> oneQuery -> results, tempResults -> oneResult ) ;
			tempQueries -> oneQuery -> numFound++ ;
			tempResults = ( resultList* ) tempResults -> link ;
		}
		
		tempQueries = ( queryList* ) tempQueries -> link ;
	}
	
	_debug( __FILE__, __LINE__, 5, "exiting FSqueryTool" ) ;
	
	return listpointer ;	
}


/************************************************************************
 * Function:	attributeID						*
 *									*
 * Description:	Returns filenames with matching ID in a linked list.	*
 ************************************************************************/
 
resultList* attributeID( query *oneQuery, int numClauses )
{
	int isFileQuery( char * ) ;
	
	struct stat statBuffer ;	// a required arguement in unix stat() function
	
	resultList *resultID = NULL ;
	
	if( isFileQuery( oneQuery -> myClauses[numClauses].value ) )
	{		
		char tempPath[BUFFER_SIZE] = {'\0'} ;
		char resultPath[BUFFER_SIZE] = {'\0'} ;
		int numslash = 0 ;
   	
		numslash = FSqueryValidator( oneQuery -> myClauses[numClauses].value ) ;
		strcpy( searchFile, ( oneQuery -> myClauses[numClauses].value ) + 5 ) ;
		
		switch( numslash )
		{
			case 2: 	strcpy( tempPath, directoryPath ) ;
					strcat( tempPath, searchFile + 1 ) ;
					if( strlen( tempPath ) > ( strlen( directoryPath ) + 1 ) )
						if( stat( tempPath, &statBuffer ) == 0 )
						{
							sprintf( resultPath, "file://%s", tempPath ) ;
							resultID = addResultItem( resultID, resultPath ) ;
						}					
					break ;

  			case 3:		if( strlen( searchFile ) > 3 )
						if( stat( searchFile + 2, &statBuffer ) == 0 )
							resultID = addResultItem( resultID, oneQuery -> myClauses[numClauses].value ) ;
					break ;
	
			default: 	oneQuery -> removeTag = 1 ;
		}
	}
	
	return resultID ;
}

/************************************************************************
 * Function:	attributeDATE						*
 *									*
 * Description:	Walks through the tree structure for matching DATES.	*
 ************************************************************************/
			
void attributeDATE( )
{
	int getDate( const char *filename, const struct stat *statptr, int flag ) ;	
	
	char searchPath[BUFFER_SIZE] = { '\0' } ;	// the value of SEARCHDIR in the configuration file
	getPath( searchPath,"SEARCHDIR" ) ;	
	
	_debug( __FILE__, __LINE__, 5, "searchPath is %s", searchPath ) ;
	ftw( searchPath, getDate, 5 ) ;
}

/************************************************************************
 * Function:	attributeNAME						*
 *									*
 * Description:	Walks through the tree structure for matching NAMES.	*
 ************************************************************************/

void attributeNAME( ) 
{
	int getFile( const char *filename, const struct stat *statptr, int flag ) ;
	
	char searchPath[BUFFER_SIZE] ;	// the value of SEARCHDIR in the configuration file
	getPath( searchPath,"SEARCHDIR" ) ;	
	
	_debug( __FILE__, __LINE__, 5, "searchPath is %s", searchPath) ;	

	
	_debug( __FILE__, __LINE__, 5, "attributeNAME( ) : queryName is %s", queryName ) ;
	_debug( __FILE__, __LINE__, 5, "attributeNAME( ) : searchPath is %s", searchPath ) ;	
	

	ftw( searchPath, getFile, 5 ) ;
}

/************************************************************************
 * Function:	getFile							*
 *									*
 * Description:	Returns filenames of NAMES that matches their		*
 *		corresponding operators of ~, EQ.			*
 ************************************************************************/
     
int getFile( const char *filename, const struct stat *statptr, int flag )
{
	char *position ;				// pointer to the requested filename
	char target[BUFFER_SIZE] = { '\0' } ;		// value of the requested filename
	char theFileName[BUFFER_SIZE] = { '\0' } ;
	int index = 0 ;					// used in determining the postion of the filename

	switch ( flag )
	{
		case FTW_F : 	position = strrchr(filename, '/' ) ; 
       				_assert( __FILE__, __LINE__, position ) ;
				index = filename - position; 
				
				if( index < 0 )
					index *= -1;

    				strcpy( target, filename + index + 1 ) ;
    				
    				strcat( theFileName, "file://" ) ;
    				
    				if( strcmp( operatorType, "~" ) == 0 )
    				{	    	
    					_debug( __FILE__, __LINE__, 5, "queryName is %s", queryName ) ;
    					_debug( __FILE__, __LINE__, 5, "target is %s", target ) ;										
					if( strstr( target, queryName ) != NULL )
					{
						strcat( theFileName, filename ) ;
						nameResults = addResultItem ( nameResults, theFileName );
					}
						
				}
				else if( strcmp( operatorType, "EQ" ) == 0 )
				{
					if( strcmp( target, queryName ) == 0 )
					{
						strcat( theFileName, filename ) ;
						nameResults = addResultItem ( nameResults, theFileName );
					}
				}

				_debug( __FILE__, __LINE__, 5, "getFile theFileName is %s", theFileName ) ;
				
				break ;
	}
	
	
	return 0 ;
}

/************************************************************************
 * Function:	getDate							*
 *									*
 * Description:	Returns filenames of DATEs that matches their		*
 *		corresponding operators of EQ, LT, LE, GT, GE.		*
 ************************************************************************/

int getDate( const char *filename, const struct stat *statptr, int flag )
{
	void getFileTimeStamp( char * , char * ) ;

	char theFileName[BUFFER_SIZE] = { '\0' } ;
	char fileDate[20] = { '\0' } ;
	
	time_t fileDateTime ;
		
	switch ( flag )
	{
		case FTW_F : 	getFileTimeStamp( fileDate, ctime(  &statptr -> st_mtime  ) ) ;
				_debug( __FILE__, __LINE__, 5, "fileDate is %s", fileDate ) ;
				
				fileDateTime = parsedate( fileDate, NULL ) ;
				
				_debug( __FILE__, __LINE__, 5, "FTW file is %s", filename ) ;
				_debug( __FILE__, __LINE__, 5, "statptr -> st_mtime is %s", ctime(  &statptr -> st_mtime  ) ) ;
				_debug( __FILE__, __LINE__, 5, "fileQueryTime is %s", ctime(  &fileQueryTime ) ) ;
				_debug( __FILE__, __LINE__, 5, "fileDateTime is %s", ctime(  &fileDateTime ) ) ;
								
				strcat( theFileName, "file://" ) ;
				
				if( strcmp( operatorType, "EQ" ) == 0 )
    				{	    											
					if( difftime( fileDateTime, fileQueryTime ) == 0 )
					{
						strcat( theFileName, filename ) ;
						dateResults = addResultItem( dateResults, theFileName ) ;
					}
				}
				else if( strcmp( operatorType, "LT" ) == 0 )
				{
					if( difftime( fileDateTime, fileQueryTime ) < 0 )
					{
						strcat( theFileName, filename ) ;
						dateResults = addResultItem( dateResults, theFileName ) ;
					}
				}
				else if( strcmp( operatorType, "LE" ) == 0 )
				{
					if( ( difftime( fileDateTime, fileQueryTime ) == 0 ) ||
					    ( difftime( fileDateTime, fileQueryTime ) < 0 ) )
					{
						strcat( theFileName, filename ) ;
						dateResults = addResultItem( dateResults, theFileName ) ;
					}
				}
				else if( strcmp( operatorType, "GT" ) == 0 )
				{
					if( difftime( fileDateTime, fileQueryTime ) > 0 )
					{
						strcat( theFileName, filename ) ;
						dateResults = addResultItem( dateResults, theFileName ) ;
					}
				}
				else if( strcmp( operatorType, "GE" ) == 0 )
				{
					if( ( difftime( fileDateTime, fileQueryTime ) == 0 ) ||
					    ( difftime( fileDateTime, fileQueryTime ) > 0 ) )
					{
						strcat( theFileName, filename ) ;
						dateResults = addResultItem( dateResults, theFileName ) ;
					}
				}
				
				_debug( __FILE__, __LINE__, 5, "getDate theFileName is %s", theFileName ) ;				
				
				break ;
	}
	
	return 0 ;
}

/************************************************************************
 * Function:	getFileTimeStamp					*
 *									*
 * Description:	Sets the time stamp of a file into yyyy/mm/dd hh:mm:ss 	*
 *		format							*
 ************************************************************************/

void getFileTimeStamp( char *newFileTime, char *oldFileTime )
{
	char* convertMonth( char * ) ;
	char *word, *toParse, *year, *month, *day, *time ;
	int numParses ;
		
	numParses = 0 ;
	word = toParse = NULL ;
	
	_debug( __FILE__, __LINE__, 5, "oldFileTime is %s", oldFileTime ) ;
	
	toParse = strtok( oldFileTime, "\n" ) ;
	if( toParse != NULL )
		word = strtok( toParse, " " ) ;
	
	while( word != NULL )
	{
		_debug( __FILE__, __LINE__, 5, "word is %s, numParses is %d", word, numParses ) ;
		switch( numParses )
		{
			case 1	: 	month = strdup( convertMonth ( word ) ) ;
					break ;
								
			case 2	: 	day = strdup( word ) ;
					break ;
			
			case 3	: 	time = strdup( word ) ;
					break ;
								
			case 4	: 	year = strdup( word ) ;
					break ;
		}
		numParses++ ;
		word = strtok( NULL, " " ) ;		
	}
	
	_debug( __FILE__, __LINE__, 5, "year is %s", year ) ;
	_debug( __FILE__, __LINE__, 5, "month is %s", month ) ;
	_debug( __FILE__, __LINE__, 5, "date is %s", day ) ;
	_debug( __FILE__, __LINE__, 5, "time is %s", time ) ;
	
	if( strlen( day ) == 1 )
		sprintf( newFileTime,"%s/%s/0%s %s", year, month, day, time ) ;
	else
		sprintf( newFileTime,"%s/%s/%s %s", year, month, day, time ) ;
	
	free( month ) ;
	free( day ) ;
	free( time ) ;
	free( year ) ;	
		
	_debug( __FILE__, __LINE__, 5, "newFileTime is %s", newFileTime ) ;
}

/************************************************************************
 * Function:	convertMonth						*
 *									*
 * Description:	Function coverts the month to numerical string value	*
 *		and returns the corresponding string.	 		*
 ************************************************************************/

char* convertMonth( char *theMonth )
{
	if( strcmp( theMonth, "Jan" ) == 0 )
		return "01" ;
	else if( strcmp( theMonth, "Feb" ) == 0 )
		return "02" ;
	else if( strcmp( theMonth, "Mar" ) == 0 )
		return "03" ;
	else if( strcmp( theMonth, "Apr" ) == 0 )
		return "04" ;
	else if( strcmp( theMonth, "May" ) == 0 )
		return "05" ;
	else if( strcmp( theMonth, "Jun" ) == 0 )
		return "06" ;
	else if( strcmp( theMonth, "Jul" ) == 0 )
		return "07" ;
	else if( strcmp( theMonth, "Aug" ) == 0 )
		return "08" ;
	else if( strcmp( theMonth, "Sep" ) == 0 )
		return "09" ;
	else if( strcmp( theMonth, "Oct" ) == 0 )
		return "10" ;
	else if( strcmp( theMonth, "Nov" ) == 0 )
		return "11" ;
	else if( strcmp( theMonth, "Dec" ) == 0 )
		return "12" ;
	else
		return NULL ;
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
	_assert( __FILE__, __LINE__, repository ) ;
	
	_debug( __FILE__, __LINE__, 5, "repository is %s", repository ) ;
	_debug( __FILE__, __LINE__, 5, "strcmp( ) is %d", strcmp( "file", repository ) ) ;		
					
	if( ( strcmp( "file", repository ) == 0 ) )
		return 1 ;
	else
		return 0 ;
}

/************************************************************************
 * Function:	FSqueryValidator					*
 *									*
 * Description:	Determines if the query is a file query			*
 *									*
 *		Returns 1 if a file query, otherwise returns 0		*
 ************************************************************************/
 
int FSqueryValidator( char *value )
{
	char testValue[BUFFER_SIZE] = {'\0'} ;
	int numslash = 1 ;
	int slashResult = 0 ;
   	
	strcpy( testValue, value + 5 ) ;
	while( strncmp( testValue + numslash, "/", 1 ) == 0 )
		numslash++ ;
		
	_debug( __FILE__, __LINE__, 5, "testValue is %s", testValue ) ;	
	_debug( __FILE__, __LINE__, 5, "testValue + numslash is %s", testValue + numslash ) ;	
	switch( numslash )
	{
		case 2: 	_debug( __FILE__, __LINE__, 5, "strlen( testValue + numslash ) is %d", strlen( testValue + numslash ) ) ;	
				if( strlen( testValue + numslash ) )
					slashResult = 2 ;
				else
					slashResult = 0 ;
				break ;

		case 3:		_debug( __FILE__, __LINE__, 5, "strlen( testValue + numslash ) is %d", strlen( testValue + numslash ) ) ;	
				if( strlen( testValue + numslash ) )
					slashResult = 3 ;
				else
					slashResult = 0 ;
				break ;
				break ;
	
		default: 	slashResult = 0 ;
				break ;
	}
	return slashResult ;
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
  		if ( getcwd( directoryPath, BUFFER_SIZE ) == NULL )
		{
			_debug( __FILE__, __LINE__, 5, "error getting directory path" ) ;
			exit( 0 ) ;
		}
		else
			doneDirectoryPath = 1 ;
  	}
}



	

