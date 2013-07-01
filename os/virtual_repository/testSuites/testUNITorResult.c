/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : testUNITorResult.c					*
 ************************************************************************/

/************************************************************************
 * Description:	Test the unit implementation of orResult		*
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
	strcat( expectedString, "currentFile1\n\n" ) ;
	strcat( expectedString, "currentFile1\n\n" ) ;
	strcat( expectedString, "currentFile\ncurrentFile11\ncurrentFile2\ncurrentFile1\ncurrentFile111\ncurrentFile3\n\n" ) ;
	
	expectedResultFile = fopen ( "UNITorResultExpectedResult.txt", "w" ) ;
	_assert( __FILE__, __LINE__, expectedResultFile ) ;
	
	fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
	
	fclose( expectedResultFile ) ;	
	
	// both resultLists being NULL

	currentResults = newResults = theResults = NULL ;	
	theResults = andResult( currentResults, newResults ) ;
	printResultList( theResults ) ;
		
	// resultList newResults being NULL
	
	currentResults = newResults = theResults = NULL ;
	currentResults = addResultItem ( currentResults, "currentFile1" ) ;
	theResults = orResult( currentResults, newResults ) ;
	printResultList( theResults ) ;
	
	// resultList currentResults being NULL
	
	currentResults = newResults = theResults = NULL ;
	newResults = addResultItem ( newResults, "currentFile1" ) ;
	theResults = orResult( currentResults, newResults ) ;
	printResultList( theResults ) ;
	
	// both resultLists has content
	
	currentResults = newResults = theResults = NULL ;
	
	currentResults = addResultItem ( currentResults, "currentFile" ) ;
	currentResults = addResultItem ( currentResults, "currentFile11" ) ;
	currentResults = addResultItem ( currentResults, "currentFile2" ) ;
	
	newResults = addResultItem ( newResults, "currentFile" ) ;
	newResults = addResultItem ( newResults, "currentFile1" ) ;
	newResults = addResultItem ( newResults, "currentFile111" ) ;
	newResults = addResultItem ( newResults, "currentFile2" ) ;
	newResults = addResultItem ( newResults, "currentFile3" ) ;
	
	theResults = orResult( currentResults, newResults ) ;

	printResultList( theResults ) ;
	
	return 0 ;
}
	
	
