/*****************************************************************
 * File:        events.c
 * Author:      Jigar Shah
 * Last Modified : 11/9/03        
 * Description: Event handling functions.
 *****************************************************************/   

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef PALM
#include <dirent.h>
#include <sys/types.h>
#include <assert.h>
#include <time.h>
#else
#include <PalmOS.h>
#endif
#include "process.h"
#include "events.h"
#include "resources.h"
#include "graph_engine.h"
#include "process_table.h"


/* Forward declarations. */
#ifndef PALM
extern char *find_model_file(char *model);
#endif

void error_msg(char *s) 
{
    fprintf(stderr, "error: %s\n", s);
}


#ifdef PALM

/*
	The variant of peos_list_models for PalmOS lists all databases which have the type field set to "peos", case sensitive.
*/
char **peos_list_models()
{
	LocalID db_id;
	char **list;
	char type[4];
	char nameP[33]; //fixed size
	Int16 i,dbc;
	Int16 numPML;
	
	dbc = DmNumDatabases(0);
	if (dbc == 0)
		return NULL;

	numPML=0;
	
	list = malloc( (dbc +1) * sizeof(char *));

	for (i=0; i< dbc; i++) /* scan through all DB */
	{
		db_id = DmGetDatabase(0,i); 
		
		/* get database name info*/
		DmDatabaseInfo(0,db_id,nameP, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, (UInt32 *)&type /* type */,NULL);
		if (strcmp(type, "peos\0") == 0)
		{
			list[numPML]=malloc(sizeof(nameP));
			list[numPML]=strdup(nameP);		
			numPML++;
		}
	}
	list[numPML] = NULL;

	return list; 
}
#else
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
#endif
	    
vm_exit_code peos_notify(int pid, char *action, peos_event event)
{
     vm_exit_code status;
     vm_exit_code update_status;
     
    if(load_process_table() < 0) {
        fprintf(stderr, "System Error: Cannot Load Process Table\n");
	exit(EXIT_FAILURE);
    }
    
    switch (event) {
        case(PEOS_EVENT_RESOURCE_CHANGE):    
		status = update_process_state(pid);
		break;
	
    /* 
     * update the process state for start, suspend, abort and 
     * finish. 
     */ 
      
	case(PEOS_EVENT_START):
		status = handle_action_change(pid, action, ACT_RUN);
		update_status = update_process_state(pid);
		break;
		
	case(PEOS_EVENT_SUSPEND):
		status = handle_action_change(pid, action, ACT_SUSPEND);
		update_status = update_process_state(pid);
		break;
		
	case(PEOS_EVENT_ABORT):
		status = handle_action_change(pid, action, ACT_ABORT);
		update_status = update_process_state(pid);
		break;

	case(PEOS_EVENT_FINISH):
		status = handle_action_change(pid, action, ACT_DONE);
		update_status = update_process_state(pid);
		break;

	default:
		fprintf(stderr, "peos_notify: Unknown Event\n");
		return VM_INTERNAL_ERROR;
    }


    if(save_process_table() < 0) {
        fprintf(stderr, "System Error: Cannot Save Process Table\n");
	exit(EXIT_FAILURE);
    }

    if(update_status == VM_INTERNAL_ERROR) {
        return update_status;
    }
    
    return status;
       
}

int peos_set_resource_binding(int pid, char *resource_name, char *value)
{
    int status;	
    vm_exit_code update_status;

    if(load_process_table() < 0) {
        fprintf(stderr, "System Error: Cannot Load Process Table\n");
	exit(EXIT_FAILURE);
    }
    status = set_resource_binding(pid, resource_name, value);    //see process_table.c
    /* update the process state */
    update_status = update_process_state(pid);

    if(save_process_table() < 0) {    //see process_table.c
        fprintf(stderr, "System Error: Cannot Save Process Table\n");
	exit(EXIT_FAILURE);
    }

    if(update_status == VM_INTERNAL_ERROR){
	fprintf(stderr, "could not update process state\n");
        return -1;
    }
    
    return status;
    
}    

#ifndef PALM
peos_resource_t *peos_get_resource_list_action(int pid,char *name,int *num_resources)
{
    peos_resource_t *resources;
	
    if(load_process_table() < 0) {
        fprintf(stderr, "System Error: Cannot Load Process Table\n");
	exit(EXIT_FAILURE);
    }
    
    resources =  get_resource_list_action(pid,name,num_resources);
    /* No need to update process state here */

    if(save_process_table() < 0) {
        fprintf(stderr, "System Error: Cannot Save Process Table\n");
	exit(EXIT_FAILURE);
    }

    return resources;
}


peos_resource_t *peos_get_resource_list_action_requires(int pid,char *name,int *num_req_resources)
{
    peos_resource_t *resources;	

    if(load_process_table() < 0) {
        fprintf(stderr, "System Error: Cannot Load Process Table\n");
	exit(EXIT_FAILURE);
    }
    
    resources = get_resource_list_action_requires(pid,name,num_req_resources);

    /* No need to update process state here */

    if(save_process_table() < 0) {
        fprintf(stderr, "System Error: Cannot Save Process Table\n");
	exit(EXIT_FAILURE);
    }

    return resources;
}


peos_resource_t *peos_get_resource_list_action_provides(int pid,char *name,int *num_pro_resources)
{
    peos_resource_t *resources;
	
    if(load_process_table() < 0) {
        fprintf(stderr, "System Error: Cannot Load Process Table\n");
	exit(EXIT_FAILURE);
    }
    
    resources =  get_resource_list_action_provides(pid,name,num_pro_resources);

    /* No need to update process state here */

    if(save_process_table() < 0) {
        fprintf(stderr, "System Error: Cannot Save Process Table\n");
	exit(EXIT_FAILURE);
    }

    return resources;
}
#endif



peos_resource_t *peos_get_resource_list(char *model,int *num_resources)
{
    peos_resource_t *resources;	
    char *model_file = find_model_file(model);    //see process.c
    /*
     *  No need to load and save process table as context is not 
     * accessed at all 
     * 
     */
    //printf("model = %s\nmodel_file = %s\n", model, model_file);
    resources =  get_resource_list(model_file,num_resources);    //see resources.c
    
    /* No need to update process state here */

    return resources;

}

peos_resource_t *peos_get_resource_list_context(int pid, int *num_resources)
{
    peos_resource_t *resources;	

    peos_context_t *context;
    
    if(load_process_table() < 0) {
        fprintf(stderr, "System Error: Cannot Load Process Table\n");
	exit(EXIT_FAILURE);
    }
    
    context = peos_get_context(pid);

    if (context == NULL) {
        fprintf(stderr, "peos_get_resource_list_context: context null\n");
	exit(EXIT_FAILURE);
    }
    
    resources = context -> resources;
    *num_resources = context -> num_resources; 

    /* No need to update process state here */

    if(save_process_table() < 0) {
        fprintf(stderr, "System Error: Cannot Save Process Table\n");
	exit(EXIT_FAILURE);
    }

    return resources;

}


char *peos_get_script(int pid, char *act_name)
{
    char *script;	

    if(load_process_table() < 0) {
        fprintf(stderr, "System Error: Cannot Load Process Table\n");
	exit(EXIT_FAILURE);
    }
    
    script = get_script(pid,act_name);

    /* No need to update process state here */

    if(save_process_table() < 0) {
        fprintf(stderr, "System Error: Cannot Save Process Table\n");
	exit(EXIT_FAILURE);
    }

    return script;

}
 
int peos_delete_process_instance(int pid)
{

    int status;	

    if(load_process_table() < 0) {
        fprintf(stderr, "System Error: Cannot Load Process Table\n");
	exit(EXIT_FAILURE);
    }
    
    status = delete_entry(pid);

    /* No need to update process state here */

    if(save_process_table() < 0) {
        fprintf(stderr, "System Error: Cannot Save Process Table\n");
	exit(EXIT_FAILURE);
    }

    return status;
}

int peos_run(char *process, peos_resource_t *resources,int num_resources)
{
    int pid;
    char msg[256];
    vm_exit_code update_status;
    
    char *model_file = find_model_file(process);
   

    if (model_file == NULL) {
        fprintf(stderr, "peos_run: can't find model file for process %s\n",process);
	return -1;
    }

    if(load_process_table() < 0) {
        fprintf(stderr, "System Error: Cannot Load Process Table\n");
	exit(EXIT_FAILURE);
    }
    // this is the only change in peos_run
    pid = peos_create_instance(model_file,resources,num_resources);

    sprintf(msg, " %s start %s %d",login_name, model_file, pid);
    log_event(msg);
    
     
    /* update process state if pid >= 0 */

    if(pid >= 0) {
        update_status = update_process_state(pid);
    }	
    
    if(save_process_table() < 0) {
        fprintf(stderr,"System Error: Cannot Save Process Table\n");
        exit(EXIT_FAILURE);
    }
     
    if(update_status == VM_INTERNAL_ERROR) {
        return -1;
    }
    
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
