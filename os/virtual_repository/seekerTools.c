/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : seekerTools.c						*

************************************************************************/


#include "seekerTools.h"

resultList* andResult( resultList* tempResults, resultList* newResults )
{
	resultList *andResultList = NULL ;	
	while ( newResults != NULL )
	{
		while ( tempResults != NULL )
		{
			if( strcmp( tempResults -> oneResult, newResults -> oneResult ) == 0 )
				andResultList = addResultItem( andResultList, tempResults -> oneResult ) ;
			tempResults = ( resultList* ) tempResults -> link ;
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
