#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "getcgi.h"
#include "kernel/events.h"
#include "kernel/action.h"
#include "kernel/process_table.h"
#include "html.h" 


char *process_file_name;

int main()
{
    
    char **cgivars;
    int i;
    char *action_event;
    int pid;
    char *action_name;
    

    /** First, get the CGI variables into a list of strings         **/
    cgivars = getcgivars();
 
    action_event = (char *) getvalue("action_event", cgivars);
    pid = atoi((char *) getvalue("pid", cgivars));
    action_name = (char *) getvalue("act_name", cgivars);
    process_file_name = (char *) getvalue("process_filename", cgivars);
    
    peos_set_process_table_file(process_file_name);

    if(strcmp(action_event, "Run") == 0) {
        peos_notify(pid, action, PEOS_EVENT_START);
    }
    
    print_header("Action List");
        
    list_actions();
    printf("</body>\n") ;
    printf("</html>\n") ;

    /** Free anything that needs to be freed **/
    for (i=0; cgivars[i]; i++) free(cgivars[i]) ;
    free(cgivars) ;

    exit(0) ;    
}	
