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
#define BUFFER 1000

/************************************************************************
 * Function:	MailqueryTool						*
 *									*
 * Description:	Taking queries with mail: request and searching for 	*
 *		matching senders in the unix mail system.		*
 ************************************************************************/

queryList* EMAILqueryTool( queryList *listpointer )
{
	int is_email(char *value);
	void getMailPath(char *mailPath, char *value );
	void getMailBox( char *mailPath ) ;
	resultList* EMAILmsgTokenizer( char *mailPath, char *value, int attributeType );
	void getMAilDate ( char * ) ;
	
	int doneSearchBox,msgResult, numClauses, dateResult, attributeType ;
	struct stat statBuffer ;	
	FILE *mailFile ;
	char *repository ;		
	char oneLine[BUFFER] ;
	char searchMsgID[BUFFER] ;
	char mailPath[BUFFER] = { '\0' } ;
	   	
   	queryList *tempQueries ;	// a pointer to the querylist arguement
   	resultList *tempResults ;
   	
   	tempQueries = listpointer ;   	
   	tempResults = NULL ;
   	   	
	while(tempQueries != NULL)
	{
		msgResult = 0;
		
		for( numClauses = 0 ; numClauses <= tempQueries -> oneQuery -> numClauses ; numClauses++ )
		{
			_debug( __FILE__, __LINE__, 5, "numClauses is %d, qnumClauses is %d", numClauses, tempQueries -> oneQuery -> numClauses ) ;
   			_debug( __FILE__, __LINE__, 5, "attribute is %s", tempQueries -> oneQuery ->myClauses[numClauses].attribute ) ;
   			_debug( __FILE__, __LINE__, 5, "value is %s", tempQueries -> oneQuery ->myClauses[numClauses].value ) ;
   			
   			if ( strcmp ( tempQueries -> oneQuery ->myClauses[numClauses].attribute, "DATE" ) == 0 )
   			{
   				_debug( __FILE__, __LINE__, 5, "date attribute is %s", tempQueries -> oneQuery ->myClauses[numClauses].attribute ) ;
   				_debug( __FILE__, __LINE__, 5, "date value is %s", tempQueries -> oneQuery ->myClauses[numClauses].value ) ;
   				getMailBox( mailPath ) ;
   				attributeType = 2 ;
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
			
			if ( strcmp ( tempQueries -> oneQuery ->myClauses[numClauses].attribute, "NAME" ) == 0 )
   			{
   				_debug( __FILE__, __LINE__, 5, "date attribute is %s", tempQueries -> oneQuery ->myClauses[numClauses].attribute ) ;
   				_debug( __FILE__, __LINE__, 5, "date value is %s", tempQueries -> oneQuery ->myClauses[numClauses].value ) ;
   				getMailBox( mailPath ) ;
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
				if( is_email( tempQueries -> oneQuery ->myClauses[0].value) )
				{
					getMailPath( mailPath, tempQueries -> oneQuery ->myClauses[0].value) ;
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
			
			while ( tempResults != NULL )
			{
				tempQueries -> oneQuery -> results = addResultItem( tempQueries -> oneQuery -> results, tempResults -> oneResult ) ;
				tempQueries -> oneQuery -> numFound++ ;
				tempResults = ( resultList* ) tempResults -> link ;
			}
		}
		tempQueries = ( queryList* ) tempQueries -> link ;
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
	char tempValue[BUFFER] ;
	
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

void getMailPath(char *mailPath, char *value )
{
	void getMailBox( char *mailPath );
	int doneSearchBox ;
	FILE *configFile ;		
	char oneLine[BUFFER] ;		
	char *word, *ptrPath ;			
	char tempValue[BUFFER] ;
	char searchBox[BUFFER] = { '\0' } ;
	int numslash = 1;
   	
	strcpy( tempValue, value + 6) ;
	
	while( strncmp( tempValue+numslash, "/", 1 ) == 0 )
		numslash++;
			
	switch(numslash)
	{
		case 2: 
			/*doneSearchBox = 0 ;
   			if( ( configFile = fopen( "vr.rc", "r" ) ) != NULL  ||  (configFile = fopen(strcat(getenv("HOME"),"/vr.rc"),"r")) != NULL)
			{	
				while( fgets( oneLine, 500, configFile ) != NULL || !doneSearchBox )
   				{
   					word = strtok( oneLine, "=" ) ;
					_assert( __FILE__, __LINE__, word ) ;
					
					if( strncmp( "MAILBOX", word, 7 ) == 0 )
    					{
       						word = strtok( NULL, "\n" ) ;
       						_assert( __FILE__, __LINE__, word ) ;
   						strcpy( searchBox, word ) ;
   						doneSearchBox = 1 ;
					}
	    			}
        			fclose( configFile ) ;
        			strcpy( mailPath, searchBox ) ;*/
        			getMailBox(mailPath);
        			break;
  			//}
  			
  		case 3:
  			ptrPath = rindex( tempValue, '/' ) ;
  			_assert( __FILE__, __LINE__, ptrPath ) ;
  			strncpy( mailPath, tempValue + 2, ptrPath - ( tempValue + 2 ) ) ;
  			break;
//  		default: puts( "inValid query" ) ;			

			
	}		
}

void getMailBox( char *mailPath )
{
	FILE *configFile ;		
	char oneLine[BUFFER] ;		
	char *word ;			
	char searchBox[BUFFER] = { '\0' } ;
	int doneSearchBox ;
			
	doneSearchBox = 0 ;
	if( ( configFile = fopen( "vr.rc", "r" ) ) != NULL  || (configFile = fopen(strcat(getenv("HOME"),"/vr.rc"),"r") )!= NULL)
	{	
		while( fgets( oneLine, 500, configFile ) != NULL || !doneSearchBox )
   		{
   			word = strtok( oneLine, "=" ) ;
			_assert( __FILE__, __LINE__, word ) ;
					
			if( strncmp( "MAILBOX", word, 7 ) == 0 )
    			{
       				word = strtok( NULL, "\n" ) ;
       				_assert( __FILE__, __LINE__, word ) ;
   				strcpy( searchBox, word ) ;
   				doneSearchBox = 1 ;
			}
	    	}
        	fclose( configFile ) ;
        	strcpy( mailPath, searchBox ) ;
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
	resultList* msgTokenizer( char *, char *, FILE *, char *, int ) ;
	
	FILE *mailFile ;
	
	if( ( mailFile = fopen( mailPath, "r" ) ) != NULL )
	{
		char *fromHeader = "From" ;
		char *msgHeader = "Message-Id:" ;
		resultList* theResults ;
	
		theResults = msgTokenizer( fromHeader, msgHeader, mailFile, queryValue, attributeType ) ;
			
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
 
resultList* msgTokenizer( char *fromHeader, char *msgHeader, FILE *mailFile, char *queryValue, int attributeType )
{
	int EMAILidCompare ( char *mailQuery, char *mailHeader) ;
	int EMAILdateCompare ( char *queryValue, char *dateLine ) ;
	void getMessageID( char *idBuffer, char *messageLine ) ;
	
	char *token ;
	char oneLine[BUFFER] = {'\0'} ;
	char testLine[BUFFER] = {'\0'} ;
	char messageLine[BUFFER] = {'\0'} ;
	char messageID[BUFFER] = {'\0'} ;
	char dateLine[BUFFER] = {'\0'} ;
	char subjectLine[BUFFER] = {'\0'} ;
	int msgStage, fromStage, lineNumber, blankLine, fromLine, msgResult, numAttributes ;
	resultList *tempResults ;
	
	tempResults = NULL ;
	numAttributes = blankLine = fromLine = msgResult = 0 ;	
	msgStage = lineNumber = 0 ; // first message is without blank
		
	while( ( fgets( oneLine, 500, mailFile ) != NULL ) && ( !msgResult ) )
	{
		_debug( __FILE__, __LINE__, 5, "oneLine is %s\n", oneLine ) ;
		
		strcpy( testLine, oneLine ) ;
		
		if( ( strlen( testLine ) == 1 ) && ( strcmp( "\n", testLine ) == 0 ) )
			blankLine = lineNumber ;
		else
		{		
			token = strtok( testLine , " " ) ;
			_assert( __FILE__, __LINE__, token ) ;
			_debug( __FILE__, __LINE__, 5, "token is %s", token ) ;
					
			if( strcmp( fromHeader, token ) == 0 )
			{
				fromLine = lineNumber ;
				_debug( __FILE__, __LINE__, 5, "fromLine is %d", fromLine) ;
				_debug( __FILE__, __LINE__, 5, "blankLine is %d", blankLine) ;
				if( fromLine - blankLine == 1 )
				{
					if( numAttributes != 0 )
						numAttributes = 0 ;
				}
			}

			if( ( strcmp( "Message-Id:", token ) == 0 ) ||
			    ( strcmp( "Message-ID:", token ) == 0 ) ||
			    ( strcmp( "Message-id:", token ) == 0 ) )
			{
				oneLine[ strlen( oneLine ) - 1 ] = '\0' ;
			    	strcpy( messageLine, oneLine ) ;
			    	_debug( __FILE__, __LINE__, 5, "messageLine is %s\n", messageLine ) ;
			    	numAttributes++ ;
			    	_debug( __FILE__, __LINE__, 5, "message numAttributes is %d\n", numAttributes ) ;
			}
					
			if( strcmp( "Date:", token ) == 0 )
			{
				oneLine[ strlen( oneLine ) - 1 ] = '\0' ;
				strcpy( dateLine, oneLine ) ;
				_debug( __FILE__, __LINE__, 5, "dateLine is %s\n", dateLine ) ;
				numAttributes++ ;
				_debug( __FILE__, __LINE__, 5, "date numAttributes is %d\n", numAttributes ) ;
			}
			
			if( strcmp( "Subject:", token ) == 0 )
			{
				oneLine[ strlen( oneLine ) - 1 ] = '\0' ;
				strcpy( subjectLine, oneLine ) ;
				_debug( __FILE__, __LINE__, 5, "subjectLine is %s\n", subjectLine ) ;
				//numAttributes++ ;
				_debug( __FILE__, __LINE__, 5, "subject numAttributes is %d\n", numAttributes ) ;
			}
		}
		
		if( numAttributes == 2 )
		{	
			_debug( __FILE__, __LINE__, 5, "with 2 numAttributes" ) ;
			getMessageID( messageID, messageLine ) ;				
			switch ( attributeType ) 					
			{
				case 1:	msgResult = EMAILidCompare ( queryValue, messageID );
				_debug( __FILE__, __LINE__, 5, "attributeType is %d", attributeType ) ;
					numAttributes = msgStage = 0 ;
					break ;
							
				case 2:	msgResult = EMAILdateCompare ( queryValue, dateLine ) ;
					_debug( __FILE__, __LINE__, 5, "attributeType is %d", attributeType ) ;
					numAttributes = msgStage = 0 ;
					break ;
				
				case 3:	msgResult = EMAILsubjectCompare ( queryValue, subjectLine ) ;
					_debug( __FILE__, __LINE__, 5, "attributeType is %d", attributeType ) ;
					numAttributes = msgStage = 0 ;
					break ;				
			}
		}
		
		if( msgResult )
		{
			_debug( __FILE__, __LINE__, 5, "with msgResult" ) ;
			tempResults = addResultItem( tempResults, messageID ) ;
			msgResult = 0 ;
		}
		
		lineNumber++ ;

	}							
	
	return tempResults ;
}

/************************************************************************
 * Function:	EMAILidCompare						*
 *									*
 * Description:	Given mail message and mail query, returns 1 if both 	*
 *		matches. Otherwise returns 0.				*
 ************************************************************************/
 
void getMessageID( char *idBuffer, char *messageLine )
{
	char msg[BUFFER] = { '\0' } ;
	char *ptrMsg1, *ptrMsg2 ;
	int length = 0 ;
	
	strcpy ( msg, messageLine ) ;
	
	_debug( __FILE__, __LINE__, 5, "msg is %s\n", msg ) ;
       	ptrMsg1 = strchr ( msg, '<' ) ;
	_assert( __FILE__, __LINE__, ptrMsg1 ) ; 
	ptrMsg2 = strrchr ( msg , '>' ) ;
	_assert( __FILE__, __LINE__, ptrMsg2 ) ; 
	length = ptrMsg2 - ptrMsg1 ;
	
	strncpy ( idBuffer, ptrMsg1 + 1, length - 1 ) ;
}


int EMAILidCompare ( char *queryValue, char *messageID ) 
{
	char *ptrQuery ;

	ptrQuery = rindex( queryValue, '/' ) ;
	_assert( __FILE__, __LINE__, ptrQuery ) ; 
	
	_debug( __FILE__, __LINE__, 5, "ptrQuery + 1 is %s\n", ptrQuery + 1 ) ;
	_debug( __FILE__, __LINE__, 5, "messageID is %s\n", messageID ) ;
	if ( strcmp ( ptrQuery + 1, messageID ) == 0 )
	{
		_debug( __FILE__, __LINE__, 3, "found - %s\n", messageID ) ;
		return 1 ;
	}
	else
	{
		_debug( __FILE__, __LINE__, 3, "NOT found - %s\n", messageID ) ;
		return 0 ;
	}
}

/************************************************************************
 * Function:	EMAILdateCompare					*
 *									*
 * Description:	Given mail message and mail query, returns 1 if both 	*
 *		matches. Otherwise returns 0.				*
 ************************************************************************/

int EMAILdateCompare ( char *queryValue, char *dateLine )
{
	void getMailDate( char *value, char *dateLine ) ;
	void getMailQueryDate( char *, char * ) ;
	
	char mailDate[11] ;
	char queryDate[11] ;
	
	getMailDate( mailDate, dateLine ) ;
	getMailQueryDate( queryDate, queryValue ) ;
	
	_debug( __FILE__, __LINE__, 5, "mailDate is %s, queryDate is %s", mailDate, queryDate ) ;
	
	if( strcmp( queryDate, mailDate ) == 0 )
		return 1 ;
	else
		return 0 ;	
}


int EMAILsubjectCompare ( char *queryValue, char *subjectLine )
{
	_debug( __FILE__, __LINE__, 5, "subjectLine is %s", subjectLine ) ;
	_debug( __FILE__, __LINE__, 5, "strpbrk(subjectLine," ") %s", (strpbrk(subjectLine," ") +1) ) ;
	_debug( __FILE__, __LINE__, 5, "queryValue %s", queryValue ) ;
			
	if( strstr( (strpbrk(subjectLine," ") +1), queryValue) != NULL )
		return 1 ;
	else
		return 0 ;	
}

void getMailDate( char *value, char *dateLine )
{
	char* convertMonth( char * ) ;
	char *word, *toParse, *year, *month, *day ;
	int numParses ;
		
	numParses = 0 ;
	word = toParse = NULL ;
	
	word = strtok( dateLine, " " ) ;
	
	_debug( __FILE__, __LINE__, 5, "word is %s", word ) ;
	
	while( word != NULL )
	{
		numParses++ ;
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
		}
		word = strtok( NULL, " " ) ;
	}
	
	if( strlen( day ) == 1 )
		sprintf( value,"%s/%s/0%s", year, month, day ) ;
	else
		sprintf( value,"%s/%s/%s", year, month, day ) ;
	
	free( month ) ;
	free( day ) ;
	free( year ) ;
		
	_debug( __FILE__, __LINE__, 5, "value is %s", value ) ;
}

void getMailQueryDate( char *value, char *queryValue )
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
}

