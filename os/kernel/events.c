/*****************************************************************
 * File:        events.c
 * Author:      Jigar Shah
 * Last Modified : 11/9/03        
 * Description: Event handling functions.
 *****************************************************************/   

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "process.h"
#include "events.h"
#include "resources.h"
#include "graph_engine.h"

/* XXX need to declare this as a real api function.*/
extern char *act_state_name(vm_act_state state);

/* Forward declarations. */
extern char *find_model_file(char *model);

void error_msg(char *s) 
{
    fprintf(stderr, "error: %s\n", s);
}

char **peos_list_models()
{
    static char **result;
    int r = 0, rsize = 256;
    DIR * dir;
    struct dirent * ent;
    char * ext;
    char * COMP_DIR = getenv("COMPILER_DIR");
  
    if (COMP_DIR == NULL) {
        COMP_DIR = ".";
    }
    dir = opendir(COMP_DIR);
    if (dir == NULL) {
        error_msg("COMPILER_DIR directory does not exist\n");
	return NULL;
    }

    result = calloc(rsize, sizeof(char *));
    ent = readdir(dir);
    while (ent) {
	ext = strrchr(ent->d_name, '.');
	if (ext != NULL) {
	    if (strcmp(ext, ".pml") == 0) {
	        ext[0] = 0;
		if (r >= rsize) {
		    rsize += 256;
		    result = (char **) realloc(result, rsize * sizeof(char *));
		}
		result[r++] = strdup(ent->d_name);
	    }
	}
	 
	ent = readdir(dir);
    }
    result[r] = NULL;
    return result;
}


vm_exit_code peos_set_action_state(int pid, char *action, vm_act_state state)
{
    return handle_action_change(pid, action, state);
}
	    
vm_exit_code peos_notify(int pid, char *action, peos_event event)
{
    switch (event) {
        case(PEOS_EVENT_REQUIRES): 
		return handle_resource_event(pid, action, REQUIRES_TRUE);
		break;
	
	case(PEOS_EVENT_PROVIDES):
		return handle_resource_event(pid, action, PROVIDES_TRUE);
		break;
	
	case(PEOS_EVENT_START):
		return peos_set_action_state(pid, action, ACT_RUN);
		break;
		
	case(PEOS_EVENT_SUSPEND):
		return peos_set_action_state(pid, action, ACT_SUSPEND);
		break;
		
	case(PEOS_EVENT_ABORT):
		return peos_set_action_state(pid, action, ACT_ABORT);
		break;

	case(PEOS_EVENT_FINISH):
		return peos_set_action_state(pid, action, ACT_DONE);
		break;

	default:
		fprintf(stderr, "Unknown Event\n");
		return VM_INTERNAL_ERROR;
    }
}


peos_resource_t *peos_get_resource_list_action(int pid,char *name,int *num_resources)
{
    return get_resource_list_action(pid,name,num_resources);
}


peos_resource_t *peos_get_resource_list_action_requires(int pid,char *name,int *num__req_resources)
{
    return get_resource_list_action_requires(pid,name,num__req_resources);
}


peos_resource_t *peos_get_resource_list_action_provides(int pid,char *name,int *num_pro_resources)
{
    return get_resource_list_action_provides(pid,name,num_pro_resources);
}


peos_resource_t *peos_get_resource_list(char *model,int *num_resources)
{
    char *model_file = find_model_file(model);
    return get_resource_list(model_file,num_resources);
}

char *peos_get_script(int pid, char *act_name)
{
    return get_script(pid,act_name);
}
    


int peos_run(char *process, peos_resource_t *resources,int num_resources)
{
    int pid;
    char msg[256];
    
    char *model_file = find_model_file(process);
   

    if (model_file == NULL) {
        fprintf(stderr, "peos_run: can't find model file for process %s\n",process);
	return -1;
    }

    // this is the only change in peos_run
    pid = peos_create_instance(model_file,resources,num_resources);

    sprintf(msg, " jnoll start %s %d", model_file, pid);
    log_event(msg);
    
    if (pid >= 0) {  
        return pid;
    } 
    else {
	fprintf(stderr, "peos_run: can't run process %s\n",process);
	return -1;
    }
}


#ifdef UNIT_TEST
#include "test_events.c"
#endif
