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
#include "vm.h"
#include "process.h"
#include "events.h"

/* Global variabless. */


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
	    if (strcmp(ext, ".txt") == 0) {
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

int peos_set_ready(char *action)
{
    return handle_action_change(action, ACT_READY);
}

/*
 * Set action state to 'running.'
 */
int peos_run_action(char *action)
{
    return handle_action_change(action, ACT_RUN);
}

/*
 * Set action state to 'suspended'.
 */
int peos_suspend_action(char *action)
{
    return handle_action_change(action, ACT_SUSPEND);
}

/*
 * Set action state to 'done', awaken any waiting processes.
 */
vm_exit_code peos_finish_action(char *action)
{
    return handle_action_change(action, ACT_DONE);
}

/*
 * Set action state to 'aborted', kill owner and children. 
 */
int peos_abort_action(peos_action_t action)
{
    return 1;
}



int peos_run(char *process, int line)
{
    vm_exit_code status;
    int pid = peos_create_instance(process);
    
    if (pid >= 0 
	&& (status = peos_resume(pid)) != VM_ERROR 
	&& status != VM_INTERNAL_ERROR) {
	return pid;
    } else {
	return -1;
    }

}


#ifdef UNIT_TEST
#include "test_events.c"
#endif
