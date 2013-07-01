/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : testUNITandResult.c					*
 ************************************************************************/

/************************************************************************
 * Description:	Test the unit implementation of andResult		*
 ************************************************************************/
 
#include "form.h"
#include "debug.h"
#include "resultLinkedList.h"
#include "seekerTools.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 1000

int main( void )
{	
	FILE *expectedResultFile ;
	
	char string1[] = " SEARCHDIR=/users/student/peos/src/os/virtual_repository" ;
	char string2[] = "SEARCHDIR=/users/student/peos/src/os/virtual_repository " ;
	char string3[] = " SEARCHDIR=/users/student/peos/src/os/virtual_repository " ;
	char string4[] = "SEARCHDIR =/users/student/peos/src/os/virtual_repository" ;
	char string5[] = "SEARCHDIR= /users/student/peos/src/os/virtual_repository" ;
	char string6[] = "SEARCHDIR = /users/student/peos/src/os/virtual_repository" ;
	char string7[] = "S E A R C H D I R = / u s e r s / s t u d e n t / p e o s / s r c / o s / v i r t u a l _ r e p o s i t o r y" ;
	char string8[BUFFER_SIZE] = { '\0' } ;
	
	char *expectedString = "SEARCHDIR=/users/student/peos/src/os/virtual_repository\n" ;
	char *expectedNULLString = "\n\n" ;
	
	expectedResultFile = fopen ( "UNITcompactStringExpectedResult.txt", "w" ) ;
	_assert( __FILE__, __LINE__, expectedResultFile ) ;
	
	
	_debug( __FILE__, __LINE__, 5, "expectedString is %s", expectedString ) ;
	
	fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
	fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
	fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
	fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
	fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
	fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
	fwrite( expectedString, sizeof( char ), strlen( expectedString ), expectedResultFile ) ;
	fwrite( expectedNULLString, sizeof( char ), strlen( expectedNULLString ), expectedResultFile ) ;
	
	fclose( expectedResultFile ) ;	
	
	compactString( string1 ) ;
	_debug( __FILE__, __LINE__, 5, "compact string1 is %s", string1 ) ;
	
	compactString( string2 ) ;
	_debug( __FILE__, __LINE__, 5, "compact string2 is %s", string2 ) ;
	
	compactString( string3 ) ;
	_debug( __FILE__, __LINE__, 5, "compact string3 is %s", string3 ) ;

	compactString( string4 ) ;
	_debug( __FILE__, __LINE__, 5, "compact string4 is %s", string4 ) ;
	
	compactString( string5 ) ;
	_debug( __FILE__, __LINE__, 5, "compact string5 is %s", string5 ) ;
	
	compactString( string6 ) ;
	_debug( __FILE__, __LINE__, 5, "compact string6 is %s", string6 ) ;
	
	compactString( string7 ) ;
	_debug( __FILE__, __LINE__, 5, "compact string7 is %s", string7 ) ;
	
	compactString( string8 ) ;
	_debug( __FILE__, __LINE__, 5, "compact string8 is %s", string8 ) ;
	
	printf( "%s%s%s%s%s%s%s%s\n", string1, string2, string3, string4, string5, string6, string7, string8 ) ;
		
	return 0 ;
}
	
	
