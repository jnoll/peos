#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "events.h"
#include "process_table.h"
#include "pmlheaders.h"
#include "graph_engine.h"
#include "process.h"
#include "resources.h"

extern char *act_state_name(vm_act_state state);


int is_requires_true(int pid, char *act_name)
{
    peos_resource_t *resources;
    int num_resources;
    int i;

    resources = get_resource_list_action_requires(pid,act_name,&num_resources);
    if (num_resources == 0) {
        return 1;
    }
    else {
        struct stat buf;
	for(i = 0; i < num_resources; i++)
	{
	    if(strcmp(resources[i].qualifier, "abstract") != 0) {
	        if(strcmp(resources[i].value,"$$") == 0) return 0;
	        if(stat(resources[i].value, &buf) == -1) {
	            if(errno == ENOENT) { /* If stat failed because file didn't exist */
	                return 0;
	            }
	            else {
	                fprintf(stderr, "Required Resource Detection Error for %s\n",resources[i].name);
		        return 0;
	            }
	        }
	    }
	}
        return 1;
    }
     
}


int is_provides_true(int pid, char *act_name)
{
    peos_resource_t *resources;
    int num_resources;
    int i;

    resources = get_resource_list_action_provides(pid,act_name,&num_resources);
    if (num_resources == 0) {
        return 1;
    }
    else {
        struct stat buf;
	for(i = 0; i < num_resources; i++)
	{
	    if(strcmp(resources[i].qualifier, "abstract") != 0) {
	        if((strcmp(resources[i].value,"$$") == 0) && (strcmp(resources[i].qualifier, "abstract") != 0)) return 0;
	        if(stat(resources[i].value, &buf) == -1) {
	            if(errno == ENOENT) { /* If stat failed because file didn't exist */
	                return 0;
	            }
	            else {
	                fprintf(stderr, "Provided Resource Detection Error for %s\n",resources[i].name);
		        return 0;
	            }
	        }
	    }
	}
        return 1;
    }
}


#ifdef UNIT_TEST
#include "test_predicate_evaluator.c"
#endif
