/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : Mailseeker.c						*
 ************************************************************************/

#include "form.h"
#include "variables.h"
#include "debug.h"
#include "resultLinkedList.h"
#include "queryLinkedList.h"
#include <ftw.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

/************************************************************************
 * Function:	MailqueryTool						*
 *									*
 * Description:	Taking queries with mail: request and searching for 	*
 *		matching senders in the unix mail system.		*
 ************************************************************************/

queryList* MailqueryTool( queryList *listpointer )
{
	char* getMailPath( char * ) ;
	
	int doneSearchBox ;
	struct stat statBuffer ;	
	FILE *mailFile ;
	char *repository ;		
	char oneLine[100] ;
	char searchMsgID[100] ;
	char mailPath[100] = { '\0' } ;
	   	
   	queryList *tempQueries ;	// a pointer to the query list arguement
   	
   	tempQueries = listpointer ;
   	
	getMailPath( mailPath ) ; 
	if( mailPath == NULL )
		printf( "no mail path found...\n" ) ;
	
	else
	{
		printf( "mailPath in seeker %s\n", mailPath ) ;	
		while( tempQueries != NULL )
		{
			strcpy( searchMsgID, tempQueries -> oneQuery -> myClauses[0].value ) ;
			repository = strtok( searchMsgID , ":" ) ;
			if( strncmp( "mail", repository, 4 ) == 0 )
			{
				if( ( mailFile = fopen( mailPath, "r" ) ) != NULL )
				{
					while( fgets( oneLine, 100, mailFile ) != NULL )
	   					printf( "%s\n", oneLine ) ;
        				fclose( mailFile ) ;
        	  		}
  				else
  					printf( "error opening %s\n", mailPath ) ;
			}	
		tempQueries = ( queryList* ) tempQueries -> link ;
		}
	}
	return listpointer ;
}
   	
char* getMailPath(char *mailPath )
{
	int doneSearchBox ;
	struct stat statBuffer ;	
	FILE *configFile ;		
	char oneLine[100] ;		
	char *word ;			
	char directoryPath[] = "/var/spool/mail/" ;
	char searchBox[100] = { '\0' } ;
   	
	doneSearchBox = 0 ;
   	if( !strlen( searchBox ) )
   	{
		if( ( configFile = fopen( "config.txt", "r" ) ) != NULL )
		{
      			while( fgets( oneLine, 100, configFile ) != NULL || !doneSearchBox )
   			{
   				word = strtok( oneLine, "=" ) ;
				if( strncmp( "MAILBOX", word, 7 ) == 0 )
    				{
       					word = strtok( NULL, "\n" ) ;
   					strcpy( searchBox, word ) ;
   					doneSearchBox = 1 ;
				}
	    		}
        		fclose( configFile ) ;
  		}
	}
  	
  	strcat( mailPath, directoryPath ) ;
	strcat( mailPath, searchBox ) ;
	printf( "mailPath is %s\n", mailPath ) ;
	if( stat( mailPath, &statBuffer ) == 0 )
	{
		printf( "mailPath is %s...\n", mailPath ) ;
		return mailPath ;	
	}
	else
		return NULL ;

}


	

