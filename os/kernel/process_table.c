/* -*-C-*-
*****************************************************************************
*
* File:         $RCSFile: process_table.c$
* Version:      $Id: process_table.c,v 1.2 2003/06/30 20:26:06 jnoll Exp $ ($Name:  $)
* Description:  process table manipulation and i/o.
* Author:       John Noll, Santa Clara University
* Created:      Sun Jun 29 13:41:31 2003
* Modified:     Mon Jun 30 13:17:39 2003 (John Noll, SCU) jnoll@carbon.cudenver.edu
* Language:     C
* Package:      N/A
* Status:       $State: Exp $
*
* (c) Copyright 2003, Santa Clara University, all rights reserved.
*
*****************************************************************************
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "process_table.h"
/* Globals. */

peos_context_t process_table[PEOS_MAX_PID+1];
int num_proc = 0;
int cur_pid = -1;		/* Initially, start below proc table. */
peos_context_t *current_process = NULL; 

/* Forward declarations. */
char *find_model_file(char *model);


/* 
 * These functions allow manipulation of processes without access to
 * process table.
 */
int peos_get_pid(peos_context_t *context)
{
    int pid = process_table - context;
    return pid >=0 && pid <= PEOS_MAX_PID ? pid : -1;
}

peos_context_t *peos_get_context(int pid)
{
    if (pid < 0 || pid > PEOS_MAX_PID) {
	return NULL;
    }

    return &(process_table[pid]);
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

peos_action_t *add_act(peos_action_t *act_array, char *act, int num, int *size)
{
    int len;
    if (num == *size) {
	*size += INST_ARRAY_INCR;
	act_array = (peos_action_t *) realloc(act_array, *size * sizeof(peos_action_t));
    }
    len = strcspn(act, " \t\n");
    strncpy(act_array[num].name, act, len);
    act_array[num].name[len] = '\0';
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
	    act_array = add_act(act_array, s, i++, &asize);
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


int
load_context(FILE *in, peos_context_t *context)
{
    int pid, i, start;
    char *model_file;
    if (fscanf(in, "pid: %d\nmodel: %s\n", &pid, context->model) != 2) return 0;
    /* Load instructions and actions first, to initialize context. */
    model_file = find_model_file(context->model);
    if ((start = load_instructions(model_file, 
				   &(context->vm_context.inst_array), 
				   &(context->vm_context.num_inst),
				   &(context->actions), 
				   &(context->num_actions))) >= 0) 
	{
	    init_context(&(context->vm_context), start);
	} else {
	    /* XXX Error - should mark entry somehow? */
	    return 0;
	}
	  
    /* Now, set context state to saved state. */
    if (fscanf(in, "PC: %d\nSP: %d\nA: %d\n", &context->vm_context.PC, 
	    &context->vm_context.SP, &context->vm_context.A) != 3) return 0;
    if (fscanf(in, "stack:") < 0) return 0;
    for (i = 0; i < MAX_STACK_SIZE; i++) {
	if (fscanf(in, " %d", &context->vm_context.stack[i]) != 1) return 0;
    }
    if (fscanf(in, "\n") < 0) return 0;

    if (fscanf(in, "variables: ") < 0) return 0;;
    if (fscanf(in, "%d ", &context->vm_context.num_variables) != 1) return 0;
    context->vm_context.variables = (vm_vbinding_t *)calloc(context->vm_context.num_variables, sizeof(vm_vbinding_t));
    for (i = 0; i < context->vm_context.num_variables; i++) {
	if (fscanf(in, " %s %d", context->vm_context.variables[i].name,
		&context->vm_context.variables[i].value) != 2) {
	    free(context->vm_context.variables);
	    return 0;
	}
    }
    if (fscanf(in, "\n") < 0) return 0;

    if (fscanf(in, "parameters:") < 0) return 0;
    if (fscanf(in, " %d", (int *)&context->vm_context.parameters.call) != 1) return 0;
    if (fscanf(in, " %d", (int *)&context->vm_context.parameters.act.state) != 1) return 0;
    if (fscanf(in, " %d", &context->vm_context.parameters.act.num_act) != 1) return 0;
    for (i = 0; i < context->vm_context.parameters.act.num_act; i++) {
	if (fscanf(in, " %s", context->vm_context.parameters.act.actions[i]) != 1) return 0;
    }
    if (fscanf(in, "\n") < 0) return 0; 

    if (fscanf(in, "actions: ") < 0) return 0; 
    if (fscanf(in, "%d ", &context->num_actions) != 1) return 0;
    context->actions = (peos_action_t *)calloc(context->num_actions, sizeof(peos_action_t));
    for (i = 0; i < context->num_actions; i++) {
	if (fscanf(in, "%s %d", context->actions[i].name, 
		   (int *)&context->actions[i].state) != 2) {
	    free(context->actions);
	    return 0; 
	}
    }
    if (fscanf(in, "\n\n") < 0) return 0; 

return 1;
}

int load_proc_table(char *file)
{
    int cur, status = -1;
    FILE *in = fopen(file, "r");
    num_proc = 0;
    current_process = &(process_table[0]);
    if (in) {
	while (load_context(in, &process_table[num_proc]))
	    num_proc++;
	if ((fscanf(in, "current_process: %d", &cur) == 1)
	    && cur >= 0 && cur <= PEOS_MAX_PID) {
	    current_process = &(process_table[cur]);
	    status = 0;
	}
	fclose(in);
    }
    return status;
}


int save_context(int pid, peos_context_t *context, FILE *out)
{
    int i;

    fprintf(out, "pid: %d\nmodel: %s\n", pid, context->model);
    fprintf(out, "PC: %d\nSP: %d\nA: %d\n", context->vm_context.PC, 
	    context->vm_context.SP, context->vm_context.A);
    fprintf(out, "stack:");
    for (i = 0; i < MAX_STACK_SIZE; i++) {
	fprintf(out, " %d", context->vm_context.stack[i]);
    }
    fprintf(out, "\n");

    fprintf(out, "variables: ");
    fprintf(out, "%d ", context->vm_context.num_variables);
    for (i = 0; i < context->vm_context.num_variables; i++) {
	fprintf(out, " %s %d", context->vm_context.variables[i].name,
		context->vm_context.variables[i].value);
    }
    fprintf(out, "\n");

    fprintf(out, "parameters:");
    fprintf(out, " %d", context->vm_context.parameters.call);
    fprintf(out, " %d", context->vm_context.parameters.act.state);
    fprintf(out, " %d", context->vm_context.parameters.act.num_act);
    for (i = 0; i < context->vm_context.parameters.act.num_act; i++) {
	fprintf(out, " %s", context->vm_context.parameters.act.actions[i]);
    }
    fprintf(out, "\n"); 

    fprintf(out, "actions: "); 
    fprintf(out, "%d ", context->num_actions);
    for (i = 0; i < context->num_actions; i++) {
	fprintf(out, " %s %d", context->actions[i].name, context->actions[i].state); 
    }
    fprintf(out, "\n\n"); 
    return 1;
}

int 
save_proc_table(char *file)
{
    int i;
    FILE *out = fopen(file, "w");

    if (out) {
	for (i = 0; i <= PEOS_MAX_PID; i++) {
	    save_context(i, &(process_table[i]), out);
	}
	fprintf(out, "current_process: %d\n", peos_get_pid(current_process));
	fclose(out);
    }
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

char (*peos_list_instances())[]
{
    static char result[PEOS_MAX_PID+1][256];
    int i;
    for (i = 0; i <= PEOS_MAX_PID; i++) {
	strcpy(result[i], process_table[i].model);
    }
    return result;
}

char *find_model_file(char *model)
{
    char *ext, model_file[BUFSIZ];
    char *model_dir;
    FILE *f;

    model_dir = getenv("COMPILER_DIR");
    if (model_dir == NULL) {
	model_dir = ".";
    }

    sprintf(model_file, "%s/", model_dir);

    ext = strrchr(model, '.');
    if (ext != NULL) {
	strncat(model_file, model, ext - model);
    } else {
	strncat(model_file, model, strlen(model));
    }

    /* XXX should look for .cpml also. */
    strcat(model_file, ".txt");
    if ((f = fopen(model_file, "r"))) {
	fclose(f);
	return strdup(model_file);
    } else {
	return NULL;
    }
}

int delete_entry(int pid)
{
    peos_context_t *context;

    if (pid >= 0 && pid <= PEOS_MAX_PID) {
	context = &(process_table[pid]);
	if (context->actions) {
	    free(context->actions);
	    context->actions = NULL;
	}
	context->num_actions = 0;
	if (context->vm_context.inst_array) {
	    free(context->vm_context.inst_array);
	    context->vm_context.inst_array = NULL;
	}
	context->vm_context.num_inst = 0;
	context->vm_context.PC = -1;
	context->vm_context.A = -1;
	context->vm_context.SP = -1;
	context->model[0] = '\0';
	num_proc--;

	return 1;

    } else {
	return 0;
    }
}

peos_context_t *find_free_entry()
{
    int i;
    if (num_proc <= PEOS_MAX_PID) {
	for (i = cur_pid + 1; i != cur_pid; i++) {
	    if (i > PEOS_MAX_PID) { 
		i = 0;
	    }
	    if (process_table[i].model[0] == '\0') {
		cur_pid = i;
		return &(process_table[i]);
	    }
	}
    }
    return NULL;
}

int peos_create_instance(char *model)
{
    int start = -1;
    char *model_file;
    peos_context_t *context;

    model_file = find_model_file(model);

    if ((context = find_free_entry()) == NULL) {
	return -1;
    }

    if ((start = load_instructions(model_file, 
				   &(context->vm_context.inst_array), 
				   &(context->vm_context.num_inst),
				   &(context->actions), 
				   &(context->num_actions))) >= 0) 
	{
	    init_context(&(context->vm_context), start);
	    strcpy(context->model, model);
	    num_proc++;
	    return (context - process_table); 
	}
    return -1;
}


#ifdef UNIT_TEST
#include "test_process_table.c"
#endif
