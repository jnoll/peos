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

#define BUFFER 1000

int main( void )
{	
	void callback( int size, resultList *listPointer , int *data ) ;
	void ( *call )( int, resultList *, int * data ) ;
	void setExpectedResult ( char *, FILE * )  ;
	void setTestData ( char *, FILE * ) ;
	
	char queryString[1000] ;
	char *testString ;
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
	
	expectedResultFile = fopen ( "FSsearchEnd2EndExpectedResult.txt", "w" ) ;
	_assert( __FILE__, __LINE__, expectedResultFile ) ;
	testFile = fopen ( "FSsearchEnd2EndQuery.dat", "r" ) ;
	_assert( __FILE__, __LINE__, testFile ) ;	
	while ( !feof( testFile ) ) 
	{
		fgets ( queryString, sizeof ( queryString ), testFile ) ;
		if( strlen( queryString ) )
		{
			setExpectedResult( queryString, expectedResultFile ) ;
			queryString[0] = '\0' ;
		}
	}
	fclose( testFile ) ;
	fclose( expectedResultFile ) ;	
	
	testInput = fopen ( "FSsearchEnd2EndQuery.dat", "r" ) ;
	_assert( __FILE__, __LINE__, testInput ) ;
	while ( !feof( testInput ) ) 
	{
		fgets ( queryString, sizeof ( queryString ), testInput ) ;
		if( strlen( queryString ) )
		{
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

void setExpectedResult ( char *queryString, FILE *expectedResultFile ) 
{
	char *pValue ;
	char testString[BUFFER] = { '\0' } ;
	char tempQuery[BUFFER] = { '\0' } ;
	
	strcpy( tempQuery, queryString ) ;
		
	if ( strstr( queryString, "file://" ) != NULL ) 
	{
		pValue = strpbrk( tempQuery, ":" ) ;
		_assert( __FILE__, __LINE__, pValue ) ;
		strcpy( testString, pValue - 4 ) ;
		strcat( testString, "\n" ) ;
		fwrite( testString, sizeof( char ), strlen( testString ), expectedResultFile ) ;
	}
	/*else
	{
		pValue = strrchr( tempQuery, ' ' ) ;
		_assert( __FILE__, __LINE__, pValue ) ;
		strcat( testString, "./" ) ;
		strcat( testString, pValue + 1 ) ;
	}*/
	
}

void setTestData ( char *queryString, FILE *testQuery ) 
 {
 	char *token, *value, *pQuery ;
	char cwd[BUFFER] = { '\0' } ;
	char tempQuery[BUFFER] = { '\0' } ;
	char testString[BUFFER] = { '\0' } ;
	
	strcpy( tempQuery, queryString ) ;	
		
	if ( strstr( queryString, "///" ) != NULL ) 
	{
		if( getcwd ( cwd , BUFFER ) == NULL )
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
	
	fwrite( testString, sizeof( char ), strlen( testString ), testQuery ) ;
}
