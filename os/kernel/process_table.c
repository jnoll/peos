/* -*-C-*-
*****************************************************************************
*
* File:         $RCSFile: process_table.c$
* Version:      $Id: process_table.c,v 1.16 2003/11/07 02:44:12 jshah1 Exp $ ($Name:  $)
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
#include "graph_engine.h"


/* Globals. */
peos_context_t process_table[PEOS_MAX_PID+1];
int cur_pid = -1;		/* Initially, start below proc table. */


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

int peos_set_resource_value(int pid, char *resource_name, char *resource_value)
{
    int i;
    peos_context_t *context = peos_get_context(pid);
    peos_resource_t *resources = context -> resources;
    int num_resources = context -> num_resources;

    for(i = 0; i < num_resources; i++) {
        if(strcmp(resources[i].name,resource_name) == 0) {
	    strcpy(resources[i].value,resource_value);
	    return 1;
	}
    }

    return -1;
}


int load_actions(char *file, peos_action_t **actions, int *num_actions,peos_other_node_t **other_nodes,int *num_other_nodes, Graph *process_graph)
{
    Graph g;
    Node n;
    int num_act = 0;
    int num_nodes = 0;
    int asize = INST_ARRAY_INCR;
    int osize = INST_ARRAY_INCR;
    peos_action_t *act_array = (peos_action_t *) calloc(asize, sizeof(peos_action_t));

    peos_other_node_t *node_array = (peos_other_node_t *) calloc(osize, sizeof(peos_other_node_t));

    if (file [0]!= '\0') {
        g = makegraph(file);
	if (g != NULL) {
	    *process_graph = g;	
	    for(n = g -> source;n != NULL; n = n -> next) {
	        if (n -> type == ACTION) {
		    strcpy(act_array[num_act].name, n -> name);
		    act_array[num_act].state = STATE(n);
		    act_array[num_act].script = n -> script;
		    num_act ++;
		    if(num_act > asize) {
			asize = asize + INST_ARRAY_INCR;
			if ((act_array = realloc(act_array,asize*sizeof(peos_action_t))) == NULL) {
			    fprintf(stderr, "Too Many Actions\n");
			    return -1;
			}
		    }	
		    
		}
		 
		else {
		    if((n->type == SELECTION) || (n->type == BRANCH)) {
                        strcpy(node_array[num_nodes].name, n -> name);
		        node_array[num_nodes].state = STATE(n);
			num_nodes ++;
			if(num_nodes > osize) {
               		    osize = osize + INST_ARRAY_INCR;
			    if((node_array = realloc(node_array,osize*sizeof(peos_other_node_t))) == NULL) {
			        fprintf(stderr,"Too many nodes\n");
				return -1;
			    }
			}
		    }
		}
	    }
	    *actions = act_array;
	    *num_actions = num_act;
	    *other_nodes = node_array;
	    *num_other_nodes = num_nodes;
	    return 1;
	}
	else
	    return -1;
    }
    else
        return -1;
	
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
    
    
    /* Load  actions first, to initialize context.This is also needed to load the script into the context */
    if ((start = load_actions(context->model,&(context->actions),&(context->num_actions),&(context->other_nodes),&(context->num_other_nodes),&(context->process_graph))) >= 0) 
    {
    } 
    else {
        return 0;
    }
	  
    if (fscanf(in, "actions: ") < 0) return 0; 
    
    if (fscanf(in, "%d ", &context->num_actions) != 1) return 0;
    
    for (i = 0; i < context->num_actions; i++) {
        if (fscanf(in, "%s %d", context->actions[i].name,(int *)&context->actions[i].state) != 2) {
	    free(context->actions);
	    return 0; 
	}
	context->actions[i].pid = context->pid;
    }

    fscanf(in, "\n");

    if (fscanf(in, "other_nodes: ") < 0) return 0;
                                                                        
    if (fscanf(in, "%d ", &context->num_other_nodes) != 1) return 0;
                                                                         
    for (i = 0; i < context->num_other_nodes; i++) {
        if (fscanf(in, "%s %d", context->other_nodes[i].name,(int *)&context->other_nodes[i].state) != 2) {
	    free(context->other_nodes);
            return 0;
	}
        context->other_nodes[i].pid = context->pid;
    }
     

    fscanf(in, "\n");
    
    if (fscanf(in, "resources: ") < 0) return 0;
                                                                                
    if (fscanf(in, "%d ", &context->num_resources) != 1) return 0;

    
    context->resources = (peos_resource_t *) calloc(context->num_resources,sizeof(peos_resource_t));
	                                                                                
    for (i = 0; i < context->num_resources; i++) {
        if (fscanf(in, "%s %s", context->resources[i].name,context->resources[i].value) != 2) {
	    free(context->resources);
            return 0;
        }
        context->resources[i].pid = context->pid;
     }
                                                                           

    if (fscanf(in, "\n\n") < 0) return 0; 

    if(annotate_graph(context->process_graph,context) < 0) return 0;

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
    fprintf(out, "actions: "); 
    fprintf(out, "%d ", context->num_actions);
    for (i = 0; i < context->num_actions; i++) {
        fprintf(out, " %s %d", context->actions[i].name, context->actions[i].state); 
    }

    fprintf(out, "\nother_nodes: ");
    fprintf(out, "%d ", context->num_other_nodes);
    for (i = 0; i < context->num_other_nodes; i++) {
        fprintf(out, " %s %d", context->other_nodes[i].name, context->other_nodes[i].state);
    }

    fprintf(out, "\nresources: ");
    fprintf(out, "%d ", context->num_resources);
    for (i = 0; i < context->num_resources; i++) {
        fprintf(out, " %s %s", context->resources[i].name, context->resources[i].value);
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
	context->model[0] = '\0';
	return 1;

    } 
    else {
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
        for (p = process_table[i].actions;p - process_table[i].actions < process_table[i].num_actions; p++) {
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
