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
	void setExpectedResult ( ) ;
	void setTestData ( char *, FILE * ) ;
	
	char queryString[BUFFER_SIZE] = { '\0' } ;
	//char *testString ;
	int *d, index ;
	queryList *tempQueries ;
	FILE *sampleFile, *testQuery, *testFile, *expectedResultFile, *testInput ;

	repos_ctr = 0;
	myQueries = NULL;
	
	setup_fs( );	
	call = callback ;

	sampleFile = fopen ( "FSsearchEnd2End.dat", "r" ) ;
	_assert( __FILE__, __LINE__, sampleFile ) ;
	
	testQuery = fopen ( "FSsearchEnd2EndQuery.dat", "w" ) ;
	_assert( __FILE__, __LINE__, testQuery ) ;
	
	while ( !feof( sampleFile ) ) 
	{
		fgets ( queryString, sizeof ( queryString ), sampleFile ) ;
		if( strlen( queryString ) )
		{
			setTestData( queryString, testQuery ) ;
			queryString[0] = '\0' ;
		}
	}
	fclose( sampleFile ) ;
	fclose( testQuery ) ;
	
	setExpectedResult ( ) ;	
	
	testInput = fopen ( "FSsearchEnd2EndQuery.dat", "r" ) ;
	_assert( __FILE__, __LINE__, testInput ) ;
	
	while ( !feof( testInput ) ) 
	{
		fgets ( queryString, sizeof ( queryString ), testInput ) ;
		if( strlen( queryString ) )
		{
			_debug( __FILE__, __LINE__, 5, "query_wait() with %s", queryString ) ;
			query_wait( queryString, call, d ) ;
			queryString[0] = '\0' ;
		}
	}
	fclose( testInput ) ;

	poll_vr( ) ;

	return 0 ;
}

void callback( int size, resultList *listpointer, int *data )
{	
	printResultList( listpointer ) ;	
}

void setExpectedResult ( ) 
{
	FILE *expectedResultFile ;
	char expectedString[BUFFER_SIZE] = { '\0' } ;
	
	expectedResultFile = fopen ( "FSsearchEnd2EndExpectedResult.txt", "w" ) ;
	_assert( __FILE__, __LINE__, expectedResultFile ) ;
	
	// test for ID attribute
	
	strcat( expectedString, "file:///users/student/cgoh/peos/src/os/virtual_repository/testSuites/testDir/testfile1.c\n\n" ) ;
	strcat( expectedString, "file:///users/student/cgoh/peos/src/os/virtual_repository/testSuites/FSsearchEnd2End.dat\n\n" ) ;
	fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
	expectedString[0] = '\0' ;
	
	// test for AND conjecture - ID && DATE( EQ, LT, GT )
	
	strcat( expectedString, "file:///users/student/cgoh/peos/src/os/virtual_repository/testSuites/testDir/testfile1.c\n\n" ) ;
	strcat( expectedString, "file:///users/student/cgoh/peos/src/os/virtual_repository/testSuites/testDir/testfile1.c\n\n" ) ;
	strcat( expectedString, "file:///users/student/cgoh/peos/src/os/virtual_repository/testSuites/testDir/testfile1.c\n\n" ) ;
	fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
	expectedString[0] = '\0' ;
	
	// test for AND conjecture - ID && NAME( EQ, ~ )
	
	strcat( expectedString, "file:///users/student/cgoh/peos/src/os/virtual_repository/testSuites/testDir/testfile1.c\n\n" ) ;
	strcat( expectedString, "file:///users/student/cgoh/peos/src/os/virtual_repository/testSuites/testDir/testfile1.c\n\n" ) ;
	fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;		
	
	fclose( expectedResultFile ) ;	
}

void setTestData ( char *queryString, FILE *testQuery ) 
 {
 	char *token, *value ;
	char cwd[BUFFER_SIZE] = { '\0' } ;
	char tempQuery[BUFFER_SIZE] = { '\0' } ;
	char testString[BUFFER_SIZE] = { '\0' } ;
	
	_debug( __FILE__, __LINE__, 5, "begin setTestData() with %s", queryString ) ;
	
	strcpy( tempQuery, queryString ) ;	
	
	if( ( strstr( queryString, "file:///" ) ) != NULL ) 
	{
		if( getcwd ( cwd , BUFFER_SIZE ) == NULL )
		{
			puts( "error" ) ;
		}
		else
		{
			token = strtok( tempQuery, ":" ) ;
			_assert( __FILE__, __LINE__, token  ) ;
			strcat( testString, token ) ;
			strcat( testString, "://" ) ;
			strcat( testString, cwd ) ;
			value = strpbrk( queryString, ":" ) ;
			_assert( __FILE__, __LINE__, value  ) ;
			strcat( testString, value + 3 ) ;
		}
	}
	else
		strcpy( testString, queryString ) ;
	
	_debug( __FILE__, __LINE__, 5, "end setTestData() with %s", testString ) ;
	
	fwrite( testString, sizeof( char ), strlen( testString ), testQuery ) ;
}
