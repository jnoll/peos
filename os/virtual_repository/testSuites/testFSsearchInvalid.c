/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : testFSsearchInvalid.c					*
 ************************************************************************/

/************************************************************************
 * Description:	Test the implementation of Virtual Repository for the	*
 		Unix file system with invalid queries			*
 ************************************************************************/

#include "form.h"
#include "debug.h"
#include "variables.h"
#include "vrepo.h"
#include "setup_repositories.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define BUFFER_SIZE 1000

int main( void )
{	
	void callback( int size, resultList *listPointer , void *data ) ;
	void ( *call )( int, resultList *, void * data ) ;
	void setInvalidResult( int, FILE * ) ;
	
	char queryString[BUFFER_SIZE] = { '\0' } ;
	char *testString ;
	int *d, index, numQueries ;
	queryList *tempQueries ;
	FILE *expectedResultInvalidFile, *testInputInvalid ;

	repos_ctr = 0;
	myQueries = NULL;
	
	setup_fs( );	
	call = callback ;
	
	testInputInvalid = fopen ( "FSsearchInvalid.dat", "r" ) ;
	_assert( __FILE__, __LINE__, testInputInvalid ) ;
	numQueries = 0 ;
	while ( !feof( testInputInvalid ) ) 
	{
		fgets ( queryString, sizeof ( queryString ), testInputInvalid ) ;
		if( strlen( queryString ) )
		{
			query_wait( queryString, call, d ) ;
			queryString[0] = '\0' ;
			numQueries++ ;
		}
	}
	fclose( testInputInvalid ) ;
	
	expectedResultInvalidFile = fopen ( "FSsearchInvalidExpectedResult.txt", "w" ) ;
	_assert( __FILE__, __LINE__, expectedResultInvalidFile ) ;
	setInvalidResult( numQueries, expectedResultInvalidFile ) ;
	fclose( expectedResultInvalidFile ) ;

	poll_vr( ) ;

	return 0 ;
}

void callback( int size, resultList *listpointer, void *data )
{	
	printResultList( listpointer ) ;	
}

void setInvalidResult( int invalids, FILE *expectedResultInvalidFile )
{
	int i ;
	char invalidString[] = "invalid query...\n" ;
	
	for( i = 0 ; i < invalids ; i++ )
		fwrite( invalidString, sizeof( char ), strlen( invalidString ), expectedResultInvalidFile ) ;		
}

