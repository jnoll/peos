/* 
**	Senior Design Project - PEOS Virtual Repository
**	Author : TASK4ONE
**	Filename : vrepo.c
*/

#include "form.h"
#include "variables.h"
#include "vrepo.h"
#include "FSseeker.h"
#include "resultLinkedList.h"
#include "queryLinkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void query_wait( char *queryString, void ( *cback )( int, resultList *, int * ), int *d )
{
	bool isValidAttribute( char * ) ; 
	bool isValidOperator( char * ) ; 
	bool isValidValue( char * ) ;
	
	char *word, *toParse ;
	int numParses, numClauses ;
	query *newQuery ;
	
	newQuery = ( query * ) malloc ( sizeof ( query ) ) ;
	numParses = numClauses = 0 ;
	word = toParse = NULL ;
	
	toParse = strtok( queryString, "\n" ) ;	// from keyboard input, otherwise remove this line...

	if( toParse != NULL )
		word = strtok( toParse, " " ) ;

	while( word != NULL )
	{
		switch( numParses )
		{
			case 0 :	if (isValidAttribute( word) )
					{
						newQuery -> myClauses[0].attribute = strdup( word ) ;
						numParses++ ;
					}
					break ;
						
					
			case 1 :	if( isValidOperator(  word) )
					{
						newQuery -> myClauses[0].operator = strdup( word ) ;
						numParses++ ;
					}						
					break ;
					
			case 2 : 	if( isValidValue(  word) )
					{
						newQuery -> myClauses[0].value = strdup( word ) ;
						numParses++ ;
					}
					break ;
		}
		
		word = strtok( NULL, " " ) ;
	}
	
	if( numParses == 3 )
	{
		newQuery -> callback = cback;
		newQuery -> data = d ;
		newQuery -> numFound = 0 ;
		newQuery -> removeTag = 0 ;
		newQuery -> numClauses = numClauses ;
		newQuery -> results = NULL ;
		myQueries = addQueryItem( myQueries, newQuery ) ;
		numParses = 0 ;
	}
	else
	{
		if( numParses != 0 )
		{
			int k ;
		
			for( k = 0 ; k < numParses ; k++ )
			{
				switch( k )
				{
					case 0 :	
							free( newQuery -> myClauses[0].attribute ) ;
							break ;
					
					case 1 :	free( newQuery -> myClauses[0].operator ) ;
							break ;
					
					case 2 :	free( newQuery -> myClauses[0].value ) ;
							break ;
				}
			}
		
			free( newQuery ) ;
			printf( "invalid query...\n" ) ;
		}
		else
			printf( "empty query...\n" ) ;			
	}
}

void poll_vr( ) 
{
	queryList* queryTool( queryList *listpointer ) ;
		
	queryList *tempQueries ;
	
	int tag = 0 ;

	if( myQueries != NULL )
	{
		myQueries = FSqueryTool( myQueries ) ;
		
			
		tempQueries = myQueries ;
	
		while( tempQueries != NULL )
		{
			if( tempQueries -> oneQuery -> numFound )
			{
				tag = 1;
				tempQueries -> oneQuery -> callback( tempQueries -> oneQuery -> numFound,
							             tempQueries -> oneQuery -> results,
							             tempQueries -> oneQuery -> data ) ;
				tempQueries -> oneQuery -> removeTag = 1 ;
			}
			tempQueries = ( queryList* ) tempQueries -> link ;
		}
	
		if ( tag )
			myQueries = filterQueryList( myQueries ) ;
	}
}



bool isValidAttribute( char *attr )
{
	int i ;
	char attributes[1][2] = { "ID" } ;
	
	for( i = 0 ; i < sizeof( attributes ) / sizeof( attributes[0] ) ; i++ )
	{
		if( attr == NULL || ( strcmp( attributes[i], attr ) != 0 ) )
			return false ;
	}
	return true ;
}

bool isValidOperator( char *op )
{
	int i ;
	char operators[1][2] = { "EQ" } ;
	
	for( i = 0 ; i < sizeof(operators) / sizeof(operators[0] ) ; i++ )
	{
		if( op == NULL || ( strcmp( operators[i], op ) != 0 ) )
			return false ;
	}
	return true ;
}

bool isValidValue( char *val )
{
	if ( val == NULL )
		return false ;
	return true ;
}
