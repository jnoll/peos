/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : EMAILmsgTokenizer.c					*
 ************************************************************************/

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
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main( void )
{
	int EMAILmsgTokenizer( char *mailPath ) ;
	
	char mailPath[] = "/var/spool/mail/XXX" ;  	// need a valid mail path to work
							// eg. /var/spool/mail/cgoh
	EMAILmsgTokenizer( mailPath ) ;
	
	return 0 ;
}

int EMAILmsgTokenizer( char *mailPath )
{
	int msgTokenizer( char *, char *,  FILE * ) ;
	
	FILE *mailFile ;
	
	if( ( mailFile = fopen( mailPath, "r" ) ) != NULL )
	{
		int msgCount = 0 ;			
		char *fromHeader = "From" ;
		char *msgHeader = "Message-Id:" ;
	
		msgCount = msgTokenizer( fromHeader, msgHeader, mailFile ) ;

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
 * Description:	reading of every line in the mailbox to determine each	*
 *		message boundary					*
 *									*
 *		returns the number of message-ids obtained		*
 *									*
 ************************************************************************/
 
int msgTokenizer( char *fromHeader, char *msgHeader,  FILE *mailFile )
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

