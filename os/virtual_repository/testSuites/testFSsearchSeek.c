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

#define BUFFER_SIZE 1000

int main( void )
{	
	void callback( int size, resultList *listPointer , int *data ) ;
	void ( *call )( int, resultList *, int * data ) ;
	void setSeekResult( char *, FILE * ) ;
	void setTestData ( char *, FILE * ) ;
	
	char queryString[BUFFER_SIZE] = { '\0' } ;
	char *testString ;
	int *d, index ;
	queryList *tempQueries ;
	FILE *sampleSeekFile, *testSeekQuery, *expectedResultSeekFile, *testInputSeek ;

	_debug( __FILE__, __LINE__, 5, "sentinel" ) ;
	repos_ctr = 0;
	myQueries = NULL;
	
	setup_fs( );
	_debug( __FILE__, __LINE__, 5, "sentinel" ) ;	
	call = callback ;
	_debug( __FILE__, __LINE__, 5, "sentinel" ) ;
	testInputSeek = fopen ( "FSsearchSeek.dat", "r" ) ;
	_assert( __FILE__, __LINE__, testInputSeek ) ;
	
	while ( !feof( testInputSeek ) ) 
	{
		fgets ( queryString, sizeof ( queryString ), testInputSeek ) ;
		_debug( __FILE__, __LINE__, 5, "queryString is %s", queryString ) ;
		if( strlen( queryString ) )
		{
			_debug( __FILE__, __LINE__, 2, "queryString is %s", queryString ) ;
			query_wait( queryString, call, d ) ;			
			queryString[0] = '\0' ;
		}
	}
	
	fclose( testInputSeek ) ;	
	_debug( __FILE__, __LINE__, 5, "sentinel" ) ;
	poll_vr( ) ;
	_debug( __FILE__, __LINE__, 5, "sentinel" ) ;
	expectedResultSeekFile = fopen ( "FSsearchSeekExpectedResult.txt", "w" ) ;	
	_assert( __FILE__, __LINE__, expectedResultSeekFile ) ;
	_debug( __FILE__, __LINE__, 5, "begin FSsearchSeekExpectedResult.txt" ) ;
	tempQueries = myQueries ;
	while( tempQueries != NULL )
	{	
		setSeekResult( tempQueries -> oneQuery -> myClauses[0].value, expectedResultSeekFile ) ;	
		printf( "queries seeks %s\n", tempQueries -> oneQuery -> myClauses[0].value ) ;
		tempQueries = ( queryList * ) tempQueries -> link ;
	}
	fclose( expectedResultSeekFile ) ;
	_debug( __FILE__, __LINE__, 5, "end FSsearchSeekExpectedResult.txt" ) ;
	return 0 ;
}

void callback( int size, resultList *listpointer, int *data )
{	
	printResultList( listpointer ) ;	
}

void setSeekResult( char *queryValue, FILE *expectedResultSeekFile )
{
	char seekString[BUFFER_SIZE] = "queries seeks " ;
	
	strcat( seekString, queryValue ) ;
	strcat( seekString, "\n" ) ;
	fwrite( seekString, sizeof( char ), strlen( seekString ), expectedResultSeekFile ) ;
}

void setTestData ( char *queryString, FILE *testQuery ) 
{
 	char *token, *value, *pQuery ;
	char cwd[BUFFER_SIZE] = { '\0' } ;
	char tempQuery[BUFFER_SIZE] = { '\0' } ;
	char testString[BUFFER_SIZE] = { '\0' } ;
	
	strcpy( tempQuery, queryString ) ;	
		
	if ( strstr( queryString, "///" ) != NULL ) 
	{
		if( getcwd ( cwd , BUFFER_SIZE ) == NULL )
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
