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
    char *action;
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
    action = (char *) getvalue("action", cgivars);
    
    peos_set_process_table_file(process_filename);
    peos_set_loginname(process_filename);
     
    if(strcmp(resource_type,"requires") == 0) {
        resources = peos_get_resource_list_action_requires(pid, action_name, &num_resources);
    }
    else {
        resources = peos_get_resource_list_action_provides(pid, action_name, &num_resources);
    }

    if(resources == NULL) {
        //goto_error_page(process_filename, "resource list null");
        for (i=0; cgivars[i]; i++)
           free(cgivars[i]) ;
        free(cgivars);
	exit(0);
    }
    
    unbound_resource_list = (peos_resource_t *) calloc(num_resources+1, sizeof(peos_resource_t));
    num_unbound_resources = 0;
    for(i=0; i < num_resources; i++) {
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

    char name[12];
    //for(i=0; i < num_unbound_resources; i++) {
    for(i=0; i < num_resources; i++) {
	char *value;    
	sprintf(name, "resource%d", i);
	value = (char *) getvalue(name, cgivars);    
	//peos_set_resource_binding(pid, unbound_resource_list[i].name, value);
	peos_set_resource_binding(pid, resources[i].name, value);
    }


    if ((action != NULL) && (strcmp(action, "change") == 0)) {
        printf("Location: action_page.cgi?process_filename=%s&pid=%d&action_name=%s\r\n\r\n",process_filename,pid,action_name);
    }

    if(strcmp(resource_type,"requires") == 0) {
        peos_notify(pid, action_name, PEOS_EVENT_START);
        printf("Location: action_page.cgi?process_filename=%s&pid=%d&action_name=%s\r\n\r\n",process_filename,pid,action_name);
    }
    else {
        peos_notify(pid, action_name, PEOS_EVENT_FINISH);
        printf("Location: active_processes.cgi?action=continue&process_filename=%s\r\n\r\n",process_filename);
    }

    if(unbound_resource_list) free(unbound_resource_list);
    
    if(resources) free(resources);   
     
    /** Free anything that needs to be freed **/
    for (i=0; cgivars[i]; i++)
    free(cgivars[i]) ;
    free(cgivars);

    exit(0);    
}	
