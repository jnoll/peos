#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "getcgi.h"
#include "kernel/events.h"
#include "kernel/action.h"
#include "kernel/process_table.h"
#include "html.h" 
#include <crypt.h>

#define _XOPEN_SOURCE

char *process_filename = NULL;

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
    int num_rows=0;
		                                                                                                     
    char ** result = peos_list_instances();

    printf("<form name=\"pickform\" action=\"multiple_dones.cgi\">");
    printf("<table cellpadding=\"2\" cellspacing=\"2\" border=\"1\" style=\"text-align: left; width: 100%%;\">");
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
		    num_rows++;
		}
	    }
	    i++;
	}
    }
    printf("<tr>");
    printf("<td style=\"vertical-align: top;\"></td>");
    printf("<td style=\"vertical-align: top;\"></td>");
    printf("<td style=\"vertical-align: top;\"></td>");
    printf("<td style=\"vertical-align: top;\"><input type=\"Submit\" name=\"Submit\" value=\"Submit Dones\" onclick=\"return validate(%d)\"><br></td></tr>",num_rows);
    printf("<input type=\"hidden\" name=\"process_filename\" value=\"%s\">",process_filename);
    printf("</tbody>");
    printf("</table>");
    printf("</form>");
	 
}



int main()
{
    
    char **cgivars, *login_name;
    int i ;
    char *start;
    
    /** First, get the CGI variables into a list of strings         **/
    cgivars = getcgivars();

    start = (char *) getvalue("start", cgivars);

    if(start) {
        login_name = (char *) getvalue("process_filename",cgivars);
        process_filename = (char *) malloc(strlen(login_name) * sizeof(char));
        strcpy(process_filename, login_name);
    }
    else {
	char *enc_loginname;    
        login_name = (char *) getenv("REMOTE_USER");
	enc_loginname = (char *) crypt(login_name, "df");
        for(i=0; i < strlen(enc_loginname); i++) {
	    if(enc_loginname[i] == '/') enc_loginname[i] = 'A';
	    if(enc_loginname[i] == '.') enc_loginname[i] = 'X';
        }
        process_filename = (char *) malloc((strlen(enc_loginname) + strlen(".dat")) * sizeof(char));
        strcpy(process_filename, enc_loginname);
        strcat(process_filename, ".dat"); 
    }


    peos_set_process_table_file(process_filename);
    peos_set_loginname(process_filename);
    
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
    printf("function validate(numboxes) {\n");
    printf("var i;\n");
    printf("var flag = 0;\n");
    printf("if(numboxes == 1) {");
    printf("if(document.pickform.actions.checked == true) {\n");
    printf("flag = 1;");
    printf("}");
    printf("}");
    printf("else {");
    printf("for (i = 0; i < numboxes; i++) {\n");
    printf("if (document.pickform.actions[i].checked == true) {\n");
    printf("flag = 1;\n");
    printf("}\n");
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
    print_createprocess_link(process_filename);
    printf("</body>\n") ;
    printf("</html>\n") ;

    /** Free anything that needs to be freed **/
    for (i=0; cgivars[i]; i++) free(cgivars[i]) ;
    free(cgivars) ;

    exit(0) ;    
}	
