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
 * Function:	MailqueryTool						*
 *									*
 * Description:	Taking queries with mail: request and searching for 	*
 *		matching senders in the unix mail system.		*
 ************************************************************************/

queryList* EMAILqueryTool( queryList *listpointer )
{
	int isEMAILQuery(char *value);
	void getMailPath(char *mailPath, char *value, queryList *tempQueries  );

	resultList* EMAILmsgTokenizer( char *mailPath, char *value, int attributeType );
	void getMAilDate ( char * ) ;
	
	int doneSearchBox,msgResult, numClauses, dateResult, attributeType ;
	struct stat statBuffer ;	
	FILE *mailFile ;
	char *repository ;		
	char oneLine[BUFFER_SIZE] ;
	char searchMsgID[BUFFER_SIZE] ;
	char mailPath[BUFFER_SIZE] = { '\0' } ;
	   	
   	queryList *tempQueries ;	// a pointer to the querylist arguement
   	resultList *tempResults ;
   	
   	tempQueries = listpointer ;   	
   	tempResults = NULL ;
   	   	
	_debug( __FILE__, __LINE__, 5, "sentinel" ) ;
	while(tempQueries != NULL)
	{
		msgResult = 0;
		
		_debug( __FILE__, __LINE__, 5, "=========================================" ) ;
		for( numClauses = 0 ; numClauses <= tempQueries -> oneQuery -> numClauses ; numClauses++ )
		{
			_debug( __FILE__, __LINE__, 5, "numClauses is %d, qnumClauses is %d", numClauses, tempQueries -> oneQuery -> numClauses ) ;
   			_debug( __FILE__, __LINE__, 5, "attribute is %s", tempQueries -> oneQuery ->myClauses[numClauses].attribute ) ;
   			_debug( __FILE__, __LINE__, 5, "value is %s", tempQueries -> oneQuery ->myClauses[numClauses].value ) ;
   			
   			if ( strcmp ( tempQueries -> oneQuery ->myClauses[numClauses].attribute, "DATE" ) == 0 )
   			{
   				_debug( __FILE__, __LINE__, 5, "attribute is %s", tempQueries -> oneQuery ->myClauses[numClauses].attribute ) ;
   				_debug( __FILE__, __LINE__, 5, "DATE value is %s", tempQueries -> oneQuery ->myClauses[numClauses].value ) ;
   				strcpy ( operatorType , tempQueries -> oneQuery ->myClauses[numClauses].operator ) ;
   				getPath(mailPath,"MAILBOX");
   				_debug( __FILE__, __LINE__, 5, "mailPath is %s", mailPath) ;
   				attributeType = 2 ;
   				if( numClauses == 0 )
					tempResults = EMAILmsgTokenizer( mailPath,tempQueries -> oneQuery ->myClauses[numClauses].value, attributeType ) ;

				else
				{
					if( strcmp( tempQueries -> oneQuery -> myClauses[numClauses-1].conjecture, "AND" ) == 0 )
					{
						_debug( __FILE__, __LINE__, 5, "conjecture is AND in DATE") ;
						tempResults = andResult( tempResults, EMAILmsgTokenizer( mailPath,tempQueries -> oneQuery ->myClauses[numClauses].value, attributeType ) ) ;
					}

					if( strcmp( tempQueries -> oneQuery -> myClauses[numClauses-1].conjecture, "OR" ) == 0 )
					{
						_debug( __FILE__, __LINE__, 5, "conjecture is OR") ;
						tempResults = orResult(tempResults, EMAILmsgTokenizer( mailPath,tempQueries -> oneQuery ->myClauses[numClauses].value, attributeType ) ) ;
					}
				}
			} 
			
			if ( strcmp ( tempQueries -> oneQuery ->myClauses[numClauses].attribute, "NAME" ) == 0 )
   			{
   				_debug( __FILE__, __LINE__, 5, "attribute is %s", tempQueries -> oneQuery ->myClauses[numClauses].attribute ) ;
   				_debug( __FILE__, __LINE__, 5, "NAME value is %s", tempQueries -> oneQuery ->myClauses[numClauses].value ) ;
   				strcpy ( operatorType , tempQueries -> oneQuery ->myClauses[numClauses].operator ) ;
   				_debug( __FILE__, __LINE__, 5, "operator type is %s", operatorType ) ;
   				getPath(mailPath,"MAILBOX");
   				_debug( __FILE__, __LINE__, 5, "mailPath is ", mailPath) ;
   				attributeType = 3 ;
   				if( numClauses == 0 )
					tempResults = EMAILmsgTokenizer( mailPath,tempQueries -> oneQuery ->myClauses[numClauses].value, attributeType ) ;
				else
				{
					if( strcmp( tempQueries -> oneQuery -> myClauses[numClauses-1].conjecture, "AND" ) == 0 )
					{
						_debug( __FILE__, __LINE__, 5, "conjecture is AND") ;
						tempResults = andResult( tempResults, EMAILmsgTokenizer( mailPath,tempQueries -> oneQuery ->myClauses[numClauses].value, attributeType ) ) ;
					}

					if( strcmp( tempQueries -> oneQuery -> myClauses[numClauses-1].conjecture, "OR" ) == 0 )
					{
						_debug( __FILE__, __LINE__, 5, "conjecture is OR") ;
						tempResults = orResult(tempResults, EMAILmsgTokenizer( mailPath,tempQueries -> oneQuery ->myClauses[numClauses].value, attributeType ) ) ;
					}
				}
			}
			
		
			if ( strcmp ( tempQueries -> oneQuery ->myClauses[numClauses].attribute, "ID" ) == 0 )
			{
				_debug( __FILE__, __LINE__, 5, "id attribute is %s", tempQueries -> oneQuery ->myClauses[numClauses].attribute ) ;
				_debug( __FILE__, __LINE__, 5, "id value is %s", tempQueries -> oneQuery ->myClauses[numClauses].value ) ;
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
							{
								_debug( __FILE__, __LINE__, 5, "conjecture is AND") ;
								tempResults = andResult( tempResults, EMAILmsgTokenizer( mailPath,tempQueries -> oneQuery ->myClauses[numClauses].value, attributeType ) ) ;
							}
							
							if( strcmp( tempQueries -> oneQuery -> myClauses[numClauses-1].conjecture, "OR" ) == 0 )
							{
								_debug( __FILE__, __LINE__, 5, "conjecture is OR") ;
								tempResults = orResult(tempResults, EMAILmsgTokenizer( mailPath,tempQueries -> oneQuery ->myClauses[numClauses].value, attributeType ) ) ;
							}
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
 * Description:	Takes in the Value from the query andchecks whether it	*
 *		is to be satisfied by the email repository. If the	*
 *		repository in value specifies "email" the function	*
 *		returns 1 else it returns 0.				*
 ************************************************************************/

int isEMAILQuery(char *value)
{
	char *repository ;		// a token that defines the repository type of the query request
	char tempValue[BUFFER_SIZE] ;
	
	strcpy( tempValue, value ) ;
	
	repository = strtok(tempValue , ":" ) ;
	_assert( __FILE__, __LINE__, repository ) ;
						
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
 *		repository.						*
 ************************************************************************/

void getMailPath(char *mailPath, char *value, queryList *tempQueries )
{
	int doneSearchBox ;
	FILE *configFile ;		
	char oneLine[BUFFER_SIZE] ;		
	char *word, *ptrPath ;			
	char tempValue[BUFFER_SIZE] ;
	char searchBox[BUFFER_SIZE] = { '\0' } ;
	int numslash = 1;
   	
	strcpy( tempValue, value + 6) ;
		
	while( strncmp( tempValue+numslash, "/", 1 ) == 0 )
		numslash++;
			
	switch(numslash)
	{

		case 2	:	getPath(mailPath,"MAILBOX");
   				_debug( __FILE__, __LINE__, 5, "mailPath is ", mailPath) ; 
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
		//char *fromHeader = "From" ;
		//char *msgHeader = "Message-Id:" ;
		resultList* theResults ;
	
		//theResults = msgTokenizer( fromHeader, msgHeader, mailFile, queryValue, attributeType ) ;
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
	int EMAILidCompare ( char *mailQuery, char *mailHeader) ;
	int EMAILdateCompare ( char *queryValue, char *dateLine ) ;
	void getMessageID( char *idBuffer, char *messageLine ) ;
	

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
		_debug( __FILE__, __LINE__, 5, "numAttributes before oneline is %d", numAttributes) ;
		_debug( __FILE__, __LINE__, 5, "oneLine is %s\n", oneLine ) ;
		
		strcpy( testLine, oneLine ) ;
		
		if( ( strlen( testLine ) == 1 ) && ( strcmp( "\n", testLine ) == 0 ) )
			blankLine = lineNumber ;
		else
		{		
			token = strtok( testLine , " " ) ;
			_assert( __FILE__, __LINE__, token ) ;
			_debug( __FILE__, __LINE__, 5, "token is %s", token ) ;
					
			if( strcmp( "From", token ) == 0 )
			{
				fromLine = lineNumber ;
				_debug( __FILE__, __LINE__, 5, "fromLine is %d", fromLine) ;
				_debug( __FILE__, __LINE__, 5, "testLine is %s", testLine) ;
				_debug( __FILE__, __LINE__, 5, "blankLine is %d", blankLine) ;
				if( fromLine - blankLine == 1 )
				{
					strcpy( previousFromWhom, fromWhom ) ;
					strcpy( fromWhom, oneLine ) ;
					if( numAttributes == 2  )
					{
						
						getMessageID( messageID, messageLine ) ;
						_debug( __FILE__, __LINE__, 5, "=========================================" ) ;
						_debug( __FILE__, __LINE__, 5, "previousFromWhom is %s", previousFromWhom ) ;
						_debug( __FILE__, __LINE__, 5, "messageID is %s", messageID ) ;
						_debug( __FILE__, __LINE__, 5, "date is %s", dateLine ) ;
						_debug( __FILE__, __LINE__, 5, "subject is %s", subjectLine ) ;
						_debug( __FILE__, __LINE__, 5, "atribut type is %d", attributeType ) ;
										
						switch ( attributeType ) 					
						{
							case 1	:	msgResult = EMAILidCompare ( queryValue, messageID );
									//numAttributes = readSubjectLine = 0 ;
									break ;
											
							case 2	:	_debug( __FILE__, __LINE__, 5, "in switch attribute is %d",  attributeType ) ;
									_debug( __FILE__, __LINE__, 5, "queryValue is %s",  queryValue ) ;
									_debug( __FILE__, __LINE__, 5, "dateLine is %s",  dateLine ) ;
									
									msgResult = EMAILdateCompare ( queryValue, dateLine ) ;
									
									//numAttributes = readSubjectLine = 0 ;
									break ;
					
							case 3	:	msgResult = EMAILsubjectCompare ( queryValue, subjectLine ) ;
									//numAttributes = readSubjectLine = 0 ;
									subjectLine[0] = '\0' ;
									break ;				
						}
						
						//subjectLine[0] = '\0' ;
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
				
			//tempResults = addResultItem( tempResults, messageID ) ;
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
	
	_debug( __FILE__, __LINE__, 5, "msg is %s of %d characters", msg, strlen( msg ) ) ;
	
       	ptrMsg1 = strchr ( msg, '<' ) ;
	_assert( __FILE__, __LINE__, ptrMsg1 ) ; 
	ptrMsg2 = strrchr ( msg , '>' ) ;
	_assert( __FILE__, __LINE__, ptrMsg2 ) ; 
	
	length = ptrMsg2 - ptrMsg1 ;	
	_debug( __FILE__, __LINE__, 5, "length is %d", length ) ;
	
	strncpy ( idBuffer, ptrMsg1 + 1, length - 1 ) ;
	idBuffer[ length - 1 ] = '\0' ; 
	
	_debug( __FILE__, __LINE__, 5, "idBuffer is %s of %d characters", idBuffer, strlen( idBuffer ) ) ;
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
	
	_debug( __FILE__, __LINE__, 5, "ptrQuery + 1 is %s\n", ptrQuery + 1 ) ;
	_debug( __FILE__, __LINE__, 5, "messageID is %s\n", messageID ) ;
	if ( strcmp ( ptrQuery + 1, messageID ) == 0 )
	{
		_debug( __FILE__, __LINE__, 5, "found - %s\n", messageID ) ;
		return 1 ;
	}
	else
	{
		_debug( __FILE__, __LINE__, 5, "NOT found - %s\n", messageID ) ;
		return 0 ;
	}
}

/************************************************************************
 * Function:	EMAILdateCompare					*
 *									*
 * Description:	Given date query and the date from mail, return 1 if  	*
 *		both matches. Otherwise returns 0.			*
 ************************************************************************/

int EMAILdateCompare ( char *queryValue, char *dateLine )
{
	void getMailDate( char *value, char *dateLine ) ;
		
	char mailDate[22] = { '\0' } ;
	char queryDate[22] = { '\0' } ;
	time_t mailTimeStamp, queryTimeStamp ;	
	int x = 9 ;
	
	_debug( __FILE__, __LINE__, 5, "x is %d ", x ) ;	
	_debug( __FILE__, __LINE__, 5, "dateLine is %s, queryValue is %s ", dateLine, queryValue ) ;
		
	getMailDate( mailDate, dateLine ) ;
	formatTimeStamp( queryDate, queryValue ) ;
	
	_debug( __FILE__, __LINE__, 5, "queryDate is %s", queryDate ) ;
	
	mailTimeStamp = parsedate( mailDate, NULL ) ;
	queryTimeStamp = parsedate( queryDate, NULL) ;
	
	_debug( __FILE__, __LINE__, 5, "mailDate is %s, queryDate is %s", mailDate, queryValue ) ;
	_debug( __FILE__, __LINE__, 5, "dateLine is %s",  dateLine ) ;
	_debug( __FILE__, __LINE__, 5, "mailDate %s",  ctime( &mailTimeStamp ) ) ;
	_debug( __FILE__, __LINE__, 5, "queryDate %s",  ctime( &queryTimeStamp )  ) ;
	
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
			_debug( __FILE__, __LINE__, 2, "subjectLine for ~ is %s", subjectLine ) ;

			if( strstr( (strpbrk(subjectLine," ") +1), queryValue) != NULL )
				return 1 ;
			else
				return 0 ;	
		}
	
		else if ( strcmp ( operatorType, "EQ" ) == 0 )
		{
			_debug( __FILE__, __LINE__, 2, "subjectLine for EQ is %s", subjectLine ) ;
			
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
	char dateValue[1000] = {'\0'} ;
	int numParses ;
		
	numParses = 0 ;
	word = toParse = NULL ;
	
	strcpy ( dateValue, dateLine ) ;
	word = strtok( dateValue, " " ) ;
//	word = strtok( dateLine, " " ) ;
	
	_debug( __FILE__, __LINE__, 5, "word is %s", word ) ;
	
	while( word != NULL )
	{
		numParses++ ;
		_debug( __FILE__, __LINE__, 5, "word beginning is %s ", word ) ;
		switch( numParses )
		{
			case 3	: 	_debug( __FILE__, __LINE__, 5, "Date is %s", word ) ;
					day = strdup( word ) ;
					_debug( __FILE__, __LINE__, 5, "new Date is %s", day ) ;
					break ;
					
			case 4	: 	_debug( __FILE__, __LINE__, 5, "Month is %s", word ) ;
					month = strdup( convertMonth ( word ) ) ;
					_debug( __FILE__, __LINE__, 5, " new Month is %s", month ) ;
					break ;
								
			case 5	: 	_debug( __FILE__, __LINE__, 5, "Year is %s", word ) ;
					year = strdup( word ) ;
					_debug( __FILE__, __LINE__, 5, "new Year is %s", year ) ;
					break ;
			case 6	:	_debug( __FILE__, __LINE__, 5, "time is %s", time ) ;
					time = strdup( word ) ;
					_debug( __FILE__, __LINE__, 5, "new time is %s", time ) ;
					break ;
			
		}
		word = strtok( NULL, " " ) ;
		_debug( __FILE__, __LINE__, 5, "word in at bottom is while is %s", word ) ;
	}
	
	if( strlen( day ) == 1 )
		sprintf( value, "%s/%s/0%s %s", year, month, day, time ) ;
	else
		sprintf( value, "%s/%s/%s %s", year, month, day, time ) ;
	
	free( month ) ;
	free( day ) ;
	free( year ) ;
	free( time ) ;	
		
	_debug( __FILE__, __LINE__, 5, "value is %s", value ) ;
}



int EMAILqueryValidator( char *value )
{
	char testValue[BUFFER_SIZE] = {'\0'} ;
	int numslash = 1 ;
	int slashResult = 0 ;
   	
	strcpy( testValue, value + 6 ) ;
	while( strncmp( testValue + numslash, "/", 1 ) == 0 )
		numslash++ ;
		
	_debug( __FILE__, __LINE__, 5, "testValue is %s", testValue ) ;	
	_debug( __FILE__, __LINE__, 5, "testValue + numslash is %s", testValue + numslash ) ;	
	switch( numslash )
	{
		case 2: 	_debug( __FILE__, __LINE__, 5, "strlen( testValue + numslash ) is %d", strlen( testValue + numslash ) ) ;	
				if( strlen( testValue + numslash ) )
					slashResult = 2 ;
				else
					slashResult = 0 ;
				break ;

		case 3:		_debug( __FILE__, __LINE__, 5, "strlen( testValue + numslash ) is %d", strlen( testValue + numslash ) ) ;	
				if( strlen( testValue + numslash ) )
					slashResult = 3 ;
				else
					slashResult = 0 ;
				break ;
	
		default: 	slashResult = 0 ;
				break ;
	}
	return slashResult ;
}


/*void getMailQueryDate( char *value, char *queryValue )
{
	char *word, *toParse, *year, *month, *day ;
	int numParses ;
	
	numParses = 0 ;
	word = NULL ;
	
	toParse = strdup( queryValue ) ;
	
	word = strtok( toParse, "/" ) ;

	while( word != NULL )
	{
		numParses++ ;
		switch( numParses )
		{
			case 1	: 	month = strdup( word ) ;
					break ;
								
			case 2	: 	day = strdup( word ) ;
					break ;
								
			case 3	: 	year = strdup( word ) ;
					break ;
		}
		word = strtok( NULL, "/" ) ;
	}

	sprintf( value,"%s/%s/%s", year, month, day ) ;	
	
	free( month ) ;
	free( day ) ;
	free( year ) ;
	free( toParse ) ;
}*/

