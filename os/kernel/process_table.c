/* -*-C-*-
*****************************************************************************
*
* File:         $RCSFile: process_table.c$
* Version:      $Id: process_table.c,v 1.42 2004/05/14 00:58:39 jshah1 Exp $ ($Name:  $)
* Description:  process table manipulation and i/o.
* Author:       John Noll, Santa Clara University
* Created:      Sun Jun 29 13:41:31 2003
* Modified:     Thu Dec  4 11:48:43 2003 (John Noll, SCU) jnoll@carbon.cudenver.edu
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
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include "graph.h"
#include "process_table.h"
#include "graph_engine.h"
#include "resources.h"
#include "process.h"


/* Globals. */
peos_context_t process_table[PEOS_MAX_PID+1];

int save_proc_table_xml();

char *process_table_filename = "proc_table.dat";

int filedes;


/* 
 * These functions allow manipulation of processes without access to
 * process table.
 */


/* This function tries to get a lock for the file descriptor */ 

int get_lock(int fd)
{

    struct flock lck;
    int num_attempts = 0;

    /* initialize the lock struct for a write lock */
    lck.l_type = F_WRLCK; /* get a write exclusive lock */
    lck.l_whence = 0;
    lck.l_start = 0L;
    lck.l_len = 0L; /* lock the whole file address space */

    while (fcntl(fd, F_SETLK, &lck) < 0) {
        if ((errno == EAGAIN) || (errno == EACCES)) {
	    if(++num_attempts <= MAX_LOCK_ATTEMPTS) {
		fprintf(stderr, "Attempting Process File Lock ...\n");     
	        sleep(2);
		continue;
	    }
	    
	    fprintf(stderr, "File Lock Error: File Busy \n  Error Msg  : %s\n", strerror(errno));
	    return -1;
	}
	fprintf(stderr, "File Lock Error: Unknown Error\n");
	fprintf(stderr, "System Error Message: %s\n", strerror(errno));
	return -1;
    }

    return 1;
}


int release_lock(int fd)
{
    
    struct flock lck;

    lck.l_type = F_UNLCK; 
    lck.l_whence = 0;
    lck.l_start = 0L;
    lck.l_len = 0L; 

    if(fcntl(fd, F_SETLK, &lck) < 0) 
        return -1;
    else
        return 1;
}
    

void peos_set_process_table_file(char *file_name)
{
    if(file_name) {
        process_table_filename = file_name;
    }
}



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
	    

void peos_set_loginname(char *loginname)
{
    if(loginname) {
	login_name = loginname;
    }
}


int set_resource_binding(int pid, char *resource_name, char *resource_value)
{
    int i;

    peos_resource_t *resources;
    peos_context_t *context = peos_get_context(pid);
    int num_resources;

    if(context == NULL) return -1;
    resources = context -> resources;
    num_resources = context -> num_resources;

    if(resources == NULL) return -1;
    
    for(i = 0; i < num_resources; i++) {
        if(strcmp(resources[i].name,resource_name) == 0) {
	    if(strlen(resource_value) < RESOURCE_FIELD_LENGTH) {	
                strcpy(resources[i].value,resource_value);
	        return 1;
	    }
	    else {
	        fprintf(stderr, "buffer overflow in set resource binding\n");
		return -1;
	    }
	}
    }	
    return -1;
}


char *get_resource_binding(int pid, char *resource_name)
{
    int i;
    int num_resources;

    peos_context_t *context = peos_get_context(pid);
    peos_resource_t *resources;
    if(context == NULL) return NULL;
    resources = context -> resources;
    num_resources = context -> num_resources;
    if(resources == NULL) return NULL;
         
    for(i = 0; i < num_resources; i++) {
        if(strcmp(resources[i].name,resource_name) == 0) {
	    return resources[i].value;
	}
    }
    return NULL;
}


char *get_resource_qualifier(int pid, char *resource_name)
{
    int i;
    int num_resources;

    peos_context_t *context = peos_get_context(pid);
    peos_resource_t *resources;
    if(context == NULL) return NULL;
    resources = context -> resources;
    num_resources = context -> num_resources;
    if(resources == NULL) return NULL;
         
    for(i = 0; i < num_resources; i++) {
        if(strcmp(resources[i].name,resource_name) == 0) {
	    return resources[i].qualifier;
	}
    }
    return NULL;
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
	    

/*
 * Set state of nodes in g from state info in actions and other nodes.
 * XXX collapse these two lists into one; there's no need to distinguish.
 */
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
	context->process_graph = NULL;
    }

    if (fscanf(in, "status: %d\n", (int *)&context->status) != 1) return 0;
    
    if (context->status != PEOS_NONE && context->model[0]) {
	if ((context->process_graph = makegraph(context->model)) == NULL) {
	    return 0;
	} else {
	    initialize_graph(context->process_graph, context->pid);
	}
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

    if (context->process_graph) {
	if (annotate_graph(context->process_graph, actions, num_actions, 
		       other_nodes, num_other_nodes) < 0) {
	return 0;
	}
    }

    if (num_actions) free(actions);
    if (num_other_nodes) free(other_nodes);
    return 1;
}

int load_proc_table(char *file)
{   
    int i, status = -1;
    FILE *in;

    int num_proc = 0;
    
    
   
    filedes = open(file, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (filedes < 0) {
        fprintf(stderr, "Cannot Get Process Table File Descriptor\n");
	exit(EXIT_FAILURE);
    }
    
    if(get_lock(filedes) < 0) {
        fprintf(stderr, "Cannot Obtain Process Table File Lock\n");
	exit(EXIT_FAILURE);
    }
    
    in = fdopen(filedes, "r+");
   
    for (i = 0; i <= PEOS_MAX_PID; i++) {
	process_table[i].status = PEOS_NONE;
    }
    if (in) {
	status = 0;
	while (load_context(in, &process_table[num_proc]))
	    num_proc++;
     /* fclose(in); */
	/* XXX This is an issue here. If I close the file stream or the file descriptor, I loose the lock on the file. So right now this will result in some open file streams. */
    }
    else {
	fprintf(stderr, "Error in getting file pointer for load process table");
        release_lock(filedes);
	close(filedes);
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
    FILE *out; 

    out = fopen(file, "w");
   
    if (out) {
        for (i = 0; i <= PEOS_MAX_PID; i++) {
	    save_context(i, &(process_table[i]), out);
	}
	release_lock(filedes);
	fclose(out);
	close(filedes);
    }
    else {
        fprintf(stderr, "File Pointer Error: %s \n", strerror(errno));
	release_lock(filedes);
	close(filedes);
	return -1;
    }
    return 0;
}

int load_process_table()
{
    return load_proc_table(process_table_filename);
}

int save_process_table()
{
    save_proc_table_xml();	
    return save_proc_table(process_table_filename);
}

void print_after_escaping(char *str, FILE *fp)
{
    int i;
    if(str!=NULL) {
        for(i=0; i<strlen(str); i++) {
	    switch(str[i]) {
	        case '<' : fprintf(fp,"&lt;");
			   break;
		case '>' : fprintf(fp, "&gt;");
			   break;
		case '&' : fprintf(fp, "&amp;");
			   break;
	        case '"' : fprintf(fp, "&quot;");
                            break;		
		default : fprintf(fp, "%c",str[i]);
			  break;
	    }
	}
    }
    else fprintf(fp, "(null)");
}


void print_action_node(Node n, FILE *fp)
{
    int num_req_resources;
    int num_prov_resources;
    int i;
    
    peos_resource_t *req_resources;
    peos_resource_t *prov_resources;    
    
    req_resources = get_resource_list_action_requires(PID(n), n->name, &num_req_resources);
    prov_resources = get_resource_list_action_provides(PID(n), n->name, &num_prov_resources);
  
    fprintf(fp, "<action name=\"");
    print_after_escaping(n->name,fp);
    fprintf(fp, "\" state=\"%s\">\n", (char *)act_state_name(STATE(n)));
    fprintf(fp, "<script>\n");
    print_after_escaping(n->script, fp);
    fprintf(fp, "\n</script>\n");

    for(i=0; i < num_req_resources; i++) {
	fprintf(fp, "<req_resource name=\"");
	print_after_escaping(req_resources[i].name, fp);
	fprintf(fp, "\" value=\"");
	print_after_escaping(req_resources[i].value, fp);
	fprintf(fp, "\" qualifier=\"%s\"></req_resource>\n",req_resources[i].qualifier);
	
    }

    for(i=0; i < num_prov_resources; i++) {

	fprintf(fp, "<prov_resource name=\"");
	print_after_escaping(prov_resources[i].name, fp);
	fprintf(fp, "\" value=\"");
	print_after_escaping(prov_resources[i].value, fp);
	fprintf(fp, "\" qualifier=\"%s\"></prov_resource>\n",prov_resources[i].qualifier);
	
    }
	
    fprintf(fp, "</action>\n");
}

   

void print_graph(Graph g, FILE *fp)
{
    Node n, child, parent;
    int i;

    for(n = g->source->next; n!=NULL; n = n->next) {
	
        for(i = 0; i < ListSize(n->predecessors); i++) {
       	    parent = (Node) ListIndex(n->predecessors, i);
	    if((parent->type == SELECTION) || (parent->type == BRANCH)) {
	        fprintf(fp, "<sequence>\n");
	    }		
	}

        for(i = 0; i < ListSize(n->predecessors); i++) {
       	    parent = (Node) ListIndex(n->predecessors, i);
            if(ORDER(parent) > ORDER(n)) {
	        fprintf(fp, "<iteration>\n");
	    }
        }

        if(n->type == ACTION) {
            print_action_node(n,fp);
        }

        if(n->type == JOIN) {
            fprintf(fp, "</selection>\n");
        }

        if(n->type == RENDEZVOUS) {
            fprintf(fp, "</branch>\n");
        }

        if(n->type == SELECTION) {
            fprintf(fp, "<selection>\n");
	}

	if(n->type == BRANCH) {
            fprintf(fp, "<branch>\n");	
	}

        for(i = 0; i < ListSize(n->successors); i++) {
       	    child = (Node) ListIndex(n->successors, i);
            if(ORDER(child) < ORDER(n)) {
	        fprintf(fp, "</iteration>\n");
	    }
	}

        for(i = 0; i < ListSize(n->successors); i++) {
       	    child = (Node) ListIndex(n->successors, i);
	    if((child->type == JOIN) || (child->type == RENDEZVOUS)) {
	        fprintf(fp, "</sequence>\n");
	    }		
        }
    }
}


int save_proc_table_xml()
{
    int i;
    Graph g;
    FILE *fp;
    char *xml_filename = (char *) malloc((strlen(process_table_filename)+strlen(".xml")+1) * sizeof(char));
    strcpy(xml_filename, process_table_filename);
    strcat(xml_filename, ".xml");
    

    fp = fopen(xml_filename, "w");
    fprintf(fp, "<process_table>\n");

    for(i=0; i <= PEOS_MAX_PID; i++) {
        g = process_table[i].process_graph;
	if(g != NULL) {
	    fprintf(fp, "<process pid=\"%d\" model=\"%s\" status=\"%d\">\n", i, process_table[i].model, process_table[i].status);
	    print_graph(g, fp);
	    fprintf(fp, "</process>\n");
        }
    }
    fprintf(fp, "</process_table>\n");
    fclose(fp);
    return 0;
}

	

char **peos_list_instances()
{
    static char *result[PEOS_MAX_PID+1];
    int i;

    if(load_process_table() < 0) {
        fprintf(stderr, "System Error: Cannot Load Process Table\n");
	exit(EXIT_FAILURE);
    }
    
    for (i = 0; i <= PEOS_MAX_PID; i++) {
        result[i] = process_table[i].model;
    }
    result[i] = NULL;
  
    if(save_process_table() < 0) {
        fprintf(stderr, "System Error: Cannot Save Process Table\n");
	exit(EXIT_FAILURE);
    }

    return result;
}

int delete_entry(int pid)
{
    peos_context_t *context;

    if (pid >= 0 && pid <= PEOS_MAX_PID) {  
        context = &(process_table[pid]);
	if(context -> process_graph != NULL) {
	    context->model[0] = '\0';
	    context->status = PEOS_NONE;
	    GraphDestroy(context->process_graph);
	    context->process_graph = NULL;
	    context->num_resources = 0;
	    free(context->resources);
	    return 1;
	}
	else return -1;
    } 
    else {
        return -1;
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

    if(load_process_table() < 0) {
        fprintf(stderr, "System Error: Cannot Load Process Table\n");
	exit(EXIT_FAILURE);
    }
    
    *num_actions = 0;
    if (process_table[pid].status & (PEOS_DONE | PEOS_NONE | PEOS_ERROR)) {
        if(save_process_table() < 0) {
            fprintf(stderr, "System Error: Cannot Save Process Table\n");
	    exit(EXIT_FAILURE);
        }
        return NULL;
    }

    if(make_node_lists(process_table[pid].process_graph,&actions,&num_act,&other_nodes,&num_other_nodes) == -1) {
        if(save_process_table() < 0) {
            fprintf(stderr, "System Error: Cannot Save Process Table\n");
	    exit(EXIT_FAILURE);
        }
        return NULL;
    }
    
    for (i = 0; i < num_act; i++) {
	actions[i].pid = pid;
    }
    for (i = 0; i < num_other_nodes; i++) {
	other_nodes[i].pid = pid;
    }
    *num_actions = num_act;  

    if(save_process_table() < 0) {
        fprintf(stderr, "System Error: Cannot Save Process Table\n");
	exit(EXIT_FAILURE);
    }
    
    return actions;
}





#ifdef UNIT_TEST
#include "test_process_table.c"
#endif
