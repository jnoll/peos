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
	bool isValidClauses( struct clause *, int ) ;
	
	char *word, *toParse ;
	int i, j ;
	query *newQuery ;
	
	newQuery = ( query * ) malloc ( sizeof ( query ) ) ;
		
	toParse = strtok( queryString, "\n" ) ;	// from keyboard input, otherwise remove this line...
	
	i = j = 0 ;				// i to keep track of data content, j to keep track of num clauses
	word = strtok( toParse, " " ) ;
	while( word != NULL )
	{
		switch( i )
		{
			case 0 :	newQuery -> myClauses[j].attribute = 
						(char *) malloc ( strlen( word ) * sizeof( char ) ) ;
					strcpy( newQuery -> myClauses[j].attribute, word ) ;
					break ;
					
			case 1 :	newQuery -> myClauses[j].operator = 
						(char *) malloc ( strlen( word ) * sizeof( char ) ) ;
					strcpy( newQuery -> myClauses[j].operator, word ) ;
					break ;
					
			case 2 :	newQuery -> myClauses[j].value = 
						(char *) malloc ( strlen( word ) * sizeof( char ) ) ;
					strcpy( newQuery -> myClauses[j].value, word ) ;
					break ;
		}
		
		if( i == 2 )	
		{
			i = 0 ;
			j++ ;
		}
		else
			i++ ;
		word = strtok( NULL, " " ) ;
	}
	
	if( isValidClauses( newQuery -> myClauses, j ) )
	{
		newQuery -> numClauses = j ;
		newQuery -> callback = cback;
		newQuery -> data = d ;
		myQueries = addQueryItem( myQueries, newQuery ) ;
	}
	else
	{
		for( i = 0 ; i < j ; i++ )
		{
			free( newQuery -> myClauses[i].attribute ) ;
			free( newQuery -> myClauses[i].operator ) ;
			free( newQuery -> myClauses[i].value ) ;
			free( newQuery ) ;
		}
		printf( "\ninvalid query...\n" ) ;
	}			
}

bool isValidClauses( struct clause *theClauses, int numClauses )
{
	bool isValidOperator( char *op ) ; 
	
	int i ;
	bool valid ;	
	
	valid = true ;
	for( i = 0 ; i < numClauses ; i++ )
	{
		valid = isValidOperator( theClauses[i].operator ) ;
	}
	return valid ;
}

bool isValidOperator( char *op )
{
	int i ;
	char operators[1][2] = { "EQ" } ;
	
	for( i = 0 ; i < sizeof(operators) / sizeof(operators[0]) ; i++ )
	{
		if( strncmp( operators[i], op, 2 ) == 0 )
			return true ;
	}
	return false ;
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
			//	printf( "%d record(s) found!\n", tempQueries -> oneQuery -> numFound ) ;
			//	printResultList( tempQueries -> oneQuery -> results ) ;
				tempQueries -> oneQuery -> removeTag = 1 ;
			}
			tempQueries = ( queryList* ) tempQueries -> link ;
		}
	
		if ( tag )
			myQueries = filterQueryList( myQueries ) ;
	}
}

