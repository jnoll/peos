/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : testUNITandResult.c					*
 ************************************************************************/

/************************************************************************
 * Description:	Test the unit implementation of andResult		*
 ************************************************************************/
 
#include "form.h"
#include "debug.h"
#include "resultLinkedList.h"
#include "seekerTools.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 1000

int main( void )
{	
	resultList *currentResults, *newResults, *theResults ;
	int counter = 0 ;
	FILE *expectedResultFile ;
	char expectedString[BUFFER_SIZE] = { '\0' } ;
	
	strcat( expectedString, "result list is empty!\n\n" ) ;
	strcat( expectedString, "result list is empty!\n\n" ) ;
	strcat( expectedString, "result list is empty!\n\n" ) ;
	strcat( expectedString, "currentFile\ncurrentFile11\ncurrentFile2\n\n" ) ;
		
	expectedResultFile = fopen ( "UNITandResultExpectedResult.txt", "w" ) ;
	_assert( __FILE__, __LINE__, expectedResultFile ) ;
	
	_debug( __FILE__, __LINE__, 5, "expectedString is %s", expectedString ) ;
	
	fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
	
	fclose( expectedResultFile ) ;	
	
	// both resultLists being NULL
	
	currentResults = newResults = theResults = NULL ;
	theResults = andResult( currentResults, newResults ) ;
	printResultList( theResults ) ;
		
	// resultList newResults being NULL
	
	currentResults = newResults = theResults = NULL ;
	currentResults = addResultItem ( currentResults, "currentFile1" ) ;
	theResults = andResult( currentResults, newResults ) ;
	printResultList( theResults ) ;

	// resultList currentResults being NULL
	
	currentResults = newResults = theResults = NULL ;
	newResults = addResultItem ( newResults, "currentFile1" ) ;
	theResults = andResult( currentResults, newResults ) ;
	printResultList( theResults ) ;
	
	// both resultLists has content
	
	currentResults = newResults = theResults = NULL ;
	currentResults = addResultItem ( currentResults, "currentFile" ) ;
	currentResults = addResultItem ( currentResults, "currentFile11" ) ;
	currentResults = addResultItem ( currentResults, "currentFile2" ) ;
		
	newResults = addResultItem ( newResults, "currentFile" ) ;
	newResults = addResultItem ( newResults, "currentFile1" ) ;
	newResults = addResultItem ( newResults, "currentFile11" ) ;
	newResults = addResultItem ( newResults, "currentFile111" ) ;
	newResults = addResultItem ( newResults, "currentFile2" ) ;
	
	theResults = andResult( currentResults, newResults ) ;
	
	printResultList( theResults ) ;
	
	return 0 ;
}
	
	
