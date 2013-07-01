/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : testUNITsortResult.c					*
 ************************************************************************/

/************************************************************************
 * Description:	Test the unit implementation of sortResult		*
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
	resultList *currentResults, *theResults ;
	FILE *expectedResultFile ;
	char expectedString[BUFFER_SIZE] = { '\0' } ;
	
	strcat( expectedString, " \n" ) ;
	strcat( expectedString, "1\n" ) ;
	strcat( expectedString, "11\n" ) ;
	strcat( expectedString, "111\n" ) ;
	strcat( expectedString, "121\n" ) ;
	strcat( expectedString, "Acurrent\n" ) ;
	strcat( expectedString, "acurrent\n" ) ;
	strcat( expectedString, "current\n" ) ;
	strcat( expectedString, "currentFile\n" ) ;																	
	strcat( expectedString, "currentFile0\n" ) ;
	strcat( expectedString, "currentFile1\n" ) ;
	strcat( expectedString, "currentFile2\n" ) ;
	strcat( expectedString, "currentFile3\n" ) ;		
	strcat( expectedString, "currentFile4\n" ) ;
	strcat( expectedString, "currentFile7\n" ) ;
	strcat( expectedString, "currentFileName\n\n" ) ;
				
	expectedResultFile = fopen ( "UNITsortResultExpectedResult.txt", "w" ) ;
	_assert( __FILE__, __LINE__, expectedResultFile ) ;
	
	fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
	
	fclose( expectedResultFile ) ;	
	
	currentResults = theResults = NULL ;
	currentResults = addResultItem ( currentResults, "currentFile3" ) ;
	currentResults = addResultItem ( currentResults, "currentFile1" ) ;
	currentResults = addResultItem ( currentResults, "currentFile2" ) ;
	currentResults = addResultItem ( currentResults, "currentFile4" ) ;
	currentResults = addResultItem ( currentResults, "currentFile0" ) ;
	currentResults = addResultItem ( currentResults, "currentFile7" ) ;
	currentResults = addResultItem ( currentResults, "currentFile" ) ;
	currentResults = addResultItem ( currentResults, "currentFileName" ) ;
	currentResults = addResultItem ( currentResults, "current" ) ;
	currentResults = addResultItem ( currentResults, "acurrent" ) ;
	currentResults = addResultItem ( currentResults, "Acurrent" ) ;
	currentResults = addResultItem ( currentResults, "1" ) ;
	currentResults = addResultItem ( currentResults, " " ) ;
	currentResults = addResultItem ( currentResults, "11" ) ;
	currentResults = addResultItem ( currentResults, "111" ) ;
	currentResults = addResultItem ( currentResults, "121" ) ;
		
	theResults = sortResult( currentResults ) ;
	
	printResultList( theResults ) ;
	
	return 0 ;
}
	
	
