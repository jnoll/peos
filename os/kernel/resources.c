#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "pmlheaders.h"
#include "action.h"
#include "process_table.h"
#include "graph.h"
#include "tclinterp.h"
/*
 ******* this variable is global
 */
peos_tcl p;
/*********/
void insert_resource(char *id, peos_resource_t **rlist, int *num_resources, int *rsize, char *qualifier) 
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
	strcpy(resource_list[*num_resources].qualifier, qualifier);
	*num_resources = *num_resources + 1;
    }
    *rlist = resource_list;
}

void make_resource_list(Tree t, peos_resource_t **rlist, int *num_resources, int *rsize, char *qualifier)
{
    char *qual = qualifier;	
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
		make_resource_list(t->left, &resource_list, num_resources, rsize, "\0");
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
	} else if (IS_ID_TREE(t)) {
	    insert_resource(TREE_ID(t), &resource_list, num_resources, rsize, qual);
	} else {                       
	    make_resource_list(t->left, &resource_list,num_resources, rsize, "\0");
	    make_resource_list(t->right, &resource_list,num_resources, rsize, "\0");
	}
    }
    *rlist = resource_list;
    return;
}

peos_resource_t *get_resource_list_action_requires(int pid, char *act_name, int *total_resources)
{

    char* result_str=NULL; /*********/
    Graph g;
    Node n;
    int i,j;
    int rsize = 256;
    int num_resources = 0;
    peos_context_t *context = peos_get_context(pid);
    peos_resource_t *proc_resources = context -> resources;
    int num_proc_resources = context -> num_resources;

    peos_resource_t *act_resources;
    /************/
    if(debug)fprintf(stderr,"___________________get_resource_list_action_requires____________\n");
    if(started==0){
       if(peos_tcl_start(&p)==TCL_ERROR){
          if(debug)fprintf(stderr,"ERROR: TCL_ERROR creating a Tcl interpreter\n");
          return TCL_ERROR;
       }
    }
    if(!result_str){
       result_str = (char*)malloc(sizeof(char)*(255));
    }
    started =1;
    /***************/
    g = context -> process_graph;
    if(g != NULL) {
        n = find_node(g,act_name);
        if(n == NULL) {
            fprintf(stderr,"get_resource_list_action :cannot find action\n");
	    if(result_str) free(result_str);
	    return NULL;
	}

	act_resources = (peos_resource_t *) calloc(rsize,sizeof(peos_resource_t));
	make_resource_list(n -> requires, &act_resources, &num_resources, &rsize, "\0");
	*total_resources = num_resources;
        for(i = 0; i < num_resources; i++) {
            for(j = 0; j < num_proc_resources; j++) {
	        if(strcmp(act_resources[i].name,proc_resources[j].name) == 0) {
		    /********/
		    peos_tcl_eval(&p,proc_resources[j].name , proc_resources[j].value, result_str );
		    
		    //peos_tcl_eval(&p,"something" , "value", result_str );
		    if(debug)fprintf(stderr,"action requires = %s\n",proc_resources[j].value);
		    if(debug)fprintf(stderr,"ar result_str = %s\n", result_str);
	            strcpy(act_resources[i].value,result_str);//proc_resources[j].value);
		    /*******/
		    break;
		}
	    }
	}
        if(debug)fprintf(stderr,"________________________________________________________________\n");
	if(result_str) free(result_str);
        return act_resources;
    }    
    else {
        fprintf(stderr, "System Error: Unable to find graph: get_resource_list_action_requires\n");
	if(result_str) free(result_str);
        return NULL;
    }
}

peos_resource_t *get_resource_list_action_provides(int pid, char *act_name, int
		*total_resources)
{
    char* result_str=NULL; /*********/
    Graph g;
    Node n;
    int i,j;
    int rsize = 256;
    int num_resources = 0;
    peos_context_t *context = peos_get_context(pid);
    peos_resource_t *proc_resources = context -> resources;
    int num_proc_resources = context -> num_resources;
 
    peos_resource_t *act_resources;
    /************/
    if(debug)fprintf(stderr,"___________________get_resource_list_action_provides____________\n");
    if(started==0){
       if(peos_tcl_start(&p)==TCL_ERROR){
          if(debug)fprintf(stderr,"ERROR: TCL_ERROR creating a Tcl interpreter\n");
          return TCL_ERROR;
       }
    }
    if(!result_str){
       result_str = (char*)malloc(sizeof(char)*(255));
    }
    started =1;
    /***************/
    g = context -> process_graph;   

    if(g != NULL) {
        n = find_node(g,act_name);
        if(n == NULL) {
            fprintf(stderr,"get_resource_list_action :cannot find action");
            return NULL;
        }

	act_resources = (peos_resource_t *) calloc(rsize,sizeof(peos_resource_t));
	make_resource_list(n -> provides, &act_resources, &num_resources, &rsize, "\0");
        *total_resources = num_resources;
        for(i = 0; i < num_resources; i++) {
            for(j = 0; j < num_proc_resources; j++) {
                if(strcmp(act_resources[i].name,proc_resources[j].name) == 0) {
		     /********/
		    peos_tcl_eval(&p,proc_resources[j].name , proc_resources[j].value, result_str );
		    
		    //peos_tcl_eval(&p,"something" , "value", result_str );
		    if(debug)fprintf(stderr,"action provides = %s\n",proc_resources[j].value);
		    if(debug)fprintf(stderr,"ap result_str = %s\n", result_str);
	            strcpy(act_resources[i].value,result_str);//proc_resources[j].value);
		    /*******/
                    //strcpy(act_resources[i].value,proc_resources[j].value);
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
    char* result_str=NULL; /*********/
    Graph g;
    Node n;
    int i,j;
    int rsize = 256;
    int num_resources = 0;
    peos_context_t *context = peos_get_context(pid);
    peos_resource_t *proc_resources = context -> resources;
    int num_proc_resources = context -> num_resources;
    peos_resource_t *act_resources;
    /************/
    if(debug)fprintf(stderr,"___________________get_resource_list_action____________\n");
    if(started==0){
       if(peos_tcl_start(&p)==TCL_ERROR){
          if(debug)fprintf(stderr,"ERROR: TCL_ERROR creating a Tcl interpreter\n");
          return TCL_ERROR;
       }
    }
    if(!result_str){
       result_str = (char*)malloc(sizeof(char)*(255));
    }
    started =1;
    /***************/
    g = context -> process_graph;
    if(g != NULL) {
        n = find_node(g,act_name);
	if(n == NULL) {
	    fprintf(stderr,"get_resource_list_action :cannot find action");
	    return NULL;
	}

	act_resources = (peos_resource_t *) calloc(rsize,sizeof(peos_resource_t));
	
	make_resource_list(n -> requires, &act_resources,&num_resources, &rsize, "\0");
	make_resource_list(n -> provides, &act_resources,&num_resources, &rsize, "\0");
	*total_resources = num_resources;
	for(i = 0; i < num_resources; i++) {
	    for(j = 0; j < num_proc_resources; j++) {
 	        if(strcmp(act_resources[i].name,proc_resources[j].name) == 0) {
		     /*******/
		    peos_tcl_eval(&p,proc_resources[j].name , proc_resources[j].value, result_str );
		    
		    //peos_tcl_eval(&p,"something" , "value", result_str );
		    if(debug)fprintf(stderr,"list action = %s\n",proc_resources[j].value);
		    if(debug)fprintf(stderr,"list action result_str = %s\n", result_str);
	            strcpy(act_resources[i].value,result_str);//proc_resources[j].value);
		    /*******/
		    //strcpy(act_resources[i].value,proc_resources[j].value);
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
                make_resource_list(n->requires, &resource_list, &num_resources, &rsize, "\0");
		make_resource_list(n->provides, &resource_list, &num_resources, &rsize, "\0");
	    }
	}
	*total_resources = num_resources;
	GraphDestroy(g);
	return resource_list;
    }
    else
        return NULL;
}



#ifdef UNIT_TEST
#include "test_resources.c"
#endif
