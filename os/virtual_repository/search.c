/* 
**	Senior Design Project - PEOS Virtual Repository
**	Author : TASK4ONE
**	Filename : search.c
*/

#include "form.h"
#include "variables.h"
#include "vrepo.h"
#include "setup_repositories.h"
#include <sys/time.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define STDIN 0 

int main( void )
{	
	void selectLoop( )  ;
	
	repos_ctr = 0;
	myQueries = NULL;
	
	setup_fs( );	
	while( 1 )
		selectLoop( ) ;
	
	return 0 ;
}

void selectLoop( ) 
{
	void callback( int size, resultList *listPointer , int *data ) ;
	void ( *call )( int, resultList *, int * data ) ;
	
	struct timeval tv ;
	fd_set readfds ;
	char queryString[1000] ;
	int *d ;
	queryList *tempQueries ;
	FILE *testFile ;

	tv.tv_sec = 5 ;
	tv.tv_usec = 500000 ;
	call = callback ;

	FD_ZERO( &readfds ) ;
	FD_SET( STDIN, &readfds ) ;
	
	select( STDIN+1, &readfds, NULL, NULL, &tv ) ;

	if ( FD_ISSET( STDIN, &readfds ) )
	{
		printf( "\nreceiving...\n\n" ) ;		
		fgets( queryString, sizeof( queryString), stdin ) ;
		if ( strcmp ( "test\n", queryString ) == 0 )
		{
			testFile = fopen ( "test.dat", "r" ) ;
			puts("here");
			while ( !feof( testFile ) ) 
			{
				puts("here1");
				fgets ( queryString, sizeof ( queryString ), testFile ) ;
				if( strlen( queryString ) )
				{
					query_wait( queryString, call, d ) ;
					queryString[0] = '\0' ;	
				}					
			}
			fclose( testFile ) ;
		}
		else
		{
			query_wait( queryString, call, d ) ;
			queryString[0] = '\0' ;	
		}
		fflush( stdin ) ;
	}
	else
	{
		printf( "\ntimed out....\n\n" ) ;
		poll_vr( ) ;
		tempQueries = myQueries ;
		while( tempQueries != NULL )
		{	
			printf( "queries seeks %s\n", tempQueries -> oneQuery -> myClauses[0].value ) ;
			tempQueries = ( queryList * ) tempQueries -> link ;
		}

	}
	FD_CLR( STDIN, &readfds ) ;
}

void callback( int size, resultList *listpointer, int *data )
{	
	printf( "calling back...\n" ) ;
	printf( "%d record(s) found!\n", size ) ;
	printResultList( listpointer ) ;
}

