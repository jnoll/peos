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

/************************************************************************
 * Function:	andResult						*
 *									*
 * Description:	Returns the intersection of tempResults and newResults 	*
 ************************************************************************/
 

void getPath(char *pathBuffer, char *attribute)
{
FILE *configFile ;		
char oneLine[1000] ;		
char *word;			
char searchBox[1000] = { '\0' } ;
char home[1000] = { '\0' } ;
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
