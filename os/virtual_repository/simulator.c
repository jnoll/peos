/* 
**	Senior Design Project - PEOS Virtual Repository
**	Author : TASK4ONE
**	Filename : sdp.c
*/

#include "form.h"
#include "variables.h"
#include "vrepo.h"
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
		
	while( 1 )
		selectLoop( ) ;
	
	return 0 ;
}

void selectLoop( ) 
{
//	void callback(int size, char myresults[][] , int * data);
//	void (*call)(int, char [][], int *data);

	void callback(int size, resultList *listPointer , int * data);
	void (*call)(int, resultList *, int *data);
	
	struct timeval tv;
	fd_set readfds;
	char queryString[1000] ;
	int * d;
	int i ;

	tv.tv_sec = 5 ;
	tv.tv_usec = 500000 ;
	call = callback;
	
	FD_ZERO(&readfds);
	FD_SET(STDIN, &readfds);
	
	select(STDIN+1, &readfds, NULL, NULL, &tv);
	
	if ( FD_ISSET(STDIN, &readfds) )
	{
		printf("\nreceiving...\n\n");		
		fgets( queryString, sizeof( queryString ), stdin ) ;	
		query_wait( queryString, call, d ) ;
		fflush( stdin ) ;
	}
	else
	{
		printf("\ntimed out....\n\n");
		poll_vr( ) ;
		
		for( i = 0; i < numQueries ; i++ )
		{	
			printf( "queries %d seeks %s\n", i, Queries[i].myClauses[0].value ) ;
		}
	}
	
	FD_CLR(STDIN, &readfds) ;
}

//void callback(int size, char myresults[][], int * data)
void callback(int size, resultList *listPointer, int * data)
{	
	printf( "calling back...\n" ) ;
}

