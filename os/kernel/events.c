/*****************************************************************
 * File:        events.c
 * Author:      Tingjun Wen
 * Date:        7/19/99
 * Description: Event handling functions.
 *****************************************************************/   

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "vm.h"
#include "process.h"
#include "events.h"
#include "graph_engine.h"





/* Global variables. */

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


int peos_set_action_state(int pid,char *action, vm_act_state state)
{
    switch(state) {
        case ACT_READY : return handle_action_change(pid,action,ACT_READY);
			 break;
        case ACT_RUN : return handle_action_change(pid,action,ACT_RUN);
		       break;
		               
        case ACT_DONE : return handle_action_change(pid,action,ACT_DONE);
			break;
        case ACT_SUSPEND : return handle_action_change(pid,action,ACT_SUSPEND);
			   break;
        case ACT_ABORT : return handle_action_change(pid,action,ACT_ABORT);
			 break;
	default : {
	              fprintf(stderr,"\nInvalid Action State\n");
		      return -1;
		  }
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


int peos_run(char *process, peos_resource_t *resources,int num_resources)
{
    int pid;
    char *model_file = find_model_file(process), times[20];
    FILE *file;
    struct tm *current_info;
    time_t current;


    if (model_file == NULL) {
        fprintf(stderr, "peos_run: can't find model file for process %s\n",process);
	return -1;
    }

    // this is the only change in peos_run
    pid = peos_create_instance(model_file,resources,num_resources);

    time(&current);
    current_info = localtime(&current);
    current = mktime(current_info);
    strftime(times,25,"%b %d %Y %H:%M",localtime(&current));
    file = fopen("event.log", "a");
    fprintf(file, "%s jnoll start %s %d\n", times, model_file, pid);
    fclose(file);
    
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
