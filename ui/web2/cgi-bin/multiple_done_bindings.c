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
    char *pidstr;
    int pid;
    char **actions = (char **) calloc(256, sizeof(char **));
    int num_actions;
    char *process_filename;
    peos_resource_t *unbound_resource_list;
    int num_unbound_resources;
 
    

    /** First, get the CGI variables into a list of strings         **/
    cgivars = getcgivars();
 
    process_filename = (char *) getvalue("process_filename", cgivars);
    pidstr = (char *) getvalue("pid", cgivars);
    pid = atoi(pidstr);
 
    for(i=0,num_actions=0; cgivars[i]; i+=2) {
        if(strcmp(cgivars[i],"actions") == 0) {
	    actions[num_actions] = cgivars[i+1];
            num_actions++;
	}
    }
    
    peos_set_process_table_file(process_filename);
    peos_set_loginname(process_filename);
    num_unbound_resources=0;

    unbound_resource_list = (peos_resource_t *) calloc(256, sizeof(peos_resource_t));

        
    for(i=0; i<num_actions; i++) {
	int num_resources;    
	peos_resource_t *resources;
	int j;
 	
        resources = peos_get_resource_list_action_provides(pid, actions[i], &num_resources);
    
	if(resources == NULL) {
	    goto_error_page(process_filename, "resource list null");
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


    for(i=0; i < num_unbound_resources; i++) {
	char *value;    
	value = (char *) getvalue(unbound_resource_list[i].name, cgivars);    
	peos_set_resource_binding(pid, unbound_resource_list[i].name, value);
    }

    for(i=0; i<num_actions; i++) {
        peos_notify(pid, actions[i], PEOS_EVENT_FINISH);
    }

    printf("Location: action_list.cgi?process_filename=%s&start=false\r\n\r\n",process_filename);

    if(unbound_resource_list) free(unbound_resource_list);
        
    /** Free anything that needs to be freed **/
    for (i=0; cgivars[i]; i++)
    free(cgivars[i]) ;
    free(cgivars);

    exit(0);    
}	
