/* 
**	Senior Design Project - PEOS Virtual Repository
**	Author : TASK4ONE
**	Filename : test_search.c
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
	
	char queryString[1000] ;
	int *d ;
	queryList *tempQueries ;
	FILE *testFile ;

	repos_ctr = 0;
	myQueries = NULL;
	
	setup_fs( );	
	call = callback ;

	testFile = fopen ( "test.dat", "r" ) ;
	while ( !feof( testFile ) ) 
	{
		fgets ( queryString, sizeof ( queryString ), testFile ) ;
		if( strlen( queryString ) )
		{
			query_wait( queryString, call, d ) ;
			queryString[0] = '\0' ;
				
		}					
	}
	fclose( testFile ) ;
	

	poll_vr( ) ;
	return 0;
}

void callback( int size, resultList *listpointer, int *data )
{	
	
	printResultList( listpointer ) ;
}

