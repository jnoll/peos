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

resultList* andResult( resultList* tempResults, resultList* newResults )
{
	resultList *andResultList = NULL ;
	resultList *tempOftempResults = NULL ;
		
	while ( newResults != NULL )
	{
		tempOftempResults = tempResults ;
		while ( tempOftempResults != NULL )
		{
			_debug( __FILE__, __LINE__, 5, "tempOftempResults -> oneResult is %s", tempOftempResults -> oneResult ) ;
			_debug( __FILE__, __LINE__, 5, "newResults -> oneResult is %s", newResults -> oneResult ) ;
			if( strcmp( tempOftempResults -> oneResult, newResults -> oneResult ) == 0 )
				andResultList = addResultItem( andResultList, tempOftempResults -> oneResult ) ;
			tempOftempResults = ( resultList* ) tempOftempResults -> link ;
		}
		newResults = ( resultList* ) newResults -> link ;
	}
	
	return andResultList ;
}

resultList* orResult( resultList* tempResults, resultList* newResults )
{
	int found ;
	resultList *orResultList, *listpointer ; 
	orResultList = listpointer = tempResults ;

	found = 0 ;
	
	while ( newResults != NULL )
	{
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
