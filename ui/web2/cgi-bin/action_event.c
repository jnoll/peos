#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "getcgi.h"
#include "kernel/events.h"
#include "kernel/action.h"
#include "kernel/process_table.h"
#include "html.h" 



int main()
{
    
    char **cgivars;
    int i;
    char *action_event;
    int pid;
    char *action_name;
    char *process_filename;

    /** First, get the CGI variables into a list of strings **/
    cgivars = getcgivars();
 
    action_event = (char *) getvalue("action_event", cgivars);
    pid = atoi((char *) getvalue("pid", cgivars));
    action_name = (char *) getvalue("act_name", cgivars);
    process_filename = (char *) getvalue("process_filename", cgivars);
    
    peos_set_process_table_file(process_filename);
    peos_set_loginname(process_filename);
  

    if(strcmp(action_event, "Abort") == 0) {
        peos_notify(pid, action_name, PEOS_EVENT_ABORT);
	printf("Location: active_processes.cgi?action=continue&process_filename=%s\r\n\r\n", process_filename);
    }
    

    if(strcmp(action_event, "Suspend") == 0) {
        peos_notify(pid, action_name, PEOS_EVENT_SUSPEND);
	printf("Location: active_processes.cgi?action=continue&process_filename=%s\r\n\r\n", process_filename);
    }


    if(strcmp(action_event, "Run") == 0) {
	printf("Location: action_page.cgi?resource_type=requires&process_filename=%s&pid=%d&action_name=%s\r\n\r\n", process_filename, pid, action_name);
    }
    
    
    if(strcmp(action_event, "Finish") == 0) {
	printf("Location: action_page.cgi?resource_type=provides&process_filename=%s&pid=%d&action_name=%s\r\n\r\n", process_filename, pid, action_name);
    }
    
    
    /** Free anything that needs to be freed **/
    for (i=0; cgivars[i]; i++) free(cgivars[i]) ;
    free(cgivars) ;

    exit(0) ;    
}
