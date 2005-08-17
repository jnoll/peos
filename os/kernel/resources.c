#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pmlheaders.h"
#include "action.h"
#include "process_table.h"
#include "graph.h"
#ifndef PALM 
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "tclinterp.h"
#include "predicate_evaluator.h"
#endif

void insert_resource(char *id, peos_resource_t **rlist, int *num_resources, int *rsize, char *qualifier) {
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
	strcpy(resource_list[*num_resources].qualifier, qualifier);
	*num_resources = *num_resources + 1;
    }
    *rlist = resource_list;
}

void make_resource_list(Tree t, peos_resource_t **rlist, int *num_resources, int *rsize, char *qualifier) {
    char *qual = qualifier;	
    peos_resource_t *resource_list = *rlist;
    if(t) {
	if (IS_OP_TREE(t)) {
	    switch TREE_OP(t) {
	    case DOT:
	    make_resource_list(t->left, &resource_list, num_resources, rsize, "\0");
	    break;
	    case EQ:
	    case NE:
	    case GE:
	    case LE:
	    case LT:
	    case GT: {
		make_resource_list(t->left, &resource_list, num_resources, rsize, "\0");
		make_resource_list(t->right, &resource_list, num_resources, rsize, "\0");
	    }
	    break;
	    case QUALIFIER: {
		make_resource_list(t->right, &resource_list, num_resources, rsize, TREE_ID(t->left));
	    }
	    break;
	    case AND: {
                make_resource_list(t->left, &resource_list,num_resources, rsize, "\0");
	        make_resource_list(t->right, &resource_list,num_resources, rsize, "\0");
			      
	    }
	    break;
	    case OR: {
                make_resource_list(t->left, &resource_list,num_resources, rsize, "\0");
	        make_resource_list(t->right, &resource_list,num_resources, rsize, "\0");
			      
	    }
            break;		      
	    }
	} else  
	if (IS_ID_TREE(t)) {
		if(strlen(TREE_ID(t))>0){
			if (TREE_ID(t)[0]!='\"'){
	    			insert_resource(TREE_ID(t), &resource_list, num_resources, rsize, qual);
			}
		}
		
	} else { 
	    make_resource_list(t->left, &resource_list,num_resources, rsize, "\0");
	    make_resource_list(t->right, &resource_list,num_resources, rsize, "\0");
	}
    }
    *rlist = resource_list;
    return;
}

#ifndef PALM
peos_resource_t *get_resource_list_action_requires(int pid, char *act_name, int *total_resources) {
    int rsize = 256;
    Node n;
    peos_resource_t *act_resources;
    int num_resources = 0;
    peos_context_t *context = peos_get_context(pid);
    Graph g = context -> process_graph;

    if (g == NULL) {
        fprintf(stderr, "System Error: Unable to find graph: get_resource_list_action_requires\n");
        return NULL;
    }

    n = find_node(g, act_name);

    if(n == NULL) {
        fprintf(stderr,"get_resource_list_action :cannot find action\n");
        return NULL;
    }

    act_resources = (peos_resource_t *) calloc(rsize,sizeof(peos_resource_t));
    make_resource_list(n -> requires, &act_resources, &num_resources, &rsize, "\0");
    *total_resources = num_resources;
    return act_resources;
}

peos_resource_t *get_resource_list_action_provides(int pid, char *act_name, int *total_resources) {
    int rsize = 256;
    Node n;
    peos_resource_t *act_resources;
    int num_resources = 0;
    peos_context_t *context = peos_get_context(pid);
    Graph g = context -> process_graph;

    if (g == NULL) {
        fprintf(stderr, "System Error: Unable to find graph: get_resource_list_action_requires\n");
        return NULL;
    }

    n = find_node(g, act_name);

    if(n == NULL) {
        fprintf(stderr,"get_resource_list_action :cannot find action\n");
        return NULL;
    }

    act_resources = (peos_resource_t *) calloc(rsize,sizeof(peos_resource_t));
    make_resource_list(n -> provides, &act_resources, &num_resources, &rsize, "\0");
    *total_resources = num_resources;
    return act_resources;
}

peos_resource_t *get_resource_list_action(int pid, char *act_name, int *total_resources) {
    int rsize = 256;
    Node n;
    peos_resource_t *act_resources;
    int num_resources = 0;
    peos_context_t *context = peos_get_context(pid);
    Graph g = context -> process_graph;

    if (g == NULL) {
        fprintf(stderr, "System Error: Unable to find graph: get_resource_list_action_requires\n");
        return NULL;
    }

    n = find_node(g, act_name);

    if(n == NULL) {
        fprintf(stderr,"get_resource_list_action :cannot find action\n");
        return NULL;
    }

    act_resources = (peos_resource_t *) calloc(rsize,sizeof(peos_resource_t));
    make_resource_list(n -> requires, &act_resources,&num_resources, &rsize, "\0");
    make_resource_list(n -> provides, &act_resources,&num_resources, &rsize, "\0");
    *total_resources = num_resources;
    return act_resources;
}
#endif

/*  
 * Get the list of resource names for a process. 
 */
peos_resource_t *get_resource_list(char *model, int *total_resources)
{
#ifndef PALM
    int rsize = 256;
#else
    int rsize=15;
#endif;

    Graph g;
    Node n;
    int num_resources = 0;
    peos_resource_t *resource_list;
    g = makegraph(model);    //see graph.c
    if(g != NULL) {
        resource_list = (peos_resource_t *) calloc(rsize,sizeof(peos_resource_t));
        for(n = g->source->next; n != NULL; n = n -> next) {
            if(n -> type == ACTION) {
                make_resource_list(n->requires, &resource_list, &num_resources, &rsize, "\0");
                make_resource_list(n->provides, &resource_list, &num_resources, &rsize, "\0");
            }
        }
        *total_resources = num_resources;
        GraphDestroy(g);
        return resource_list;
    }
    else {
        *total_resources = 386; /* checking path test is going down */
        return NULL;
    }
}

#ifdef UNIT_TEST
#include "test_resources.c"
#endif
