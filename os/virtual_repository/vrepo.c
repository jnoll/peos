/* 
**	Senior Design Project - PEOS Virtual Repository
**	Author : TASK4ONE
**	Filename : vrepo.c
*/

#include "form.h"
#include "variables.h"
#include "vrepo.h"
#include "seeker.h"
#include "resultLinkedList.h"
#include "queryLinkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void query_wait( char *queryString, void ( *cback )( int, resultList *, int * ), int *d )
{
	bool isValidOp( char *op ) ; 
	
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
					
			case 1 :	if( isValidOp( word ) )
					{
						newQuery -> myClauses[j].operator = 
							(char *) malloc ( strlen( word ) * sizeof( char ) ) ;
						strcpy( newQuery -> myClauses[j].operator, word ) ;
					}
					else
						printf( "invalid operator!" ) ;
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
	
	newQuery -> numClauses = j ;
	newQuery -> callback = cback;
	newQuery -> data = d ;
	myQueries = addQueryItem( myQueries, newQuery ) ;
	numQueries++ ;
}

bool isValidOp( char *op )
{
	int i ;
	char operators[6][2] = { "EQ", "NE", "LT", "LE", "GT", "GE" } ;
	
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
	void deregister( int index ) ;
	
	queryList *tempQueries ;
	
	int tag = 0 ;

	myQueries = queryTool( myQueries ) ;
	
	tempQueries = myQueries ;
	
	while( tempQueries != NULL )
	{
		if( tempQueries -> oneQuery -> numFound )
		{
			tag = 1;
			tempQueries -> oneQuery -> callback( tempQueries -> oneQuery -> numFound,
						             tempQueries -> oneQuery -> results,
						             tempQueries -> oneQuery -> data ) ;
			printf( "%d record(s) found!\n", tempQueries -> oneQuery -> numFound ) ;
			printResultList( tempQueries -> oneQuery -> results ) ;
			tempQueries -> oneQuery -> removeTag = 1 ;
		}
		tempQueries = ( queryList* ) tempQueries -> link ;
	}
	
	if ( tag )
		myQueries = filterQueryList( myQueries ) ;
}

