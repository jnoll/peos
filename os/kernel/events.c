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
#include "error.h"
#include "events.h"
#include "resources.h"
#include "graph_engine.h"
#include "process_table.h"
#include "peos_util.h"

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

int trim(char* str)
{
    int fi;    //an index of the first character after whitespace(s)
    int li;    //an index of the last character before whitespace(s)
    int old_len = strlen(str);    //orginal string length
    int i;
    int new_len;    //trimmed string length

    for (fi = 0; (fi < old_len) && (str[fi] == ' '); fi++)    //get fi
        ;

    for (li = old_len - 1; (li >= 0) && ((str[li] == ' ') || (str[li] == '\n')); li--)    //get li
        ;

    new_len = li - fi + 1;

    for (i = 0; i < new_len; i++)   //shift char forward
        str[i] = str[fi + i];

    for (; i < old_len; i++)    //clear whitespace(s) and newline
        str[i] = '\0';

    return strlen(str);
}

int peos_bind_resource_file(int pid, char* res_file) {
    FILE* f;
    char* res_name;
    char* res_value;
    char line[600];
    vm_exit_code update_status;
    
    if (!(f = fopen(res_file, "r"))) {
        fprintf(stderr, "resource file does not exist\n");
        return 0;
    }
    
    if(load_process_table() < 0) {
        fprintf(stderr, "System Error: Cannot Load Process Table\n");
        exit(EXIT_FAILURE);
    }
    
    while (fgets(line, 600, f))
    {
        if ((res_name = strtok(line, ":")) && (res_value = strtok(NULL, ":"))) {
            trim(res_name);
            trim(res_value);
            set_resource_binding(pid, res_name, res_value);
        }
    }
    fclose(f);
    
    update_status = update_process_state(pid);
    
    if(save_process_table() < 0) {    //see process_table.c
        fprintf(stderr, "System Error: Cannot Save Process Table\n");
        exit(EXIT_FAILURE);
    }
    if(update_status == VM_INTERNAL_ERROR){
        fprintf(stderr, "could not update process state\n");
        return -1;
    }
    return 1;
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

int peos_eval_resource_list(int pid, peos_resource_t** resources, int num_resources) {
    peos_resource_t *proc_resources;
    peos_context_t *context;
    int i;

    context = peos_get_context(pid);
    proc_resources = (peos_resource_t *) calloc(context->num_resources, sizeof(peos_resource_t));
    for (i = 0; i < context->num_resources; i++) {
        strcpy(proc_resources[i].name, context->resources[i].name);
        strcpy(proc_resources[i].value, context->resources[i].value);
    }
    if (eval_resource_list(&proc_resources, context->num_resources) == 0)
        return 0;
    fill_resource_list_value(proc_resources, context->num_resources, resources, num_resources);
    free(proc_resources);
    return 1;
}

peos_resource_t *peos_get_resource_list_action(int pid,char *name,int *num_resources)
{
    peos_resource_t *resources;

    if(load_process_table() < 0) {
        fprintf(stderr, "System Error: Cannot Load Process Table\n");
	exit(EXIT_FAILURE);
    }

    resources =  get_resource_list_action(pid, name, num_resources);
    peos_eval_resource_list(pid, &resources, *num_resources);
    
    /* No need to update process state here */

    if(save_process_table() < 0) {
        fprintf(stderr, "System Error: Cannot Save Process Table\n");
	exit(EXIT_FAILURE);
    }
    return resources;
}


peos_resource_t *peos_get_resource_list_action_requires(int pid,char *name,int *num_resources)
{
    peos_resource_t *resources;

    if(load_process_table() < 0) {
        fprintf(stderr, "System Error: Cannot Load Process Table\n");
	exit(EXIT_FAILURE);
    }

    resources = get_resource_list_action_requires(pid,name,num_resources);
    peos_eval_resource_list(pid, &resources, *num_resources);

    /* No need to update process state here */

    if(save_process_table() < 0) {
        fprintf(stderr, "System Error: Cannot Save Process Table\n");
	exit(EXIT_FAILURE);
    }

    return resources;
}


peos_resource_t *peos_get_resource_list_action_provides(int pid,char *name,int *num_resources)
{
    peos_resource_t *resources;
    	
    if(load_process_table() < 0) {
        fprintf(stderr, "System Error: Cannot Load Process Table\n");
	exit(EXIT_FAILURE);
    }

    resources =  get_resource_list_action_provides(pid,name,num_resources);
    peos_eval_resource_list(pid, &resources, *num_resources);

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
    char *model_file = find_file(model);
    /*
     *  No need to load and save process table as context is not 
     * accessed at all 
     * 
     */
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

char* peos_get_resource_file(char* process){
    int i;
    char* res_file, *res_path;
    if (process == NULL || (strlen(process) < 5))
        return NULL;
    i = strlen(process) - 4;
    if (strcmp(process + i, ".pml") != 0)
        return NULL;
    res_file = strdup(process);
    res_file[++i] = 'r';
    res_file[++i] = 'e';
    res_file[++i] = 's';
    if ((res_path = find_file(res_file))) {
        return res_path;
    }
    if (res_file)
        free(res_file);		/* XXX should be freed above? */
    if (res_path)
        free(res_path);
    return NULL;
}

int peos_run(char *process, peos_resource_t *resources,int num_resources)
{
    int pid;
    char msg[256];
    vm_exit_code update_status;
    char *model_file;
    
    model_file = find_file(process);
    
    if (model_file == NULL) {
        fprintf(stderr, "peos_run: can't find model file for process %s\n",process);
	return -1;
    }

    if (load_process_table() < 0) {
        fprintf(stderr, "peos_run: cannot load process table\n");
	exit(EXIT_FAILURE);
    }
    // this is the only change in peos_run
    pid = peos_create_instance(model_file,resources,num_resources);

    sprintf(msg, " %s start %s %d",login_name, model_file, pid);
    log_event(msg);

    if (pid < 0) {
        fprintf(stderr, "peos_run: can't run process %s\n",process);
        return -1;
    }
    
    if ((update_status = update_process_state(pid)) == VM_INTERNAL_ERROR) {
	fprintf(stderr, "peos_run: error updating process state\n");
        return -1;
    }
    
    if (save_process_table() < 0) {
        fprintf(stderr, "peos_run: cannot save process table\n");
        exit(EXIT_FAILURE);
    }
    
    return pid;
}


#ifdef UNIT_TEST
#include "test_events.c"
#endif
