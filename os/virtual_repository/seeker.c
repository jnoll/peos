/* 
**	Senior Design Project - PEOS Virtual Repository
**	Author : TASK4ONE
**	Filename : seeker.c
*/

#include "form.h"
#include "variables.h"
#include "resultLinkedList.h"
#include <ftw.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void queryTool( )
{
	int getFile( const char *filename, const struct stat *statptr, int flag ) ;
	
	int i ;
	
	for (i = 0; i < numQueries; i++)
	{
		Queries[i].numFound = 0 ;
	}
		
	ftw( ".", getFile, 5 ) ;
}
     
int getFile( const char *filename, const struct stat *statptr, int flag )
{
	char* position ;
	char target[100] ;
	int index = 0 ;
	int i;
	int temp_numQueries;
	
	temp_numQueries = numQueries;
	
	for (i = 0; i < temp_numQueries; i++)
	{
		switch (flag)
	 	{
			case FTW_F : 	position = strrchr(filename, '/' ) ; 
					index = filename - position; 

					if( index < 0 )
						index *= -1;

	    				strcpy( target, filename + index + 1 ) ;
					
					if( strncmp( Queries[i].myClauses[0].value, target,
					    strlen( Queries[i].myClauses[0].value ) ) == 0 )
					   
					{
					 	Queries[i].results = AddItem (Queries[i].results, filename );
					    	Queries[i].numFound++ ;
   				    	}
					break;
		}
	}
	return 0 ;
}



