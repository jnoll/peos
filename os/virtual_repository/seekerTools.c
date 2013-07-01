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
		insertedResults = addResultItem( insertedResults, data ) ;
	else
	{
		// data is the smallest result
		if( strcmp( data, unInsertedResults -> oneResult ) <= 0 ) 
		{
			insertedResults = addResultItem( insertedResults, data ) ;
			while ( unInsertedResults != NULL )
			{
				insertedResults = addResultItem( insertedResults, unInsertedResults -> oneResult ) ;
				unInsertedResults = ( resultList* ) unInsertedResults -> link ;
			}
			placed = 1 ;				
		}
		
		// data is in the middle
		while ( unInsertedResults != NULL && !placed )
		{
			if( strcmp( data, unInsertedResults -> oneResult ) >= 0 )
			{
				insertedResults = addResultItem( insertedResults, unInsertedResults -> oneResult ) ;
				unInsertedResults = ( resultList* ) unInsertedResults -> link ;
			}
			else
			{
				insertedResults = addResultItem( insertedResults, data ) ;
				
				while ( unInsertedResults != NULL )
				{
					insertedResults = addResultItem( insertedResults, unInsertedResults -> oneResult ) ;
					unInsertedResults = ( resultList* ) unInsertedResults -> link ;
				}
				placed = 1 ;
			}
		}	
		
		// data is the largest result
		if( !placed )
			insertedResults = addResultItem( insertedResults, data ) ;				
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

/************************************************************************
 * Function:	getPath							*
 *									*
 * Description:	Returns the path for the requested attribute type  	*
 ************************************************************************/

void getPath( char *pathBuffer, char *attribute )
{
	FILE *configFile ;		
	char *word;			
	char oneLine[BUFFER_SIZE] = { '\0' } ;
	char searchBox[BUFFER_SIZE] = { '\0' } ;
	char home[BUFFER_SIZE] = { '\0' } ;
	int doneSearchBox ;
			
	doneSearchBox = 0 ;
	strcpy( home, getenv( "HOME" ) ) ;	
	
	if( ( configFile = fopen( "vr.rc", "r" ) ) != NULL  || ( configFile = fopen( strcat( home,"/vr.rc" ),"r" ) ) != NULL )
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

/************************************************************************
 * Function:	formatTimeStamp						*
 *									*
 * Description:	sets date and time value in variable newTimeStamp	*
 *		in the form of	yyyy/mm/dd hh:mm:ss given a text string	*
 *		of date and year					*
 ************************************************************************/

void formatTimeStamp( char *newTimeStamp, char *oldTimeStamp )
{
	void formatTime( char *, char * ) ;
	void formatDate( char *, char * ) ;
	
	char *word, *toParse, *date, *time ;
	char formattedTime[9] = { '\0' } ;
	char formattedDate[11] = { '\0' } ;
	char oldTime[9] = { '\0' } ;
	char oldDate[11] = { '\0' } ;
	int numParses ;
		
	numParses = 0 ;	
	word = toParse = date = time = NULL ;
	toParse = strdup( oldTimeStamp ) ;
	
	word = strtok( toParse, "-" ) ;
	
	while( word != NULL )
	{
		switch( numParses )
		{
			case 0	: 	strcpy( oldDate, word ) ;					
					break ;
								
			case 1	: 	strcpy( oldTime, word ) ;
					break ;
		}	
		numParses++ ;
		word = strtok( NULL, "-" ) ;
	}
	
	formatDate( formattedDate, oldDate ) ;
	date = strdup( formattedDate ) ;
	
	formatTime( formattedTime, oldTime ) ;
	time = strdup( formattedTime ) ;
	
	sprintf( newTimeStamp,"%s %s", date, time ) ;	
	
	free( date ) ;
	free( time ) ;
	free( toParse ) ;
}

/************************************************************************
 * Function:	formatTime						*
 *									*
 * Description:	sets time value in variable newTime in the form of	*
 *		hh:mm:ss given a text string of date and year		*
 ************************************************************************/

void formatTime( char *newTime, char *oldTime )
{
	char *word, *toParse, *hours, *minutes, *seconds ;
	int numParses ;
		
	numParses = 0 ;	
	word = toParse = hours = minutes = seconds = NULL ;
	toParse = strdup( oldTime ) ;
	
	word = strtok( toParse, ":" ) ;
	
	while( word != NULL )
	{
		switch( numParses )
		{
			case 0	: 	hours = strdup( word ) ;
					break ;
								
			case 1	: 	minutes = strdup( word ) ;
					break ;
					
			case 2	: 	seconds = strdup( word ) ;
					break ;
			
		}	
		numParses++ ;
		word = strtok( NULL, ":" ) ;
	}
	
	sprintf( newTime,"%s:%s:%s", hours, minutes, seconds ) ;
	
	free( hours ) ;
	free( minutes ) ;
	free( seconds ) ;
	free( toParse ) ;
}

/************************************************************************
 * Function:	formatDate						*
 *									*
 * Description:	sets date value in variable newDate in the form of	*
 *		yyyy/mm/dd given a text string of date and year		*
 ************************************************************************/
 
void formatDate( char *newDate, char *oldDate )
{
	char *word, *toParse, *year, *month, *day ;
	int numParses ;
	
	numParses = 0 ;
	word = toParse = year = month = day = NULL ;
	
	toParse = strdup( oldDate ) ;
	
	word = strtok( toParse, "/" ) ;

	while( word != NULL )
	{
		switch( numParses )
		{
			case 0	: 	month = strdup( word ) ;
					break ;
								
			case 1	: 	day = strdup( word ) ;
					break ;
								
			case 2	: 	year = strdup( word ) ;
					break ;
		}
		numParses++ ;
		word = strtok( NULL, "/" ) ;
	}

	sprintf( newDate,"%s/%s/%s", year, month, day ) ;
	
	free( month ) ;
	free( day ) ;
	free( year ) ;
	free( toParse ) ;
}
