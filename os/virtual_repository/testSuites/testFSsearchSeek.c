/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : testFSsearchSeek.c					*
 ************************************************************************/

/************************************************************************
 * Description:	Test the implementation of Virtual Repository for the	*
 		Unix file system with valid but not found queries	*
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
	void setSeekResult( char *, FILE * ) ;
	void setTestData ( char *, FILE * ) ;
	
	char queryString[1000] ;
	char *testString ;
	int *d, index ;
	queryList *tempQueries ;
	FILE *sampleSeekFile, *testSeekQuery, *expectedResultSeekFile, *testInputSeek ;

	repos_ctr = 0;
	myQueries = NULL;
	
	setup_vr( );	
	call = callback ;
	
	testInputSeek = fopen ( "FSsearchSeek.dat", "r" ) ;
	_assert( __FILE__, __LINE__, testInputSeek ) ;
	
	while ( !feof( testInputSeek ) ) 
	{
		fgets ( queryString, sizeof ( queryString ), testInputSeek ) ;
		if( strlen( queryString ) )
		{
			query_wait( queryString, call, d ) ;
			queryString[0] = '\0' ;
		}
	}
	
	fclose( testInputSeek ) ;	
	
	poll_vr( ) ;

	expectedResultSeekFile = fopen ( "FSsearchSeekExpectedResult.txt", "w" ) ;	
	_assert( __FILE__, __LINE__, expectedResultSeekFile ) ;
	
	tempQueries = myQueries ;
	while( tempQueries != NULL )
	{	
		setSeekResult( tempQueries -> oneQuery -> myClauses[0].value, expectedResultSeekFile ) ;	
		printf( "queries seeks %s\n", tempQueries -> oneQuery -> myClauses[0].value ) ;
		tempQueries = ( queryList * ) tempQueries -> link ;
	}
	fclose( expectedResultSeekFile ) ;

	return 0 ;
}

void callback( int size, resultList *listpointer, int *data )
{	
	printResultList( listpointer ) ;	
}

void setSeekResult( char *queryValue, FILE *expectedResultSeekFile )
{
	char seekString[BUFFER] = "queries seeks " ;
	
	strcat( seekString, queryValue ) ;
	strcat( seekString, "\n" ) ;
	fwrite( seekString, sizeof( char ), strlen( seekString ), expectedResultSeekFile ) ;
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
			_assert( __FILE__, __LINE__, token ) ;
			strcat( testString, token ) ;
			strcat( testString, "://" ) ;
			strcat( testString, cwd ) ;
			value = strpbrk( queryString, ":" ) ;
			_assert( __FILE__, __LINE__, value ) ;
			strcat( testString, value + 3 ) ;
		}
	}
	else
		strcpy( testString, queryString ) ;
	
	fwrite( testString, sizeof( char ), strlen( testString ), testQuery ) ;
}