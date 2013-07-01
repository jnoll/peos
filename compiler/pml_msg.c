#include <stdio.h>

#ifndef _PML_MSG_H
#include "pml_msg.h"
#endif

char *pmlexp[] = {
	"Invalid inputs detected ",
	"Syntax Error ",
	"Datadictionary structure ",
	"Memory allocation failure ",
	"File open failure ",
        "Parsing comments ",
	"C pre-processor not executed ",
   	"PML Compiler Version 2.0 ",
	"Compilation comlete ",
	NULL
};

void
pmlprint(pmltype msgtype,pmlerr errornum,char *module,char *additional)
{
	printf("%s",pmlmsg[msgtype]);
	if(module != NULL) {
		printf("[%s] ",module);
	}

	if(errornum != PMLNULL) {
		printf("%s",pmlexp[errornum]);
	}

	if(additional != NULL) {
		printf("- %s",additional);
	}	
	printf("\n");
}
