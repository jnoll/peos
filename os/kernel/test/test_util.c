#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <getopt.h>
#include <stdlib.h>
#include "vm.h"
#include "process.h"
#include "test_util.h"
#include "action.h"
#include "graph_engine.h"

/* Test control flags. */

int fork_status = CK_NOFORK;
int verbosity = CK_NORMAL;
/* Utility functions. */


Tree make_tree(char *sval,int ival, Tree left, Tree right)
{
	Tree t = (Tree) malloc (sizeof (struct tree));
	t -> sval = sval;
	t -> ival = ival;
	t -> left = left;
	t -> right = right;
	return t;
}

Node make_node(char *name, vm_act_state state, int type)
{

     Node n = (Node) malloc (sizeof(struct node));	
     n -> data = (void *) malloc (sizeof (struct data));
     n->name = name;
     STATE(n) = state;
     n->type = type;
     return n;
}

peos_action_t *make_actions(int size, vm_act_state state)
{
    int i;
    peos_action_t *actions = (peos_action_t *)calloc(size, sizeof(peos_action_t));
    for (i = 0; i < size; i++) {
	sprintf(actions[i].name, "act_%d", i);
	actions[i].state = state;
	actions[i].pid = -1;
    }
    return actions;
}

void free_actions(peos_action_t *actions, int size)
{
    free(actions);
}


int stub_load_actions(char *file, peos_action_t **actions, int *num_actions,peos_other_node_t **other_nodes, int *num_other_nodes)
{
	peos_action_t *act_array = (peos_action_t *) calloc(2,sizeof(peos_action_t));
	peos_other_node_t *nodes_array = (peos_other_node_t *) calloc(1,sizeof(peos_other_node_t));
	
        if(strcmp(file,"no") == 0) return -1; 
	strcpy(act_array[0].name,"act_0");
	strcpy(act_array[1].name,"act_1");
        act_array[0].state = ACT_NONE;
        act_array[1].state = ACT_NONE;	
	strcpy(nodes_array[0].name,"sel");
	nodes_array[0].state = ACT_NONE;
	*actions = act_array;
	*other_nodes = nodes_array; 
	
        *num_other_nodes = 1;	

        *num_actions = 2;	
	return 1;
        
}


Graph stub_makegraph(char *file)
{
   Graph g = (Graph) malloc (sizeof(struct graph));
   Node source = (Node) malloc (sizeof(struct node));
   Node sink = (Node) malloc (sizeof(struct node));
   Node act_0 = (Node) malloc (sizeof(struct node));
   Node act_1 = (Node) malloc (sizeof(struct node));
   Node sel = (Node) malloc (sizeof(struct node));
   Node join = (Node) malloc (sizeof(struct node));
   Tree t1 = make_tree("y",0,NULL,NULL);
   Tree t2 = make_tree("modified",0,NULL,NULL);
   Tree t3 = make_tree(NULL,DOT,t1,t2);
   Tree t4 = make_tree("true",0,NULL,NULL);
   Tree t5 = make_tree(NULL,EQ,t3,t4);

   Tree t6 = make_tree("x",0,NULL,NULL);

   Tree t7 = make_tree("x",0,NULL,NULL);
   Tree t8 = make_tree("z",0,NULL,NULL);
   Tree t9 = make_tree(NULL,AND,t7,t8);

   sel->name = "sel";
   join->name = "sel";
   sel->type = SELECTION;
   sel -> data = (void *) malloc (sizeof (struct data));
   STATE(sel) = ACT_NONE;
   join->type = JOIN;
   join -> data = (void *) malloc (sizeof (struct data));
   STATE(join) = ACT_NONE;

   act_0 -> requires = t5;
   act_0 -> provides = t6;
   act_1 -> requires = t9;
   act_1 -> provides = NULL;
   
   g -> source = source;
   g -> sink = sink;
   source->type = PROCESS;
   sink->type = PROCESS;
   source->name = "p";  
   sink -> name = "p";
   
   source->next = sel;
   sel->next = act_0;
   act_0->next = act_1;
   act_1->next = join;
   join->next=sink;
   sink->next=NULL;
   
   act_0 -> type = ACTION;
   act_0 -> name = "act_0";
   act_0 -> data = (void *) malloc (sizeof (struct data));
   act_0 -> script = "test script";
   MARKED(act_0) = 0;
   STATE(act_0) = ACT_NONE;
			  
   
   act_1->type = ACTION;
   act_1 -> name = "act_1";
   act_1 -> data = (void *) malloc (sizeof (struct data));
   act_1 -> script = "test script";
   MARKED(act_1) = 0;
   STATE(act_1) = ACT_NONE;

  
   return g;
   
}





void
parse_args(int argc, char *argv[])
{
    int c;

    while(( c = getopt(argc, argv, "vfq")) != -1)
    {
	switch (c){
	case 'v':
	    verbosity = CK_VERBOSE;
	    break;
	case 'q':
	    verbosity = CK_MINIMAL;
	    break;
	case 'f': 
	    fork_status = CK_FORK;
	    break;
	default:
	    fprintf(stderr, "usage: %s [-vf] (verbose, fork)\n", argv[0]);
	    exit(1);
	}
     }
}
