/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : testFSsearchEnd2End.c					*
 ************************************************************************/

/************************************************************************
 * Description:	Test the implementation of Virtual Repository for the	*
 		Unix file system with valid and found queries		*
 ************************************************************************/

#include "form.h"
#include "debug.h"
#include "variables.h"
#include "vrepo.h"
#include "setup_repositories.h"
#include "queryLinkedList.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define BUFFER_SIZE 1000

int main( void )
{	
	void callback( int size, resultList *listPointer , int *data ) ;
	void ( *call )( int, resultList *, int * data ) ;
	void setExpectedResult( ) ;
	void setTestData( queryList * ) ;
	
	char queryString[BUFFER_SIZE] = { '\0' } ;
	int *d ;
	FILE *sampleFile ;

	repos_ctr = 0;
	myQueries = NULL;
	
	setup_fs( );	
	call = callback ;

	sampleFile = fopen ( "FSsearchEnd2End.dat", "r" ) ;
	_assert( __FILE__, __LINE__, sampleFile ) ;
	
	while ( !feof( sampleFile ) ) 
	{
		fgets ( queryString, sizeof ( queryString ), sampleFile ) ;
		if( strlen( queryString ) )
		{
			_debug( __FILE__, __LINE__, 5, "query_wait() with %s", queryString ) ;
			query_wait( queryString, call, d ) ;
			queryString[0] = '\0' ;
		}
	}
	fclose( sampleFile ) ;
	
	setTestData( myQueries ) ;
	setExpectedResult( ) ;	
	
	poll_vr( ) ;

	return 0 ;
}

void callback( int size, resultList *listpointer, int *data )
{	
	printResultList( listpointer ) ;	
}

void setExpectedResult ( ) 
{
	char cwd[BUFFER_SIZE] = { '\0' } ;
					
	if( getcwd ( cwd , BUFFER_SIZE ) == NULL )
		_debug( __FILE__, __LINE__, 5, "error in getcwd" ) ;
	else
	{
		FILE *expectedResultFile ;
		char oneResult[BUFFER_SIZE] = { '\0' } ;
		char expectedString[BUFFER_SIZE] = { '\0' } ;
		
		expectedResultFile = fopen ( "FSsearchEnd2EndExpectedResult.txt", "w" ) ;
		_assert( __FILE__, __LINE__, expectedResultFile ) ;
	
		// test for ID attribute
	
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testfile1.c\n\n" ) ;
		strcat( expectedString, oneResult ) ;
	
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/FSsearchEnd2End.dat\n\n" ) ;
		strcat( expectedString, oneResult ) ;

		fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
		expectedString[0] = '\0' ;
	
		// test for AND conjecture - ID && DATE( EQ, LT, GT )
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testfile1.c\n\n" ) ;
		strcat( expectedString, oneResult ) ;
		strcat( expectedString, oneResult ) ;
		strcat( expectedString, oneResult ) ;
		
		fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
		expectedString[0] = '\0' ;
	
		// test for AND conjecture - ID && NAME( EQ, ~ )
	
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testfile1.c\n\n" ) ;
		strcat( expectedString, oneResult ) ;
		strcat( expectedString, oneResult ) ;
	
		fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;		
	
		fclose( expectedResultFile ) ;	
	}
}

void setTestData( queryList *listpointer )
{
	queryList *tempQueries ;
	int numClauses ;
	
	tempQueries = listpointer ;
	
	while( tempQueries != NULL )
	{
		char testString[BUFFER_SIZE] = { '\0' } ;		
		char valueString[BUFFER_SIZE] = { '\0' } ;
		
		for( numClauses = 0 ; numClauses <= tempQueries -> oneQuery -> numClauses ; numClauses++ )
		{
			strcpy( valueString, tempQueries -> oneQuery -> myClauses[numClauses].value ) ;
			
			if( strcmp( tempQueries -> oneQuery -> myClauses[numClauses].attribute, "ID" ) == 0 )
			{
				if( ( strstr( valueString, "file:///" ) ) != NULL ) 
				{
					char cwd[BUFFER_SIZE] = { '\0' } ;
					
					if( getcwd ( cwd , BUFFER_SIZE ) == NULL )
						_debug( __FILE__, __LINE__, 5, "error in getcwd" ) ;
					else
					{		
						char *token, *value ;
						char tempQuery[BUFFER_SIZE] = { '\0' } ;
							
						strcpy( tempQuery, valueString ) ;
						_debug( __FILE__, __LINE__, 5, "tempQuery is %s", tempQuery ) ;
						
						token = strtok( tempQuery, ":" ) ;
						_assert( __FILE__, __LINE__, token  ) ;
			
						strcat( testString, token ) ;
						strcat( testString, "://" ) ;
						strcat( testString, cwd ) ;
						
						value = strpbrk( valueString, ":" ) ;
						_assert( __FILE__, __LINE__, value  ) ;
						
						strcat( testString, value + 3 ) ;						
					}
				}
				else
					strcpy( testString, valueString ) ;
				
				free( tempQueries -> oneQuery -> myClauses[numClauses].value ) ;
				tempQueries -> oneQuery -> myClauses[numClauses].value = strdup( testString ) ;	
			}
		}
		tempQueries = ( queryList* ) tempQueries -> link ;
	}
}
