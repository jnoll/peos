
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <fcntl.h>
#include <unistd.h>
#include "events.h"
#include "process_table.h"
#include "pmlheaders.h"
#include "graph_engine.h"
#include "process.h"

extern char *act_state_name(vm_act_state state);


void list_actions()
{
    peos_action_t *alist;
    int i=0;
    int j;
    
    char ** result = peos_list_instances();
	
    printf("process action (status)\n");
    while((i <= PEOS_MAX_PID)) {
	int num_actions;    
	if (result[i] != NULL) {
            alist = peos_list_actions(i,&num_actions);
	    if(alist) {
	        for(j = 0; j < num_actions; j++) {
	            printf("%d  %s  %s\n",i,alist[j].name,
			   act_state_name(alist[j].state));
	        }
	    }
	    i++;
	}
    }
}    

int is_requires_true(int pid, char *act_name)
{
    peos_resource_t *resources;
    int num_resources;

    resources = peos_get_resource_list_action_requires(pid,act_name,&num_resources);
    if (num_resources == 0) {
        return 1;
    }
    else {
        struct stat buf;
	if(stat(resources[0].value, &buf) == -1) {
	    if(errno == ENOENT) { /* If stat failed because file didn't exist */
	        return 0;
	    }
	    else {
	        fprintf(stderr, "Required Resource Detection Error for %s\n",resources[0].name);
		return 0;
	    }
	}
        else {
	    return 1;
	}
    }
}


int is_provides_true(int pid, char *act_name)
{
    peos_resource_t *resources;
    int num_resources;

    resources = peos_get_resource_list_action_provides(pid,act_name,&num_resources);
    if (num_resources == 0) {
        return 1;
    }
    else {
        struct stat buf;
	if(stat(resources[0].value, &buf) == -1) {
	    if(errno == ENOENT) { /* If stat failed because file didn't exist */
	        return 0;
	    }
	    else {
	        fprintf(stderr, "Provided Resource Detection Error for %s\n",resources[0].name);
		return 0;
	    }
	}
        else {
	    return 1;
	}
    }
}


int
main()
{

    int pid, j;
    peos_action_t *alist;
    char **result = peos_list_instances();

    fprintf(stderr, "Enactment Daemon started .. \n");

    while (1) {

        for (pid = 0; pid <= PEOS_MAX_PID; pid++) {
            int num_actions;
            if (result[pid] != NULL) {
                alist = peos_list_actions(pid, &num_actions);
                if (alist) {
                    for (j = 0; j < num_actions; j++) {
			if(is_requires_true(pid, alist[j].name)) {
			    fprintf(stderr,"setting REQUIRES_TRUE for action %s \n",alist[j].name);      
			    peos_notify(pid, alist[j].name, PEOS_EVENT_REQUIRES);
			    list_actions();
			}
			if(is_provides_true(pid, alist[j].name)) {
			    fprintf(stderr,"setting PROVIDES_TRUE for action %s \n",alist[j].name);      
			    peos_notify(pid, alist[j].name, PEOS_EVENT_PROVIDES);
			    list_actions();
			}
		    }
		}
	    }
	}
        sleep(60);
    }
}


