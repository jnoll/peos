/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : search.c							*
 ************************************************************************/

#include "form.h"
#include "variables.h"
#include "vrepo.h"
#include "setup_repositories.h"
#include "debug.h"
#include <sys/time.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define STDIN 0 

/************************************************************************
 * Function:	 main							*
 *									*
 * Description:	 Driver for program.					*
 ************************************************************************/

int main(int argc, char *argv[])
{
	void selectLoop( ) ;
	
	repos_ctr = 0;
	myQueries = NULL;
	
	set_debug(argv[1]);

	setup_fs( ) ;	
	while( 1 )
		selectLoop( ) ;
	
	return 0 ;
}

/************************************************************************
 * Function:	selectLoop						*
 *									*
 * Description:	A simulated engine that reads query from standard 	*
 *		input and pass the queryto function query_wait.		*
 *	        Upon timeout, it polls the virtual repository to        *
 *		search for the requested query.				*
 ************************************************************************/

void selectLoop( ) 
{
	void callback( int size, resultList *listPointer , int *data ) ;
	void ( *call )( int, resultList *, int * data ) ;
	
	struct timeval tv ;			// struct timeval with seconds and microseconds
	fd_set readfds ;			// set of descriptor watched
	char queryString[1000] ;		// buffer that holds the query 
	int *d ;				// pointer to data in callback 
	queryList *tempQueries ;		// pointer to queryList
	
	tv.tv_sec = 5 ;
	tv.tv_usec = 500000 ;
	call = callback ;

	FD_ZERO( &readfds ) ;
	FD_SET( STDIN, &readfds ) ;
	
	select( STDIN+1, &readfds, NULL, NULL, &tv ) ;

	if ( FD_ISSET( STDIN, &readfds ) )
	{
		_debug( __FILE__, __LINE__, 1, "receiving..." ) ;		
		fgets( queryString, sizeof( queryString), stdin ) ;
		query_wait( queryString, call, d ) ;
		queryString[0] = '\0' ;	
	}
	else
	{
		_debug( __FILE__, __LINE__, 2, "timed out...." ) ;
		poll_vr( ) ;
		tempQueries = myQueries ;
		while( tempQueries != NULL )
		{	
			printf( "queries seeks %s\n", tempQueries -> oneQuery -> myClauses[0].value ) ;
			tempQueries = ( queryList * ) tempQueries -> link ;
		}

	}
	fflush( stdin ) ;
	FD_CLR( STDIN, &readfds ) ;
}

/************************************************************************
 * Function:	callback						*
 *									*
 * Description:	Prints result of the requested query.			*
 ************************************************************************/

void callback( int size, resultList *listpointer, int *data )
{	
	_debug(__FILE__,__LINE__,1,"calling back..." ) ;
	printf( "%d record(s) found!\n", size ) ;
	printResultList( listpointer ) ;
}

