/* 
**	Senior Design Project - PEOS Virtual Repository
**	Author : TASK4ONE
**	Filename : test_searchInvalid.c
*/

#include "form.h"
#include "variables.h"
#include "vrepo.h"
#include "setup_repositories.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

int main( int argc, char * argv[] )
{	
	void callback( int size, resultList *listPointer , int *data ) ;
	void ( *call )( int, resultList *, int * data ) ;
	void setInvalidResult( int, FILE * ) ;
	void setExpectedResult ( char *, FILE * )  ;
	void setTestData ( char *, FILE * ) ;
	
	char queryString[1000] ;
	char *testString ;
	int *d, index ;
	queryList *tempQueries ;
	FILE *expectedResultInvalidFile, *testInputInvalid ;

	repos_ctr = 0;
	myQueries = NULL;
	
	setup_fs( );	
	call = callback ;
	
	expectedResultInvalidFile = fopen ( "expectedResultInvalid.txt", "w" ) ;
	setInvalidResult( 5, expectedResultInvalidFile ) ;
	fclose( expectedResultInvalidFile ) ;	
	
	testInputInvalid = fopen ( "testInvalid.dat", "r" ) ;
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

