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
#include <stdio.h>
 
query *ftwQuery ;
char searchFile[100];
static char path[100] ;

queryList* FSqueryTool( queryList *listpointer )
{
	int getFile( const char *filename, const struct stat *statptr, int flag ) ;
	int done ;
	FILE *configFile ;
   	char line[100], *word, *repository ;
   	struct stat statBuffer ;
   	queryList *tempQueries ;
   	
   	done = 0 ;
   	if( !strlen(path) )
   	{
      		if( (configFile = fopen( "config.txt", "r" )) != NULL )
   		{
      			while( fgets( line, 100, configFile ) != NULL)
   			{
       				word = strtok( line, "=" ) ;
       				while( !done )
    				{
       					if( strncmp( "SEARCHDIR", word, 9 ) == 0 )
    					{
       						word = strtok( NULL, "\n" ) ;
   						strcpy( path, word ) ;
						done = 1 ;
					}
				}
    			}
        		fclose( configFile ) ;
  		}
  	}

	tempQueries = listpointer ;

	while( tempQueries != NULL )
	{
		strcpy( searchFile, tempQueries -> oneQuery -> myClauses[0].value ) ;
			
		repository = strtok( searchFile , ":" ) ;
		
		if( strncmp( "file", repository, 4 ) == 0 )
		{
			strcpy( searchFile, strpbrk( tempQueries -> oneQuery -> myClauses[0].value, "/" ) ) ;
			if( strchr( searchFile + 2, '/' ) != NULL )
			{
				if( stat( searchFile + 1, &statBuffer ) == 0 )
				{
					tempQueries -> oneQuery -> results = addResultItem ( tempQueries -> oneQuery -> results, 
											     tempQueries -> oneQuery -> myClauses[0].value ) ;
			    		tempQueries -> oneQuery -> numFound++ ;
			    		
				}
			}
			else
			{
				ftwQuery = tempQueries -> oneQuery ;
				strcpy(searchFile, searchFile + 2 ) ;
				ftw( path, getFile, 5 ) ;
			}		
		}		
		tempQueries = ( queryList* ) tempQueries -> link ;
	}
	return listpointer ;	
}
     
int getFile( const char *filename, const struct stat *statptr, int flag )
{
	char* position ;
	char target[100] ;
	int index = 0 ;

	switch ( flag )
	{
		case FTW_F : 	position = strrchr(filename, '/' ) ; 
				index = filename - position; 
				
				if( index < 0 )
					index *= -1;

    				strcpy( target, filename + index + 1 ) ;
	    											
				if( strncmp( searchFile, target, strlen( searchFile) ) == 0 )
				{
					ftwQuery -> results = addResultItem ( ftwQuery -> results, filename );
				    	ftwQuery -> numFound++ ;
				}
				break ;
	}
	
	return 0 ;
}

	

