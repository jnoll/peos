
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


int resource_changed(char *resource_value)
{
    struct stat buf;
    
    if(stat(resource_value, &buf) == -1) {
        if(errno == ENOENT) { /* If stat failed because file didn't exist */
	    return 0;
	}
	else {
	    fprintf(stderr, "Required resource detection error for %s\n", resource_value);
	    return 0;
	}
    }
    else {
        return 1;
    }
}

int
main()
{

    int pid, j;
    char **result = peos_list_instances();
    peos_resource_t *resource_list;
    fprintf(stderr, "Enactment Daemon started .. \n");

    while (1) {

        for (pid = 0; pid <= PEOS_MAX_PID; pid++) {
            int num_resources;
	    int resource_change_flag = 0;
            if (result[pid] != NULL) {
                resource_list = peos_get_resource_list_context(pid, &num_resources);
                if (resource_list) {
                    for (j = 0; j < num_resources; j++) {
			if(resource_changed(resource_list[j].value)) {
			    fprintf(stderr,"Detected change in resource %s \n",resource_list[j].name);      
			    resource_change_flag = 1;
			}
		    }
		    if (resource_change_flag == 1) {
		        if(peos_notify(pid, "dummy_action", PEOS_EVENT_RESOURCE_CHANGE) == VM_INTERNAL_ERROR) {
			    peos_perror("Enactment daemon: Error in notifying resource change event");
			    exit(EXIT_FAILURE);
			}
		    }
		}
	    }
	}
	sleep(60);
    }
}
		    	    


