/*****************************************************************/
/* File:        pmlvm.c
/* Author:      Crystal Zhu
/* Date:        11/17/1999
/* Description: Main Function of Virual Machine
/*****************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "pmlevent.h"
#include "vm.h"

const int PFLAG = 1;
const int AFLAG = 2;
const int EFLAG = 4;
const int OFLAG = 8;
const int CFLAG = 16;
const int LFLAG = 32;

int main(int argc, char * argv[])
{
    extern char *optarg;
    extern int  optind;
    char  c;
    char  processName[256];
    char  actionName[64];
    char  eventName[4];
    char  onAction[64];
    char  childExitStatus[64];
    char  startLineNum[64];
    int   FLAG = 0;
    int   optcount = 0;
    char  * usageMsg = "Usage: pmlvm -p process_name { -a action_name | "
                       "-e event -o on_action | -c child_exit_status | "
                       "-l start_line_number}";
    int lineNum = 0;
    int eventCode = 0;
    int i=0, n = 0;

    n = pml_open_repository(NULL, APPEND); 
    if (argc < 2) 
    {
	fprintf(stderr,usageMsg);
	exit(1);
    }

/*  parse the input and set the cooresponding option flags */

    while(( c = getopt(argc, argv, "p:a:e:c:l:o:")) != -1)
    {
	switch(c){
	case 'p':		   /* "p" option to specify path */
	    FLAG |= PFLAG;
	    strcpy(processName, optarg);
	    break;
	case 'a':		   /* "i" option for print the i node */
	    FLAG |= AFLAG;
	    optcount++;
	    strcpy(actionName, optarg);
	    break;
	case 'e':  		   /* for the "s" option */
	    FLAG |= EFLAG;
	    optcount ++;
	    strcpy(eventName, optarg);
	    break;
	case 'o':  		   /* for the "s" option */
	    FLAG |= OFLAG;
	    optcount ++;
	    strcpy(onAction, optarg);
	    break;
	case 'c':  		   /* for the "s" option */
	    FLAG |= CFLAG;
	    optcount ++;
	    strcpy(childExitStatus, optarg);
	    break;
	case 'l':  		   /* for the "s" option */
	    FLAG |= LFLAG;
	    optcount ++;
	    strcpy(startLineNum, optarg);    
	    break;
	default:
	    fprintf(stderr,usageMsg);
	    exit(1);
	}
     }

    if ( FLAG & LFLAG)
    {
	for (i=0, n = strlen(startLineNum); i<n; i++)
	  if(!isdigit(startLineNum[i]))
	     break;

	if(n== 0 || i != n) /* there are non-digit value in line number */
	    optcount += 10;  /* setup error condition */
	else
	   lineNum = atoi(startLineNum);
    }

    if ( FLAG & EFLAG)
    {
	eventCode = atoi(eventName);
	if(eventCode < 0 || eventCode > LAST ) /* not a valid event */
	    optcount += 10;  /* setup error condition */
    }
     /* check if the parsed option is valid. */

    /* invalid arguments  or no process specified */
    if( optind > argc || optcount > 2 || !(FLAG & PFLAG) )
    {
	fprintf(stderr,usageMsg);
	exit(1);
    }

    if(( FLAG&AFLAG || FLAG&CFLAG || FLAG&LFLAG ) && optcount > 1) /* too many opt.*/
    {
	fprintf(stderr,usageMsg);
	exit(1);
    }

    if( optcount == 2 && !( FLAG & OFLAG && FLAG & EFLAG))  /* O & E must go together*/
    {
	fprintf(stderr,usageMsg);
	exit(1);
    }

     /* pass the parameter checking, time to do something */

     switch(optcount){
     case 0:   /* run process with no parameter */
//	 fprintf(stderr, "runProcess");
	 runProcess(processName, lineNum);
	 break;
     case 1:
	 if( FLAG & AFLAG)  /* run an action */ 
	     runAction(processName, actionName);
//	     fprintf(stderr, "runAction " );
	 else if( FLAG & CFLAG ) /* child has exit, continue */
	     runChildExit(processName, childExitStatus);
//	     fprintf(stderr, "runChildExit ");
	 else    /* run process with a specific line */
	     runProcess(processName, lineNum);
//	     fprintf(stderr,"runProcessWithlineNum ");
	 break;
     case 2:	 
	 runActionOnEvent(processName, onAction, eventCode);
//	 fprintf(stderr, "runActionOnEvent");
	 break;
     }

     pml_close_repository(); 
     fprintf(stderr,exitStatus);
     return 0;
}

