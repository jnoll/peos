/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : seekerTools.c						*
 ************************************************************************/

#include "form.h"
#include "debug.h"
#include "variables.h"
#include "resultLinkedList.h"
#include "queryLinkedList.h"
#include "seekerTools.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define BUFFER_SIZE 1000

/************************************************************************
 * Function:	sortResult						*
 *									*
 * Description:	Returns the intersection of tempResults and newResults 	*
 ************************************************************************/

resultList* sortResult( resultList* theResults )
{
	resultList* insertSortResultItem( resultList*, char * ) ;
	
	resultList *unSortedResults = NULL ;
	resultList *sortedResults = NULL ;

	unSortedResults = theResults ;
	
	//puts( "begin sortResult" ) ;
	//printResultList( sortedResults ) ;
	
	if( unSortedResults == NULL )
		return unSortedResults ;
	else
	{
		while ( unSortedResults != NULL )
		{
			sortedResults = insertSortResultItem( sortedResults, unSortedResults -> oneResult ) ;
			unSortedResults = ( resultList* ) unSortedResults -> link ;
		}		
	}
	
	//puts( "end sortResult" ) ;
	//printResultList( sortedResults ) ;
	
	return sortedResults ;
}

/************************************************************************
 * Function:	insertSortResultItem					*
 *									*
 * Description:	Returns the intersection of tempResults and newResults 	*
 ************************************************************************/

resultList* insertSortResultItem( resultList* unSortedResults, char *data )
{
	resultList *unInsertedResults = NULL ;
	resultList *insertedResults = NULL ;
	int placed = 0 ;

	unInsertedResults = unSortedResults ;
	
	// data in an empty list
	if( unInsertedResults == NULL )
	{
		//printf( "start - if( unInsertedResults == NULL )\n" ) ;
		//printf( "data is %s\n", data ) ;
		//printResultList( insertedResults ) ;
		insertedResults = addResultItem( insertedResults, data ) ;
		//printResultList( insertedResults ) ;
		//printf( "end - if( unInsertedResults == NULL )\n" ) ;
		
	}
	else
	{
		// data is the smallest result
		if( strcmp( data, unInsertedResults -> oneResult ) <= 0 ) 
		{
			//printf( "start - if( strcmp( data, unInsertedResults -> oneResult ) <= 0 )\n" ) ;
			//printf( "data is %s\n", data ) ;
			//printf( "unInsertedResults -> oneResult is %s\n", unInsertedResults -> oneResult ) ;
			//printResultList( insertedResults ) ;
			insertedResults = addResultItem( insertedResults, data ) ;
			//unInsertedResults = ( resultList* ) unInsertedResults -> link ;
			while ( unInsertedResults != NULL )
			{
				insertedResults = addResultItem( insertedResults, unInsertedResults -> oneResult ) ;
				unInsertedResults = ( resultList* ) unInsertedResults -> link ;
			}
			placed = 1 ;				
			//printResultList( insertedResults ) ;	
			//printf( "end - if( strcmp( data, unInsertedResults -> oneResult ) <= 0 )\n" ) ;	
		}
		
		// data is in the middle
		while ( unInsertedResults != NULL && !placed )
		{
			
			if( strcmp( data, unInsertedResults -> oneResult ) >= 0 )
			{
				//printf( "start - if( strcmp( data, unInsertedResults -> oneResult ) >= 0 )\n" ) ;
				//printResultList( insertedResults ) ;
				insertedResults = addResultItem( insertedResults, unInsertedResults -> oneResult ) ;
				//printResultList( insertedResults ) ;
				unInsertedResults = ( resultList* ) unInsertedResults -> link ;
				//printf( "end - if( strcmp( data, unInsertedResults -> oneResult ) >= 0 )\n" ) ;			
			}
			else
			{
				//printf( "start - else\n" ) ;
				//printResultList( insertedResults ) ;
				insertedResults = addResultItem( insertedResults, data ) ;
				
				while ( unInsertedResults != NULL )
				{
					insertedResults = addResultItem( insertedResults, unInsertedResults -> oneResult ) ;
					unInsertedResults = ( resultList* ) unInsertedResults -> link ;
				}
				placed = 1 ;
				//printResultList( insertedResults ) ;	
				//printf( "end - else\n" ) ;
			}
			
		}	
		
		// data is the largest result
		if( !placed )
		{
			//printf( "start - if( !placed )\n" ) ;
			//printf( "data is %s\n", data ) ;
			//printResultList( insertedResults ) ;
			insertedResults = addResultItem( insertedResults, data ) ;				
			//printResultList( insertedResults ) ;
			//printf( "end - if( !placed )\n" ) ;
		}
	}
	
	return insertedResults ;
}


/************************************************************************
 * Function:	andResult						*
 *									*
 * Description:	Returns the intersection of tempResults and newResults 	*
 ************************************************************************/

resultList* andResult( resultList* tempResults, resultList* newResults )
{
	resultList *andResultList = NULL ;
	resultList *tempOftempResults = NULL ;
		
	while ( newResults != NULL )
	{
		tempOftempResults = tempResults ;
		while ( tempOftempResults != NULL )
		{
			if( strcmp( tempOftempResults -> oneResult, newResults -> oneResult ) == 0 )
				andResultList = addResultItem( andResultList, tempOftempResults -> oneResult ) ;
			tempOftempResults = ( resultList* ) tempOftempResults -> link ;
		}
		newResults = ( resultList* ) newResults -> link ;
	}
	
	return andResultList ;
}

/************************************************************************
 * Function:	orResult						*
 *									*
 * Description:	Returns the union of tempResults and newResults 	*
 ************************************************************************/
 
resultList* orResult( resultList* tempResults, resultList* newResults )
{
	int found ;
	resultList *orResultList, *listpointer ; 
	orResultList = listpointer = tempResults ;

	while ( newResults != NULL )
	{
		found = 0 ;
		tempResults = listpointer ;
		while ( tempResults != NULL && !found )
		{
			if( strcmp( tempResults -> oneResult, newResults -> oneResult ) == 0 )
				found = 1 ;
			tempResults = ( resultList* ) tempResults -> link ;
		}
		if( !found )
			orResultList = addResultItem( orResultList, newResults -> oneResult ) ;
		
		newResults = ( resultList* ) newResults -> link ;
	}
	
	return orResultList ;
}

void getPath(char *pathBuffer, char *attribute)
{
	FILE *configFile ;		
	char *word;			
	char oneLine[BUFFER_SIZE] = { '\0' } ;
	char searchBox[BUFFER_SIZE] = { '\0' } ;
	char home[BUFFER_SIZE] = { '\0' } ;
	int doneSearchBox ;
			
	doneSearchBox = 0 ;
	strcpy(home,getenv("HOME"));
	
	
	if( ( configFile = fopen( "vr.rc", "r" ) ) != NULL  || (configFile = fopen(strcat(home,"/vr.rc"),"r") )!= NULL)
	{	
		while( fgets( oneLine, 500, configFile ) != NULL || !doneSearchBox )
   		{
   			word = strtok( oneLine, "=" ) ;
			_assert( __FILE__, __LINE__, word ) ;
			
			if( strcmp( attribute, word ) == 0 )
    			{
       				word = strtok( NULL, "\n" ) ;
       				_assert( __FILE__, __LINE__, word ) ;
   				strcpy( searchBox, word ) ;
   				doneSearchBox = 1 ;
			}
	    	}
        	fclose( configFile ) ;
        	strcpy( pathBuffer, searchBox ) ;
	}
}
