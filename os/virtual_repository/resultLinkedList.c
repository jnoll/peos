/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : resultLinkedList.c					*
 ************************************************************************/

#include "form.h"
#include "debug.h"
#include "variables.h"
#include "resultLinkedList.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>
#include <unistd.h>


/************************************************************************
 * Function:	printResultList						*
 *									*
 * Description:	Prints the contents of resultList.			*
 ************************************************************************/

void printResultList( resultList *listpointer )
{
	if ( listpointer == NULL )
		printf ( "result list is empty!\n" ) ;
	else
	{
		int i = 1;
		resultList *temp = listpointer;
    		while ( temp != NULL )
		{
			printf ( "%d. %s\n", i,temp -> oneResult ) ;
			temp = ( resultList* ) temp -> link ;
			i++;
		}
	}
	printf( "\n" );
}


/************************************************************************
 * Function:	printResultList						*
 *									*
 * Description:	Clears the resultList.					*
 ************************************************************************/
 
void clearResultList( resultList *listpointer )
{
	while ( listpointer != NULL )
		listpointer = ( resultList* ) removeResultItem( listpointer ) ;
}


/************************************************************************
 * Function:	printResultList						*
 *									*
 * Description:	Removes the first element in the resultList.		*
 ************************************************************************/
 
resultList* removeResultItem( resultList *listpointer )
{
	resultList *temp ;	// pointer to result list
		
	temp = ( resultList* ) listpointer -> link ;
	free( listpointer ) ;
	return temp ;
}



/************************************************************************
 * Function:	addResultItem						*
 *									*
 * Description:	Adds the data at the end of the resultList.		*
 ************************************************************************/
 
resultList* addResultItem( resultList *listpointer, const char *data )
{
	resultList *lp = listpointer ;	// pointer to result list

	if ( listpointer != NULL )
	{
		while( listpointer -> link != NULL )
			listpointer = ( resultList* ) listpointer -> link ;
		
		listpointer -> link = ( struct resultList  * ) malloc ( sizeof ( resultList ) ) ;
		_assert( __FILE__, __LINE__, listpointer -> link) ;
		listpointer = ( resultList* ) listpointer -> link ;
		listpointer -> link = NULL ;
		listpointer -> oneResult = ( char * ) malloc ( ( strlen( data ) + 1 ) * sizeof( char ) ) ;
		_assert( __FILE__, __LINE__, listpointer -> oneResult ) ;
		strcpy( listpointer -> oneResult, data ) ;
		return lp ;
    	}
	else
	{
		listpointer = ( resultList* ) malloc ( sizeof ( resultList ) ) ;
		_assert( __FILE__, __LINE__, listpointer ) ;
		listpointer -> link = NULL ;
		listpointer -> oneResult = ( char * ) malloc ( ( strlen( data ) + 1 )  * sizeof( char ) ) ;
		_assert( __FILE__, __LINE__, listpointer -> oneResult ) ;
		strcpy( listpointer -> oneResult, data ) ;
		return listpointer ;
    	}
}

resultList* copyResultList( resultList *copyFrom )
{
	resultList *tempCopyFrom,*copyTo;
	tempCopyFrom = copyFrom;
	copyTo = NULL;
	while ( tempCopyFrom != NULL )
	{
		copyTo = addResultItem( copyTo, tempCopyFrom -> oneResult ) ;
		tempCopyFrom = ( resultList* ) tempCopyFrom -> link ;
	}
	
	return copyTo;
}
