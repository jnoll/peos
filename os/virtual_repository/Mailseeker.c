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
	void getMailPath( char *mailPath, char *value );
	int is_email(char *value);
	int EMAILmsgTokenizer( char *mailPath );
	
	int doneSearchBox ;
	struct stat statBuffer ;	
	FILE *mailFile ;
	char *repository ;		
	char oneLine[100] ;
	char value[100] = { '\0' } ;
	char searchMsgID[100] ;
	char mailPath[100] = { '\0' } ;
	   	
   	queryList *tempQueries ;	// a pointer to the querylist arguement
   	
   	tempQueries = listpointer ;
   	getMailPath( mailPath, value) ;
   	
   	while(tempQueries != NULL)
   	{
		if( is_email( tempQueries -> oneQuery ->myClauses[0].value) )
		{
			if( mailPath == NULL )
			
				/*do nothing */;
			else
			{
				printf( "mailPath in seeker %s\n", mailPath ) ;	
				
				EMAILmsgTokenizer( mailPath );
			}
				
			tempQueries = ( queryList* ) tempQueries -> link ;
			
		}
	}
	return listpointer ;
}

/************************************************************************
 * Function:	is_email						*
 *									*
 * Description:	Takes in the Value from the query andchecks whether it	*
 *		is to be satisfied by the email repository. If the	*
 *		repository in value specifies "email" the function	*
 *		returns 1 else it returns 0.				*

************************************************************************/

int is_email(char *value)
{
	char *repository ;		// a token that defines the repository type of the query request
	char tempValue[100] ;
	
	strcpy( tempValue, value ) ;
	
	repository = strtok(tempValue , ":" ) ;
						
		if( strcmp( "email", repository) == 0 )
			return 1;
		else
			return 0;
}


/************************************************************************
 * Function:	getMailPath						*
 *									*
 * Description:	Takes in a string mailPath(NULL) and the value from the *
 *		query and returns the appropriate mailPath for the	*
 		repository.						*

************************************************************************/

void getMailPath(char *mailPath, char *value )
{
	int doneSearchBox ;
	FILE *configFile ;		
	char oneLine[100] ;		
	char *word ;			
	char tempValue[100] ;
	char searchBox[100] = { '\0' } ;
	int numslash = 1;
   	
	strcpy( tempValue, value + 6) ;
	
	while( strncmp( tempValue+numslash, "/", 1 ) == 0 )
		numslash++;
		
		
	switch(numslash)
	{
		case 2: 
			doneSearchBox = 0 ;
   			if( ( configFile = fopen( "config.txt", "r" ) ) != NULL )
			{	
				while( fgets( oneLine, 100, configFile ) != NULL || !doneSearchBox )
   				{
   					word = strtok( oneLine, "=" ) ;
					if( strncmp( "MAILBOX", word, 7 ) == 0 )
    					{
       						word = strtok( NULL, "\n" ) ;
   						strcpy( searchBox, word ) ;
   						printf( "searchbox is %s \n",searchBox ) ;
   						doneSearchBox = 1 ;
					}
	    			}
        			fclose( configFile ) ;
        			strcpy( mailPath, searchBox ) ;
				strcat( mailPath, tempValue + 1 ) ;
        			
        			break;
  			}
  			//else
  				//return NULL;
  			
  		case 3:
  			strcpy( mailPath, tempValue + 2 ) ;
  			break;
  			
  		default: puts("Invalid Query");
			
	}		
	
	//return mailPath;
}

/************************************************************************
 * Function:	EMAILidCompare						*
 *									*
 * Description:	Given mail message and mail query,
returns 1 if both 	*
 *		matches. Otherwise returns 0.				*

************************************************************************/
 
int EMAILidCompare ( char *mailQuery, char *mailHeader
) 
{
	char testHeader[100] = { '\0' } ;
	char msg[100] = { '\0' } ;
	char *ptrMsg1, *ptrMsg2 , *id ;
	int length ;
	
	strcpy ( msg, mailHeader ) ;
		
        id = strtok( msg, ":" );

	if ( strcmp ( "Message-Id", id ) == 0 )
	{
		ptrMsg1 = strchr ( mailHeader, '<' ) ;
		ptrMsg2 = strrchr ( mailHeader , '>' ) ;
		length = ptrMsg2 - ptrMsg1 ;
	}
	
	strncpy ( testHeader, ptrMsg1 +1, length-1 ) ;
		
	if ( strcmp ( mailQuery, testHeader ) == 0 )
	{
		printf ( "found - %s\n", testHeader ) ;
		return 1 ;
	}
	else
	{
		printf ( "NOT found - %s\n", testHeader  ) ;
		return 0 ;
	}
}

/************************************************************************
 * Function:	EMAILmsgTokenizer					*
 *									*
 * Description:	calls msgTokenizer to tokenize the individual messages 	*
 *		of the unix mailbox					*
 *									*
 *		returns the number of message-ids obtained		*
 *		if mailPath is a valid path				*
 *									*
 *		returns -1 if the mailPath is NOT a valid path		*
 *									*
************************************************************************/
 
int EMAILmsgTokenizer( char *mailPath )
{
	int msgTokenizer( char *, char *,  FILE * ) ;
	
	FILE *mailFile ;
	
	if( ( mailFile = fopen( mailPath, "r" ) ) != NULL )
	{
		int msgCount = 0 ;			
		char *fromHeader = "From" ;
		char *msgHeader = "Message-Id:" ;
	
		msgCount = msgTokenizer( fromHeader, msgHeader,
mailFile ) ;

		printf( "total messages read: %d\n", msgCount ) ;

		fclose( mailFile ) ;		
		return msgCount ;
	}			
	else
		return -1 ;
}

/************************************************************************
 * Function:	msgTokenizer						*
 *									*
 * Description:	reading of every line in the mailbox
to determine each	*
 *		message boundary					*
 *									*
 *		returns the number of message-ids obtained		*
 *									*

************************************************************************/
 
int msgTokenizer( char *fromHeader, char *msgHeader, 
FILE *mailFile )
{
	char *token ;
	char oneLine[100] = {'\0'} ;
	char testLine[100] = {'\0'} ;
	int msgStage, msgCount, lineNumber, blankLine, fromLine ;
	msgCount = blankLine = fromLine = 0 ;
	
	msgStage = lineNumber = 1 ; // first message is without blank
		
	while( fgets( oneLine, 100, mailFile ) != NULL )
	{
		strcpy( testLine, oneLine ) ;
					
		switch( msgStage )
		{
			case 0 : 	if( ( strlen( testLine ) == 1 ) && ( strcmp( "\n", testLine ) == 0 ) )
					{
						//printf( "blank line found...\n" ) ;
						blankLine = lineNumber ;
						msgStage++ ;
					}
					break ;
						
			case 1 :	token = strtok( testLine , " " ) ;
					if( strcmp( fromHeader, token ) == 0 )
					{
						fromLine = lineNumber ;
						if( fromLine - blankLine == 1 )
						{
							//printf( "new message...\n" ) ;
							msgStage++ ;
							msgCount++ ;
						}
						else	
							msgStage = 0 ;
					}
					else
					{
						if( ( strlen( testLine ) == 1 ) && ( strcmp( "\n", testLine ) == 0 ) )
						{
							//printf( "blank line found...\n" ) ;
							blankLine = lineNumber ;
						}
						else
							msgStage = 0 ;
					}
					break ;
						
			case 2 :	token = strtok( testLine , " " ) ;
					if( strcmp( msgHeader, token ) == 0 )
					{
						oneLine[ strlen( oneLine ) - 1 ] = '\0' ;
						
						// do what you want with the message-ID
						printf( "%d. %s\n", msgCount, oneLine ) ;
						
						msgStage = 0 ;
					}
					break ;
		}
		lineNumber++ ;
	}							
	return msgCount ;
}

