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
    int pid;
    char *action_name;
    char *process_filename;
    int num_resources;
    peos_resource_t *resources;
    peos_resource_t *unbound_resource_list;
    int num_unbound_resources;
    char *resource_type;

    /** First, get the CGI variables into a list of strings         **/
    cgivars = getcgivars();
 
    pid = atoi((char *) getvalue("pid", cgivars));
    action_name = (char *) getvalue("act_name", cgivars);
    process_filename = (char *) getvalue("process_filename", cgivars);
    resource_type = (char *) getvalue("resource_type", cgivars);
    
    peos_set_process_table_file(process_filename);

    if(strcmp(resource_type,"requires") == 0) {
        resources = peos_get_resource_list_action_requires(pid, action_name, &num_resources);
    }
    else {
        resources = peos_get_resource_list_action_provides(pid, action_name, &num_resources);
    }
	    

//    printf("Content-Type: text/plain\n\n") ;
    
    unbound_resource_list = (peos_resource_t *) calloc(num_resources+1, sizeof(peos_resource_t));
    num_unbound_resources = 0;
    for(i=0; i < num_resources; i++) {
//	printf("%s=%s=%s\n",resources[i].name,resources[i].value,resources[i].qualifier);    
        if((strcmp(resources[i].qualifier,"any") == 0) || (strcmp(resources[i].qualifier,"new") == 0)) {
            strcpy(unbound_resource_list[num_unbound_resources].name,resources[i].name);
            num_unbound_resources++;
	}
	else {
	    if((strcmp(resources[i].value,"$$") == 0) && (strcmp(resources[i].qualifier,"abstract") != 0)) {
	        strcpy(unbound_resource_list[num_unbound_resources].name,resources[i].name);
	        num_unbound_resources++;
	    }
	}
    }

//    printf("num_unbound=%d\n",num_unbound_resources);
//    printf("num_resources=%d\n",num_resources); 

    if((num_unbound_resources == 0) || (num_resources == 0)) {
        if(strcmp(resource_type,"requires") == 0) {
	    peos_notify(pid, action_name, PEOS_EVENT_START);
	    printf("Location: action_page.cgi?process_filename=%s&pid=%d&act_name=%s\r\n\r\n",process_filename,pid,action_name);
	}
	else {
	    peos_notify(pid, action_name, PEOS_EVENT_FINISH);
	    printf("Location: action_list.cgi?process_filename=%s&start=false\r\n\r\n",process_filename);
	}
    }
	
    else {
        print_header("Please Supply Resource Bindings");
        printf("<form name=\"form\" action=\"bind_resources.cgi?pid=%d&process_filename=%s&act_name=%s&resource_type=%s\">",pid,process_filename,action_name,resource_type);
        printf("<table cellpadding=\"2\" cellspacing=\"2\" border=\"1\" width=\"100%\">");
        printf("<tbody>");
	for(i=0;i < num_unbound_resources; i++) {
	    printf("<tr>");
	    printf("<td style=\"vertical-align: top;\">%s<br></td>",unbound_resource_list[i].name);
	    printf("<td style=\"vertical-align: top;\"><input type=\"text\" name=\"%s\"><br></td>",unbound_resource_list[i].name);
            printf("</tr>");
	}

	printf("<tr>");
	printf("<td style=\"vertical-align: top;\" colspan=\"2\" align=\"center\"><input type=\"Submit\" name=\"Submit\" value=\"Submit\"><br></td>");
        printf("</tr>");
	printf("</tbody>");
	printf("</table>");
        printf("<input type = \"hidden\" name =\"pid\" value=\"%d\">",pid);
        printf("<input type = \"hidden\" name =\"process_filename\" value=\"%s\">",process_filename);
        printf("<input type = \"hidden\" name =\"act_name\" value=\"%s\">",action_name);
        printf("<input type = \"hidden\" name =\"resource_type\" value=\"%s\">",resource_type);
	printf("</form>");
	printf("</body>");
	printf("</html>");

	if(unbound_resource_list) free(unbound_resource_list);
    }
     
    /** Free anything that needs to be freed **/
    for (i=0; cgivars[i]; i++)
    free(cgivars[i]) ;
    free(cgivars);

    exit(0);    
}	
