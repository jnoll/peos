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
    printf("<td style=\"vertical-align: top;\"><input type =\"checkbox\" name=\"actions\" value=\"%d$%s\" onclick=\"checkChoice(this)\"><br></td>",pid,name);
    printf("</tr>");
 
}


void list_actions()
{
    peos_action_t *alist;
    int i=0;
    int j;
		                                                                                                     
    char ** result = peos_list_instances();

    if(result[0] == NULL) {
        printf("No Processes Instantiated\n");
	return;
    }

    printf("<form name=\"pickform\" action=\"multiple_dones.cgi\">");
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
    printf("<tr>");
    printf("<td style=\"vertical-align: top;\"></td>");
    printf("<td style=\"vertical-align: top;\"></td>");
    printf("<td style=\"vertical-align: top;\"></td>");
    printf("<td style=\"vertical-align: top;\"><input type=\"Submit\" name=\"Submit\" value=\"Submit Dones\" onclick=\"return validate()\"><br></td></tr>");
    printf("<input type=\"hidden\" name=\"process_filename\" value=\"%s\">",process_filename);
    printf("</tbody>");
    printf("</table>");
    printf("</form>");
	 
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
     
    printf("<SCRIPT LANGUAGE=\"JavaScript\">");
    printf("<!-- Begin\n");
    printf("function checkChoice(field) {\n");
    printf("var i;\n");
    printf("var frag = new Array();\n");
    printf("frag = field.value.split('$');\n");
    printf("if(field.checked == true) {\n");
    printf("for (i = 0; i < document.pickform.actions.length; i++) {\n");
    printf("var tmp = new Array();");
    printf("tmp = document.pickform.actions[i].value.split('$');\n");
    printf("if(tmp[0] != frag[0]) {\n");
    printf("document.pickform.actions[i].checked = false;\n");
    printf("}\n");
    printf("}\n");
    printf("}\n");
    printf("}\n");
    printf("function validate() {\n");
    printf("var i;\n");
    printf("var flag = 0;\n");
    printf("for (i = 0; i < document.pickform.actions.length; i++) {\n");
    printf("if (document.pickform.actions[i].checked == true) {\n");
    printf("flag = 1;\n");
    printf("}\n");
    printf("}\n");
    printf("if (flag == 0) {\n");
    printf("alert(\"Please select atleast one action\");");
    printf("return false;");
    printf("}\n");
    printf("return true;");
    printf("}\n");
    printf(" //  End -->");
    printf("//  </script>");

    print_noscript();

    list_actions();
    printf("<a href=process_listing.cgi?process_filename=%s>create process</a>",process_filename);
    printf("</body>\n") ;
    printf("</html>\n") ;

    /** Free anything that needs to be freed **/
    for (i=0; cgivars[i]; i++) free(cgivars[i]) ;
    free(cgivars) ;

    exit(0) ;    
}	
