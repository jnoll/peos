/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : vrepo.c							*
 ************************************************************************/

#include "form.h"
#include "debug.h"
#include "variables.h"
#include "vrepo.h"
#include "FSseeker.h"
#include "resultLinkedList.h"
#include "queryLinkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BUFFER_SIZE 1000

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
	bool isValidOperator( char *, char * ) ; 
	bool isValidValue( char * ) ;
	bool isValidConjecture( char * ) ;
	int isValidQuery( query *, int ) ;
	
	int validated ;
	char *word, *toParse ; 			// tokens during string tokenizations 	
	int numParses, numClauses, numTokens ;	// keeps track of the token in the tokenizing phase
						// numClauses stores the number of clauses in the queryString
					 
	query *newQuery ; 			// stores the new query
	
	newQuery = ( query * ) malloc ( sizeof ( query ) ) ;
	numParses = numClauses = numTokens = 0 ;
	word = toParse = NULL ;
	
	toParse = strtok( queryString, "\n" ) ;	

	if( toParse != NULL )
		word = strtok( toParse, " " ) ;

	
	while( word != NULL )
	{
		numTokens++;
		
		if(numParses == 4)
		{
			numParses = 0;
			numClauses++;
		}
		
		switch( numParses )
		{
			case 0 :	if (isValidAttribute( word) )
					{
						newQuery -> myClauses[numClauses].attribute = strdup( word ) ;
						numParses++ ;
						_debug( __FILE__, __LINE__, 5, "Attribute is %s", word ) ;
					}
					break ;
						
					
			case 1 :	if( isValidOperator( word, newQuery -> myClauses[numClauses].attribute ) )
					{
						newQuery -> myClauses[numClauses].operator = strdup( word ) ;
						numParses++ ;
						_debug( __FILE__, __LINE__, 5, "Operator is %s", word ) ;
					}						
					break ;
					
			case 2 : 	if( isValidValue( word ) )
					{
						newQuery -> myClauses[numClauses].value = strdup( word ) ;
						numParses++ ;
						_debug( __FILE__, __LINE__, 5, "Value is %s", newQuery -> myClauses[numClauses].value ) ;
					}
					break ;
			
			case 3 :	if( isValidConjecture( word ) )
					{
						newQuery -> myClauses[numClauses].conjecture = strdup( word ) ;
						numParses++ ;
						_debug( __FILE__, __LINE__, 5, "Conjecture is %s", word ) ;
					}

					break ;
		}
		
		word = strtok( NULL, " " ) ;
	}
	
	_debug( __FILE__, __LINE__, 5, "numClauses is %d, numParses is %d, numTokens is %d, calculated numTokens is %d", 
					numClauses, numParses, numTokens, ( numClauses * 4 + numParses ) ) ;	
	
	validated = 0 ;	
	if( ( ( numClauses * 4 + numParses ) == numTokens ) && ( numParses == 3 ) )
	{		
		validated = isValidQuery( newQuery, numClauses ) ;	
		_debug( __FILE__, __LINE__, 5, "validated is %d", validated ) ;
	}
	
	if( validated )
	{	
		_debug( __FILE__, __LINE__, 5, "Storing Clause" ) ;
		newQuery -> callback = cback;
		newQuery -> data = d ;
		newQuery -> numFound = 0 ;
		newQuery -> removeTag = 0 ;
		newQuery -> numClauses = numClauses ;
		newQuery -> results = NULL ;
		newQuery -> myClauses[numClauses].conjecture = NULL ;
		myQueries = addQueryItem( myQueries, newQuery ) ;
		numParses = 0 ;
	}
	else
	{
		if( numParses != 0 )
		{
			int k ;
								
			for( k = 0 ; k != numClauses; k++ )
			{
				free( newQuery -> myClauses[k].attribute );
				free( newQuery -> myClauses[k].operator ) ;
				free( newQuery -> myClauses[k].value ) ;
				free( newQuery -> myClauses[k].conjecture ) ;
			}
			
			for( k = 0 ; k < numParses ; k++ )
			{
				switch( k )
				{
					case 0 : free( newQuery -> myClauses[numClauses].attribute ) ;
						 break ;
					
					case 1 : free( newQuery -> myClauses[numClauses].operator ) ;
						 break ;
					
					case 2 : free( newQuery -> myClauses[numClauses].value ) ;
						 break ;
						 
					case 3 : free( newQuery -> myClauses[numClauses].conjecture ) ;
						 break ;
				}
			}
			free( newQuery ) ;
			printf( "invalid query...\n" ) ;
		}
		else
			printf( "empty query...\n" ) ;			
	}
	_debug( __FILE__, __LINE__, 5, "exiting query_wait( )" ) ;
}

int isValidQuery( query *theQuery, int numClauses )
{
	int i ;
	int validateTimeStamp(char *timeStamp);
	char testValue[BUFFER_SIZE] = { '\0' } ;
	int validResult = 1 ;
	
	_debug( __FILE__, __LINE__, 5, "numClauses is %d", numClauses ) ;
	for( i = 0 ; i <= numClauses ; i++ )
	{
		if( strcmp( "ID", theQuery -> myClauses[i].attribute ) == 0 ) 
		{
			strcpy ( testValue ,theQuery -> myClauses[i].value ) ;
			_debug( __FILE__, __LINE__, 5, "query Value is %s : %d", theQuery -> myClauses[i].value, strlen( theQuery -> myClauses[i].value ) ) ;
			_debug( __FILE__, __LINE__, 5, "testValue is %s : %d", testValue, strlen( testValue ) ) ;
			if( isFileQuery( testValue ) )
				validResult = FSqueryValidator ( testValue ) ;
			else if( isEMAILQuery( testValue ) )
				validResult = EMAILqueryValidator ( testValue ) ;
			else
				validResult = 0 ;
		}
		
		else if( strcmp( "DATE", theQuery -> myClauses[i].attribute ) == 0 ) 
		{
			
			validResult = validateTimeStamp(theQuery -> myClauses[i].value);
			
		}
			
	}
	return validResult ;
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

	_debug( __FILE__, __LINE__, 5, "entering poll_vr( )" ) ;
	
	if( myQueries != NULL )
	{
		
		for(i=0; i<repos_ctr ; i++)
			myQueries = repos_list[i].queryTool(myQueries);
		
		tempQueries = myQueries ;
	
		while( tempQueries != NULL )			
		{
			if( tempQueries -> oneQuery -> numFound )
			{
				tag = 1 ;
				tempQueries -> oneQuery -> callback( tempQueries -> oneQuery -> numFound,
							             tempQueries -> oneQuery -> results,
							             tempQueries -> oneQuery -> data ) ;
				tempQueries -> oneQuery -> removeTag = 1 ;
			}
			
			else if( tempQueries -> oneQuery -> removeTag )
			{
				tag = 1 ;
			}
			
			tempQueries = ( queryList* ) tempQueries -> link ;
		}
	
		if ( tag )
			myQueries = filterQueryList( myQueries ) ;
	
	}
	_debug( __FILE__, __LINE__, 5, "exiting poll_vr( )" ) ;
}

/************************************************************************
 * Function:	isValidAttribute					*
 *									*
 * Description:	Returns true if the attribute in the query is an  	*
 *		attribute of the repository. 				*
  ************************************************************************/

bool isValidAttribute( char *attr )
{
	int i ;						// used in for loop
	char *attributes[3] = { "ID","DATE", "NAME" } ;	// array that stores repository attributes
		
	if( attr == NULL )
		return false;
		
	for( i = 0 ; i < 3 ; i++ )
	{
		if( ( strcmp( attributes[i], attr ) == 0 ) )
			return true ;
	}     
	return false ;
}

/************************************************************************
 * Function:	isValidOperator						*
 *									*
 * Description:	Returns true if the operator in the query is a  	*
 *		operator of the repository. 				*
 ************************************************************************/

bool isValidOperator( char *op, char *attr )
{
	int i ;							// used in for loop
	
			
	if( op == NULL )
		return false;
		
	if( strcmp( "ID", attr ) == 0 )
	{
		char *operators[1] = { "EQ" } ;	
		for( i = 0 ; i < 1 ; i++ )
		{
			if( ( strcmp( operators[i], op ) == 0 ) )
				return true ;
		}
	}
	else if( strcmp ( "NAME", attr ) == 0 )
	{
		char *operators[2] = { "EQ", "~" } ;
		for( i = 0 ; i < 2 ; i++ )
		{
			if( ( strcmp( operators[i], op ) == 0 ) )
				return true ;
		}
	}
	else  if( strcmp ( "DATE", attr ) == 0 )
	{
		char *operators[5] = { "EQ", "LT", "LE", "GT", "GE" } ;
		for( i = 0 ; i < 5 ; i++ )
		{
			if( ( strcmp( operators[i], op ) == 0 ) )
				return true ;
		}
	}
	
	return false;
}


/************************************************************************
 * Function:	isValidValue						*
 *									*
 * Description:	Returns false if the value is NULL		  	*
 ************************************************************************/

bool isValidValue( char *val )
{
	if ( val == NULL )
		return false ;
	return true ;
}

/************************************************************************
 * Function:	isValidConjecture					*
 *									*
 * Description:	Returns true if the operator in the query is a  	*
 *		operator of the repository. 				*
 ************************************************************************/

bool isValidConjecture( char *con )
{
	int i ;						// used in for loop
	char *conjecture[2] = { "AND","OR" } ;	// array that stores repository operators
	
	if( con == NULL ) 
		return false;
	
	for( i = 0 ; i < 2; i++ )
	{
		_debug(__FILE__,__LINE__,5,"in conjecture con is %s conjecture[i] is %s ", con, conjecture[i]);
		if( ( strcmp( conjecture[i], con ) == 0 ) )
			return true ;
	}
	return false ;
}

int validateTimeStamp(char *temp)
{
	
	
	_debug(__FILE__,__LINE__,5,"(temp+2)[0] == %c && (temp+5)[0] == %c && 
				    (temp+10)[0] == %c && (temp+13)[0] == %c && 
				    (temp+16)[0]  == %c && (temp+19)[0]  == %c ",(temp+2)[0],(temp+5)[0], (temp+10)[0],(temp+13)[0],(temp+16)[0] , (temp+19)[0]);
	if( (temp+2)[0] =='/' && (temp+5)[0] == '/' && (temp+10)[0] == '-' && (temp+13)[0] == ':' && (temp+16)[0]  == ':' && (temp+19)[0]  == '\0')
		return 1;
	else 
		return 0;
}
	
