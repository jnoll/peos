/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : testEMAILsearchEmpty.c					*
 ************************************************************************/

/************************************************************************
 * Description:	Test the implementation of Virtual Repository for the	*
 		mail box with empty queries				*
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
	void setEmptyResult( int, FILE * ) ;
	
	char queryString[BUFFER_SIZE] = { '\0' } ;
	char *testString ;
	int *d, index, numEmptyQueries ;
	queryList *tempQueries ;
	FILE *expectedResultEmptyFile, *testInputEmpty ;

	repos_ctr = 0;
	myQueries = NULL;
	
	setup_email( );	
	call = callback ;
	
	testInputEmpty = fopen ( "EMAILsearchEmpty.dat", "r" ) ;
	_assert( __FILE__, __LINE__, testInputEmpty ) ;
	
	numEmptyQueries = 0 ;
	while ( !feof( testInputEmpty ) ) 
	{
		fgets ( queryString, sizeof ( queryString ), testInputEmpty ) ;
		if( strlen( queryString ) )
		{
			query_wait( queryString, call, d ) ;
			queryString[0] = '\0' ;
			numEmptyQueries++ ;
		}
	}
	fclose( testInputEmpty ) ;
	
	expectedResultEmptyFile = fopen ( "EMAILsearchEmptyExpectedResult.txt", "w" ) ;
	_assert( __FILE__, __LINE__, expectedResultEmptyFile ) ;
	setEmptyResult( numEmptyQueries, expectedResultEmptyFile ) ;
	fclose( expectedResultEmptyFile ) ;	

	poll_vr( ) ;

	return 0 ;
}

void callback( int size, resultList *listpointer, void *data )
{	
	printResultList( listpointer ) ;	
}

void setEmptyResult( int emptys, FILE *FSexpectedResultEmptyFile )
{
	int i ;
	char emptyString[] = "empty query...\n" ;
	
	for( i = 0 ; i < emptys ; i++ )
		fwrite( emptyString, sizeof( char ), strlen( emptyString ), FSexpectedResultEmptyFile ) ;		
}
