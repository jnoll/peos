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

	setup_vr( );	
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
	void callback( int size, resultList *listPointer , void *data ) ;
	void ( *call )( int, resultList *, void * data ) ;
	
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
		_debug( __FILE__, __LINE__, 5, "receiving..." ) ;		
		fgets( queryString, sizeof( queryString), stdin ) ;
		query_wait( queryString, call, d ) ;
		queryString[0] = '\0' ;	
	}
	else
	{
		_debug( __FILE__, __LINE__, 5, "timed out...." ) ;
		poll_vr( ) ;
		if(myQueries != NULL && debug_level)
		{
			
			_debug(__FILE__,__LINE__,0, "Pending Queries : " ) ;
			printQueryList(myQueries) ;
			_debug(__FILE__,__LINE__,0, "End of Pending Queries\n " ) ;
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

void callback( int size, resultList *listpointer, void *data )
{	
	_debug(__FILE__,__LINE__,5,"calling back..." ) ;
	printf( "%d record(s) found!\n", size ) ;
	printResultList( listpointer ) ;
}

