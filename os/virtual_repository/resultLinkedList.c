/* 
**	Senior Design Project - PEOS Virtual Repository
**	Author : TASK4ONE
**	Filename : resultLinkedList.c
*/

#include "form.h"
#include "variables.h"
#include "resultLinkedList.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

void printResultList( resultList *listpointer )
{
	if ( listpointer == NULL )
		printf ( "result list is empty!\n" ) ;
	else
	{
    		while ( listpointer != NULL )
		{
			printf ( "%s\n", listpointer -> oneResult ) ;
			listpointer = ( resultList* ) listpointer -> link ;
		}
	}
	printf( "\n" );
}

void clearResultList( resultList *listpointer )
{
	while ( listpointer != NULL )
		listpointer = ( resultList* ) removeResultItem( listpointer ) ;
}

resultList* removeResultItem( resultList *listpointer )
{
	resultList *temp ;
	temp = ( resultList* ) listpointer -> link ;
	free( listpointer ) ;
	return temp ;
}

resultList* addResultItem( resultList *listpointer, const char *data )
{
	resultList *lp = listpointer ;

	if ( listpointer != NULL )
	{
		while( listpointer -> link != NULL )
			listpointer = ( resultList* ) listpointer -> link ;

		listpointer -> link = ( struct resultList  * ) malloc ( sizeof ( resultList ) ) ;
		listpointer = ( resultList* ) listpointer -> link ;
		listpointer -> link = NULL ;
		listpointer -> oneResult = ( char * ) malloc ( strlen( data ) * sizeof( char ) ) ;
		strcpy( listpointer -> oneResult, data ) ;
		return lp ;
    	}
	else
	{
		listpointer = ( resultList* ) malloc ( sizeof ( resultList ) ) ;
		listpointer -> link = NULL ;
		listpointer -> oneResult = ( char* ) malloc ( strlen( data ) * sizeof( char ) ) ;
		strcpy( listpointer -> oneResult, data ) ;
		return listpointer ;
    	}
}
