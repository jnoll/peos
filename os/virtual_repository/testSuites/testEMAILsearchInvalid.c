/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : testEMAILsearchInvalid.c					*
 ************************************************************************/

/************************************************************************
 * Description:	Test the implementation of Virtual Repository for the	*
 		mail box with invalid and empty queries			*
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
	void setEmptyResult( int, FILE * ) ;
	
	char queryString[BUFFER_SIZE] ;
	char *testString ;
	int *d, index, numInvalidQueries ;
	queryList *tempQueries ;
	FILE *expectedResultInvalidFile, *testInputInvalid ;

	repos_ctr = 0;
	myQueries = NULL;
	
	setup_email( );	
	call = callback ;
	
	testInputInvalid = fopen ( "EMAILsearchInvalid.dat", "r" ) ;
	_assert( __FILE__, __LINE__, testInputInvalid ) ;
	
	numInvalidQueries = 0 ;
	while ( !feof( testInputInvalid ) ) 
	{
		fgets ( queryString, sizeof ( queryString ), testInputInvalid ) ;
		if( strlen( queryString ) )
		{
			query_wait( queryString, call, d ) ;
			queryString[0] = '\0' ;
			numInvalidQueries++ ;
		}
	}
	fclose( testInputInvalid ) ;
	
	expectedResultInvalidFile = fopen ( "EMAILsearchInvalidExpectedResult.txt", "w" ) ;
	_assert( __FILE__, __LINE__, expectedResultInvalidFile ) ;
	setInvalidResult( numInvalidQueries, expectedResultInvalidFile ) ;
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
