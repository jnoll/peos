/* -*-C-*-
*****************************************************************************
*
* File:         $RCSFile: process.c$
* Version:      $Id: process.c,v 1.1 2003/06/24 21:01:54 jnoll Exp $ ($Name:  $)
* Description:  Functions for manipulating process instances.
* Author:       John Noll, Santa Clara University
* Created:      Sat Feb  8 20:55:52 2003
* Modified:     Wed Apr  2 15:14:25 2003 (John Noll, SCU) jnoll@carbon.cudenver.edu
* Language:     C
* Package:      N/A
* Status:       $State: Exp $
*
* (c) Copyright 2003, Santa Clara University, all rights reserved.
*
*****************************************************************************
*/
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "vm.h"
#include "process.h"

/* Globals. */
peos_context_t process_table[1];
int num_proc = 1;
peos_context_t *current_process;

/* Forward declarations. */

void push_ctx(int arg, vm_context_t *context)
{
    context->stack[++(context->SP)] = arg;
    assert(context->SP < MAX_STACK_SIZE);
} /* push */


int 
load_context(vm_context_t *context, peos_action_t **actions, int *num_actions, char *file)
{
    int i;
    char buf[BUFSIZ];
    FILE *in = fopen(file, "r");

    if (!in) {
	context->PC = -1;
	context->SP = 0;
	context->A = -1;
	return -1;
    }

    /* Registers. */
    fscanf(in, "%s: ", buf);
    if (strcmp(buf, "registers:") != 0) {
	return -1;
    }
    fscanf(in, "%d %d %d", &(context->PC), &(context->SP), &(context->A));

    fscanf(in, "%s: ", buf);
    if (strcmp(buf, "stack:") != 0) {
	return -1;
    }
    for(i = 0; i < context->SP; i++) {
	fscanf(in, "%d ", &(context->stack[i]));
    }

    fscanf(in, "%s: ", buf);
    if (strcmp(buf, "variables:") != 0) {
	return -1;
    }
    fscanf(in, "%d", &(context->num_variables));
    context->variables = (vm_vbinding_t *)calloc(context->num_variables + 1,
						 sizeof(vm_vbinding_t));
    for(i = 0; i < context->num_variables; i++) {
	fscanf(in, "%s %i", context->variables[i].name, &(context->variables[i].value));
    }
    context->variables[i].name = NULL;
    context->num_variables = i;

    fscanf(in, "%s: ", buf);
    if (strcmp(buf, "actions:") != 0) {
	return -1;
    }
    fscanf(in, "%d", num_actions);
    *actions = (peos_action_t *)calloc(*num_actions + 1,
				       sizeof(peos_action_t));
    for(i = 0; i < *num_actions; i++) {
	char buf[BUFSIZ];
	fscanf(in, "%s %d", buf, (int *)&(*actions)[i].state);
	(*actions)[i].name = strdup(buf);
    }
    (*actions)[i].name = NULL;

    return 0;
}

int 
save_context(vm_context_t *context, peos_action_t *actions, int num_actions, char *file)
{
    int i;
    FILE *out = fopen(file, "w");

    if (!out) {
	return -1;
    }

    fprintf(out, "registers: %d %d %d\n", 
	    context->PC, context->SP, context->A);
    fprintf(out, "stack: ");
    for (i = 0; i < context->SP; i++) {
	fprintf(out, "%d ", context->stack[i]);
    }
    fprintf(out, "\n");

    fprintf(out, "variables: %d\n", context->num_variables);
    for (i = 0; i < context->num_variables; i++) {
	fprintf(out, "%s %d\n", context->variables[i].name, 
		context->variables[i].value);
    }

    fprintf(out, "actions: %d\n", num_actions);
    for (i = 0; i < num_actions; i++) {
	fprintf(out, "%s %d\n", actions[i].name, actions[i].state);
    }

    fclose(out);
    return 0;
}

int find_start(char *inst_array[], int size)
{
    int i;
    for(i = 0; inst_array[i] && i < size; i++) {
	if (strncmp(inst_array[i], "start", strlen("start")) == 0) {
	    return i;
	}
    }
    return -1;			/* Not found - error. */
}


int init_context(vm_context_t *context, int pc)
{
    context->PC = pc;
    context->A = -1;
    context->SP = 0;			/* XXX 0 or -1? */
    /* XXX What should stack contents be at initialization? */

    return context->PC;
}

char** add_inst(char **inst_array, char *inst, int num, int *size)
{
    if (num == *size) {
	*size += INST_ARRAY_INCR;
	inst_array = (char **) realloc(inst_array, *size * sizeof(char *));
    }
    inst_array[num] = inst;
    return inst_array;		/* So realloc can be captured. */
}

peos_action_t *add_act(peos_action_t *act_array, char *inst, int num, int *size)
{
    char *act; int len;
    if (num == *size) {
	*size += INST_ARRAY_INCR;
	act_array = (peos_action_t *) realloc(act_array, *size * sizeof(peos_action_t));
    }
    len = strcspn(inst, " \t\n");
    act = calloc(len+1, sizeof(char));
    strncpy(act, inst, len);
    act_array[num].name = act;
    act_array[num].state = ACT_NONE;
    return act_array;		/* So realloc can be captured. */
}


int load_instructions(char *file, char ***inst, int *num_inst,
			 peos_action_t **actions, int *num_actions)
{
    int i, start;
    char buf[BUFSIZ], *s;
    FILE *in;

    in = fopen(file, "r");
    if (in) {
	int asize = INST_ARRAY_INCR, isize = INST_ARRAY_INCR;
	peos_action_t *act_array = (peos_action_t *) calloc(asize, sizeof(peos_action_t));
	char **inst_array = (char **) calloc(isize, sizeof(char *));

	i = 0;
	while (fgets(buf, BUFSIZ, in)) {
	    s = buf;
	    s += strcspn(buf, " \t\n");
	    s += strspn(s, " \t\n");
	    inst_array = add_inst(inst_array, strdup(s), i, &isize);
	    if (strncmp(s, "start", strlen("start")) == 0) {
		*num_actions = i;
		start = i++; 
		break;
	    }
	    act_array = add_act(act_array, strdup(s), i++, &asize);
	}
	*actions = act_array;


	while (fgets(buf, BUFSIZ, in)) {
	    char *s = buf;
	    s += strcspn(buf, " \t\n");
	    s += strspn(s, " \t\n");
	    inst_array = add_inst(inst_array, strdup(s), i++, &isize);
	}
	inst_array = add_inst(inst_array, NULL, i, &isize);
	fclose(in);
	*num_inst = i;
	*inst =  inst_array;
	return start;
    } else {
	return -1;
    }

}

peos_action_t *peos_list_actions(vm_act_state state)
{
    return peos_find_actions(state, current_process->actions, 
			     current_process->num_actions);
}

int *peos_list_instances()
{
    int *result = calloc(2, sizeof(int));
    result[0] = 0;
    result[1] = -1;
    return result;
	
#ifdef NOTUSED
    char *endp, **result;
    int r = 0, rsize = 256;
    DIR *dir;
    struct dirent *ent;
    char *ext;
    char *peos_dir = getenv("PEOS_DIR");
  
    if (peos_dir == NULL) {
	peos_dir = "process_table";
    }

    dir = opendir(peos_dir);
    if (dir == NULL) {
	fprintf(stderr, "peos_list_instances: PEOS_DIR directory does not exist\n");
	return NULL;
    }

    result = calloc(rsize, sizeof(char *));
    ent = readdir(dir);
    while (ent) {
	ext = strrchr(ent->d_name, '.');
	if (ext != NULL) {
	    strtol(ext + 1, &endp, 0);
	    if (*(ext + 1) != '\0' && *endp == '\0') {
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
#endif
    return result;
}

int peos_create_instance(char *model)
{
    int start = -1;
    char *ext,  model_name[BUFSIZ];
    char *model_dir, *peos_dir;
    peos_context_t *context;


    peos_dir = getenv("PEOS_DIR");
    if (peos_dir == NULL) {
	fprintf(stderr, "peos_create_instances: PEOS_DIR not set.\n");
	return 0;
    }

    model_dir = getenv("COMPILER_DIR");
    if (model_dir == NULL) {
	fprintf(stderr, "peos_create_instances: COMPILER_DIR not set.\n");
	return 0;
    }

#ifdef NOTUSED
/*    XXX Until    future, we don't currently save contexts.*/
    /* Find a pid. */
    instances = peos_list_instances();
    for (p = instances; p && *p; p++) { 
	ext = strrchr(*p, '.');
	if (ext != NULL) {
	    cur = strtol(ext + 1, &endp, 0);
	    if (*(ext + 1) != '\0' && *endp == '\0') {
		if (cur > max) {
		    max = cur;
		}
	    }
	}
    }

    max++;
    if (max > PEOS_MAX_PID) {
	/* Wrap around. */
	for (p = instances; *p; p++) { 
	    ext = strrchr(*p, '.');
	    if (ext != NULL) {
		cur = strtol(ext + 1, &endp, 0);
		if (*(ext + 1) != '\0' && *endp == '\0') {
		    if (cur > max + 1) {
			max++;
			break;
		    }
		    max = cur;
		}
	    }
	}

	if (!*p || max > PEOS_MAX_PID) { /* XXX fencepost error? */
	    return 0;	/* Process table full. */
	}
    }

    sprintf(iname, "%s/", peos_dir);
#endif

    sprintf(model_name, "%s/", model_dir);

    ext = strrchr(model, '.');
    if (ext != NULL) {
	strncat(model_name, model, ext - model);
    } else {
	strncat(model_name, model, strlen(model));
    }

    strcat(model_name, ".txt");
    context = &(process_table[0]); /* XXX could cause memory leak if we don't
				      free actions & instructions on exit. */
    if ((start = load_instructions(model_name, 
				   &(context->vm_context.inst_array), 
				   &(context->vm_context.num_inst),
				   &(context->actions), 
				   &(context->num_actions))) >= 0) 
	{
	    init_context(&(context->vm_context), start);

	    /* XXX don't save until multitasking. 
	       sprintf(pid_buf, ".%d", max);
	       strcat(iname, pid_buf);
	       save_context(&context, actions, num_actions, iname);
	    */

	    return 0; /* XXX will always be zero until multi-tasking. */
	}
    return -1;
}

char *act_state_name(vm_act_state state) 
{
    switch (state) {
    case ACT_NEW:
	return "NEW";
	break;
    case ACT_READY:
	return "READY";
	break;
    case ACT_RUN:
	return "RUN";
	break;
    case ACT_DONE:
	return "DONE";
	break;
    case ACT_SUSPEND:
	return "SUSPEND";
	break;
    case ACT_ABORT:
	return "ABORT";
	break;
    case ACT_NONE:
	return "NONE";
	break;
    default:
	return "unknown syscall";
	break;
    }
}
/*
 * Return 0 if call blocks waiting for act state change.
 */
int handle_system_call(vm_context_t *context, peos_action_t *actions, 
		       int num_actions)
{
    int status = 1; /* 0 indicates blocking call. */
    int i;
    vm_syscall_param_t *parameters = &(context->parameters);

    switch (parameters->call) {
    case VM_SET:
	/* Set should always succeed. */
	for (i = 0; i < parameters->act.num_act; i++) {
	    set_act_state(parameters->act.actions[i],
			  parameters->act.state,
			  actions, num_actions);
	}
	push_ctx(1, context);		/* Success code. */
	printf("SET {");
	for (i = 0; i < parameters->act.num_act; i++) {
	    printf(" %s", parameters->act.actions[i]);
	}
	printf(" } %s\n", act_state_name(parameters->act.state));
	break;

    case VM_WAIT:
	/* Wait for user to do something. */
	status = 0;
	printf("WAIT {");
	for (i = 0; i < parameters->act.num_act; i++) {
	    printf(" %s", parameters->act.actions[i]);
	}
	printf(" } %s\n", act_state_name(parameters->act.state));
	break;

    case VM_SELECT:
    case VM_ASSERT:
	/* Simulate successful query. */
	push_ctx(1, context);
	status = 1;		/* XXX Should block for result. */
	printf("SELECT/ASSERT\n");
	break;

    case VM_FORK:
    case VM_JOIN:
	printf("FORK/JOIN\n");
	assert("0 == 1");
	break;

    case VM_EXIT:
	/* Arrange for parent to unblock. */
    default:
	break;
    }

    return status;
}

vm_exit_code handle_action_change(char *action, vm_act_state state)
{
    char **pp;
    vm_syscall_param_t *parameters = &(current_process->vm_context.parameters);
    if (set_act_state(action, state, current_process->actions, 
		      current_process->num_actions) < 0) {
	return VM_INTERNAL_ERROR;
    }

    if (parameters->call != VM_WAIT) {
	/* No process is waiting for an action to finish. */
	return VM_ERROR;
    }

    if (parameters->act.state != state) {
	if (parameters->act.state == ACT_DONE && state == ACT_RUN) {
	    /* For sequences, the compiler generates code to wait for
               DONE, without waiting for RUN first. */
	    return VM_CONTINUE;
	}
	return VM_ERROR;
    }
    for (pp = parameters->act.actions;
	 pp - parameters->act.actions < parameters->act.num_act; pp++) {
	if (strcmp(*pp, action) == 0) {
	    push_ctx(pp - parameters->act.actions + 1,
		     &(current_process->vm_context));
	    return peos_resume(current_process - process_table); /* pid is offset into table. */
	}
    }

    return VM_INTERNAL_ERROR;
}

int peos_resume(int pid)
{
    vm_exit_code result;
    int cont = 1;

    if (pid < 0 || pid >= num_proc) {
	return VM_INTERNAL_ERROR;
    }
    current_process = &(process_table[pid]);

    while(cont) {
	result = execute(&(current_process->vm_context));
	switch(result) {
	case VM_DONE:
	    cont = 0;
	    break;

	case VM_SYSCALL:
	    cont = handle_system_call(&(current_process->vm_context),
				      current_process->actions,
				      current_process->num_actions);
	    break;

	case VM_ERROR:
	case VM_INTERNAL_ERROR:
	default:
	    cont = 0;
	    break;
	}
    }
    return result;
}


#ifdef UNIT_TEST
#include "test_process.c"
#endif
