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

static char *path = NULL ;
query *ftwQuery ;
char *ftwValue;

queryList* FSqueryTool( queryList *listpointer )
{
	int getFile( const char *filename, const struct stat *statptr, int flag ) ;
	
	int done ;
	FILE *configFile ;
   	char line[100], *word, *rep_type, *temp_value ;
   	struct stat statBuffer ;
   	queryList *theQueries ;
   	
   	done = 0 ;

   	if( path == NULL )
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
    						path = (char *) malloc ( strlen( word ) * sizeof( char ) ) ;
						strcpy( path, word ) ;
						done = 1 ;
					}
				}
    			}
        		fclose( configFile ) ;
  		}
  	}

	theQueries = listpointer ;
		
	
	
	while( theQueries != NULL )
	{
		
		temp_value =  (char *) malloc ( strlen( theQueries -> oneQuery -> myClauses[0].value ) * sizeof( char ) ) ;
		strcpy(temp_value, theQueries -> oneQuery -> myClauses[0].value ) ;
		rep_type = strtok( temp_value , ":" ) ;
		
		
		if( strncmp( "file", rep_type, 4 ) == 0 )
		{
			puts(theQueries -> oneQuery -> myClauses[0].value);
			puts("repository type is file");
			
					
			strcpy(temp_value, strpbrk(theQueries -> oneQuery -> myClauses[0].value,"/"));
				
			
			
			if( strchr(temp_value+2, '/' ) != NULL )
			{
				
				
				if( stat( temp_value+1, &statBuffer ) == 0 )
				{
					puts("opening by stat");
					theQueries -> oneQuery -> results = 
						addResultItem ( theQueries -> oneQuery -> results, 
								theQueries -> oneQuery -> myClauses[0].value ) ;
			    		theQueries -> oneQuery -> numFound++ ;
				}
			}
			else
			{
				ftwQuery = theQueries -> oneQuery ;
				ftwValue = (char *) malloc ( strlen( temp_value+1 ) * sizeof( char ) );
				strcpy(ftwValue, temp_value+2 ) ;
				
				
			
				ftw( path, getFile, 5 ) ;
				
				free(ftwValue);
				
			}		
			theQueries = ( queryList* ) theQueries -> link ;
			
			
			
			
		}
		free(temp_value);
		
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
	    									
				//if( strncmp( ftwQuery -> myClauses[0].value, target,
				//	     strlen( ftwQuery -> myClauses[0].value ) ) == 0 )
				
				if( strncmp( ftwValue, target,
					     strlen( ftwValue) ) == 0 )
				{
					puts("opening by ftw");
					ftwQuery -> results = addResultItem ( ftwQuery -> results, filename );
				    	ftwQuery -> numFound++ ;
				}
				break ;
	}
	return 0 ;
}

	

