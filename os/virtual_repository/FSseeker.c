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
char queryDate[11] ;
char queryName[BUFFER_SIZE] ;
time_t filequeryTime;
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
	void getFSQueryDate( char * ) ;
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
				_debug( __FILE__, __LINE__, 5, "attribute is DATE" ) ;
				dateResults = NULL;
				getFSQueryDate( tempQueries -> oneQuery -> myClauses[numClauses].value ) ;
				filequeryTime = parsedate(tempQueries -> oneQuery -> myClauses[numClauses].value,NULL);				
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
	
	return listpointer ;	
}

resultList* attributeID( query *oneQuery, int numClauses )
{
	int isFileQuery( char * ) ;
	
	struct stat statBuffer ;	// a required arguement in unix stat() function
	
	resultList *resultID = NULL ;
	
	if( isFileQuery( oneQuery -> myClauses[numClauses].value ) )
	{		
		char tempPath[BUFFER_SIZE] = {'\0'} ;
		char resultPath[BUFFER_SIZE] = {'\0'} ;
		int numslash = 1 ;
   	
		strcpy( searchFile, ( oneQuery -> myClauses[numClauses].value ) + 5 ) ;
		while( strncmp( searchFile + numslash, "/", 1 ) == 0 )
			numslash++ ;
			
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
			
void attributeDATE( )
{
	int getDate( const char *filename, const struct stat *statptr, int flag ) ;
	
	char searchPath[BUFFER_SIZE] ;	// the value of SEARCHDIR in the configuration file
	getSearchPath( searchPath ) ;	
	
	_debug( __FILE__, __LINE__, 5, "queryDate is %s", queryDate ) ;
	ftw( searchPath, getDate, 5 ) ;
}

void attributeNAME( ) 
{
	int getFile( const char *filename, const struct stat *statptr, int flag );
	
	char searchPath[BUFFER_SIZE] ;	// the value of SEARCHDIR in the configuration file
	getSearchPath( searchPath ) ;	
	
	_debug( __FILE__, __LINE__, 5, "queryName is %s", queryName ) ;	
	ftw( searchPath, getFile, 5 ) ;
}

int getDate( const char *filename, const struct stat *statptr, int flag )
{
	void getFileDate( char * , char * ) ;

	char fileDate[11] ;
		
	switch ( flag )
	{
		case FTW_F : 	_debug( __FILE__, __LINE__, 1, "FTW file is %s", filename ) ;
				_debug( __FILE__, __LINE__, 1, "statptr -> st_mtime is %s", ctime(  &statptr -> st_mtime  ) ) ;
				_debug( __FILE__, __LINE__, 1, "filequeryTime is %s", ctime(  &filequeryTime  ) ) ;
				getFileDate( fileDate, ctime(  &statptr -> st_mtime  ) ) ;
				if( difftime(filequeryTime,parsedate(fileDate,NULL)) == 0 )
					dateResults = addResultItem( dateResults, filename ) ;
								
				break ;
	}
	return 0 ;
}

void getFSQueryDate( char *dateValue )
{
	char *word, *value, *toParse, *year, *month, *day ;
	int numParses ;
	
	numParses = 0 ;
	word = NULL ;
	
	value = ( char * ) malloc( sizeof( char ) ) ;
	toParse = strdup( dateValue ) ;
	
	word = strtok( toParse, "/" ) ;

	while( word != NULL )
	{
		numParses++ ;
		switch( numParses )
		{
			case 1	: 	month = strdup( word ) ;
					break ;
								
			case 2	: 	day = strdup( word ) ;
					break ;
								
			case 3	: 	year = strdup( word ) ;
					break ;
		}
		word = strtok( NULL, "/" ) ;
	}

	sprintf( value,"%s/%s/%s", year, month, day ) ;	
	strcpy( queryDate, value ) ;
	
	free( month ) ;
	free( day ) ;
	free( year ) ;
	free( toParse ) ;
	free( value ) ;	
}

void getFileDate( char *value, char *fileTime )
{
	char* convertMonth( char * ) ;
	char *word, *toParse, *year, *month, *day ;
	int numParses ;
		
	numParses = 0 ;
	word = toParse = NULL ;
	
	_debug( __FILE__, __LINE__, 5, "fileTime is %s", fileTime ) ;
	
	toParse = strtok( fileTime, "\n" ) ;
	if( toParse != NULL )
		word = strtok( toParse, " " ) ;
	

	_debug( __FILE__, __LINE__, 5, "word is %s", word ) ;
	while( word != NULL )
	{
		_debug( __FILE__, __LINE__, 5, "word is %s", word ) ;
		numParses++ ;
		_debug( __FILE__, __LINE__, 5, "numParses is %d", numParses ) ;
		switch( numParses )
		{
			case 2	: 	_debug( __FILE__, __LINE__, 5, "Month is %s", word ) ;
					month = strdup( convertMonth ( word ) ) ;
					_debug( __FILE__, __LINE__, 5, " new Month is %s", month ) ;
					break ;
								
			case 3	: 	_debug( __FILE__, __LINE__, 5, "Date is %s", word ) ;
					day = strdup( word ) ;
					_debug( __FILE__, __LINE__, 5, "new Date is %s", day ) ;
					break ;
								
			case 5	: 	_debug( __FILE__, __LINE__, 5, "Year is %s", word ) ;
					year = strdup( word ) ;
					_debug( __FILE__, __LINE__, 5, "new Year is %s", year ) ;
					break ;
		}
		word = strtok( NULL, " " ) ;
	}
	
	if( strlen( day ) == 1 )
		sprintf( value,"%s/%s/0%s", year, month, day ) ;
	else
		sprintf( value,"%s/%s/%s", year, month, day ) ;
	
	free( month ) ;
	free( day ) ;
	free( year ) ;
		
	_debug( __FILE__, __LINE__, 5, "value is %s", value ) ;
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
 
void getSearchPath( char *searchPath )
{
	int doneSearchPath ;		// boolean-like sentinel for the extracting of searchPath
	FILE *configFile ;		// file pointer to the configuration file
   	char oneLine[BUFFER_SIZE] ;	// one line from the an opened file
   	char *word ;			// a token during string tokenization
   	
   	doneSearchPath = 0 ;
	
   	if( !strlen( searchPath ) )
   	{
      		
      		
      		if( ( configFile = fopen( "vr.rc", "r" ) ) != NULL ||  (configFile = fopen(strcat(getenv("HOME"),"/vr.rc"),"r")) != NULL)
   		{
   			
      			while( fgets( oneLine, BUFFER_SIZE, configFile ) != NULL )
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
  		if ( getcwd( directoryPath, BUFFER_SIZE ) == NULL )
		{
			_debug( __FILE__, __LINE__, 0, "error getting directory path" ) ;
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
	char target[BUFFER_SIZE] ;		// value of the requested filename
	int index = 0 ;			// used in determining the postion of the filename

	switch ( flag )
	{
		case FTW_F : 	position = strrchr(filename, '/' ) ; 
       				_assert( __FILE__, __LINE__, position ) ;
				index = filename - position; 
				
				if( index < 0 )
					index *= -1;

    				strcpy( target, filename + index + 1 ) ;
    				
    				if( strcmp( operatorType, "~" ) == 0 )
    				{	    											
					if( strstr(target,queryName ) != NULL )
					{
						nameResults = addResultItem ( nameResults, filename );
						_debug(__FILE__,__LINE__,2,"adding name result");
					}
				}
				else if( strcmp( operatorType, "EQ" ) == 0 )
				{
					if( strcmp( target, queryName ) == 0 )
					{
						nameResults = addResultItem ( nameResults, filename );
						_debug(__FILE__,__LINE__,2,"adding name result");
					}
				}
				
				break ;
	}
	
	return 0 ;
}

	

