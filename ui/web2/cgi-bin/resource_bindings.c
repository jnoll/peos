#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "getcgi.h"
#include "kernel/events.h"
#include "kernel/action.h"
#include "kernel/process_table.h"
#include "html.h" 

void print_javascript()
{
    printf("<SCRIPT LANGUAGE=\"JavaScript\">");
    printf("<!-- Begin\n");
    printf("function validate() {\n");
    printf("var i;\n");
    printf("for (i = 0; i < document.pickform.elements.length; i++) {\n");
    printf("if (document.pickform.elements[i].type == \"text\") {\n");
    printf("if (document.pickform.elements[i].value == \" \") {\n");
    printf("alert(\"Please enter all resource bindings\");");
    printf("return false;");
    printf("}\n");
    printf("}\n");
    printf("}\n");
    printf("return true;");
    printf("}\n");
    printf(" //  End -->");
    printf("</script>");

    print_noscript();
}


int main()
{
    
    char **cgivars;
    int i;
    int pid;
    char *action_name;
    char *process_filename;
    int num_resources;
    peos_resource_t *resources;
    char *resource_type;

    /** First, get the CGI variables into a list of strings         **/
    cgivars = getcgivars();
 
    pid = atoi((char *) getvalue("pid", cgivars));
    action_name = (char *) getvalue("act_name", cgivars);
    process_filename = (char *) getvalue("process_filename", cgivars);
    resource_type = (char *) getvalue("resource_type", cgivars);
    
    peos_set_process_table_file(process_filename);
    peos_set_loginname(process_filename);


    if(strcmp(resource_type,"requires") == 0) {
        resources = peos_get_resource_list_action_requires(pid, action_name, &num_resources);
    }
    else {
        resources = peos_get_resource_list_action_provides(pid, action_name, &num_resources);
    }

    if(resources == NULL) {
        goto_error_page(process_filename);
	for (i=0; cgivars[i]; i++)
	    free(cgivars[i]) ;
	free(cgivars);
	exit(0);
    }


    print_header("Please Supply Resource Bindings");
    print_javascript();    
    printf("<form name=\"pickform\" action=\"change_resource_bindings.cgi\">");
    printf("<table cellpadding=\"2\" cellspacing=\"2\" border=\"1\" width=\"100%%\">");
    printf("<tbody>");
    for(i=0;i < num_resources; i++) {
	if(strcmp(resources[i].qualifier, "abstract") != 0) {    
            printf("<tr>");
            printf("<td style=\"vertical-align: top;\">%s<br></td>",resources[i].name);
            printf("<td style=\"vertical-align: top;\"><input type=\"text\" name=\"%s\" value=\"%s\" maxlength=\"256\"><br></td>",resources[i].name,resources[i].value);
            printf("</tr>");
        }
    }

    printf("<tr>");
    printf("<td style=\"vertical-align: top;\" colspan=\"2\" align=\"center\"><input type=\"Submit\" name=\"Submit\" value=\"Submit\" onclick=\"return validate()\"><br></td>");
    printf("</tr>");
    printf("</tbody>");
    printf("</table>");
    printf("<input type = \"hidden\" name =\"pid\" value=\"%d\">",pid);
    printf("<input type = \"hidden\" name =\"process_filename\" value=\"%s\">",process_filename);
    printf("<input type = \"hidden\" name =\"act_name\" value=\"%s\">",action_name);
    printf("<input type = \"hidden\" name =\"resource_type\" value=\"%s\">",resource_type);
    printf("</form>");
    print_actionlist_link(process_filename);
    print_createprocess_link(process_filename);
    printf("</body>");
    printf("</html>");
    
     
    /** Free anything that needs to be freed **/
    for (i=0; cgivars[i]; i++)
    free(cgivars[i]) ;
    free(cgivars);

    if (resources) free(resources);

    exit(0);    
}	
