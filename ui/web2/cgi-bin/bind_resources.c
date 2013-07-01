#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "getcgi.h"
#include "kernel/events.h"
#include "kernel/action.h"
#include "kernel/process_table.h"
#include "html.h"
#include "xmlparse.h"


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
    char name[12];
    char unbound_value[256];
    int change = 0;

    /** First, get the CGI variables into a list of strings         **/
    cgivars = getcgivars();
 
    pid = atoi((char *) getvalue("pid", cgivars));
    action_name = (char *) getvalue("act_name", cgivars);
    process_filename = (char *) getvalue("process_filename", cgivars);
    resource_type = (char *) getvalue("resource_type", cgivars);
    action = (char *) getvalue("action", cgivars);
    
    //process_filename = get_process_filename();
    
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
            sprintf(unbound_value, "${%s}", resources[i].name);
	    if((strcmp(resources[i].value, unbound_value) == 0) && (strcmp(resources[i].qualifier,"abstract") != 0)) {
	        strcpy(unbound_resource_list[num_unbound_resources].name,resources[i].name);
	        num_unbound_resources++;
	    }
	}
    }

    for(i=0; i < num_resources; i++) {
	char *value;    
	sprintf(name, "resource%d", i);
	value = (char *) getvalue(name, cgivars);    
	peos_set_resource_binding(pid, resources[i].name, value);
    }


    /*if ((action != NULL) && (strcmp(action, "change") == 0)) {
        printf("Location: action_page.cgi?process_filename=%s&pid=%d&action_name=%s\r\n\r\n",process_filename,pid,action_name);
}*/

    if (action != NULL)
        change = !strcmp(action, "change");
    
    if(strcmp(resource_type,"requires") == 0) {
        if (!change)
            peos_notify(pid, action_name, PEOS_EVENT_START);
        printf("Location: action_page.cgi?&pid=%d&action_name=%s\r\n\r\n",pid,action_name);
    }
    else {
        _action_page *new_ap;
        char *xml_data_filename;
        if (!change) 
            peos_notify(pid, action_name, PEOS_EVENT_FINISH);
        xml_data_filename = (char *) malloc((strlen(process_filename) + strlen(".xml") + 1) * sizeof(char));
        strcpy(xml_data_filename, process_filename);
        strcat(xml_data_filename, ".xml");
        new_ap = get_action_page_details(xml_data_filename, pid, action_name);
        if (!new_ap)
            printf("Location: active_processes.cgi?action=continue\r\n\r\n");
        else {
            if (strcmp(new_ap->action_list[new_ap->total_actions - 1], action_name) == 0) {
                if (strcmp(new_ap->state_list[new_ap->total_actions - 1], "DONE") == 0) {
                    peos_notify(pid, action_name, PEOS_EVENT_FINISH);
                    printf("Location: active_processes.cgi?action=continue\r\n\r\n");
                }
                else
                    printf("Location: action_page.cgi?pid=%d&action_name=%s\r\n\r\n", pid, action_name);
            }
            else {
                for (i = 0; i < new_ap->total_actions; i++)
                    if (strcmp(new_ap->action_list[i], action_name) == 0)
                        if (change)
                            printf("Location: action_page.cgi?pid=%d&action_name=%s\r\n\r\n", pid, new_ap->action_list[i]); //stay at the same action
                        else
                            printf("Location: action_page.cgi?pid=%d&action_name=%s\r\n\r\n", pid, new_ap->action_list[i + 1]);  //go to next action
            }
        }
    }

    if(unbound_resource_list) free(unbound_resource_list);
    
    if(resources) free(resources);   
     
    /** Free anything that needs to be freed **/
    for (i=0; cgivars[i]; i++)
    free(cgivars[i]) ;
    free(cgivars);

    exit(0);    
}	
