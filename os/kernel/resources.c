#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "pmlheaders.h"
#include "action.h"
#include "process_table.h"
#include "graph.h"
//#include "process.h"
//#include "graph_engine.h"

void insert_resource(char *id, peos_resource_t **rlist, int *num_resources, int *rsize) 
{
    int i = 0;
    peos_resource_t *resource_list = *rlist;

    while((i < *num_resources) && (strcmp(resource_list[i].name, id)) != 0) {
        i++;
    }
    if (i == *num_resources) 
    {
        if(*num_resources == *rsize) {
	    *rsize = *rsize + INST_ARRAY_INCR;
	    resource_list = realloc(resource_list,(*rsize) * sizeof(peos_resource_t));
	}
	strcpy(resource_list[*num_resources].name, id);
	strcpy(resource_list[*num_resources].value,"\0");
	*num_resources = *num_resources + 1;
    }
    *rlist = resource_list;
}

void make_resource_list(Tree t, peos_resource_t **rlist, int *num_resources, int *rsize)
{
    peos_resource_t *resource_list = *rlist;
    if(t) {
	if (IS_OP_TREE(t)) {
	    switch TREE_OP(t) {
	    case DOT: 
	    case EQ: 
	    case NE:
	    case GE:
	    case LE:
	    case LT:
	    case GT: {
		make_resource_list(t->left, &resource_list, num_resources, rsize);
	    }
	    break;
	    case QUALIFIER: {
		make_resource_list(t->right, &resource_list, num_resources, rsize);
	    }
	    break;
	    }
	} else if (IS_ID_TREE(t)) {
	    insert_resource(TREE_ID(t), &resource_list, num_resources, rsize);
	} else {                       
	    make_resource_list(t->left, &resource_list,num_resources, rsize);
	    make_resource_list(t->right, &resource_list,num_resources, rsize);
	}
    }
    *rlist = resource_list;
    return;
}

peos_resource_t *get_resource_list_action_requires(int pid, char *act_name, int *total_resources)
{
    Graph g;
    Node n;
    int i,j;
    int rsize = 256;
    int num_resources = 0;
    peos_context_t *context = peos_get_context(pid);
    peos_resource_t *proc_resources = context -> resources;
    int num_proc_resources = context -> num_resources;
    char *model_file;

    peos_resource_t *act_resources = (peos_resource_t *) calloc(rsize,sizeof(peos_resource_t));

    model_file = context->model;
    g = context -> process_graph;
    if(g != NULL) {
        n = find_node(g,act_name);
        if(n == NULL) {
            fprintf(stderr,"get_resource_list_action :cannot find action");
	    return NULL;
	}
	make_resource_list(n -> requires, &act_resources, &num_resources, &rsize);
	*total_resources = num_resources;
        for(i = 0; i < num_resources; i++) {
            for(j = 0; j < num_proc_resources; j++) {
	        if(strcmp(act_resources[i].name,proc_resources[j].name) == 0) {
	            strcpy(act_resources[i].value,proc_resources[j].value);
		    break;
		}
	    }
	}

        return act_resources;
    }    
    else
        return NULL;
}

peos_resource_t *get_resource_list_action_provides(int pid, char *act_name, int
		*total_resources)
{
    Graph g;
    Node n;
    int i,j;
    int rsize = 256;
    int num_resources = 0;
    peos_context_t *context = peos_get_context(pid);
    peos_resource_t *proc_resources = context -> resources;
    int num_proc_resources = context -> num_resources;
    char *model_file;
				                                                                                   
    peos_resource_t *act_resources = (peos_resource_t *) calloc(rsize,sizeof(peos_resource_t));
                                                                                    
    model_file = context->model;
    g = context -> process_graph;   
    if(g != NULL) {
        n = find_node(g,act_name);
        if(n == NULL) {
            fprintf(stderr,"get_resource_list_action :cannot find action");
            return NULL;
        }
	make_resource_list(n -> provides, &act_resources, &num_resources, &rsize);
        *total_resources = num_resources;
        for(i = 0; i < num_resources; i++) {
            for(j = 0; j < num_proc_resources; j++) {
                if(strcmp(act_resources[i].name,proc_resources[j].name) == 0) {
                    strcpy(act_resources[i].value,proc_resources[j].value);
                    break;
                }
            }
        }
        return act_resources;
    }
    else
        return NULL;
}


peos_resource_t *get_resource_list_action(int pid, char *act_name, int *total_resources)
{
    Graph g;
    Node n;
    int i,j;
    int rsize = 256;
    int num_resources = 0;
    peos_context_t *context = peos_get_context(pid);
    peos_resource_t *proc_resources = context -> resources;
    int num_proc_resources = context -> num_resources;
    char *model_file;
    peos_resource_t *act_resources = (peos_resource_t *) calloc(rsize,sizeof(peos_resource_t));

    model_file = context->model;

    g = context -> process_graph;
    if(g != NULL) {
        n = find_node(g,act_name);
	if(n == NULL) {
	    fprintf(stderr,"get_resource_list_action :cannot find action");
	    return NULL;
	}
	make_resource_list(n -> requires, &act_resources,&num_resources, &rsize);
	make_resource_list(n -> provides, &act_resources,&num_resources, &rsize);
	*total_resources = num_resources;
	for(i = 0; i < num_resources; i++) {
	    for(j = 0; j < num_proc_resources; j++) {
 	        if(strcmp(act_resources[i].name,proc_resources[j].name) == 0) {
		    strcpy(act_resources[i].value,proc_resources[j].value);
		    break;
		}
	    }
	}

        return act_resources;
    }
    else 
        return NULL;
}

/*  
 * Get the list of resource names for a process. 
 */
peos_resource_t *get_resource_list(char *model, int *total_resources)
{
    int rsize = 256;
    Graph g;
    Node n;
    int num_resources = 0;
    peos_resource_t *resource_list;

    g = makegraph(model);

    if(g != NULL) {	
	resource_list = (peos_resource_t *) calloc(rsize,sizeof(peos_resource_t));
        for(n = g->source->next; n != NULL; n = n -> next) {
	    if(n -> type == ACTION) {
                make_resource_list(n->requires, &resource_list, &num_resources, &rsize);
		make_resource_list(n->provides, &resource_list, &num_resources, &rsize);
	    }
	}
	*total_resources = num_resources;
	return resource_list;
    }
    else
        return NULL;
}



#ifdef UNIT_TEST
#include "test_resources.c"
#endif
