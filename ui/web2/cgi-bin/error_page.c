#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "getcgi.h"
#include "html.h" 



int main()
{
    
    char **cgivars;
    char *process_filename; 
    char *msg;
    int i;

    /** First, get the CGI variables into a list of strings         **/
    cgivars = getcgivars();
 
    
    process_filename = (char *) getvalue("process_filename", cgivars);
    msg = (char *) getvalue("msg", cgivars);

    print_header("System Error");
    printf("\nThe system encountered an error while processing your request.<br>\n");
    printf("\nError Message: %s\n<br>",msg);
    print_actionlist_link(process_filename);

    
    for (i=0; cgivars[i]; i++) free(cgivars[i]);
        free(cgivars) ;
	
    exit(0);


}	
