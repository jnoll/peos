#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "getcgi.h"
#include "kernel/events.h"
#include "kernel/action.h"
#include "kernel/process_table.h"
#include "html.h" 


char *process_filename;

void print_row(int pid, char *name, char *state)
{
    printf("<tr>");
    printf("<td style=\"vertical-align: top;\">%d<br></td>", pid);
    printf("<td style=\"vertical-align: top;\"><a href = action_page.cgi?pid=%d&act_name=%s&process_filename=%s>%s</a><br></td>",pid, name, process_filename, name);
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
    
    char **cgivars ;
    int i ;
    char *start;
    
    /** First, get the CGI variables into a list of strings         **/
    cgivars = getcgivars();

    start = (char *) getvalue("start", cgivars);
    process_filename = (char *) getvalue("process_filename", cgivars);
    if(strcmp(start,"true") == 0) {
        strcat(process_filename,".dat");	    
    }

    peos_set_process_table_file(process_filename);
    
    print_header("Action List");
        
    list_actions();
    printf("<a href=process_listing.cgi?process_filename=%s>create process</a>",process_filename);
    printf("</body>\n") ;
    printf("</html>\n") ;

    /** Free anything that needs to be freed **/
    for (i=0; cgivars[i]; i++) free(cgivars[i]) ;
    free(cgivars) ;

    exit(0) ;    
}	
