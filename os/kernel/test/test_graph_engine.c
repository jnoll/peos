#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <check.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "vm.h"
#include "process.h"
#include "process_table.h"
#include "events.h"
#include "test_util.h"

/* Stub (bogus) globals. */
extern Graph stub_makegraph(char *file);
extern Item stub_ListIndex(List, int);
extern Node make_node(char *, vm_act_state, int type);
extern Tree make_tree(char *,int, Tree,Tree);

/* Globals. */
int lineno = 1;
FILE *yyin ;
Graph program;
char *filename = "some file";
int cur_node,cur_len;
Node node_list[25];
int lengths[25];
peos_context_t process_table[PEOS_MAX_PID+1];
struct list {
	 int size;
	 int count;
	 int first;
	 Item *array;
};
		


/* stubs */
Item ListIndex(List l , int index)
{
	return node_list[cur_node++];
}

int ListSize(List l)
{
	return lengths[cur_len++];
}

Item ListPut(List l, Item i)
{
	Item item = (Item) malloc (sizeof (Item));
	return item;
}

List ListCreate()
{
	List l = (List) malloc (sizeof(struct list));
	return l;
}



peos_context_t *peos_get_context(int pid)
{
	return pid == 0 ? &(process_table[pid]) : NULL;
}

int yyparse()
{
	return 1;
}

START_TEST(test_find_node)
{
	Node n;
	char *file = "file";

	/* Pre : Graph exists */
	Graph g = stub_makegraph(file);

	/* Action */

	n = find_node(g,"act_0");

	/* Post */
	fail_unless(n != NULL,"return value");
	fail_unless(strcmp(n->name,"act_0") == 0, "correct node not found");

}
END_TEST


START_TEST(test_find_node_nonaction)
{
  Node n;
  char *file = "file";
			                                                                         
   /* Pre : Graph exists */
   Graph g = stub_makegraph(file);
				                                                                         
   /* Action */
                                                             
   n = find_node(g,"p");
					                                                                         
    /* Post */
    fail_unless(n == NULL, "non action node found");
                                                                     
}
END_TEST




START_TEST(test_makegraph)
{
	char *file = "file";
	Graph g;
        yyin = fopen(file ,"r");
        g = makegraph(file);
	fail_unless(g == NULL, "graph made from non existing file");
	
}
END_TEST

START_TEST(test_makegraph_error)
{
	char *file = "file";
	FILE *in = fopen(file, "w");
	Graph g;

	program = NULL;
        g = makegraph(file);
	fail_unless(g == NULL, "graph made from file with parse errors.");
}
END_TEST


START_TEST(test_mark_successors)
{
	Node source,sink,s,a,b,j;

	
 source = make_node("p",ACT_NONE,PROCESS);
 s = make_node("s", ACT_NONE,SELECTION);
 a = make_node("a",ACT_NONE,ACTION);
 b = make_node("b",ACT_NONE,ACTION);
 j = make_node("s",ACT_NONE,JOIN);
 sink = make_node("p",ACT_NONE,PROCESS);
 
 s -> matching = j;
 j -> matching = s;

 cur_node = 0;
 cur_len = 0;

 node_list[0] = a;
 node_list[1] = b;
 node_list[2] = NULL;
   
 lengths[0] = 0;
 lengths[1] = 2;
 lengths[2] = 0;
 lengths[3] = 2;
 lengths[4] = 0;
 lengths[5] = 2;
 lengths[6] = 0;

 mark_successors(s,ACT_READY);

 // Post

   fail_unless(STATE(a) == ACT_READY,"action a  not ready");
   fail_unless(STATE(b) == ACT_READY, "action b  not ready");
   fail_unless(STATE(s) == ACT_READY, "selection not ready");
   fail_unless(STATE(j) == ACT_READY, "join not ready");
   

}
END_TEST

START_TEST(test_action_done)
{
	List l = (List) malloc(sizeof(struct list));
	Graph g = (Graph) malloc(sizeof(struct graph));
	char *act_name = "act_0";
        Node source,sink,act_0,act_1;

	source = make_node("p",ACT_NONE,PROCESS);
	sink = make_node("p",ACT_NONE,PROCESS);

	act_0 = make_node("act_0",ACT_RUN,ACTION);
	act_1 = make_node("act_1",ACT_NONE,ACTION);

	g->source = source;
	g-> sink = sink;
       
        source->next = act_0;
	act_0->next = act_1;
	act_1->next = sink;
        act_0 -> predecessors = l;
	source -> predecessors = NULL;
	sink -> next = NULL;
	
	cur_node = 0;
	cur_len = 0;

	node_list[0] = act_1;
	node_list[1] = act_1;
	node_list[2] = NULL;
	
	lengths[0] = 1;
	lengths[1] = 0;
	lengths[2] = 1;
	lengths[3] = 1;


	fail_unless(action_done(g,act_name) == 1,"return value");
	fail_unless(STATE(source) != ACT_DONE, "source done");
	fail_unless(STATE(sink) != ACT_DONE, "sink done");
	fail_unless(STATE(act_0) == ACT_DONE,"action not done");
	fail_unless(STATE(act_1) == ACT_READY,"next action not ready");

}
END_TEST


START_TEST(test_action_run)
{
	List l = (List) malloc (sizeof(struct list));
	Graph g =(Graph) malloc (sizeof(struct graph));

        Node source,sink,act_0,act_1;
	source = make_node("process",ACT_NONE,PROCESS);
	sink = make_node("process",ACT_NONE,PROCESS);

	act_0 = make_node("act_0",ACT_READY,ACTION);
	act_1 = make_node("act_1",ACT_NONE,ACTION);

	g -> source = source;
	g -> sink = sink;

	source -> next = act_0;
	act_0 -> next = act_1;
	act_1 -> next = sink;
	sink -> next = NULL;

	act_0 -> predecessors = l;
	source -> predecessors = NULL;
	cur_node = 0;
	cur_len = 0;

	node_list[0] = source;
	node_list[1] = NULL;

	lengths[0] = 0;
	lengths[1] = 0;
	lengths[2] = 1;
	lengths[3] = 1;

        fail_unless(action_run(g,act_0->name) == 1,"return value");
        fail_unless(STATE(act_0) == ACT_RUN, "action state not run");
	fail_unless(STATE(act_1) == ACT_NONE, "act_1 not none");

}
END_TEST

START_TEST(test_action_done_iteration)
{
   List l = (List) malloc (sizeof(struct list));	
   Graph g =(Graph) malloc (sizeof(struct graph));
   Node source,sink,act_0,act_1;
   source = make_node("process",ACT_NONE,PROCESS);
   sink = make_node("process",ACT_NONE,PROCESS);
   act_0 = make_node("act_0",ACT_DONE,ACTION);
   act_1 = make_node("act_1",ACT_RUN,ACTION);
 
   g -> source = source;
   g -> sink = sink;
   
   source -> next = act_0;
   
   act_0 -> next = act_1;
   act_1 -> next = sink;
   
   sink -> next = NULL;
   act_0 -> predecessors = l;
   act_1 -> predecessors = l;
   source -> predecessors = NULL;
     
   cur_node = 0;
   cur_len = 0;
   
   node_list[0] = sink;
   node_list[1] = act_0;
   node_list[2] = sink;
   node_list[3] = NULL;

   lengths[0] = 2;
   lengths[1] = 2;
   lengths[2] = 1;
   lengths[3] = 1;
   
fail_unless(action_done(g,act_1->name) == 1,"return value");
fail_unless(STATE(sink) != ACT_DONE, "sink set to done");
fail_unless(STATE(source) != ACT_DONE, "source set to done");
fail_unless(STATE(act_0) == ACT_READY, "action 0  not ready");
fail_unless(STATE(act_1) == ACT_DONE, "action 1 not done");

}
END_TEST


START_TEST(test_action_run_selection)
{
   List l = (List) malloc (sizeof(struct list));	
   Graph g =(Graph) malloc (sizeof(struct graph));
   Node source,sink,act_0,act_1,sel,join;
   source = make_node("process",ACT_NONE,PROCESS);
   sink = make_node("process",ACT_NONE,PROCESS);
   act_0 = make_node("act_0",ACT_READY,ACTION);
   act_1 = make_node("act_1",ACT_READY,ACTION);
   sel = make_node("sel",ACT_NONE,SELECTION);
   join = make_node("sel",ACT_NONE,JOIN);

   
   g -> source = source;
   g -> sink = sink;
   
  source -> next = sel;
  sel -> next = act_0;
  sel -> predecessors = l;
  act_0 -> next = act_1;
  act_0 -> predecessors = l;
  source ->predecessors = NULL;
  act_1 -> next = join;
  join -> next = sink;
  sink -> next = NULL;

  sel -> matching = join;
  join -> matching = sel;

  cur_node = 0;
  cur_len = 0;
  
  node_list[0] = sel;
  node_list[1] = act_0;
  node_list[2] = act_1;
  node_list[3] = source;
  node_list[4] = NULL;
                                             
  lengths[0] = 0;
  lengths[1] = 0;
  lengths[2] = 1;
  lengths[3] = 0;
  lengths[4] = 0;
  lengths[5] = 2;
  lengths[6] = 2;
  lengths[7] = 2;
  lengths[8] = 1;
  lengths[9] = 1;
  lengths[10] = 1;
  lengths[11] = 1;
  
  fail_unless(action_run(g,act_0->name) == 1,"return value");
  fail_unless(STATE(act_0) == ACT_RUN, "action 0  not run");
  fail_unless(STATE(act_1) == ACT_NONE, "action 1 not set to none");
  fail_unless(STATE(sel) == ACT_RUN, "selection not run");
  fail_unless(STATE(join) == ACT_RUN, "join not run");
							                                                                                      
}
END_TEST


START_TEST(test_action_run_selection_recursive)
{
  List l = (List) malloc (sizeof(struct list));	
  Graph g =(Graph) malloc (sizeof(struct graph));
  Node source,sink,act_0,act_1,act_2,sel,join,sel1,join1;
  
  source = make_node("process",ACT_NONE,PROCESS);
  sink = make_node("process",ACT_NONE,PROCESS);
  act_0 = make_node("act_0",ACT_READY,ACTION);
  act_1 = make_node("act_1",ACT_READY,ACTION);
  act_2 = make_node("act_2",ACT_READY,ACTION);
  sel = make_node("sel",ACT_NONE,SELECTION);
  join = make_node("sel",ACT_NONE,JOIN);
  sel1 = make_node("sel1",ACT_NONE,SELECTION);
  join1 = make_node("sel1",ACT_NONE,SELECTION);
             
  g -> source = source;
  g -> sink = sink;
  source -> next = sel1;
  sel1 -> next = sel;
  sel1 -> predecessors = l;
  sel -> next = act_2;
  sel -> predecessors = l;
  act_2 -> next = act_0;
  act_0 -> next = act_1;
  act_0 -> predecessors = l;
  source -> predecessors = NULL;

  sel -> matching = join;
  join -> matching = sel;

  sel1 -> matching = join1;
  join1 -> matching = sel1;
  
  act_1 -> next = join;
  join -> next = join1;
  join1 -> next = sink;
  sink -> next = NULL;
  
  cur_node = 0;
  cur_len = 0;
  
  node_list[0] = sel;
  node_list[1] = act_0;
  node_list[2] = act_1;
  node_list[3] = sel1;
  node_list[4] = sel;
  node_list[5] = act_2;
  node_list[6] = source;
  node_list[7] = NULL;
  
  lengths[0] = 0;
  lengths[1] = 0;
  lengths[2] = 1;
  lengths[3] = 0;
  lengths[4] = 0;
  lengths[5] = 2;
  lengths[6] = 2;
  lengths[7] = 2;
  lengths[8] = 1;
  lengths[9] = 0;
  lengths[10] = 0;
  lengths[11] = 2;
  lengths[12] = 2;
  lengths[13] = 2;
  lengths[14] = 1;
  lengths[15] = 1;
  lengths[16] = 1;
  lengths[17] = 1;
  lengths[18] = 1;  
	  
  fail_unless(action_run(g,act_0->name) == 1,"return value");
  fail_unless(STATE(act_0) == ACT_RUN, "action 0  not run");
  fail_unless(STATE(act_1) == ACT_NONE, "action 1 not none");
  fail_unless(STATE(act_2) == ACT_NONE, "action 2 not none");
  fail_unless(STATE(sel) == ACT_RUN,"sel not run");
  fail_unless(STATE(sel1) == ACT_RUN, "sel1 not run");
  fail_unless(STATE(join) == ACT_RUN, "join not run");
  fail_unless(STATE(join1) == ACT_RUN, "join not run");
  
}
END_TEST

START_TEST(test_propogate_join_done)
{
  List l = (List) malloc (sizeof(struct list));
  Graph g =(Graph) malloc (sizeof(struct graph));
  Node source,sink,act_0,act_1,act_2,sel,join,sel1,join1;
  
  source = make_node("process",ACT_NONE,PROCESS);
  sink = make_node("process",ACT_NONE,PROCESS);
  act_0 = make_node("act_0",ACT_DONE,ACTION);
  act_1 = make_node("act_1",ACT_NONE,ACTION);
  act_2 = make_node("act_2",ACT_NONE,ACTION);
  sel = make_node("sel",ACT_RUN,SELECTION);
  join = make_node("sel",ACT_RUN,JOIN);
  sel1 = make_node("sel1",ACT_RUN,SELECTION);
  join1 = make_node("sel1",ACT_RUN,JOIN);
  
  g -> source = source;
  g -> sink = sink;
  source -> next = sel1;
  sel1 -> next = sel;
  sel1 -> predecessors = l;
  sel -> next = act_2;
  sel -> predecessors = l;
  act_2 -> next = act_0;
  act_0 -> next = act_1;
  act_0 -> predecessors = l;
  source -> predecessors = NULL;
  
  sel -> matching = join;
  join -> matching = sel;
                                                                        
  sel1 -> matching = join1;
  join1 -> matching = sel1;
  
  act_1 -> next = join;
  join -> next = join1;
  join1 -> next = sink;
  sink -> next = NULL;
							      
  cur_node = 0;
  cur_len = 0;
  
  node_list[0] = join1;
  node_list[1] = sink;
  node_list[2] = NULL;
  
  lengths[0] = 1;
  lengths[1] = 1;
  lengths[2] = 1;
  lengths[3] = 1;

  /* Action */
   propogate_join_done(join); 

  // Post
   
   fail_unless(STATE(act_0) == ACT_DONE, "act 0 not done");
   fail_unless(STATE(act_1) == ACT_NONE, "act 1 not none");
   fail_unless(STATE(act_2) == ACT_NONE, "act 2 not none");
   fail_unless(STATE(sel) == ACT_DONE, "sel not done");
   fail_unless(STATE(join) == ACT_DONE, "join not done");
   fail_unless(STATE(sel1) == ACT_DONE, "sel1 not done");
   fail_unless(STATE(join1) == ACT_DONE, "join1 not done");
   
}
END_TEST

START_TEST(test_set_rendezvous_state)
{
   List l = (List) malloc(sizeof(struct list));
   Graph g = (Graph) malloc (sizeof(struct graph));
   Node source,sink,bigbr,br,bigrn,rn,act_0,act_1,act_2;

   source = make_node("p",ACT_NONE,PROCESS);
   sink = make_node("p",ACT_NONE,PROCESS);
   act_0 = make_node("act_0",ACT_DONE,ACTION);
   act_1 = make_node("act_1",ACT_DONE,ACTION);
   act_2 = make_node("act_2",ACT_DONE,ACTION);
   bigbr = make_node("bigbr",ACT_RUN,BRANCH);
   br = make_node("br",ACT_RUN,BRANCH);
   bigrn = make_node("bigrn",ACT_RUN,RENDEZVOUS);
   rn = make_node("rn",ACT_RUN,RENDEZVOUS);

   g -> source = source;
   g -> sink = sink;

   source -> next = bigbr;
   bigbr -> next = br;
   br -> next = act_2;
   act_2 -> next = act_0;
   act_0 -> next = act_1;
   act_1 -> next = rn;
   rn -> next = bigrn;
   bigrn -> next = sink;
   sink -> next = NULL;

   bigbr -> matching = bigrn;
   bigrn -> matching = bigbr;

   br -> matching = rn;
   rn -> matching = br;

   source -> matching = sink;
   sink -> matching = source;

   source -> predecessors = NULL;
   source -> successors = l;

   cur_node = 0;
   cur_len = 0;

   node_list[0] = act_0;
   node_list[1] = act_1;
   node_list[2] = bigrn;
   node_list[3] = rn;
   node_list[4] = act_2;
   node_list[5] = sink;
   node_list[6] = NULL;

   lengths[0] = 2;
   lengths[1] = 2;
   lengths[2] = 2;
   lengths[3] = 1;
   lengths[4] = 2;
   lengths[5] = 2;
   lengths[6] = 2;
   lengths[7] = 1;
   lengths[8] = 1;
   lengths[9] = 1;
   lengths[10] = 1;
   

   set_rendezvous_state(rn);

   fail_unless(STATE(rn) == ACT_DONE, "rn not done");
   fail_unless(STATE(br) == ACT_DONE, "br not done");
   fail_unless(STATE(bigrn) == ACT_DONE, "bigrn not done");
   fail_unless(STATE(bigbr) == ACT_DONE, "bigbr not done");
   
}
END_TEST

START_TEST(test_set_process_state)
{
	 Graph g = (Graph) malloc (sizeof(struct graph));
	 Node source,sink,act_0;

	 source = make_node("p",ACT_NONE,PROCESS);
	 sink = make_node("p",ACT_NONE,PROCESS);
	 act_0 = make_node("act_0",ACT_DONE,ACTION);

	 g -> source = source;
	 g -> sink = sink;
	 source -> matching = sink;
	 sink -> matching = source;
	 
	 cur_node = 0;
	 cur_len = 0;

	 node_list[0] = act_0;
	 node_list[1] = NULL;

	 lengths[0] = 1;
	 lengths[1] = 1;
	 lengths[2] = 1;

	 set_process_state(g);

	 fail_unless(STATE(source) == ACT_DONE,"source not done");
	 fail_unless(STATE(sink) == ACT_DONE,"sink not done");
	 fail_unless(STATE(act_0) == ACT_DONE,"act_0 not done");

}
END_TEST

START_TEST(test_mark_iter_nodes)
{
	Graph g = (Graph) malloc (sizeof(struct graph));
        Node source,sink,act_0,act_1,act_2;
                                                                        
        source = make_node("p",ACT_NONE,PROCESS);
        sink = make_node("p",ACT_NONE,PROCESS);
        act_0 = make_node("act_0",ACT_READY,ACTION);
	act_1 = make_node("act_1",ACT_NONE,ACTION);
	act_2 = make_node("act_2",ACT_NONE,ACTION);
					                                                                             
        g -> source = source;
        g -> sink = sink;
        source -> matching = sink;
        sink -> matching = source;
					                                                                         
        cur_node = 0;
	cur_len = 0;

	node_list[0] = act_2;
	node_list[1] = NULL;

	lengths[0] = 1;
	lengths[1] = 0;
	lengths[2] = 1;
	lengths[3] = 1;

	mark_iter_nodes(act_0);

	fail_unless(STATE(act_0) == ACT_READY, "act_0 not ready");
	fail_unless(STATE(act_1) == ACT_NONE, "act_1 not none");
	fail_unless(STATE(act_2) == ACT_READY, "act_2 not ready");
}
END_TEST

START_TEST(test_make_resource_list)
{
	int num_resources = 0;
	char *ptr = "y";
	peos_resource_t *resource_list = (peos_resource_t *) calloc(256,sizeof(peos_resource_t));
	
	Tree t1 = make_tree("y",0,NULL,NULL);
	Tree t2 = make_tree("modified",0,NULL,NULL);
	Tree t3 = make_tree(NULL,DOT,t1,t2);
	Tree t4 = make_tree("true",0,NULL,NULL);
	Tree t5 = make_tree(NULL,EQ,t3,t4);

	make_resource_list(t5,resource_list,&num_resources);

	fail_unless(num_resources == 1,"num_resources wrong");
    fail_unless(strcmp(resource_list[0].name,ptr) == 0,"resource name wrong");

}	
END_TEST
	

START_TEST(test_initialize_graph)
{
	Graph g = (Graph) malloc (sizeof (struct graph));
        Node source,sink,act_0,act_1;

	source = (Node) malloc (sizeof (struct node));
	sink = (Node) malloc (sizeof (struct node));

	act_0 = (Node) malloc (sizeof (struct node));
	act_1 = (Node) malloc (sizeof (struct node));

	g -> source = source;
	g -> sink = sink;

	act_0 -> type = ACTION;
	act_1 -> type = ACTION;


	source -> type = PROCESS;
	sink -> type = PROCESS;
        source -> next = act_0;
	act_0 -> next = act_1;
	act_1 -> next = sink;
	sink -> next = NULL;

	cur_node = 0;
	cur_len = 0;

	node_list[0] = source;
	node_list[1] = act_1;
	node_list[2] = act_0;
	node_list[3] = sink;
	node_list[4] = act_1;
	node_list[5] = NULL;

	lengths[0] = 1;
	lengths[1] = 1;
	lengths[2] = 1;
	lengths[3] = 1;
	lengths[4] = 1;
	lengths[5] = 1;
	lengths[6] = 1;
	lengths[7] = 1;
	lengths[8] = 1;
	lengths[9] = 1;
	lengths[10] = 0;
	lengths[11] = 0;
	lengths[12] = 0;
	lengths[13] = 0;
		
	initialize_graph(g);

	fail_unless(STATE(act_0) == ACT_READY, "act 0 not ready");
	fail_unless(STATE(act_1) == ACT_NONE, "act 1 not none");
	fail_unless(MARKED(act_0) == FALSE, "act 0 not marked false");
	fail_unless(MARKED(act_1) == FALSE, "act 1 not marked false");


}
END_TEST

START_TEST(test_annotate_graph)
{
	int i;
	Graph g = (Graph) malloc (sizeof (struct graph));
	Node source,sink,act_0,act_1,sel,branch,join,rendezvous;
	peos_context_t *context = &(process_table[0]);
	context->num_actions = 2;
	context->actions = (peos_action_t *) calloc(context->num_actions, sizeof(peos_action_t));

	for(i = 0; i < context->num_actions ; i++)
	{
		sprintf(context->actions[i].name,"act_%d",i);
		context->actions[i].state = ACT_RUN;
	}

        context->num_other_nodes = 2;
        context->other_nodes = (peos_other_node_t *) calloc(context->num_other_nodes, sizeof(peos_other_node_t));
                                                                       
          sprintf(context->other_nodes[0].name,"sel");
	  sprintf(context->other_nodes[1].name,"br");
          context->other_nodes[0].state = ACT_RUN;
	  context->other_nodes[1].state = ACT_READY;
     
	source = make_node("p",ACT_NONE,PROCESS);
	sink = make_node("p",ACT_NONE,PROCESS);
	act_0 = make_node("act_0",ACT_NONE,ACTION);
	act_1 = make_node("act_1",ACT_NONE,ACTION);
	sel = make_node("sel",ACT_NONE,SELECTION);
	join = make_node("sel",ACT_NONE,JOIN);
	branch = make_node("br",ACT_NONE,BRANCH);
	rendezvous = make_node("br",ACT_NONE,RENDEZVOUS);

	g -> source = source;
	g -> sink = sink;
	source -> next = act_0;
	act_0 -> next = act_1;
	act_1 -> next = branch;
	branch -> next = sel;
	sel -> next = rendezvous;
	rendezvous -> next = join;
	join -> next = sink;
	sink -> next = NULL;

	sel -> matching = join;
	branch -> matching = rendezvous;

	fail_unless(annotate_graph(g,context) == 1, "return value");
	fail_unless(STATE(act_0) == ACT_RUN, "act 0 state not run");
	fail_unless(STATE(act_1) == ACT_RUN, "act 1 state not run");
	fail_unless(STATE(sel) == ACT_RUN, "sel not run");
	fail_unless(STATE(join) == ACT_RUN, "join not run");
	fail_unless(STATE(branch) == ACT_READY, "branch not ready");
       fail_unless(STATE(rendezvous) == ACT_READY, "rendezvous not run");

}
END_TEST

START_TEST(test_update_context)
{
	int i;
	Graph g = (Graph) malloc (sizeof (struct graph));
	Node source,sink,act_0,act_1,sel,branch,join,rendezvous;
	peos_context_t *context = &(process_table[0]);

	context -> status = PEOS_READY;
	context -> num_actions = 2;
	context -> actions = (peos_action_t *) calloc(context->num_actions, sizeof(peos_action_t));

	for(i = 0; i < 2 ; i++)
	{
		sprintf(context->actions[i].name,"act_%d",i);
		context->actions[i].state = ACT_NONE;
	}

	context->num_other_nodes = 2;
        context->other_nodes = (peos_other_node_t *) calloc(context->num_other_nodes, sizeof(peos_other_node_t));
                                                                    
        sprintf(context->other_nodes[0].name,"sel");
        sprintf(context->other_nodes[1].name,"br");
        context->other_nodes[0].state = ACT_NONE;
        context->other_nodes[1].state = ACT_NONE;
                                                          
	source = make_node("p",ACT_DONE,PROCESS);
        sink = make_node("p",ACT_DONE,PROCESS);
        act_0 = make_node("act_0",ACT_RUN,ACTION);
        act_1 = make_node("act_1",ACT_RUN,ACTION);
        sel = make_node("sel",ACT_RUN,SELECTION);
        join = make_node("sel",ACT_RUN,JOIN);
        branch = make_node("br",ACT_READY,BRANCH);
        rendezvous = make_node("br",ACT_READY,RENDEZVOUS);

        g -> source = source;
        g -> sink = sink;
        source -> next = act_0;
        act_0 -> next = act_1;
        act_1 -> next = branch;
        branch -> next = sel;
        sel -> next = rendezvous;
        rendezvous -> next = join;
        join -> next = sink;
        sink -> next = NULL;
									         sel -> matching = join;
        branch -> matching = rendezvous;

	
        fail_unless(update_context(g,context) == 1, "return value");
	fail_unless(context -> status == PEOS_DONE, "status not peos_done");
	fail_unless(context->actions[0].state == ACT_RUN, "act 0 state not run");
	fail_unless(context->actions[1].state == ACT_RUN, "act 1 state not run");
	fail_unless(context->other_nodes[0].state == ACT_RUN, "sel state not run");
	fail_unless(context->other_nodes[1].state == ACT_READY, "br state not ready");


}
END_TEST



START_TEST(test_set_act_state_graph_ready)
{
	Graph g = (Graph) malloc (sizeof(struct graph));
	Node source,sink,act_0,act_1;

	source = make_node("p",ACT_NONE,PROCESS);
	sink = make_node("p",ACT_NONE,PROCESS);
	act_0 = make_node("act_0",ACT_NONE,ACTION);
	act_1 = make_node("act_1",ACT_NONE,ACTION);


	g -> source = source;
	g -> sink = sink;

	source -> next = act_0;
	act_0 -> next = act_1;
	act_1 -> next = sink;
	sink -> next = NULL;


    fail_unless(set_act_state_graph(g,"act_0",ACT_READY) == 1, "return value");
    fail_unless(STATE(act_0) == ACT_READY, "act 0 state not changed");

}
END_TEST


START_TEST(test_set_act_state_graph_suspend)
{
	Graph g = (Graph) malloc (sizeof(struct graph));
	Node source,sink,act_0,act_1;

	source = make_node("p",ACT_NONE,PROCESS);
	sink = make_node("p",ACT_NONE,PROCESS);
	act_0 = make_node("act_0",ACT_RUN,ACTION);
	act_1 = make_node("act_1",ACT_RUN,ACTION);


	g -> source = source;
	g -> sink = sink;

	source -> next = act_0;
	act_0 -> next = act_1;
	act_1 -> next = sink;
	sink -> next = NULL;



    fail_unless(set_act_state_graph(g,"act_0",ACT_SUSPEND) == 1, "return value");
    fail_unless(STATE(act_0) == ACT_SUSPEND, "act 0 state not changed");

}
END_TEST


START_TEST(test_set_act_state_graph_none)
{
	Graph g = (Graph) malloc (sizeof(struct graph));
	Node source,sink,act_0,act_1;

	source = make_node("p",ACT_NONE,PROCESS);
	sink = make_node("p",ACT_NONE,PROCESS);
	act_0 = make_node("act_0",ACT_RUN,ACTION);
	act_1 = make_node("act_1",ACT_RUN,ACTION);


	g -> source = source;
	g -> sink = sink;

	source -> next = act_0;
	act_0 -> next = act_1;
	act_1 -> next = sink;
	sink -> next = NULL;


    fail_unless(set_act_state_graph(g,"act_0",ACT_NONE) == 1, "return value");
    fail_unless(STATE(act_0) == ACT_NONE, "act 0 state not changed");

}
END_TEST



START_TEST(test_set_act_state_graph_abort)
{
	Graph g = (Graph) malloc (sizeof(struct graph));
	Node source,sink,act_0,act_1;

	source = make_node("p",ACT_NONE,PROCESS);
	sink = make_node("p",ACT_NONE,PROCESS);
	act_0 = make_node("act_0",ACT_RUN,ACTION);
	act_1 = make_node("act_1",ACT_RUN,ACTION);


	g -> source = source;
	g -> sink = sink;

	source -> next = act_0;
	act_0 -> next = act_1;
	act_1 -> next = sink;
	sink -> next = NULL;


    fail_unless(set_act_state_graph(g,"act_0",ACT_ABORT) == 1, "return value");
    fail_unless(STATE(act_0) == ACT_ABORT, "act 0 state not changed");

}
END_TEST

START_TEST(test_set_act_state_graph_new)
{
	Graph g = (Graph) malloc (sizeof(struct graph));
	Node source,sink,act_0,act_1;

	source = make_node("p",ACT_NONE,PROCESS);
	sink = make_node("p",ACT_NONE,PROCESS);
	act_0 = make_node("act_0",ACT_RUN,ACTION);
	act_1 = make_node("act_1",ACT_RUN,ACTION);


	g -> source = source;
	g -> sink = sink;

	source -> next = act_0;
	act_0 -> next = act_1;
	act_1 -> next = sink;
	sink -> next = NULL;


    fail_unless(set_act_state_graph(g,"act_0",ACT_NEW) == 1, "return value");
    fail_unless(STATE(act_0) == ACT_NEW, "act 0 state not changed");

}
END_TEST






int
main(int argc, char *argv[])
{
    
	
    int nf;
    SRunner *sr;
    Suite *s = suite_create("graph_engine");
    TCase *tc;

    parse_args(argc, argv);

    tc = tcase_create("find_node");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_find_node);
    tcase_add_test(tc, test_find_node_nonaction);

   tc = tcase_create("makegraph");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_makegraph);
    tcase_add_test(tc,test_makegraph_error);

    tc = tcase_create("mark_successors");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_mark_successors);

    tc = tcase_create("make resource list");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_make_resource_list);

 
    tc = tcase_create("action done");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_action_done);
    tcase_add_test(tc,test_action_done_iteration);
    tcase_add_test(tc,test_propogate_join_done);

    tc = tcase_create("set_rendezvous_state");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_set_rendezvous_state);

    
    tc = tcase_create("action run");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_action_run);
    tcase_add_test(tc,test_action_run_selection);
    tcase_add_test(tc,test_action_run_selection_recursive);

    tc = tcase_create("set_process_state");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_set_process_state);

    tc = tcase_create("matk_iter_nodes");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_mark_iter_nodes);
    
    tc = tcase_create("initialize graph");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_initialize_graph);

    tc = tcase_create("annotate graph");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_annotate_graph);

    tc = tcase_create("update context");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_update_context);



    tc = tcase_create("set act state graph");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_set_act_state_graph_ready);
    tcase_add_test(tc,test_set_act_state_graph_suspend);
    tcase_add_test(tc,test_set_act_state_graph_none);
    tcase_add_test(tc,test_set_act_state_graph_abort);
    tcase_add_test(tc,test_set_act_state_graph_new);








    sr = srunner_create(s);

    srunner_set_fork_status(sr, fork_status);

    srunner_run_all(sr, verbosity);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    suite_free(s);
    if (nf == 0)
	return EXIT_SUCCESS;
         else
	/* Leave evidence. */
	return EXIT_FAILURE;

}

