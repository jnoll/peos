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

int main( void )
{	
	void callback( int size, resultList *listPointer , int *data ) ;
	void ( *call )( int, resultList *, int * data ) ;
	void setInvalidResult( int, FILE * ) ;
	void setEmptyResult( int, FILE * ) ;
	
	char queryString[1000] ;
	char *testString ;
	int *d, index ;
	queryList *tempQueries ;
	FILE *expectedResultInvalidFile, *testInputInvalid ;

	repos_ctr = 0;
	myQueries = NULL;
	
	setup_email( );	
	call = callback ;
	
	expectedResultInvalidFile = fopen ( "EMAILsearchInvalidExpectedResult.txt", "w" ) ;
	_assert( __FILE__, __LINE__, expectedResultInvalidFile ) ;
	setInvalidResult( 11, expectedResultInvalidFile ) ;
	setEmptyResult( 10, expectedResultInvalidFile ) ;
	fclose( expectedResultInvalidFile ) ;	
	
	testInputInvalid = fopen ( "EMAILsearchInvalid.dat", "r" ) ;
	_assert( __FILE__, __LINE__, testInputInvalid ) ;
	while ( !feof( testInputInvalid ) ) 
	{
		fgets ( queryString, sizeof ( queryString ), testInputInvalid ) ;
		if( strlen( queryString ) )
		{
			query_wait( queryString, call, d ) ;
			queryString[0] = '\0' ;
		}
	}
	fclose( testInputInvalid ) ;

	poll_vr( ) ;

	return 0 ;
}

void callback( int size, resultList *listpointer, int *data )
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

void setEmptyResult( int emptys, FILE *FSexpectedResultEmptyFile )
{
	int i ;
	char emptyString[] = "empty query...\n" ;
	
	for( i = 0 ; i < emptys ; i++ )
		fwrite( emptyString, sizeof( char ), strlen( emptyString ), FSexpectedResultEmptyFile ) ;
		
}
