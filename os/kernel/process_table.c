/* -*-C-*-
*****************************************************************************
*
* File:         $RCSFile: process_table.c$
* Version:      $Id: process_table.c,v 1.24 2003/11/17 07:29:59 jnoll Exp $ ($Name:  $)
* Description:  process table manipulation and i/o.
* Author:       John Noll, Santa Clara University
* Created:      Sun Jun 29 13:41:31 2003
* Modified:     Sun Nov 16 22:45:35 2003 (John Noll, SCU) jnoll@carbon.cudenver.edu
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
#include "graph.h"
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

char *get_script(int pid, char *act_name)
{
    peos_context_t *context = peos_get_context(pid);

    if (context != NULL) {
        return get_script_graph(context -> process_graph, act_name);
    }
    else {
        fprintf(stderr,"\n get_script error : context not found\n");
	return NULL;
    }
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



/* XXX remove this - it's just as easy to use the graph directly. */
int make_node_lists(Graph g, peos_action_t **actions, int *num_actions, peos_other_node_t **other_nodes, int *num_other_nodes)
{ 
    Node n;
    int num_act = 0;
    int num_nodes = 0;
    int asize = INST_ARRAY_INCR;
    int osize = INST_ARRAY_INCR;
    peos_action_t *act_array = (peos_action_t *) calloc(asize, sizeof(peos_action_t));
    
    peos_other_node_t *node_array = (peos_other_node_t *) calloc(osize, sizeof(peos_other_node_t));

    if (g != NULL) {
        for(n = g -> source;n != NULL; n = n -> next) {
	    if (n -> type == ACTION) {
                if(num_act >= asize) {
                    asize = asize + INST_ARRAY_INCR;
                    if ((act_array = realloc(act_array,asize*sizeof(peos_action_t))) == NULL) {
	                fprintf(stderr, "Too Many Actions\n");
			free(act_array);
	                return -1;
	              }
		}
	        strcpy(act_array[num_act].name, n -> name);
		act_array[num_act].state = STATE(n);
		act_array[num_act].script = n -> script;
	        num_act ++;
	    }
	    else {
                if((n->type == SELECTION) || (n->type == BRANCH)) {
		    if(num_nodes >= osize) {
		        osize = osize + INST_ARRAY_INCR;
			if((node_array = realloc(node_array,osize*sizeof(peos_other_node_t))) == NULL) {
			    fprintf(stderr,"Too many nodes\n");
			    free(node_array);
			    return -1;
			}
		    }
		    strcpy(node_array[num_nodes].name, n -> name);
		    node_array[num_nodes].state = STATE(n);
		    num_nodes ++;
		}
	    }
	}
	*actions = act_array;
	*num_actions = num_act;
	*other_nodes = node_array;
	*num_other_nodes = num_nodes;
	return 1;
    }
    else {
        free(act_array);
	free(node_array);
        return -1;
    }
}
	    

int  annotate_graph(Graph g, peos_action_t *actions, int num_actions, peos_other_node_t *other_nodes, int num_other_nodes)
{
    int i;
    Node node;
                                                                      
    for(node = g -> source; node != NULL; node = node -> next) {
        if (node -> type == ACTION) {
            STATE(node) = get_act_state(node -> name,actions,num_actions);
	}
	else {
	    if((node->type == SELECTION) || (node->type == BRANCH)) {
	        for(i=0;i < num_other_nodes; i++) {
	            if (strcmp(node->name,other_nodes[i].name)==0) {
	                STATE(node) = other_nodes[i].state;
	                STATE(node->matching) = other_nodes[i].state;
	            }
	        }
	     }
	 }
    }
	
    return 1;   
}

int
load_context(FILE *in, peos_context_t *context)
{
    int i;
    int num_actions, num_other_nodes;

    peos_action_t *actions;

    peos_other_node_t *other_nodes;

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

    if ((context->process_graph = makegraph(context->model)) == NULL) {
	return 0;
    } else {
	initialize_graph(context->process_graph);
    }
    
    if (fscanf(in, "actions: ") < 0) return 0; 
    
    if (fscanf(in, "%d ", &num_actions) != 1) return 0;
    actions = (peos_action_t *) calloc(num_actions, sizeof(peos_action_t));

    for (i = 0; i < num_actions; i++) {
        if (fscanf(in, "%s %d", actions[i].name,(int *)&actions[i].state) != 2) {
	    free(actions);
	    return 0; 
	}
	actions[i].pid = context->pid;
    }

    fscanf(in, "\n");

    if (fscanf(in, "other_nodes: ") < 0) return 0;
                                                                        
    if (fscanf(in, "%d ", &num_other_nodes) != 1) return 0;
    other_nodes = (peos_other_node_t *) 
	calloc(num_other_nodes, sizeof(peos_other_node_t));    

    for (i = 0; i < num_other_nodes; i++) {
        if (fscanf(in, "%s %d", other_nodes[i].name,(int *)&other_nodes[i].state) != 2) {
	    free(other_nodes);
            return 0;
	}
        other_nodes[i].pid = context->pid;
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

    if(annotate_graph(context->process_graph,actions,num_actions,other_nodes,num_other_nodes) < 0) return 0;

    free(actions);
    free(other_nodes);
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
    int num_actions = 0;
    int num_other_nodes = 0;

    peos_action_t *actions;
    peos_other_node_t *other_nodes;

    make_node_lists(context->process_graph,&actions,&num_actions,&other_nodes,&num_other_nodes);
    
    fprintf(out, "pid: %d\n", pid);
    fprintf(out, "model: %s\n", context->model[0] ? context->model : "none");
    fprintf(out, "status: %d\n", context->status);
    fprintf(out, "actions: "); 
    fprintf(out, "%d ", num_actions);
    for (i = 0; i < num_actions; i++) {
        fprintf(out, " %s %d", actions[i].name, actions[i].state); 
    }

    fprintf(out, "\nother_nodes: ");
    fprintf(out, "%d ", num_other_nodes);
    for (i = 0; i < num_other_nodes; i++) {
        fprintf(out, " %s %d", other_nodes[i].name, other_nodes[i].state);
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
    result[i] = NULL;
    return result;
}

int delete_entry(int pid)
{
    peos_context_t *context;

    if (pid >= 0 && pid <= PEOS_MAX_PID) {
        context = &(process_table[pid]);
	context->model[0] = '\0';
	context->status = PEOS_NONE;
	GraphDestroy(context->process_graph);
	context->process_graph = NULL;
	context->num_resources = 0;
	free(context->resources);
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

peos_action_t *peos_list_actions(int pid, int *num_actions)
{
   
    int num_act, num_other_nodes, i;
    peos_action_t *actions;
    peos_other_node_t *other_nodes;

    if (process_table[pid].status & (PEOS_DONE | PEOS_NONE | PEOS_ERROR)) return NULL;

    if(make_node_lists(process_table[pid].process_graph,&actions,&num_act,&other_nodes,&num_other_nodes) == -1) return NULL;
    
    for (i = 0; i < num_act; i++) {
	actions[i].pid = pid;
    }
    for (i = 0; i < num_other_nodes; i++) {
	other_nodes[i].pid = pid;
    }
    *num_actions = num_act;  
    return actions;
}


#ifdef UNIT_TEST
#include "test_process_table.c"
#endif
