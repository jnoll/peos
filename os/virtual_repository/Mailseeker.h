/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : EMAILseeker.h						*
 ************************************************************************/

/************************************************************************
 * Description:	Contains function declaration for EMAILseeker.c		*
 ************************************************************************/


queryList* EMAILqueryTool( queryList *listpointer );


int is_email(char *value);



void getMailPath(char *mailPath, char *value );


 
int EMAILidCompare ( char *mailQuery, char *mailHeader) ;


 
int EMAILmsgTokenizer( char *mailPath, char *value );



