/* 
**	Senior Design Project - PEOS Virtual Repository
**	Author : TASK4ONE
**	Filename : vrepo.c
*/

#include "form.h"
#include "variables.h"
#include "vrepo.h"
#include "seeker.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void query_wait( char *queryString, void (*cback)(int, char[][], int *), int *d  )
{
	bool isValidOp( char *op ) ; 
	
	char *word ;
	char *toParse ;
	int i, j ;

	toParse = strtok( queryString, "\n" ) ;	// from keyboard input, otherwise remove this line...
	
	i = j = 0 ;	// i to keep track of data content, j to keep track of num clauses
	word = strtok( toParse, " " ) ;
	while( word != NULL )
	{
		switch( i )
		{
			case 0 :	strcpy( Queries[numQueries].myClauses[j].attribute, word ) ;
					break ;
			case 1 :	if( isValidOp( word ) )
						strcpy( Queries[numQueries].myClauses[j].operator, word ) ;
					else
						printf( "invalid operator!" ) ;
					break ;
			case 2 :	strcpy( Queries[numQueries].myClauses[j].value, word ) ;
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

	Queries[numQueries].numClauses = j ;
	Queries[numQueries].callback = cback;
	Queries[numQueries].data = d ;
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
	int i;
	int temp_numQueries;
	void queryTool( ) ;
	void deregister(int index);
	
	temp_numQueries = numQueries;
	queryTool();
	
	for(i = 0; i < temp_numQueries ; i++)
	{
		if(Queries[i].numFound )
		{
			
			Queries[i].callback(Queries[i].numFound, Queries[i].results, Queries[i].data);
			printf( "%d record(s) found!\n", Queries[i].numFound ) ;
			printf( "%s\n", Queries[i].results[0] ) ;
			deregister(i);
		}
	}
	
}

void deregister(int index)
{
	int i;
	int temp_numQueries;
	
	temp_numQueries = numQueries;
	
	for (i = index; i < temp_numQueries -1; i++)
		Queries[i] = Queries[i+1];
	
	numQueries --;
}
