/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : vrepo.c							*
 ************************************************************************/

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

/************************************************************************
 * Function:	query_wait						*
 *									*
 * Description:	Tokenizes the  queryString into clauses consisting of  *
 *		Id, attribute and value. Checks for the validity of 	*
 *		queryString then makes a new query and register it	*
 * 		in the list "myQuery".					*
 ************************************************************************/

void query_wait( char *queryString, void ( *cback )( int, resultList *, int * ), int *d )
{
	bool isValidAttribute( char * ) ; 
	bool isValidOperator( char * ) ; 
	bool isValidValue( char * ) ;
	
	char *word, *toParse ; 		// tokens during string tokenizations 	
	int numParses, numClauses ;	// keeps track of the token in the tokenizing phase
					// numClauses stores the number of clauses in the queryString
					 
	query *newQuery ; 		// stores the new query
	
	newQuery = ( query * ) malloc ( sizeof ( query ) ) ;
	numParses = numClauses = 0 ;
	word = toParse = NULL ;
	
	toParse = strtok( queryString, "\n" ) ;	

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
						
					
			case 1 :	if( isValidOperator( word ) )
					{
						newQuery -> myClauses[0].operator = strdup( word ) ;
						numParses++ ;
					}						
					break ;
					
			case 2 : 	if( isValidValue( word ) )
					{
						newQuery -> myClauses[0].value = strdup( word ) ;
						numParses++ ;
					}
					break ;
			//case 3 :	numParses++;
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
					case 0 : free( newQuery -> myClauses[0].attribute ) ;
						 break ;
					
					case 1 : free( newQuery -> myClauses[0].operator ) ;
						 break ;
					
					case 2 : free( newQuery -> myClauses[0].value ) ;
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


/************************************************************************
 * Function:	poll_vr							*
 *									*
 * Description:	Goes through the repo_list and calls the queryTool 	*
 *		function for each repository.  It also calls the 	*
 *		callback function for the satisfied queries.		*
 ************************************************************************/

void poll_vr( ) 
{
	queryList *tempQueries ;	// temporary variable to store myQuery
	int tag = 0 ;			// tag is one if a query is satisfied in myQueries
	int i=0;			// used in for loop

	if( myQueries != NULL )
	{
		for(i=0; i<repos_ctr ; i++)
			myQueries = repos_list[i].queryTool(myQueries);
		
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

/************************************************************************
 * Function:	isValidAttribute					*
 *									*
 * Description:	Returns true if the attribute in the query is an  	*
 *		attribute of the repository. 				*
  ************************************************************************/

bool isValidAttribute( char *attr )
{
	int i ;					// used in for loop
	char attributes[1][2] = { "ID" } ;	// array that stores repository attributes
		
	for( i = 0 ; i < sizeof(attributes) / sizeof(attributes[0] ) ; i++ )
	{
		if( attr == NULL || ( strcmp( attributes[i], attr ) != 0 ) )
			return false ;
	}
	return true ;
}

/************************************************************************
 * Function:	isValidOperator						*
 *									*
 * Description:	Returns true if the operator in the query is a  	*
 *		operator of the repository. 				*
 ************************************************************************/

bool isValidOperator( char *op )
{
	int i ;					// used in for loop
	char operators[1][2] = { "EQ" } ;	// array that stores repository operators
	
	for( i = 0 ; i < sizeof(operators) / sizeof(operators[0] ) ; i++ )
	{
		if( op == NULL || ( strcmp( operators[i], op ) != 0 ) )
			return false ;
	}
	return true ;
}


/************************************************************************
 * Function:	isValidOperator						*
 *									*
 * Description:	Returns false if the value is NULL		  	*
 ************************************************************************/

bool isValidValue( char *val )
{
	if ( val == NULL )
		return false ;
	return true ;
}
