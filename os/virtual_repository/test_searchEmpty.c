/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : test_searchEmpty.c					*
 ************************************************************************/

/************************************************************************
 * Description:	Test the implementation of Virtual Repository for the	*
 		Unix file system with empty queries			*
 ************************************************************************/

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
	void setEmptyResult( int, FILE * ) ;
	void setExpectedResult ( char *, FILE * )  ;
	void setTestData ( char *, FILE * ) ;
	
	char queryString[1000] ;
	char *testString ;
	int *d, index ;
	queryList *tempQueries ;
	FILE *expectedResultEmptyFile, *testInputEmpty ;

	repos_ctr = 0;
	myQueries = NULL;
	
	setup_fs( );	
	call = callback ;
	
	expectedResultEmptyFile = fopen ( "expectedResultEmpty.txt", "w" ) ;
	setEmptyResult( 5, expectedResultEmptyFile ) ;
	fclose( expectedResultEmptyFile ) ;	
	
	testInputEmpty = fopen ( "testEmpty.dat", "r" ) ;
	while ( !feof( testInputEmpty ) ) 
	{
		fgets ( queryString, sizeof ( queryString ), testInputEmpty ) ;
		if( strlen( queryString ) )
		{
			query_wait( queryString, call, d ) ;
			queryString[0] = '\0' ;
		}
	}
	fclose( testInputEmpty ) ;

	poll_vr( ) ;

	return 0 ;
}

void callback( int size, resultList *listpointer, int *data )
{	
	printResultList( listpointer ) ;	
}

void setEmptyResult( int emptys, FILE *expectedResultEmptyFile )
{
	int i ;
	char emptyString[] = "empty query...\n" ;
	
	for( i = 0 ; i < emptys ; i++ )
		fwrite( emptyString, sizeof( char ), strlen( emptyString ), expectedResultEmptyFile ) ;
		
}

