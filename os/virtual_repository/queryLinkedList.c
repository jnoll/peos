/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : queryLinkedList.c						*
 ************************************************************************/

#include "form.h"
#include "debug.h"
#include "variables.h"
#include "queryLinkedList.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

/************************************************************************
 * Function:	printQueryList						*
 *									*
 * Description:	Prints the contents of queryList.			*
 ************************************************************************/
 
void printQueryList( queryList *listpointer )
{
	if ( listpointer == NULL )
		printf ( "\nqueue list is empty!\n" ) ;
	else
	{
    		while ( listpointer != NULL )
		{
			int i ;
			printf( "\nquery is" );
			for( i = 0; i <= listpointer -> oneQuery -> numClauses; i++ )
			{
				printf( " %s %s %s", listpointer -> oneQuery -> myClauses[i].attribute ,
						     listpointer -> oneQuery -> myClauses[i].operator, 
						     listpointer -> oneQuery -> myClauses[i].value ) ;
						    
				if( listpointer -> oneQuery -> myClauses[i].conjecture != NULL )
					printf( " %s", 	listpointer -> oneQuery -> myClauses[i].conjecture ) ;
			}			
			listpointer = ( queryList* ) listpointer -> link ;
		}
	}
	printf ( "\n" ) ;
}

/************************************************************************
 * Function:	addQueryItem						*
 *									*
 * Description:	Adds the data at the end of the queryList.		*
 ************************************************************************/
 
queryList *addQueryItem( queryList *listpointer, const query *data )
{
	queryList *lp = listpointer ;	// pointer to query list
	
	if ( listpointer != NULL )
	{
		while ( listpointer -> link != NULL )
			listpointer = ( queryList* ) listpointer -> link ;

		listpointer -> link = ( struct queryList * ) malloc ( sizeof ( queryList ) ) ;
		_assert( __FILE__, __LINE__, listpointer -> link ) ;
		listpointer = ( queryList * ) listpointer -> link;
		listpointer -> link = NULL;
		listpointer -> oneQuery = ( query* ) data ;
		return lp ;
    	}
	else
	{
		listpointer = ( queryList * ) malloc ( sizeof ( queryList ) ) ;
		_assert( __FILE__, __LINE__, listpointer ) ;
		listpointer -> link = NULL;
		listpointer -> oneQuery = ( query* ) data ;
		return listpointer ;
    	}
}

/************************************************************************
 * Function:	filterQueryList						*
 *									*
 * Description:	Removes the item with removetag = 1 from the queryList	*
 *		and resets the pointers in the list.			*
 ************************************************************************/
 
queryList *filterQueryList( queryList *listpointer )
{
	queryList *previous, *current, *temp ;	// pointers to query list
	previous = current = listpointer ;

	while( current != NULL )
	{
		if( current -> oneQuery -> removeTag )
			if( current == listpointer )
			{
				int numClauses ;
				listpointer = ( queryList * ) listpointer -> link ;
				
				for( numClauses = 0 ; numClauses <= current -> oneQuery -> numClauses ; numClauses++ )
				{
					free( current -> oneQuery -> myClauses[numClauses].attribute ) ;
					free( current -> oneQuery -> myClauses[numClauses].operator ) ;
					free( current -> oneQuery -> myClauses[numClauses].value ) ;
				}
				clearResultList( current -> oneQuery -> results ) ;
				free( current -> oneQuery ) ;
				free( current ) ;
				
				previous = current = listpointer ;
			}
			else
			{
				int numClauses ;
				previous -> link = current -> link ;
				
				for( numClauses = 0 ; numClauses <= current -> oneQuery -> numClauses ; numClauses++ )
				{
					free( current -> oneQuery -> myClauses[numClauses].attribute ) ;
					free( current -> oneQuery -> myClauses[numClauses].operator ) ;
					free( current -> oneQuery -> myClauses[numClauses].value ) ;
				}
				clearResultList( current -> oneQuery -> results ) ;
				free( current -> oneQuery ) ;
				free( current ) ;
				
				current = ( queryList * ) previous -> link ;
			}
		else
		{
			previous = current ;
			current = ( queryList * ) current -> link ;
		}
	}
	return listpointer ;
}

