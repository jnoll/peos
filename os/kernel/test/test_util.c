#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <getopt.h>
#include <stdlib.h>
#include "process.h"
#include "test_util.h"
#include "action.h"
#include "graph_engine.h"

/* Test control flags. */

int fork_status = CK_NOFORK;
int verbosity = CK_NORMAL;


/* forward declarations */

Graph stub_makegraph(char *file);

Tree make_tree(char *sval,int ival, Tree left, Tree right)
{
	Tree t = (Tree) malloc (sizeof (struct tree));
	t -> sval = sval;
	t -> ival = ival;
	t -> left = left;
	t -> right = right;
	return t;
}

List make_list(Item i1, Item i2, Item i3, Item i4, Item i5)
{
	List l = ListCreate();
	if(i1)
	ListPut(l,i1);
        if(i2)
	ListPut(l,i2);
        if(i3)
	ListPut(l,i3);
        if(i4)
        ListPut(l,i4);
	if(i5)
	ListPut(l,i5);
	return l;

}


Node make_node(char *name, vm_act_state state, int type,int order)
{

     Node n = (Node) malloc (sizeof(struct node));	
     n -> data = (void *) malloc (sizeof (struct data));
     n->name = name;
     STATE(n) = state;
     n->type = type;
     n -> predecessors = NULL;
     n -> successors = NULL;
     ITER_START(n) = 0;
     ITER_END(n) = 0;
     MARKED_0(n) = 0;
     MARKED_1(n) = 0;
     MARKED_2(n) = 0;
     MARKED_3(n) = 0;
     MARKED_4(n) = 0;
     ITER_START_NODES(n) = ListCreate();
     ITER_END_NODES(n) = ListCreate();
     SUPER_NODES(n) = ListCreate();
     ORDER(n) = order;
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
   MARKED_0(act_0) = 0;
   MARKED_1(act_0) = 0;
   MARKED_2(act_0) = 0;
   MARKED_3(act_0) = 0;
   MARKED_4(act_0) = 0;
   STATE(act_0) = ACT_NONE;
			  
   
   act_1->type = ACTION;
   act_1 -> name = "act_1";
   act_1 -> data = (void *) malloc (sizeof (struct data));
   act_1 -> script = "test script";
   MARKED_0(act_1) = 0;
   MARKED_1(act_1) = 0;
   MARKED_2(act_1) = 0;
   MARKED_3(act_1) = 0;
   MARKED_4(act_1) = 0;
	       
   STATE(act_1) = ACT_NONE;
  
   return g;
   
}



void 
make_pml_file(char *filename, char *text)
{
    FILE *f = fopen(filename, "w");
    fprintf(f, "%s\n", text);
    fclose(f);
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
