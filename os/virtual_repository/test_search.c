/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : test_search.c						*
 ************************************************************************/

/************************************************************************
 * Description:	Test the implementation of Virtual Repository for the	*
 		Unix file system with valid and found queries		*
 ************************************************************************/

#include "form.h"
#include "variables.h"
#include "vrepo.h"
#include "setup_repositories.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

int main( int argc, char * argv[] )
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

	sampleFile = fopen ( "test.dat", "r" ) ;
	testQuery = fopen ( "testQuery.dat", "w" ) ;
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
	
	expectedResultFile = fopen ( "expectedResult.txt", "w" ) ;
	testFile = fopen ( "testQuery.dat", "r" ) ;
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
	
	testInput = fopen ( "testQuery.dat", "r" ) ;
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
	char testString[100] = { '\0' } ;
	char tempQuery[100] = { '\0' } ;
	
	strcpy( tempQuery, queryString ) ;
		
	if ( strstr( queryString, "file://" ) != NULL ) 
	{
		pValue = strpbrk( tempQuery, ":" ) ;
		strcpy( testString, pValue - 4 ) ;
		strcat( testString, "\n" ) ;
		fwrite( testString, sizeof( char ), strlen( testString ), expectedResultFile ) ;
	}
	/*else
	{
		pValue = strrchr( tempQuery, ' ' ) ;
		strcat( testString, "./" ) ;
		strcat( testString, pValue + 1 ) ;
	}*/
	
}

void setTestData ( char *queryString, FILE *testQuery ) 
 {
 	char *token, *value, *pQuery ;
	char cwd[100] = { '\0' } ;
	char tempQuery[100] = { '\0' } ;
	char testString[100] = { '\0' } ;
	
	strcpy( tempQuery, queryString ) ;	
		
	if ( strstr( queryString, "///" ) != NULL ) 
	{
		if( getcwd ( cwd , 100 ) == NULL )
		{
			puts( "error" ) ;
		}
		else
		{
			token = strtok( tempQuery, ":" ) ;
			strcat( testString, token ) ;
			strcat( testString, "://" ) ;
			strcat( testString, cwd ) ;
			value = strpbrk( queryString, ":" ) ;
			strcat( testString, value + 3 ) ;
		}
	}
	else
		strcpy( testString, queryString ) ;
	
	fwrite( testString, sizeof( char ), strlen( testString ), testQuery ) ;
}
