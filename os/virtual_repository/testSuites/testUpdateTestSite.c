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

#define BUFFER_SIZE 1000

int main( void )
{	
	void setLineData( char *, char *, char * ) ;
	
 	char cwd[BUFFER_SIZE] = { '\0' } ;
 	char oneLine[BUFFER_SIZE] = { '\0' } ;
 	char newLine[BUFFER_SIZE] = { '\0' } ;
 	
	if( getcwd ( cwd , BUFFER_SIZE ) == NULL )
	{
		puts( "error" ) ;
	}
	else
	{
		FILE *oldConfigFile, *newConfigFile ;
		resultList *theLines ;
		
		theLines = NULL ;
		
		oldConfigFile = fopen ( "vr.rc", "r" ) ;
		_assert( __FILE__, __LINE__, oldConfigFile ) ;
	
		while ( !feof( oldConfigFile ) ) 
		{
			fgets ( oneLine, sizeof ( oneLine ), oldConfigFile ) ;
			if( strlen( oneLine ) )
			{
				_debug( __FILE__, __LINE__, 5, "oneLine is %s", oneLine ) ;
				setLineData( oneLine, newLine, cwd ) ;
				_debug( __FILE__, __LINE__, 5, "newLine is %s", newLine ) ;
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
	
	return 0 ;
}

void setLineData( char *theLine, char *newLine, char *cwd )
{
	char *lineType, *testLine ;	
	
	_debug( __FILE__, __LINE__, 5, "cwd is %s", cwd ) ;

	lineType = testLine = NULL ;
	
	testLine = strdup( theLine ) ;
	
	_debug( __FILE__, __LINE__, 5, "theLine is %s", theLine ) ;
	_debug( __FILE__, __LINE__, 5, "testLine is %s", testLine ) ;
	
	lineType = strtok( testLine , "=" ) ;
	_assert( __FILE__, __LINE__, lineType ) ;
	
	_debug( __FILE__, __LINE__, 5, "lineType is %s", lineType ) ;	
					
	if( ( strcmp( "SEARCHDIR", lineType ) == 0 ) )
		sprintf( newLine,"%s%s%s", "SEARCHDIR=", cwd, "\n" ) ;
	else if( ( strcmp( "MAILBOX", lineType ) == 0 ) )
		sprintf( newLine,"%s%s%s%s", "MAILBOX=", cwd, "/mailBox.dat", "\n" ) ;
	else
		strcpy( newLine, theLine ) ;
		
	free( testLine ) ;
}
	

