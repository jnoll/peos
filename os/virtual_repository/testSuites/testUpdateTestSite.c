/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : testUpdateTestSite.c					*
 ************************************************************************/

/************************************************************************
 * Description:	Test the implementation of Virtual Repository for the	*
 		Unix file system with valid and found queries		*
 ************************************************************************/

#include "form.h"
#include "debug.h"
#include "resultLinkedList.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <utime.h>

#define BUFFER_SIZE 1000

int main( void )
{	
	void setTimeValue( char * ) ;
	void getConfiguartionData( char *, char * ) ;
	
 	char cwd[BUFFER_SIZE] = { '\0' } ;
 	char searchDir[BUFFER_SIZE] = { '\0' } ;
 	
	if( getcwd ( cwd , BUFFER_SIZE ) == NULL )
	{
		puts( "error" ) ;
	}
	else
	{
		getConfiguartionData( cwd, searchDir ) ;
		_debug( __FILE__, __LINE__, 5, "cwd is %s", cwd ) ;
		_debug( __FILE__, __LINE__, 5, "searchDir is %s", searchDir ) ;
		
		setTimeValue( searchDir ) ;
	}

	return 0 ;
}

void setFileTime( char *testFile, time_t *theTime )
{
	struct stat statBuffer ;
	struct utimbuf newFileTime ;
				
	if( stat( testFile, &statBuffer ) == 0 )
	{
		_debug( __FILE__, __LINE__, 5, "testFile %s is good", testFile ) ;
		_debug( __FILE__, __LINE__, 5, "statBuffer.st_atime is %s", ctime(  &statBuffer.st_atime  ) ) ;
		_debug( __FILE__, __LINE__, 5, "statBuffer.st_mtime is %s", ctime(  &statBuffer.st_mtime  ) ) ;
		_debug( __FILE__, __LINE__, 5, "theTime is %s", ctime( theTime ) ) ;
				
		if( difftime( *theTime, statBuffer.st_atime ) != 0 )
		{
			newFileTime.actime = *theTime ;
			newFileTime.modtime = *theTime ;
			
			_debug( __FILE__, __LINE__, 5, "newFileTime.actime is %s", ctime(  &newFileTime.actime ) ) ;
			_debug( __FILE__, __LINE__, 5, "newFileTime.modtime is %s", ctime(  &newFileTime.modtime ) ) ;
		
			if( utime( testFile, &newFileTime ) == 0 )
			{
				if( stat( testFile, &statBuffer ) == 0 )
				{
					_debug( __FILE__, __LINE__, 5, "testFile %s is new good", testFile ) ;
					_debug( __FILE__, __LINE__, 5, "statBuffer.st_atime is %s", ctime(  &statBuffer.st_atime  ) ) ;
					_debug( __FILE__, __LINE__, 5, "statBuffer.st_mtime is %s", ctime(  &statBuffer.st_mtime  ) ) ;
				}
				else
					_debug( __FILE__, __LINE__, 5, "stat testFile %s error", testFile ) ;
			}
			else
				_debug( __FILE__, __LINE__, 5, "utime testFile %s error", testFile ) ;
		}
	}
	else
		_debug( __FILE__, __LINE__, 5, "stat testFile %s error", testFile ) ;
}

void setTimeValue( char *searchDir )
{
	void setFileTime( char *, time_t * ) ;
	
	time_t *newOldTime, *newOlderTime, *newOldestTime, *newExactTime ;
		
	char olderTime[]  = "01:00:00 2002/01/01" ;
	char oldestTime[] = "01:00:00 2001/01/01" ;
	char exactTime[] = "01:00:01 2002/01/01" ;
	char testFile[BUFFER_SIZE] = { '\0' } ;  
  	  	
	newOldTime = ( time_t * ) malloc( sizeof( time_t ) ) ;	
	newOlderTime = ( time_t * ) malloc( sizeof( time_t ) ) ; 
	newOldestTime = ( time_t * ) malloc( sizeof( time_t ) ) ;
	newExactTime = ( time_t * ) malloc( sizeof( time_t ) ) ;
	
  	*newOldTime = time( NULL ) ;
	*newOlderTime = parsedate( olderTime, NULL ) ;
	*newOldestTime = parsedate( oldestTime, NULL ) ;
	*newExactTime = parsedate( exactTime, NULL ) ;

	_debug( __FILE__, __LINE__, 5, "newOldTime is %s", ctime( newOldTime ) ) ;
	_debug( __FILE__, __LINE__, 5, "newOlderTime is %s", ctime( newOlderTime ) ) ;
	_debug( __FILE__, __LINE__, 5, "newOldestTime is %s", ctime( newOldestTime ) ) ;
	_debug( __FILE__, __LINE__, 5, "newExactTime is %s", ctime( newExactTime ) ) ;

	sprintf( testFile, "%s/%s", searchDir, "testTIME.txt" ) ;
	setFileTime( testFile, newExactTime ) ;
	
	sprintf( testFile, "%s/%s", searchDir, "testID1.c" ) ;
	setFileTime( testFile, newOldestTime ) ;
	
	sprintf( testFile, "%s/%s", searchDir, "testDATE1.c" ) ;
	setFileTime( testFile, newOldestTime ) ;
	
	sprintf( testFile, "%s/%s", searchDir, "testNAME1.c" ) ;
	setFileTime( testFile, newOldestTime ) ;	
	
	sprintf( testFile, "%s/%s", searchDir, "testID2.c" ) ;
	setFileTime( testFile, newOlderTime ) ;
	
	sprintf( testFile, "%s/%s", searchDir, "testDATE2.c" ) ;
	setFileTime( testFile, newOlderTime ) ;
	
	sprintf( testFile, "%s/%s", searchDir, "testNAME2.c" ) ;
	setFileTime( testFile, newOlderTime ) ;
	
	sprintf( testFile, "%s/%s", searchDir, "testID3.c" ) ;
	setFileTime( testFile, newOldTime ) ;
	
	sprintf( testFile, "%s/%s", searchDir, "testDATE3.c" ) ;
	setFileTime( testFile, newOldTime ) ;
	
	sprintf( testFile, "%s/%s", searchDir, "testNAME3.c" ) ;
	setFileTime( testFile, newOldTime ) ;
}

void getConfiguartionData( char *cwd, char *searchDir )
{		
	void setConfiguartionData( char *, char *, char *, char * ) ;
		
	FILE *oldConfigFile, *newConfigFile ;
	resultList *theLines ;
	char oneLine[BUFFER_SIZE] = { '\0' } ;
 	char newLine[BUFFER_SIZE] = { '\0' } ;	
	
	theLines = NULL ;
		
	oldConfigFile = fopen ( "vr.rc", "r" ) ;
	_assert( __FILE__, __LINE__, oldConfigFile ) ;
	
	while ( !feof( oldConfigFile ) ) 
	{
		fgets ( oneLine, sizeof ( oneLine ), oldConfigFile ) ;
		if( strlen( oneLine ) )
		{
			setConfiguartionData( oneLine, newLine, cwd, searchDir ) ;
			theLines = addResultItem( theLines,  newLine ) ;
			oneLine[0] = '\0' ;
		}
	}
	fclose ( oldConfigFile ) ;
		
	newConfigFile = fopen ( "vr.rc", "w" ) ;
	_assert( __FILE__, __LINE__, oldConfigFile ) ;
	
	while ( theLines != NULL )
	{
		fwrite( theLines -> oneResult, sizeof( char ), strlen( theLines -> oneResult ), newConfigFile ) ;
		theLines = ( resultList* ) theLines -> link ;
	}
	fclose ( newConfigFile ) ;		
}

void setConfiguartionData( char *theLine, char *newLine, char *cwd, char *searchDir )
{
	char *lineType, *testLine ;
	char fileTestSite[] = "/testDir" ;
	
	_debug( __FILE__, __LINE__, 5, "cwd is %s", cwd ) ;

	lineType = testLine = NULL ;
	
	testLine = strdup( theLine ) ;
	
	lineType = strtok( testLine , "=" ) ;
	_assert( __FILE__, __LINE__, lineType ) ;
	
	if( ( strcmp( "SEARCHDIR", lineType ) == 0 ) )
	{
		sprintf( newLine,"%s%s%s\n", "SEARCHDIR=", cwd, fileTestSite ) ;
		strcat( searchDir, cwd ) ;
		strcat( searchDir, fileTestSite ) ;
	}
	else if( ( strcmp( "MAILBOX", lineType ) == 0 ) )
		sprintf( newLine,"%s%s/%s\n", "MAILBOX=", cwd, "mailBox.dat" ) ;
	else
		strcpy( newLine, theLine ) ;
		
	free( testLine ) ;
}
	

