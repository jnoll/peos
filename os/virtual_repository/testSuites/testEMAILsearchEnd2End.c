/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : testEMAILsearchEnd2End.c					*
 ************************************************************************/

/************************************************************************
 * Description:	Test the implementation of Virtual Repository for the	*
 		mail box with valid and found queries			*
 ************************************************************************/

#include "form.h"
#include "debug.h"
#include "variables.h"
#include "vrepo.h"
#include "setup_repositories.h"
#include "EMAILseeker.h"
#include "queryLinkedList.h"
#include "resultLinkedList.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define BUFFER_SIZE 1000

int main( void )
{	
	void callback( int size, resultList *listPointer , void *data ) ;
	void ( *call )( int, resultList *, void * data ) ;
	void setExpectedResult( ) ;
	void setTestData( queryList * ) ;
	
	char queryString[BUFFER_SIZE] = { '\0' } ;
	int *d ;
	FILE *sampleFile ;

	repos_ctr = 0;
	myQueries = NULL;
	
	setup_email( );	
	call = callback ;

	sampleFile = fopen ( "EMAILsearchEnd2End.dat", "r" ) ;
	_assert( __FILE__, __LINE__, sampleFile ) ;
	
	while ( !feof( sampleFile ) ) 
	{
		fgets ( queryString, sizeof ( queryString ), sampleFile ) ;
		if( strlen( queryString ) )
		{
			query_wait( queryString, call, d ) ;
			queryString[0] = '\0' ;
		}
	}
	fclose( sampleFile ) ;

	setTestData( myQueries ) ;
	setExpectedResult( ) ;	
		
	poll_vr( ) ;
		
	return 0 ;
}

void callback( int size, resultList *listpointer, void *data )
{	
	printResultList( listpointer ) ;	
}

void setExpectedResult ( ) 
{
	char mailPath[BUFFER_SIZE] = { '\0' } ;
	
	getPath( mailPath, "MAILBOX" ) ;
	
	if ( mailPath == NULL ) 
		_debug( __FILE__, __LINE__, 5, "error in getMailBox" ) ;
	else
	{
		FILE *expectedResultFile ;
		char oneResult[BUFFER_SIZE] = { '\0' } ;
		char expectedString[BUFFER_SIZE] = { '\0' } ;
		
		expectedResultFile = fopen ( "EMAILsearchEnd2EndExpectedResult.txt", "w" ) ;
		_assert( __FILE__, __LINE__, expectedResultFile ) ;
		
		/* test for ID attribute */
		
		/* test : ID EQ email:///message1@dc.engr.scu.edu */
	
		sprintf( oneResult ,"%s%s%s", "email://", mailPath, "/message1@dc.engr.scu.edu\n\n" ) ;
		strcat( expectedString, oneResult ) ;
		fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
		expectedString[0] = '\0' ;
		
		/* test : ID EQ email://200210081925.MAA28947@hpux31.dc.engr.scu.edu */

		sprintf( oneResult ,"%s%s%s", "email://", mailPath, "/200210081925.MAA28947@hpux31.dc.engr.scu.edu\n\n" ) ;
		strcat( expectedString, oneResult ) ;
		fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
		expectedString[0] = '\0' ;
		
		/* test for AND conjecture - ID && DATE( EQ, LT, LE, GT, GE ) */
		/* test :   ID EQ email:///message3.test.case.12345@server.engr.scu.edu AND DATE EQ 09/19/2002-17:35:00
			    ID EQ email:///message3.test.case.12345@server.engr.scu.edu AND DATE LT 09/19/2002-17:35:01
			    ID EQ email:///message3.test.case.12345@server.engr.scu.edu AND DATE LE 09/19/2002-17:35:02
			    DATE GT 09/19/2002-17:34:59 AND ID EQ email:///message3.test.case.12345@server.engr.scu.edu
			    DATE GE 09/19/2002-17:34:59 AND ID EQ email:///message3.test.case.12345@server.engr.scu.edu */

		
		sprintf( oneResult ,"%s%s%s", "email://", mailPath, "/message3.test.case.12345@server.engr.scu.edu\n\n" ) ;
		strcat( expectedString, oneResult ) ;
		strcat( expectedString, oneResult ) ;
		strcat( expectedString, oneResult ) ; 
		strcat( expectedString, oneResult ) ; 
		strcat( expectedString, oneResult ) ; 	
		fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
		expectedString[0] = '\0' ;
		
		/* test for AND conjecture - ID && NAME( EQ, ~ ) */
		
		/* test : NAME ~ Friday */
		
		sprintf( oneResult ,"%s%s%s", "email://", mailPath, "/message2.test.case.12345@school.engr.scu.edu\n" ) ;
		strcat( expectedString, oneResult ) ;
		fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
		expectedString[0] = '\0' ;
		
		sprintf( oneResult ,"%s%s%s", "email://", mailPath, "/message3.test.case.12345@server.engr.scu.edu\n\n" ) ;
		strcat( expectedString, oneResult ) ;
		fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
		expectedString[0] = '\0' ;
				
		/* test : NAME EQ Reminder */ 
		
		sprintf( oneResult ,"%s%s%s", "email://", mailPath, "/message7@server.engr.scu.edu\n\n" ) ;
		strcat( expectedString, oneResult ) ;
		fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
		expectedString[0] = '\0' ; 
				
		/* test for OR conjecture - ID || DATE( EQ, GE ) */
		
		/* test : ID EQ email:///message5.test.case@aol.com.br OR DATE EQ 12/05/2002-05:52:26 */
		
		sprintf( oneResult ,"%s%s%s", "email://", mailPath, "/message5.test.case@aol.com.br\n" ) ;
		strcat( expectedString, oneResult ) ;
		sprintf( oneResult ,"%s%s%s", "email://", mailPath, "/message1@dc.engr.scu.edu\n\n" ) ;
		strcat( expectedString, oneResult ) ;
		fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
		expectedString[0] = '\0' ;
		
		/* test : ID EQ email:///message5.test.case@aol.com.br OR DATE GE 12/05/2002-05:52:25 */
		
		sprintf( oneResult ,"%s%s%s", "email://", mailPath, "/message5.test.case@aol.com.br\n" ) ;
		strcat( expectedString, oneResult ) ;
		sprintf( oneResult ,"%s%s%s", "email://", mailPath, "/message1@dc.engr.scu.edu\n" ) ;
		strcat( expectedString, oneResult ) ;
		sprintf( oneResult ,"%s%s%s", "email://", mailPath, "/message4@home.testing.com\n" ) ;
		strcat( expectedString, oneResult ) ;
		sprintf( oneResult ,"%s%s%s", "email://", mailPath, "/message7@server.engr.scu.edu\n\n" ) ;
		strcat( expectedString, oneResult ) ;
		fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
		expectedString[0] = '\0' ;
		
		/* test : DATE EQ 12/05/2002-05:52:26 OR ID EQ email:///message5.test.case@aol.com.br */
				
		sprintf( oneResult ,"%s%s%s", "email://", mailPath, "/message1@dc.engr.scu.edu\n" ) ;
		strcat( expectedString, oneResult ) ;
		sprintf( oneResult ,"%s%s%s", "email://", mailPath, "/message5.test.case@aol.com.br\n\n" ) ;
		strcat( expectedString, oneResult ) ;
		fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
		expectedString[0] = '\0' ;
		
		/* test : NAME ~ Reminder OR DATE EQ 04/07/2003-04:32:14 */
		
		sprintf( oneResult ,"%s%s%s", "email://", mailPath, "/message7@server.engr.scu.edu\n\n" ) ;
		strcat( expectedString, oneResult ) ;
		fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
		expectedString[0] = '\0' ;
		
		/* test : ID EQ email:///message5.test.case@aol.com.br AND DATE EQ 04/07/2003-04:32:14 OR NAME ~ Returned */
		
		sprintf( oneResult ,"%s%s%s", "email://", mailPath, "/200210081925.MAA28947@hpux31.dc.engr.scu.edu\n\n" ) ;
		strcat( expectedString, oneResult ) ;
		fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
		expectedString[0] = '\0' ;

		fclose( expectedResultFile ) ;	
	}
}

void setTestData( queryList *listpointer )
{
	queryList *tempQueries ;
	int numClauses ;
	
	tempQueries = listpointer ;
	
	while( tempQueries != NULL )
	{
		char testString[BUFFER_SIZE] = { '\0' } ;		
		char valueString[BUFFER_SIZE] = { '\0' } ;
		
		for( numClauses = 0 ; numClauses <= tempQueries -> oneQuery -> numClauses ; numClauses++ )
		{
			strcpy( valueString, tempQueries -> oneQuery -> myClauses[numClauses].value ) ;
			
			if( strcmp( tempQueries -> oneQuery -> myClauses[numClauses].attribute, "ID" ) == 0 )
			{
				if( ( strstr( valueString, "email:///" ) ) != NULL ) 
				{
					char mailPath[BUFFER_SIZE] = { '\0' } ;
					getPath( mailPath, "MAILBOX" ) ;
					
					if( mailPath == NULL )
						_debug( __FILE__, __LINE__, 0, "error in getMailBox" ) ;
					else
					{		
						char *token, *value ;
						char tempQuery[BUFFER_SIZE] = { '\0' } ;
							
						strcpy( tempQuery, valueString ) ;
						
						token = strtok( tempQuery, ":" ) ;
						_assert( __FILE__, __LINE__, token  ) ;
			
						strcat( testString, token ) ;
						strcat( testString, "://" ) ;
						strcat( testString, mailPath ) ;
						
						value = strpbrk( valueString, ":" ) ;
						_assert( __FILE__, __LINE__, value  ) ;
						
						strcat( testString, value + 3 ) ;						
					}
				}
				else
					strcpy( testString, valueString ) ;
				
				free( tempQueries -> oneQuery -> myClauses[numClauses].value ) ;
				tempQueries -> oneQuery -> myClauses[numClauses].value = strdup( testString ) ;	
			}
		}
		tempQueries = ( queryList* ) tempQueries -> link ;
	}
}
