/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : EMailseeker.c						*
 ************************************************************************/

#include "form.h"
#include "variables.h"
#include "debug.h"
#include "resultLinkedList.h"
#include "queryLinkedList.h"
#include "seekerTools.h"
#include "EMAILseeker.h"
#include <ftw.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

#define BUFFER_SIZE 1000

char operatorType[10] ;
time_t mailQueryTime ;

/************************************************************************
 * Function:	EMailqueryTool						*
 *									*
 * Description:	Taking queries with mail: request and searching for 	*
 *		matching senders in the unix mail system.		*
 ************************************************************************/

queryList* EMAILqueryTool( queryList *listpointer )
{
	int isEMAILQuery( char * ) ;
	void getMailPath( char *, char *, queryList * ) ;
	void getMAilDate ( char * ) ;
	resultList* EMAILmsgTokenizer( char *, char *, int ) ;
	
	int doneSearchBox, msgResult, numClauses, dateResult, attributeType ;
	struct stat statBuffer ;	
	FILE *mailFile ;
	char *repository ;		
	char oneLine[BUFFER_SIZE] = { '\0' } ;
	char searchMsgID[BUFFER_SIZE] = { '\0' } ;
	char mailPath[BUFFER_SIZE] = { '\0' } ;
	   	
   	queryList *tempQueries ;	// a pointer to the querylist arguement
   	resultList *tempResults ;
   	
   	tempQueries = listpointer ;   	
   	tempResults = NULL ;
   	   	
	while( tempQueries != NULL )
	{
		msgResult = 0 ;
		
		for( numClauses = 0 ; numClauses <= tempQueries -> oneQuery -> numClauses ; numClauses++ )
		{
   			if ( strcmp ( tempQueries -> oneQuery ->myClauses[numClauses].attribute, "DATE" ) == 0 )
   			{
   				attributeType = 2 ;
   				strcpy ( operatorType , tempQueries -> oneQuery ->myClauses[numClauses].operator ) ;
   				getPath( mailPath, "MAILBOX" ) ;
   				
   				if( numClauses == 0 )
					tempResults = EMAILmsgTokenizer( mailPath, tempQueries -> oneQuery ->myClauses[numClauses].value, attributeType ) ;
				else
				{
					if( strcmp( tempQueries -> oneQuery -> myClauses[numClauses-1].conjecture, "AND" ) == 0 )
						tempResults = andResult( tempResults, EMAILmsgTokenizer( mailPath,tempQueries -> oneQuery ->myClauses[numClauses].value, attributeType ) ) ;

					if( strcmp( tempQueries -> oneQuery -> myClauses[numClauses-1].conjecture, "OR" ) == 0 )
						tempResults = orResult(tempResults, EMAILmsgTokenizer( mailPath,tempQueries -> oneQuery ->myClauses[numClauses].value, attributeType ) ) ;
				}
			} 
			
			if ( strcmp ( tempQueries -> oneQuery ->myClauses[numClauses].attribute, "NAME" ) == 0 )
   			{
   				attributeType = 3 ;
   				strcpy ( operatorType , tempQueries -> oneQuery ->myClauses[numClauses].operator ) ;
   				getPath( mailPath, "MAILBOX" ) ;
   				
   				if( numClauses == 0 )
					tempResults = EMAILmsgTokenizer( mailPath,tempQueries -> oneQuery ->myClauses[numClauses].value, attributeType ) ;
				else
				{
					if( strcmp( tempQueries -> oneQuery -> myClauses[numClauses-1].conjecture, "AND" ) == 0 )
						tempResults = andResult( tempResults, EMAILmsgTokenizer( mailPath,tempQueries -> oneQuery ->myClauses[numClauses].value, attributeType ) ) ;

					if( strcmp( tempQueries -> oneQuery -> myClauses[numClauses-1].conjecture, "OR" ) == 0 )
						tempResults = orResult(tempResults, EMAILmsgTokenizer( mailPath,tempQueries -> oneQuery ->myClauses[numClauses].value, attributeType ) ) ;
				}
			}
			
			if ( strcmp ( tempQueries -> oneQuery ->myClauses[numClauses].attribute, "ID" ) == 0 )
			{
				attributeType = 1 ;

				if( isEMAILQuery( tempQueries -> oneQuery ->myClauses[numClauses].value) )
				{
					getMailPath( mailPath, tempQueries -> oneQuery ->myClauses[numClauses].value, tempQueries ) ;

					if( mailPath != NULL )
   					{
						if( numClauses == 0 )
							tempResults = EMAILmsgTokenizer( mailPath,tempQueries -> oneQuery ->myClauses[numClauses].value, attributeType ) ;
						else
						{
							if( strcmp( tempQueries -> oneQuery -> myClauses[numClauses-1].conjecture, "AND" ) == 0 )
								tempResults = andResult( tempResults, EMAILmsgTokenizer( mailPath,tempQueries -> oneQuery ->myClauses[numClauses].value, attributeType ) ) ;
							
							if( strcmp( tempQueries -> oneQuery -> myClauses[numClauses-1].conjecture, "OR" ) == 0 )
								tempResults = orResult(tempResults, EMAILmsgTokenizer( mailPath,tempQueries -> oneQuery ->myClauses[numClauses].value, attributeType ) ) ;
						}
					}
				}
			
			}
		}
	
		while ( tempResults != NULL )
		{
			tempQueries -> oneQuery -> results = addResultItem( tempQueries -> oneQuery -> results, tempResults -> oneResult ) ;
			tempQueries -> oneQuery -> numFound++ ;
			tempResults = ( resultList* ) tempResults -> link ;
		}
		
		tempQueries = ( queryList* ) tempQueries -> link ;
	}
	return listpointer ;
}

/************************************************************************
 * Function:	isEMAILQuery						*
 *									*
 * Description:	Takes in the Value from the query and checks whether it	*
 *		is to be satisfied by the email repository. If the	*
 *		repository in value specifies "email" the function	*
 *		returns 1 else it returns 0.				*
 ************************************************************************/

int isEMAILQuery( char *value )
{
	char *repository ;		// a token that defines the repository type of the query request
	char tempValue[BUFFER_SIZE] = { '\0' } ;
	
	strcpy( tempValue, value ) ;
	
	repository = strtok(tempValue , ":" ) ;
	_assert( __FILE__, __LINE__, repository ) ;
						
	if( strcmp( "email", repository) == 0 )
		return 1 ;
	else
		return 0 ;
}

/************************************************************************
 * Function:	getMailPath						*
 *									*
 * Description:	Takes in a string mailPath(NULL) and the value from the *
 *		query and returns the appropriate mailPath for the	*
 *		repository.						*
 ************************************************************************/

void getMailPath( char *mailPath, char *value, queryList *tempQueries )
{
	FILE *configFile ;		
	char *word, *ptrPath ;			
	char oneLine[BUFFER_SIZE] = { '\0' } ;		
	char tempValue[BUFFER_SIZE] = { '\0' } ;
	char searchBox[BUFFER_SIZE] = { '\0' } ;
	int doneSearchBox ;
	int numslash = 1 ;
   	
	strcpy( tempValue, value + 6) ;
		
	while( strncmp( tempValue + numslash, "/", 1 ) == 0 )
		numslash++;
			
	switch( numslash )
	{
		case 2	:	getPath( mailPath, "MAILBOX" ) ;
				break;
  			
  		case 3	:	ptrPath = rindex( tempValue, '/' ) ;
	  			_assert( __FILE__, __LINE__, ptrPath ) ;
  				strncpy( mailPath, tempValue + 2, ptrPath - ( tempValue + 2 ) ) ;
  				break;
  			
  		default	:	tempQueries -> oneQuery ->removeTag = 1 ;
  				break ;
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

resultList* EMAILmsgTokenizer( char *mailPath, char *queryValue, int attributeType )
{
	resultList* msgTokenizer( FILE *, char *, char *, int ) ;
	
	FILE *mailFile ;
	
	if( ( mailFile = fopen( mailPath, "r" ) ) != NULL )
	{
		resultList* theResults ;
	
		theResults = msgTokenizer( mailFile, queryValue, mailPath, attributeType ) ;
					
		fclose( mailFile ) ;		
		return theResults ;
	}			
	else
		return NULL ;
}

/************************************************************************
 * Function:	msgTokenizer						*
 *									*
 * Description:	reading of every line in the mailbox to determine each	*
 *		message boundary					*
 *									*
 *		returns 1 if message-id is found, else returns 0	*
 *									*
 ***********************************************************************/
 
resultList* msgTokenizer( FILE *mailFile, char *queryValue, char *mailPath, int attributeType )
{
	int EMAILidCompare ( char *, char * ) ;
	int EMAILdateCompare ( char *, char * ) ;
	void getMessageID( char *, char * ) ;	

	char *token ;
	char oneLine[BUFFER_SIZE] = {'\0'} ;
	char testLine[BUFFER_SIZE] = {'\0'} ;
	char messageLine[BUFFER_SIZE] = {'\0'} ;
	char messageID[BUFFER_SIZE] = {'\0'} ;
	char dateLine[BUFFER_SIZE] = {'\0'} ;
	char subjectLine[BUFFER_SIZE] = {'\0'} ;
	char fromWhom[BUFFER_SIZE] = {'\0'} ;
	char previousFromWhom[BUFFER_SIZE] = {'\0'} ;
	int lineNumber, blankLine, fromLine, msgResult, numAttributes, readSubjectLine ;
	resultList *tempResults ;
	
	tempResults = NULL ;
	lineNumber = blankLine = fromLine = msgResult = numAttributes = readSubjectLine = 0 ;
			
	while( ( fgets( oneLine, 500, mailFile ) != NULL ) && ( !msgResult ) ) 
	{
		lineNumber++ ;
	
		strcpy( testLine, oneLine ) ;
		
		if( ( strlen( testLine ) == 1 ) && ( strcmp( "\n", testLine ) == 0 ) )
			blankLine = lineNumber ;
		else
		{		
			token = strtok( testLine , " " ) ;
			_assert( __FILE__, __LINE__, token ) ;
						
			if( strcmp( "From", token ) == 0 )
			{
				fromLine = lineNumber ;
				if( fromLine - blankLine == 1 )
				{
					strcpy( previousFromWhom, fromWhom ) ;
					strcpy( fromWhom, oneLine ) ;
					if( numAttributes == 2  )
					{
						getMessageID( messageID, messageLine ) ;
										
						switch ( attributeType ) 					
						{
							case 1	:	msgResult = EMAILidCompare ( queryValue, messageID ) ;
									break ;
											
							case 2	:	msgResult = EMAILdateCompare ( queryValue, dateLine ) ;
									break ;
					
							case 3	:	msgResult = EMAILsubjectCompare ( queryValue, subjectLine ) ;
									subjectLine[0] = '\0' ;
									break ;				
						}
						
						numAttributes = readSubjectLine = 0 ;
																		
						if( msgResult )
						{
							char resultPath[BUFFER_SIZE] = { '\0' } ;
							
							sprintf( resultPath, "email://%s/%s", mailPath, messageID ) ;
							tempResults = addResultItem( tempResults, resultPath ) ;
							msgResult = 0 ;
						}

					}
					else					
						numAttributes = readSubjectLine = 0 ;
				}
			}

			if( ( ( strcmp( "Message-Id:", token ) == 0 ) ||
			      ( strcmp( "Message-ID:", token ) == 0 ) ||
			      ( strcmp( "Message-id:", token ) == 0 ) ) &&
			      ( numAttributes != 2 ) )
			{
				oneLine[ strlen( oneLine ) - 1 ] = '\0' ;
			    	strcpy( messageLine, oneLine ) ;
			    	numAttributes++ ;
			}
			
			if( ( strcmp( "Date:", token ) == 0 ) && ( numAttributes != 2 ) )
			{
				oneLine[ strlen( oneLine ) - 1 ] = '\0' ;
				strcpy( dateLine, oneLine ) ;
				numAttributes++ ;
			}
			
			if( ( strcmp( "Subject:", token ) == 0 ) && !readSubjectLine )
			{
				oneLine[ strlen( oneLine ) - 1 ] = '\0' ;
				strcpy( subjectLine, oneLine ) ;
			    	readSubjectLine = 1 ;
			}
		}
		
		
	}
	
	if( numAttributes == 2 )
	{
		getMessageID( messageID, messageLine ) ;
		switch ( attributeType ) 					
		{
			case 1	:	msgResult = EMAILidCompare ( queryValue, messageID );
					break ;
											
			case 2	:	msgResult = EMAILdateCompare ( queryValue, dateLine ) ;
					break ;
					
			case 3	:	msgResult = EMAILsubjectCompare ( queryValue, subjectLine ) ;
					break ;				
		}
		numAttributes = 0 ;
				
		if( msgResult )
		{
			char resultPath[BUFFER_SIZE] = { '\0' } ;
							
			sprintf( resultPath, "email://%s/%s", mailPath, messageID ) ;
			tempResults = addResultItem( tempResults, resultPath ) ;
				
			msgResult = 0 ;
		}
	}
								
	return tempResults ;
}

/************************************************************************
 * Function:	getMessageID						*
 *									*
 * Description:	Given mail message line, function gets the value of the *
 *		message id.						*
 ************************************************************************/
 
void getMessageID( char *idBuffer, char *messageLine )
{
	char msg[BUFFER_SIZE] = { '\0' } ;
	char *ptrMsg1, *ptrMsg2 ;
	int length = 0 ;
	
	strcpy ( msg, messageLine ) ;
	
	ptrMsg1 = strchr ( msg, '<' ) ;
	_assert( __FILE__, __LINE__, ptrMsg1 ) ; 
	ptrMsg2 = strrchr ( msg , '>' ) ;
	_assert( __FILE__, __LINE__, ptrMsg2 ) ; 
	
	length = ptrMsg2 - ptrMsg1 ;	
		
	strncpy ( idBuffer, ptrMsg1 + 1, length - 1 ) ;
	idBuffer[ length - 1 ] = '\0' ; 
}

/************************************************************************
 * Function:	EMAILidCompare						*
 *									*
 * Description:	Given mail message and mail query, returns 1 if both 	*
 *		matches. Otherwise returns 0.				*
 ************************************************************************/
 
int EMAILidCompare ( char *queryValue, char *messageID ) 
{
	char *ptrQuery ;

	ptrQuery = rindex( queryValue, '/' ) ;
	_assert( __FILE__, __LINE__, ptrQuery ) ; 
	
	if ( strcmp ( ptrQuery + 1, messageID ) == 0 )
		return 1 ;
	else
		return 0 ;
}

/************************************************************************
 * Function:	EMAILdateCompare					*
 *									*
 * Description:	Given date query and the date from mail, return 1 if  	*
 *		both matches. Otherwise returns 0.			*
 ************************************************************************/

int EMAILdateCompare ( char *queryValue, char *dateLine )
{
	void getMailDate( char *, char * ) ;
		
	char mailDate[22] = { '\0' } ;
	char queryDate[22] = { '\0' } ;
	time_t mailTimeStamp, queryTimeStamp ;	
	int x = 9 ;
	
	getMailDate( mailDate, dateLine ) ;
	formatTimeStamp( queryDate, queryValue ) ;
	
	mailTimeStamp = parsedate( mailDate, NULL ) ;
	queryTimeStamp = parsedate( queryDate, NULL) ;
	
	if ( strcmp ( operatorType, "EQ" ) == 0 )
	{
		if ( difftime( mailTimeStamp, queryTimeStamp ) == 0 ) 
			return 1 ;
		else
			return 0 ;
	}
	
	else if ( strcmp ( operatorType, "LT" ) == 0 )
	{
		if( difftime( mailTimeStamp, queryTimeStamp ) < 0 )
			return 1 ;
		else
			return 0 ;
	}
		
	else if( strcmp( operatorType, "LE" ) == 0 )
	{
		if( ( difftime( mailTimeStamp, queryTimeStamp ) == 0 ) ||
		    ( difftime( mailTimeStamp, queryTimeStamp ) < 0 ) )
			return 1 ;
		else
			return 0 ;
	}
	
	else if ( strcmp ( operatorType, "GT" ) == 0 )
	{
		if( difftime( mailTimeStamp, queryTimeStamp ) > 0 )
			return 1 ;
		else
			return 0 ;
	}
	
	else if( strcmp( operatorType, "GE" ) == 0 )
	{
		if( ( difftime( mailTimeStamp, queryTimeStamp ) == 0 ) ||
		    ( difftime( mailTimeStamp, queryTimeStamp ) > 0 ) )
			return 1 ;
		else
			return 0 ;
	}
}

/************************************************************************
 * Function:	EMAILsubjectCompare					*
 *									*
 * Description:	Given subject query and subject value from mail, 	*
 *		returns 1 if query is found/matches. Otherwise 		*
 *		returns 0.						*
 ************************************************************************/

int EMAILsubjectCompare ( char *queryValue, char *subjectLine )
{
	if ( strlen ( subjectLine ) == 0 )
		return 0 ;
			
	else
	{
		if ( strcmp ( operatorType, "~" ) == 0 )
		{
			if( strstr( (strpbrk(subjectLine," ") +1), queryValue) != NULL )
				return 1 ;
			else
				return 0 ;	
		}
	
		else if ( strcmp ( operatorType, "EQ" ) == 0 )
		{
			if( strcmp( (strpbrk(subjectLine," ") +1), queryValue) == 0 )
				return 1 ;
			else
				return 0 ;	
		}
	} 
}

/************************************************************************
 * Function:	getMailDate						*
 *									*
 * Description:	Given subject query and subject value from mail, 	*
 *		returns 1 if query is found. Otherwise returns 0.	*
 ************************************************************************/

void getMailDate( char *value, char *dateLine )
{
	char* convertMonth( char * ) ;
	
	char *word, *toParse, *year, *month, *day, *time ;
	char dateValue[BUFFER_SIZE] = { '\0' } ;
	int numParses ;
		
	numParses = 0 ;
	word = toParse = NULL ;
	
	strcpy ( dateValue, dateLine ) ;
	word = strtok( dateValue, " " ) ;
	
	while( word != NULL )
	{
		numParses++ ;
		switch( numParses )
		{
			case 3	: 	day = strdup( word ) ;
					break ;
					
			case 4	: 	month = strdup( convertMonth ( word ) ) ;
					break ;
								
			case 5	: 	year = strdup( word ) ;
					break ;
					
			case 6	:	time = strdup( word ) ;
					break ;
			
		}
		word = strtok( NULL, " " ) ;
	}
	
	if( strlen( day ) == 1 )
		sprintf( value, "%s/%s/0%s %s", year, month, day, time ) ;
	else
		sprintf( value, "%s/%s/%s %s", year, month, day, time ) ;
	
	free( month ) ;
	free( day ) ;
	free( year ) ;
	free( time ) ;	
}

/************************************************************************
 * Function:	EMAILqueryValidator					*
 *									*
 * Description:	Verifies the email format of the value by returning the	*
 *		number of slashes found in the value. The return value	*
 *		is either 2 or 3 for the correct format.		*
 *		Otherwise, it returns 0					*
 ************************************************************************/
 
int EMAILqueryValidator( char *value )
{
	char testValue[BUFFER_SIZE] = { '\0' } ;
	int numslash = 1 ;
	int slashResult = 0 ;
   	
	strcpy( testValue, value + 6 ) ;
	while( strncmp( testValue + numslash, "/", 1 ) == 0 )
		numslash++ ;
		
	switch( numslash )
	{
		case 2: 	if( strlen( testValue + numslash ) )
					slashResult = 2 ;
				else
					slashResult = 0 ;
				break ;

		case 3:		if( strlen( testValue + numslash ) )
					slashResult = 3 ;
				else
					slashResult = 0 ;
				break ;
	
		default: 	slashResult = 0 ;
				break ;
	}
	return slashResult ;
}
