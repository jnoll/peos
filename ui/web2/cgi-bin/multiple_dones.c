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
    char **actions;
    char **act_names = (char **) calloc(256, sizeof(char **));
    int num_actions;
    char *process_filename;
    peos_resource_t *unbound_resource_list;
    int num_unbound_resources;
 
    

    /** First, get the CGI variables into a list of strings         **/
    cgivars = getcgivars();
 
    process_filename = (char *) getvalue("process_filename", cgivars);

    for(i=0,num_actions=0; cgivars[i]; i+=2) {
        if(strcmp(cgivars[i],"actions") == 0) {
	    actions[num_actions] = cgivars[i+1];
            num_actions++;
	}
    }
    
    peos_set_process_table_file(process_filename);

    num_unbound_resources=0;

    unbound_resource_list = (peos_resource_t *) calloc(256, sizeof(peos_resource_t));

    
    for(i=0; i<num_actions; i++) {
        char *pidstr = strtok(actions[i],"$");
	pid = atoi(pidstr);
	act_names[i] = strtok(NULL, "\0");
    }
    
        
    for(i=0; i<num_actions; i++) {
	int num_resources;
	peos_resource_t *resources;
	int j;

 	
        resources = peos_get_resource_list_action_provides(pid, act_names[i], &num_resources);

	if(resources == NULL) {
	    goto_error_page(process_filename);
	    for (i=0; cgivars[i]; i++)
	        free(cgivars[i]) ;
	    free(cgivars);
	    exit(0);
	}
		 
    
        for(j=0; j < num_resources; j++) {
            if((strcmp(resources[j].qualifier,"any") == 0) || (strcmp(resources[j].qualifier,"new") == 0)) {
                strcpy(unbound_resource_list[num_unbound_resources].name,resources[j].name);
                num_unbound_resources++;
	    }
	    else {
	        if((strcmp(resources[j].value,"$$") == 0) && (strcmp(resources[j].qualifier,"abstract") != 0)) {
	            strcpy(unbound_resource_list[num_unbound_resources].name,resources[j].name);
	            num_unbound_resources++;
	        }
	    }
	}
	if(resources) free(resources);
    }


    if(num_unbound_resources == 0)  {
	for(i=0; i<num_actions; i++) {
	    peos_notify(pid, act_names[i], PEOS_EVENT_FINISH);
	}
	printf("Location: action_list.cgi?process_filename=%s&start=false\r\n\r\n",process_filename);
    }
	
    else {
        print_header("Please Supply Resource Bindings");
	print_javascript();
        printf("<form name=\"pickform\" action=\"multiple_done_bindings.cgi?\">");
        printf("<table cellpadding=\"2\" cellspacing=\"2\" border=\"1\" width=\"100%%\">");
        printf("<tbody>");
	for(i=0;i < num_unbound_resources; i++) {
	    printf("<tr>");
	    printf("<td style=\"vertical-align: top;\">%s<br></td>",unbound_resource_list[i].name);
	    printf("<td style=\"vertical-align: top;\"><input type=\"text\" name=\"%s\" value=\" \"><br></td>",unbound_resource_list[i].name);
            printf("</tr>");
	}

	printf("<tr>");
	printf("<td style=\"vertical-align: top;\" colspan=\"2\" align=\"center\"><input type=\"Submit\" name=\"Submit\" value=\"Submit\" onclick=\"return validate();\"><br></td>");
        printf("</tr>");
	printf("</tbody>");
	printf("</table>");
        printf("<input type = \"hidden\" name =\"pid\" value=\"%d\">",pid);
        printf("<input type = \"hidden\" name =\"process_filename\" value=\"%s\">",process_filename);
	for(i=0; i<num_actions; i++) {
            printf("<input type = \"hidden\" name =\"actions\" value=\"%s\">",act_names[i]);
	}
	printf("</form>");
	print_actionlist_link(process_filename);
	print_createprocess_link(process_filename);
	printf("</body>");
	printf("</html>");

    }
    
    
    /** Free anything that needs to be freed **/
    for (i=0; cgivars[i]; i++)
    free(cgivars[i]) ;
    free(cgivars);

    if(unbound_resource_list) free(unbound_resource_list);
 

    exit(0);    
}	
