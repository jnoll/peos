/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : testFSsearchEmpty.c					*
 ************************************************************************/

/************************************************************************
 * Description:	Test the implementation of Virtual Repository for the	*
 		Unix file system with empty queries			*
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
	void callback( int size, resultList *listPointer , void *data ) ;
	void ( *call )( int, resultList *, void* data ) ;
	void setEmptyResult( int, FILE * ) ;
	
	char queryString[1000] ;
	char *testString ;
	int *d, index, numQueries ;
	queryList *tempQueries ;
	FILE *FSexpectedResultEmptyFile, *testInputEmpty ;

	repos_ctr = 0;
	myQueries = NULL;
	
	setup_fs( );	
	call = callback ;
	
	testInputEmpty = fopen ( "FSsearchEmpty.dat", "r" ) ;
	_assert( __FILE__, __LINE__, testInputEmpty ) ;
	numQueries = 0 ;
	while ( !feof( testInputEmpty ) ) 
	{
		fgets ( queryString, sizeof ( queryString ), testInputEmpty ) ;
		if( strlen( queryString ) )
		{
			query_wait( queryString, call, d ) ;
			queryString[0] = '\0' ;
			numQueries++ ;
		}
	}
	fclose( testInputEmpty ) ;

	FSexpectedResultEmptyFile = fopen ( "FSsearchEmptyExpectedResult.txt", "w" ) ;
	_assert( __FILE__, __LINE__, FSexpectedResultEmptyFile ) ;
	setEmptyResult( numQueries, FSexpectedResultEmptyFile ) ;
	fclose( FSexpectedResultEmptyFile ) ;	
	
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

