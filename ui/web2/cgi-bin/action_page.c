#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <kernel/events.h>
#include <kernel/action.h>
#include <kernel/process_table.h>
#include "getcgi.h"
#include "html.h"



int main()
{

    char **cgivars;
    int i;
    int pid;
    char *action_name;
    char *state;
    char *script;
    char *process_filename;
    int num_req_resources;
    int num_prov_resources;
    peos_resource_t *req_resources;
    peos_resource_t *prov_resources;
    

    /** First, get the CGI variables into a list of strings  **/
    cgivars = getcgivars();
 
    pid = atoi((char *) getvalue("pid", cgivars));
    action_name = (char *) getvalue("act_name", cgivars);
    process_filename = (char *) getvalue("process_filename", cgivars);
   
    peos_set_process_table_file(process_filename);
    peos_set_loginname(process_filename);
    
    state = peos_get_act_state(pid, action_name);
    script = peos_get_script(pid, action_name);

    req_resources = peos_get_resource_list_action_requires(pid, action_name, &num_req_resources);
    
    prov_resources = peos_get_resource_list_action_provides(pid, action_name, &num_prov_resources);
    
    if((req_resources == NULL) || (prov_resources == NULL)) {
        for (i=0; cgivars[i]; i++) free(cgivars[i]) ;
        free(cgivars) ;
        goto_error_page(process_filename, "resource list NULL");
	exit(0);
    }
     
    print_header(action_name);
	    
    printf("<form name=\"actionform\" action=\"action_event.cgi\">");

    printf("<table cellpadding=\"2\" cellspacing=\"2\" border=\"1\" style=\"text-align: left; width: 100%%;\">");
    printf("<tbody>");
    printf("<tr>");
    printf("<td style=\"vertical-align: top;\">Name<br></td>");
    printf("<td style=\"vertical-align: top;\">%s<br></td>", action_name);
    printf("</tr>");

    printf("<tr>");
    printf("<td style=\"vertical-align: top;\">State<br></td>");
    printf("<td style=\"vertical-align: top;\">%s<br></td>", state);
    printf("</tr>");

    printf("<tr>");
    printf("<td style=\"vertical-align: top;\">Script<br></td>");
    printf("<td style=\"vertical-align: top;\">%s<br></td>",script);
    printf("</tr>");

    printf("<tr>");
    printf("<td style=\"vertical-align: top;\">Required Resources<br></td>");
    printf("<td style=\"vertical-align: top;\">");

    if(num_req_resources == 0) {
        printf("No Required Resources");
    }
    else {
	int flag = 0;    
        for(i=0; i < num_req_resources; i++) {
	    if((strcmp(req_resources[i].value, "$$") != 0) && (strcmp(req_resources[i].qualifier, "abstract") != 0)){
		char *symlink_resource;
		symlink_resource = (char *) malloc((strlen(req_resources[i].name)+strlen(process_filename)+1) * sizeof(char));
		strcpy(symlink_resource, req_resources[i].name);
		strcat(symlink_resource,process_filename);
		unlink(symlink_resource);
	        symlink(req_resources[i].value, symlink_resource);
	        printf("<a href =\"%s\">%s</a>",symlink_resource,req_resources[i].name);
		flag = 1;
	    }
            else {	    
                printf("%s=%s",req_resources[i].name,req_resources[i].value);
	    }
	    
	    if(i < num_req_resources - 1) {
	        printf(",");
	    }
        }
	if(flag == 1) {
	    printf("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"); 	
	    printf("<a href=\"resource_bindings.cgi?process_filename=%s&pid=%d&act_name=%s&resource_type=requires\">Change Bindings</a>",process_filename,pid,action_name);
	}	    
    }

    printf("<br></td>");
    printf("</tr>");

    printf("<tr>");
    printf("<td style=\"vertical-align: top;\">Provided Resources<br></td>");
    printf("<td style=\"vertical-align: top;\">");
 
    if(num_prov_resources == 0) {
        printf("No Provided Resources");
    }
    else {
	int flag = 0;    
        for(i=0; i < num_prov_resources; i++) {
	    if((strcmp(prov_resources[i].value, "$$") != 0) && (strcmp(prov_resources[i].qualifier, "abstract") != 0)) {
		char *symlink_presource;
		symlink_presource = (char *) malloc((strlen(prov_resources[i].name)+strlen(process_filename)+1) * sizeof(char));
		strcpy(symlink_presource, prov_resources[i].name);
		strcat(symlink_presource,process_filename);
		unlink(symlink_presource);
	        symlink(prov_resources[i].value, symlink_presource);
	        printf("<a href =\"%s\">%s</a>",symlink_presource,prov_resources[i].name);
		flag = 1;
	    }	
	    else {
                printf("%s=%s",prov_resources[i].name,prov_resources[i].value);
	    }
	    
	    if(i < num_prov_resources - 1) {
	        printf(",");
	    }
        }
	if(flag == 1) {
	    printf("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"); 	
	    printf("<a href=\"resource_bindings.cgi?process_filename=%s&pid=%d&act_name=%s&resource_type=provides\">Change Bindings</a>",process_filename,pid,action_name);
	}	    
    }
    
    printf("<br></td>");
    printf("</tr>");
    printf("</tbody>");
    printf("</table>");

    
    printf("<table cellpadding=\"2\" cellspacing=\"2\" border=\"1\" style=\"text-align: left; width: 100%%;\">");
    printf("<tbody>");
    printf("<tr>");
    printf("<td style=\"vertical-align: top;\">");
    printf("<input type=\"Submit\" name=\"action_event\" value=\"Run\"");
    if(strcmp(state,"RUN")==0) printf("disabled");
    printf(">");
    printf("<input type=\"Submit\" name=\"action_event\" value=\"Finish\"");
    if(strcmp(state,"DONE")==0) printf("disabled");
    printf(">");
    printf("<input type=\"Submit\" name=\"action_event\" value=\"Suspend\"");
    if(strcmp(state,"RUN")!=0) printf("disabled");
    printf(">");
    printf("<input type=\"Submit\" name=\"action_event\" value=\"Abort\"");
    if(strcmp(state,"RUN")!=0) printf("disabled");
    printf(">");
    printf("<br></td>");
    printf("</tr>");
    printf("</tbody>");
    printf("</table>");


    printf("<input type=\"hidden\" name=\"pid\" value=\"%d\">", pid);
    printf("<input type=\"hidden\" name=\"act_name\" value=\"%s\">", action_name);
    printf("<input type=\"hidden\" name=\"process_filename\" value=\"%s\">", process_filename);
    
    printf("</form>");   
    print_actionlist_link(process_filename);
    print_createprocess_link(process_filename);
    printf("</body>\n");
    printf("</html>\n");

    /** Free anything that needs to be freed **/
    for (i=0; cgivars[i]; i++) free(cgivars[i]) ;
    free(cgivars) ;

    if(req_resources) free(req_resources);
    
    if(prov_resources) free(prov_resources);
    
    return 0;
}
