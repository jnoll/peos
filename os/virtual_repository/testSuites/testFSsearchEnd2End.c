/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : testFSsearchEnd2End.c					*
 ************************************************************************/

/************************************************************************
 * Description:	Test the implementation of Virtual Repository for the	*
 		Unix file system with valid and found queries		*
 ************************************************************************/

#include "form.h"
#include "debug.h"
#include "variables.h"
#include "vrepo.h"
#include "setup_repositories.h"
#include "seekerTools.h"
#include "queryLinkedList.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define BUFFER_SIZE 1000

int main( void )
{	
	void callback( int size, resultList *listPointer , int *data ) ;
	void ( *call )( int, resultList *, int * data ) ;
	void setExpectedResult( ) ;
	void setTestData( queryList * ) ;
	
	char queryString[BUFFER_SIZE] = { '\0' } ;
	int *d ;
	FILE *sampleFile ;

	repos_ctr = 0;
	myQueries = NULL;
	
	setup_fs( );	
	call = callback ;

	sampleFile = fopen ( "FSsearchEnd2End.dat", "r" ) ;
	_assert( __FILE__, __LINE__, sampleFile ) ;
	
	while ( !feof( sampleFile ) ) 
	{
		fgets ( queryString, sizeof ( queryString ), sampleFile ) ;
		if( strlen( queryString ) )
		{
			_debug( __FILE__, __LINE__, 5, "query_wait() with %s", queryString ) ;
			query_wait( queryString, call, d ) ;
			queryString[0] = '\0' ;
		}
	}
	fclose( sampleFile ) ;
	
	setTestData( myQueries ) ;
	
	setExpectedResult( ) ;	
	
	poll_vr( ) ;

	return 0 ;
}

void callback( int size, resultList *listpointer, int *data )
{	
	resultList *sortedResultList = NULL ;
	sortedResultList = ( resultList * ) sortResult( listpointer ) ;
	printResultList( sortedResultList ) ;	
}

void setExpectedResult ( ) 
{
	char cwd[BUFFER_SIZE] = { '\0' } ;
					
	if( getcwd ( cwd , BUFFER_SIZE ) == NULL )
		_debug( __FILE__, __LINE__, 0, "error in getcwd" ) ;
	else
	{
		FILE *expectedResultFile ;
		char oneResult[BUFFER_SIZE] = { '\0' } ;
		char expectedString[BUFFER_SIZE] = { '\0' } ;
		
		expectedResultFile = fopen ( "FSsearchEnd2EndExpectedResult.txt", "w" ) ;
		_assert( __FILE__, __LINE__, expectedResultFile ) ;
	
	
	
		// test : ID EQ file:///testDir/testID1.c
		// test : ID EQ file://FSsearchEnd2End.dat
	
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testID1.c\n\n" ) ;
		strcat( expectedString, oneResult ) ;
	
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/FSsearchEnd2End.dat\n\n" ) ;
		strcat( expectedString, oneResult ) ;

		fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
		expectedString[0] = '\0' ;

	
		
		// test : ID EQ file:///testDir/testDATE1.c AND DATE LT 01/01/2002-00:00:00
		// test : ID EQ file:///testDir/testDATE2.c AND DATE EQ 01/01/2002-00:00:00
		// test : ID EQ file:///testDir/testDATE3.c AND DATE GT 01/01/2002-00:00:00
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testDATE1.c\n\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testDATE2.c\n\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testDATE3.c\n\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
		expectedString[0] = '\0' ;

	
		
		// test : ID EQ file:///testDir/testNAME3.c AND NAME EQ testNAME3.c
		// test : ID EQ file:///testDir/testNAME3.c AND NAME ~ testNAME3.c
	
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testNAME3.c\n\n" ) ;
		strcat( expectedString, oneResult ) ;
		strcat( expectedString, oneResult ) ;
	
		fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
		expectedString[0] = '\0' ;
		

		
		// test : ID EQ file:///testDir/testID1.c OR ID EQ file:///testDir/testID2.c OR ID EQ file:///testDir/testID3.c
	
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testID1.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testID2.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testID3.c\n\n" ) ;
		strcat( expectedString, oneResult ) ;
	
		fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
		expectedString[0] = '\0' ;
		


		// test : NAME EQ testNAME1.c
	
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testNAME1.c\n\n" ) ;
		strcat( expectedString, oneResult ) ;
	
		fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
		expectedString[0] = '\0' ;
		
		
		
		// test : NAME ~ .c
	
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testDATE1.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testDATE2.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testDATE3.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testID1.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testID2.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testID3.c\n" ) ;
		strcat( expectedString, oneResult ) ;
				
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testNAME1.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testNAME2.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testNAME3.c\n\n" ) ;
		strcat( expectedString, oneResult ) ;		
		
		fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
		expectedString[0] = '\0' ;
				
		
		
		// test : NAME ~ test
	
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testDATE1.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testDATE2.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testDATE3.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testID1.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testID2.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testID3.c\n" ) ;
		strcat( expectedString, oneResult ) ;
				
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testNAME1.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testNAME2.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testNAME3.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testTIME.txt\n\n" ) ;
		strcat( expectedString, oneResult ) ;		
		
		fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
		expectedString[0] = '\0' ;
		
		
		
		// test : NAME ~ testI
	
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testID1.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testID2.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testID3.c\n\n" ) ;
		strcat( expectedString, oneResult ) ;
				
		fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
		expectedString[0] = '\0' ;
				
		
		
		// test : DATE EQ 01/01/2002-00:00:00
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testDATE2.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testID2.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testNAME2.c\n\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
		expectedString[0] = '\0' ;
		
		
		
		// test : DATE LE 01/01/2002-00:00:01
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testDATE1.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testDATE2.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testID1.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testID2.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testNAME1.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testNAME2.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testTIME.txt\n\n" ) ;
		strcat( expectedString, oneResult ) ;	
		
		fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
		expectedString[0] = '\0' ;
		
		
		
		// test : DATE LT 01/01/2002-00:00:01
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testDATE1.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testDATE2.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testID1.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testID2.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testNAME1.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testNAME2.c\n\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
		expectedString[0] = '\0' ;
		
		
		
		// test : DATE GT 01/01/2002-00:00:01
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/CVS/Entries\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/CVS/Repository\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/CVS/Root\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testDATE3.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testID3.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testNAME3.c\n\n" ) ;
		strcat( expectedString, oneResult ) ;		
		
		fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
		expectedString[0] = '\0' ;
	
	
	
	
		// test : DATE GE 01/01/2002-00:00:01
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/CVS/Entries\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/CVS/Repository\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/CVS/Root\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testDATE3.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testID3.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testNAME3.c\n" ) ;
		strcat( expectedString, oneResult ) ;		
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testTIME.txt\n\n" ) ;
		strcat( expectedString, oneResult ) ;	
		
		fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
		expectedString[0] = '\0' ;
	

	
		// test : DATE EQ 01/01/2002-00:00:01 AND NAME ~ test
	
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testTIME.txt\n\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
		expectedString[0] = '\0' ;
		
		
		
		// test : DATE EQ 01/01/2002-00:00:00 AND NAME ~ test
	
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testDATE2.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testID2.c\n" ) ;
		strcat( expectedString, oneResult ) ;
				
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testNAME2.c\n\n" ) ;
		strcat( expectedString, oneResult ) ;		
		
		fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
		expectedString[0] = '\0' ;
		
		
		
		// test : DATE LT 01/01/2002-00:00:00 AND NAME ~ test
	
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testDATE1.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testID1.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testNAME1.c\n\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
		expectedString[0] = '\0' ;
		
		
		
		// test : DATE LE 01/01/2002-00:00:00 AND NAME ~ test
	
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testDATE1.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testDATE2.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testID1.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testID2.c\n" ) ;
		strcat( expectedString, oneResult ) ;
				
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testNAME1.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testNAME2.c\n\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
		expectedString[0] = '\0' ;
		
		
		
		// test : DATE GT 01/01/2002-00:00:00 AND NAME ~ test
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testDATE3.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testID3.c\n" ) ;
		strcat( expectedString, oneResult ) ;
				
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testNAME3.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testTIME.txt\n\n" ) ;
		strcat( expectedString, oneResult ) ;		
		
		fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
		expectedString[0] = '\0' ;
		
		
		
		// test : DATE GE 01/01/2002-00:00:00 AND NAME ~ test
				
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testDATE2.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testDATE3.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testID2.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testID3.c\n" ) ;
		strcat( expectedString, oneResult ) ;
				
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testNAME2.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testNAME3.c\n" ) ;
		strcat( expectedString, oneResult ) ;	
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testTIME.txt\n\n" ) ;
		strcat( expectedString, oneResult ) ;		
		
		fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
		expectedString[0] = '\0' ;
		
		
		
		// test : DATE GT 01/01/2000-00:00:00 AND NAME ~ test
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testDATE1.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testDATE2.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testDATE3.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testID1.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testID2.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testID3.c\n" ) ;
		strcat( expectedString, oneResult ) ;
				
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testNAME1.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testNAME2.c\n" ) ;
		strcat( expectedString, oneResult ) ;
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testNAME3.c\n" ) ;
		strcat( expectedString, oneResult ) ;		
		
		sprintf( oneResult ,"%s%s%s", "file://", cwd, "/testDir/testTIME.txt\n\n" ) ;
		strcat( expectedString, oneResult ) ;	
		
		fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
		expectedString[0] = '\0' ;

		fclose( expectedResultFile ) ;	
	}
}

void setTestData( queryList *listpointer )
{
	queryList *tempQueries ;
	int numClauses ;
	
	tempQueries = listpointer ;
	
	while( tempQueries != NULL )
	{
		char testString[BUFFER_SIZE] = { '\0' } ;		
		char valueString[BUFFER_SIZE] = { '\0' } ;

		for( numClauses = 0 ; numClauses <= tempQueries -> oneQuery -> numClauses ; numClauses++ )
		{
			strcpy( valueString, tempQueries -> oneQuery -> myClauses[numClauses].value ) ;
			
			if( strcmp( tempQueries -> oneQuery -> myClauses[numClauses].attribute, "ID" ) == 0 )
			{
				if( ( strstr( valueString, "file:///" ) ) != NULL ) 
				{
					char cwd[BUFFER_SIZE] = { '\0' } ;
					
					_debug( __FILE__, __LINE__, 5, "numClauses is %d", numClauses ) ;
					_debug( __FILE__, __LINE__, 5, "attribute is %s", tempQueries -> oneQuery -> myClauses[numClauses].attribute ) ;
					_debug( __FILE__, __LINE__, 5, "operator is %s", tempQueries -> oneQuery -> myClauses[numClauses].operator ) ;
					_debug( __FILE__, __LINE__, 5, "value is %s", tempQueries -> oneQuery -> myClauses[numClauses].value ) ;
					
					if( getcwd ( cwd , BUFFER_SIZE ) == NULL )
						_debug( __FILE__, __LINE__, 0, "error in getcwd" ) ;
					else
					{		
						char *token, *value ;
						char tempQuery[BUFFER_SIZE] = { '\0' } ;
						
						strcpy( tempQuery, valueString ) ;
						_debug( __FILE__, __LINE__, 5, "valueString is %s", valueString ) ;
						
						token = strtok( tempQuery, ":" ) ;
						_assert( __FILE__, __LINE__, token  ) ;
			
						strcat( testString, token ) ;
						strcat( testString, "://" ) ;
						strcat( testString, cwd ) ;
						
						value = strpbrk( valueString, ":" ) ;
						_assert( __FILE__, __LINE__, value  ) ;
						
						strcat( testString, value + 3 ) ;						
					}
				}
				else
					strcpy( testString, valueString ) ;
				
				free( tempQueries -> oneQuery -> myClauses[numClauses].value ) ;
				tempQueries -> oneQuery -> myClauses[numClauses].value = strdup( testString ) ;	
				_debug( __FILE__, __LINE__, 5, "testString is %s", testString ) ;
				testString[0] = '\0' ;
			}
		}
		tempQueries = ( queryList* ) tempQueries -> link ;
	}
}
