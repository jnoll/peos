#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "getcgi.h"
#include "kernel/events.h"
#include "kernel/action.h"
#include "kernel/process_table.h"
#include "html.h" 


char *process_file_name;

void print_row(int pid, char *name, char *state)
{
    printf("<tr>");
    printf("<td style=\"vertical-align: top;\">%d<br></td>", pid);
    printf("<td style=\"vertical-align: top;\"><a href = action_page.cgi?pid=%d&act_name=%s&state=%s&process_filename=%s>%s</a><br></td>",pid, name, state, process_file_name, name);
    printf("<td style=\"vertical-align: top;\">%s<br></td>", state);
    printf("<td style=\"vertical-align: top;\"><br></td>");
    printf("</tr>");
 
}


void list_actions()
{
    peos_action_t *alist;
    int i=0;
    int j;
		                                                                                                     
    char ** result = peos_list_instances();

    printf("<table cellpadding=\"2\" cellspacing=\"2\" border=\"1\" style=\"text-align: left; width: 100%;\">");
    printf("<tbody>");
    printf("<tr>");
    printf("<td style=\"vertical-align: top;\">Process Id<br></td>");
    printf("<td style=\"vertical-align: top;\">Action Name<br></td>");
    printf("<td style=\"vertical-align: top;\">State<br></td>");
    printf("<td style=\"vertical-align: top;\">Check to Indicate Done<br></td>");
    printf("</tr>");
     
    while((i <= PEOS_MAX_PID)) {
        int num_actions;
	if (result[i] != NULL) {
	    alist = peos_list_actions(i,&num_actions);
	    if(alist) {
	        for(j = 0; j < num_actions; j++) {
		    print_row(i,alist[j].name,act_state_name(alist[j].state));
		}
	    }
	    i++;
	}
    }
    printf("</tbody>");
    printf("</table>");
	 
}



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
