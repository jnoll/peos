/* 
**	Senior Design Project - PEOS Virtual Repository
**	Author : TASK4ONE
**	Filename : seeker.c
*/

#include "form.h"
#include "variables.h"
#include "resultLinkedList.h"
#include "queryLinkedList.h"
#include <ftw.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static queryList *theQueries ;

queryList* queryTool( queryList *listpointer )
{
	int getFile( const char *filename, const struct stat *statptr, int flag ) ;

	theQueries = listpointer ;
		
	zeroQueryList( theQueries ) ;
	
	ftw( ".", getFile, 5 ) ;
	
	return theQueries ;	
}
     
int getFile( const char *filename, const struct stat *statptr, int flag )
{
	char* position ;
	char target[100] ;
	int index = 0 ;
	queryList *tempQueries = theQueries ;
	
	while( tempQueries != NULL )
	{
		switch ( flag )
	 	{
			case FTW_F : 	position = strrchr(filename, '/' ) ; 
					index = filename - position; 

					if( index < 0 )
						index *= -1;

	    				strcpy( target, filename + index + 1 ) ;
	    									
					if( strncmp( tempQueries -> oneQuery -> myClauses[0].value, target,
					    strlen( tempQueries -> oneQuery -> myClauses[0].value ) ) == 0 )
					   
					{
						tempQueries -> oneQuery -> results = 
					 		addResultItem ( tempQueries -> oneQuery -> results, filename );
					    	tempQueries -> oneQuery -> numFound++ ;
   				    	}
   				    	
   					break ;
   		}
		tempQueries = ( queryList* ) tempQueries -> link ;
	}
	return 0 ;
}
