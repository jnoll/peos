/* -*-C-*-
*****************************************************************************
*
* File:         $RCSFile: process_table.c$
* Version:      $Id: process_table.c,v 1.5 2003/07/09 20:31:57 jnoll Exp $ ($Name:  $)
* Description:  process table manipulation and i/o.
* Author:       John Noll, Santa Clara University
* Created:      Sun Jun 29 13:41:31 2003
* Modified:     Sun Jul  6 17:07:26 2003 (John Noll, SCU) jnoll@carbon.cudenver.edu
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
#include <assert.h>
#include "process_table.h"
/* Globals. */
peos_context_t process_table[PEOS_MAX_PID+1];
int cur_pid = -1;		/* Initially, start below proc table. */

/* Forward declarations. */


/* 
 * These functions allow manipulation of processes without access to
 * process table.
 */
int peos_get_pid(peos_context_t *context)
{
    int pid = context- process_table;
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

char *read_field_data(char *input, int *len)
{
    char *s = input, *p;
    int l;
    s += strspn(input, " \t"); /* Skip whitespace. */
    if (*s != '{') return NULL;
    s++; 
    s += strspn(input, " \t"); /* Skip whitespace. */
    l = strcspn(s, "}\n\0"); 
    p = s+l;
    if (*p == '}') {
	p--;
	while (*p == ' ' || *s == '\t') {
	    p--; l--;
	}
    }
    *len = l;
    return s;
}

peos_action_t *add_act(peos_action_t *act_array, char *act, int num, int *size)
{
    int len;
    char *s;
    if (num == *size) {
	*size += INST_ARRAY_INCR;
	act_array = (peos_action_t *) realloc(act_array, *size * sizeof(peos_action_t));
    }
    len = strcspn(act, " \t\n\0"); /* action name */
    strncpy(act_array[num].name, act, len);
    act_array[num].name[len] = '\0';
    act_array[num].state = ACT_NONE;
    act_array[num].script = NULL;

    /* Fields. */
    s = act + len; 
    while (*s != '\0' && *s != '\n') {
	s += strspn(s, " \t");
	if (strncmp(s, "type", strlen("type")) == 0) {
	    s += strcspn(s, " \t\n");
	    s += strspn(s, " \t");
	    /* Get type, break if not action. */
	    if (strncmp(s, "action", strlen("action")) != 0) {
		break;
	    }
	    s += strcspn(s, " \t\n");
	} else if (strncmp(s, "mode", strlen("mode")) == 0) {
	    s += strcspn(s, " \t\n");
	    s += strspn(s, " \t");
	    /* Skip over mode. */
	    s += strcspn(s, " \t\n");
	} else if (strncmp(s, "requires", strlen("requires")) == 0) {
	    s += strcspn(s, " \t\n");
	    read_field_data(s, &len);
	    s += len;
	} else if (strncmp(s, "provides", strlen("requires")) == 0) {
	    s += strcspn(s, " \t\n");
	    read_field_data(s, &len);
	    s += len;
	} else if (strncmp(s, "agent", strlen("agent")) == 0) {
	    s += strcspn(s, " \t\n");
	    read_field_data(s, &len);
	    s += len;
	} else if (strncmp(s, "tool", strlen("tool")) == 0) {
	    s += strcspn(s, " \t\n");
	    read_field_data(s, &len);
	    s += len;
	} else if (strncmp(s, "script", strlen("script")) == 0) {
	    s += strcspn(s, " \t\n");
	    s = read_field_data(s, &len);
	    if (s) {
		act_array[num].script = (char *)calloc(len+1, sizeof(char));
		strncpy(act_array[num].script, s, len);
	    }
	    s += len;
	} else {
	    /* Just skip word, whatever it is. */
	    s += strcspn(s, " \t");
	}
    }
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
	    s += strcspn(buf, " \t\n");	/* skip number */
	    s += strspn(s, " \t\n"); /* skip whitespace */
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
    int i, start;

    if (fscanf(in, "pid: %d\n", &context->pid) != 1) {
	return 0;
    }
    if (fscanf(in, "model: %s\n", context->model) != 1) {
	return 0;
    }
    if (strcmp(context->model, "none") == 0) {
	context->model[0] = '\0';
    }
    if (fscanf(in, "status: %d\n", (int *)&context->status) != 1) return 0;

    /* Load instructions and actions first, to initialize context. */
    if ((start = load_instructions(context->model,
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
    for (i = 0; i < context->num_actions; i++) {
	if (fscanf(in, "%s %d", context->actions[i].name, 
		   (int *)&context->actions[i].state) != 2) {
	    free(context->actions);
	    return 0; 
	}
	context->actions[i].pid = context->pid;
    }
    if (fscanf(in, "\n\n") < 0) return 0; 

return 1;
}

int load_proc_table(char *file)
{
    int i, status = -1;
    FILE *in = fopen(file, "r");
    int num_proc = 0;

    for (i = 0; i <= PEOS_MAX_PID; i++) {
	process_table[i].status = PEOS_NONE;
    }

    if (in) {
	status = 0;
	while (load_context(in, &process_table[num_proc]))
	    num_proc++;
	fclose(in);
    }
    return status;
}


int save_context(int pid, peos_context_t *context, FILE *out)
{
    int i;

    fprintf(out, "pid: %d\n", pid);
    fprintf(out, "model: %s\n", context->model[0] ? context->model : "none");
    fprintf(out, "status: %d\n", context->status);
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

char **peos_list_instances()
{
    static char *result[PEOS_MAX_PID+1];
    int i;
    for (i = 0; i <= PEOS_MAX_PID; i++) {
	result[i] = process_table[i].model;
    }
    return result;
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

	return 1;

    } else {
	return 0;
    }
}

peos_context_t *find_free_entry()
{
    int i;
    for (i = 0; i < PEOS_MAX_PID + 1; i++) {
	process_status_t status = process_table[i].status;
	if (status & (PEOS_NONE|PEOS_DONE|PEOS_ERROR)) {
	    return &(process_table[i]);	
	}
    }
    return NULL;
}

peos_action_t **peos_list_actions(vm_act_state state)
{
    int i, num, size = INST_ARRAY_INCR;

    peos_action_t *p, **result;

    result = (peos_action_t **)calloc(INST_ARRAY_INCR, sizeof(peos_action_t*));

    for (i = 0, num = 0; i <= PEOS_MAX_PID; i++) {
	for (p = process_table[i].actions; 
	     p - process_table[i].actions < process_table[i].num_actions; p++) 
	    {
		if (p->state == state) {
		    if (num == size) {
			size += INST_ARRAY_INCR;
			result = (peos_action_t **)realloc(result, size * sizeof(peos_action_t *));
		    }
		    result[num++] = p;
		}
	    }
    }
    result[num] = NULL;
    return result;
}


#ifdef UNIT_TEST
#include "test_process_table.c"
#endif
