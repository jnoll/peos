#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <check.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "action.h"
#include "process.h"
#include "process_table.h"
#include "events.h"
#include "test_util.h"

/* Stub (bogus) globals. */
extern Graph stub_makegraph(char *file);
extern Node make_node(char *name, vm_act_state, int type,int order);
extern Tree make_tree(char *,int, Tree,Tree);
extern Tree make_dot_tree(char *name, char* attr);
extern Tree make_op_tree(Tree left, int op, char* value);
extern Tree make_con_tree(Tree left, Tree right, int op, char* value);
extern List make_list(Item i1, Item i2, Item i3, Item i4, Item i5);

/* Globals. */



peos_context_t process_table[PEOS_MAX_PID+1];


/* stubs */

peos_context_t *peos_get_context(int pid)
{
	return pid == 0 ? &(process_table[pid]) : NULL;
}



START_TEST(test_find_node)
{
	Node n;
	char *file;
	Graph g;
	


	file = "test_find_node.pml";
	/* Pre : Graph exists */
	g = makegraph(file);

	/* Action */

	n = find_node(g,"act_0");

	/* Post */
	fail_unless(n != NULL,"return value");
	fail_unless(strcmp(n->name,"act_0") == 0, "correct node not found");

}
END_TEST


START_TEST(test_find_node_error)
{
	Node n;
	char *file;
	Graph g;

	file = "test_find_node.pml";
	/* Pre : Graph exists */
	g = makegraph(file);

	/* Action */
	n = find_node(g,"act_2");

	/* Post */
	fail_unless(n == NULL,"non exixtent node found");

}
END_TEST


START_TEST(test_makegraph_nofile)
{
    char *file = "no_file";
    Graph g;
    g = makegraph(file);
    fail_unless(g == NULL, "graph made from non existing file");
}
END_TEST

START_TEST(test_makegraph_error)
{
    char *file = "test_makegraph_error";
    Graph g;
    g = makegraph(file);
    fail_unless(g == NULL, "graph made from file with parse errors.");
}
END_TEST


START_TEST(test_mark_successors)
{
	Node source,sink,s,a,b,j;
	

	source = make_node("p",ACT_NONE,PROCESS,0);
        s = make_node("s", ACT_NONE,SELECTION,1);
        a = make_node("a",ACT_NONE,ACTION,2);
        b = make_node("b",ACT_NONE,ACTION,3);
        j = make_node("s",ACT_NONE,JOIN,4);
        sink = make_node("p",ACT_NONE,PROCESS,5);
	
        s -> matching = j;
        j -> matching = s;
	s -> successors = (List) make_list(a,b,NULL,NULL,NULL);


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

    Graph g = (Graph) malloc(sizeof(struct graph));
    
    Node source,sink,act_0,act_1;
    source = make_node("p",ACT_NONE,PROCESS,0);
    sink = make_node("p",ACT_NONE,PROCESS,3);

    act_0 = make_node("act_0",ACT_RUN,ACTION,1);
    act_1 = make_node("act_1",ACT_NONE,ACTION,2);

    g->source = source;
    g-> sink = sink;
    source->next = act_0;
    act_0->next = act_1;
    act_1->next = sink;
    
    act_0 -> predecessors = (List) make_list(source,NULL,NULL,NULL,NULL);
    act_0 -> successors = (List) make_list(act_1,NULL,NULL,NULL,NULL);
    act_1 -> predecessors = (List) make_list(act_0,NULL,NULL,NULL,NULL);
    act_1 -> successors = (List) make_list(sink,NULL,NULL,NULL,NULL);
    sink -> predecessors = (List) make_list(act_1,NULL,NULL,NULL,NULL);
    source -> successors = (List) make_list(act_0,NULL,NULL,NULL,NULL);
    
       
    sink -> next = NULL;
    
    
    fail_unless(action_done(g,"act_0") == VM_CONTINUE,"return value");
    fail_unless(STATE(source) != ACT_DONE, "source done");
    fail_unless(STATE(sink) != ACT_DONE, "sink done");
    fail_unless(STATE(act_0) == ACT_DONE,"action not done");
    fail_unless(STATE(act_1) == ACT_READY,"next action not ready");

}
END_TEST


START_TEST(test_action_run)
{

    Graph g = (Graph) malloc(sizeof(struct graph));
    
    Node source,sink,act_0,act_1;
    source = make_node("p",ACT_NONE,PROCESS,0);
    sink = make_node("p",ACT_NONE,PROCESS,3);

    act_0 = make_node("act_0",ACT_NONE,ACTION,1);
    act_1 = make_node("act_1",ACT_NONE,ACTION,2);

    g->source = source;
    g-> sink = sink;
    source->next = act_0;
    act_0->next = act_1;
    act_1->next = sink;
    
    act_0 -> predecessors = (List) make_list(source,NULL,NULL,NULL,NULL);
    act_0 -> successors = (List) make_list(act_1,NULL,NULL,NULL,NULL);

    act_1 -> predecessors = (List) make_list(act_0,NULL,NULL,NULL,NULL);
    act_1 -> successors = (List) make_list(sink,NULL,NULL,NULL,NULL);
    sink -> predecessors = (List) make_list(act_1,NULL,NULL,NULL,NULL);
    source -> successors = (List) make_list(act_0,NULL,NULL,NULL,NULL);
    
       
    sink -> next = NULL;
    
    
    fail_unless(action_run(g,"act_0") == 1,"return value");
    fail_unless(STATE(source) != ACT_DONE, "source done");
    fail_unless(STATE(sink) != ACT_DONE, "sink done");
    fail_unless(STATE(act_0) == ACT_RUN,"action not run");
    fail_unless(STATE(act_1) == ACT_NONE,"next action not none");

}
END_TEST

    
START_TEST(test_action_run_selection)
{
   
   Graph g =(Graph) malloc (sizeof(struct graph));

   Node source,sink,act_0,act_1,sel,join;

   source = make_node("process",ACT_NONE,PROCESS,0);
   sink = make_node("process",ACT_NONE,PROCESS,5);
   act_0 = make_node("act_0",ACT_READY,ACTION,2);
   act_1 = make_node("act_1",ACT_READY,ACTION,3);
   sel = make_node("sel",ACT_NONE,SELECTION,1);
   join = make_node("sel",ACT_NONE,JOIN,4);

   source -> successors = (List) make_list(sel,NULL,NULL,NULL,NULL);
   sel -> successors = (List) make_list(act_0,act_1,NULL,NULL,NULL);
   sel -> predecessors = (List) make_list(source,NULL,NULL,NULL,NULL);
   act_0 -> predecessors = (List) make_list(sel,NULL,NULL,NULL,NULL);
   act_1 -> predecessors = (List) make_list(sel,NULL,NULL,NULL,NULL);
   act_0 -> successors = (List) make_list(join,NULL,NULL,NULL,NULL);
   act_1 -> successors = (List) make_list(join,NULL,NULL,NULL,NULL);
   join -> predecessors = (List) make_list(act_0,act_1,NULL,NULL,NULL);
   join -> successors = (List) make_list(sink,NULL,NULL,NULL,NULL);
   sink -> predecessors = (List) make_list(join,NULL,NULL,NULL,NULL);
		   
   
   g -> source = source;
   g -> sink = sink;
   
   source -> next = sel;
   sel -> next = act_0;
   act_0 -> next = act_1;
   act_1 -> next = join;
   join -> next = sink;
   sink -> next = NULL;

    sel -> matching = join;
    join -> matching = sel;

  
  fail_unless(action_run(g,act_0->name) == 1,"return value");
  fail_unless(STATE(act_0) == ACT_RUN, "action 0  not run");
  fail_unless(STATE(act_1) == ACT_NONE, "action 1 not set to none");
  fail_unless(STATE(sel) == ACT_RUN, "selection not run");
  fail_unless(STATE(join) == ACT_RUN, "join not run");

}
END_TEST


START_TEST(test_action_run_selection_recursive)
{
  Graph g =(Graph) malloc (sizeof(struct graph));
  
  Node source,sink,act_0,act_1,act_2,sel,join,sel1,join1;
  
  source = make_node("process",ACT_NONE,PROCESS,0);
  sink = make_node("process",ACT_NONE,PROCESS,8);
  act_0 = make_node("act_0",ACT_READY,ACTION,4);
  act_1 = make_node("act_1",ACT_READY,ACTION,5);
  act_2 = make_node("act_2",ACT_READY,ACTION,3);
  sel = make_node("sel",ACT_NONE,SELECTION,2);
  join = make_node("sel",ACT_NONE,JOIN,6);
  sel1 = make_node("sel1",ACT_NONE,SELECTION,1);
  join1 = make_node("sel1",ACT_NONE,SELECTION,7);

  source -> successors = (List) make_list(sel1,NULL,NULL,NULL,NULL);
  sel1 -> successors = (List) make_list(sel,act_2,NULL,NULL,NULL);
  sel1 -> predecessors = (List) make_list(source,NULL,NULL,NULL,NULL);
  sel -> successors = (List) make_list(act_0,act_1,NULL,NULL,NULL);
  sel -> predecessors = (List) make_list(sel1,NULL,NULL,NULL,NULL);
  act_2 -> predecessors = (List) make_list(sel1,NULL,NULL,NULL,NULL);
  act_2 -> successors = (List) make_list(join1,NULL,NULL,NULL,NULL);
  act_0 -> successors = (List) make_list(join,NULL,NULL,NULL,NULL);
  act_0 -> predecessors = (List) make_list(sel,NULL,NULL,NULL,NULL);
  act_1 -> successors = (List) make_list(join,NULL,NULL,NULL,NULL);
  act_1 -> predecessors = (List) make_list(sel,NULL,NULL,NULL,NULL);
  join -> successors = (List) make_list(join1,NULL,NULL,NULL,NULL);
  join -> predecessors = (List) make_list(act_0,act_1,NULL,NULL,NULL);
  join1 -> successors = (List) make_list(sink,NULL,NULL,NULL,NULL);
  join1 -> predecessors = (List) make_list(join,act_2,NULL,NULL,NULL);
  sink -> predecessors = (List) make_list(join1,NULL,NULL,NULL,NULL);
         
  g -> source = source;
  g -> sink = sink;
  source -> next = sel1;
  sel1 -> next = sel;
  sel -> next = act_2;
  act_2 -> next = act_0;
  act_0 -> next = act_1;
  act_1 -> next = join;
  join -> next = join1;
  join1 -> next = sink;
  sink -> next = NULL;
  
  sel -> matching = join;
  join -> matching = sel;

  sel1 -> matching = join1;
  join1 -> matching = sel1;

  fail_unless(action_run(g,act_0->name) == 1,"return value");
  fail_unless(STATE(act_0) == ACT_RUN, "action 0  not run");
  fail_unless(STATE(act_1) == ACT_NONE, "action 1 not none");
  fail_unless(STATE(act_2) == ACT_NONE, "action 2 not none");
  fail_unless(STATE(sel) == ACT_RUN,"sel not run");
  fail_unless(STATE(sel1) == ACT_RUN, "sel1 not run");
  fail_unless(STATE(join) == ACT_RUN, "join not run");
  fail_unless(STATE(join1) == ACT_RUN, "join1 not run");
  
}
END_TEST



START_TEST(test_propogate_join_done)
{
  Graph g =(Graph) malloc (sizeof(struct graph));
  Node source,sink,act_0,act_1,act_2,sel,join,sel1,join1;
  
  source = make_node("process",ACT_NONE,PROCESS,0);
  sink = make_node("process",ACT_NONE,PROCESS,8);
  act_0 = make_node("act_0",ACT_DONE,ACTION,4);
  act_1 = make_node("act_1",ACT_NONE,ACTION,5);
  act_2 = make_node("act_2",ACT_NONE,ACTION,3);
  sel = make_node("sel",ACT_RUN,SELECTION,2);
  join = make_node("sel",ACT_RUN,JOIN,6);
  sel1 = make_node("sel1",ACT_RUN,SELECTION,1);
  join1 = make_node("sel1",ACT_RUN,JOIN,7);
  

  source -> successors = (List) make_list(sel1,NULL,NULL,NULL,NULL);
  sel1 -> successors = (List) make_list(sel,act_2,NULL,NULL,NULL);
  sel1 -> predecessors = (List) make_list(source,NULL,NULL,NULL,NULL);
  sel -> successors = (List) make_list(act_0,act_1,NULL,NULL,NULL);
  sel -> predecessors = (List) make_list(sel1,NULL,NULL,NULL,NULL);
  act_2 -> predecessors = (List) make_list(sel1,NULL,NULL,NULL,NULL);
  act_2 -> successors = (List) make_list(join1,NULL,NULL,NULL,NULL);
  act_0 -> successors = (List) make_list(join,NULL,NULL,NULL,NULL);
  act_0 -> predecessors = (List) make_list(sel,NULL,NULL,NULL,NULL);
  act_1 -> successors = (List) make_list(join,NULL,NULL,NULL,NULL);
  act_1 -> predecessors = (List) make_list(sel,NULL,NULL,NULL,NULL);
  join -> successors = (List) make_list(join1,NULL,NULL,NULL,NULL);
  join -> predecessors = (List) make_list(act_0,act_1,NULL,NULL,NULL);
  join1 -> successors = (List) make_list(sink,NULL,NULL,NULL,NULL);
  join1 -> predecessors = (List) make_list(join,act_2,NULL,NULL,NULL);
  sink -> predecessors = (List) make_list(join1,NULL,NULL,NULL,NULL);
  
  
  g -> source = source;
  g -> sink = sink;
  source -> next = sel1;
  sel1 -> next = sel;
  sel -> next = act_2;
  act_2 -> next = act_0;
  act_0 -> next = act_1;
  act_1 -> next = join;
  join -> next = join1;
  join1 -> next = sink;
  sink -> next = NULL;

  sel -> matching = join;
  join -> matching = sel;
                                                                        
  sel1 -> matching = join1;
  join1 -> matching = sel1;
  
  act_1 -> next = join;
  join -> next = join1;
  join1 -> next = sink;
  sink -> next = NULL;
							      

  // Action 
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
   Graph g = (Graph) malloc (sizeof(struct graph));
   Node source,sink,bigbr,br,bigrn,rn,act_0,act_1,act_2;

   source = make_node("p",ACT_NONE,PROCESS,0);
   sink = make_node("p",ACT_NONE,PROCESS,8);
   act_0 = make_node("act_0",ACT_DONE,ACTION,4);
   act_1 = make_node("act_1",ACT_DONE,ACTION,5);
   act_2 = make_node("act_2",ACT_DONE,ACTION,2);
   bigbr = make_node("bigbr",ACT_RUN,BRANCH,1);
   br = make_node("br",ACT_RUN,BRANCH,3);
   bigrn = make_node("bigrn",ACT_RUN,RENDEZVOUS,7);
   rn = make_node("rn",ACT_RUN,RENDEZVOUS,6);


  source -> successors = (List) make_list(bigbr,NULL,NULL,NULL,NULL);
  bigbr -> successors = (List) make_list(br,act_2,NULL,NULL,NULL);
  bigbr -> predecessors = (List) make_list(source,NULL,NULL,NULL,NULL);
  br -> successors = (List) make_list(act_0,act_1,NULL,NULL,NULL);
  br -> predecessors = (List) make_list(bigbr,NULL,NULL,NULL,NULL);
  act_2 -> predecessors = (List) make_list(bigbr,NULL,NULL,NULL,NULL);
  act_2 -> successors = (List) make_list(bigrn,NULL,NULL,NULL,NULL);
  act_0 -> successors = (List) make_list(rn,NULL,NULL,NULL,NULL);
  act_0 -> predecessors = (List) make_list(br,NULL,NULL,NULL,NULL);
  act_1 -> successors = (List) make_list(rn,NULL,NULL,NULL,NULL);
  act_1 -> predecessors = (List) make_list(br,NULL,NULL,NULL,NULL);
  rn -> successors = (List) make_list(bigrn,NULL,NULL,NULL,NULL);
  rn -> predecessors = (List) make_list(act_0,act_1,NULL,NULL,NULL);
  bigrn -> successors = (List) make_list(sink,NULL,NULL,NULL,NULL);
  bigrn -> predecessors = (List) make_list(rn,act_2,NULL,NULL,NULL);
  sink -> predecessors = (List) make_list(bigrn,NULL,NULL,NULL,NULL);


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

  br -> matching = rn;
  rn -> matching = br;
  bigbr -> matching = bigrn;
  bigrn -> matching = bigbr;

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

	 source = make_node("p",ACT_NONE,PROCESS,0);
	 sink = make_node("p",ACT_NONE,PROCESS,2);
	 act_0 = make_node("act_0",ACT_DONE,ACTION,1);

	 source -> successors = (List) make_list(act_0,NULL,NULL,NULL,NULL);
	 act_0 -> successors = (List) make_list(sink,NULL,NULL,NULL,NULL);
	 act_0 -> predecessors = (List) make_list(source,NULL,NULL,NULL,NULL);
	 sink -> predecessors = (List) make_list(act_0,NULL,NULL,NULL,NULL);
	
	 source -> next = act_0;
	 act_0 -> next = sink;
	 sink -> next = NULL; 

	 g -> source = source;
	 g -> sink = sink;
	 source -> matching = sink;
	 sink -> matching = source;
	 

	 set_process_state(g);

	 fail_unless(STATE(source) == ACT_DONE,"source not done");
	 fail_unless(STATE(sink) == ACT_DONE,"sink not done");
	 fail_unless(STATE(act_0) == ACT_DONE,"act_0 not done");

}
END_TEST

START_TEST(test_action_run_iteration)
{
	Graph g = (Graph) malloc (sizeof(struct graph));
        Node source,sink,act_0,act_1,act_2;
                                                                        
        source = make_node("p",ACT_NONE,PROCESS,0);
        sink = make_node("p",ACT_NONE,PROCESS,4);
        act_0 = make_node("act_0",ACT_READY,ACTION,1);
	act_1 = make_node("act_1",ACT_NONE,ACTION,2);
	act_2 = make_node("act_2",ACT_READY,ACTION,3);

	source -> successors = (List) make_list(act_0,NULL,NULL,NULL,NULL);
	act_0 -> successors = (List) make_list(act_1,NULL,NULL,NULL,NULL);
	act_0 -> predecessors = (List) make_list(source,act_1,NULL,NULL,NULL);
	act_1 -> successors = (List) make_list(act_2,act_0,NULL,NULL,NULL);
	act_1 -> predecessors = (List) make_list(act_0,NULL,NULL,NULL,NULL);
	act_2 -> predecessors = (List) make_list(act_1,NULL,NULL,NULL,NULL);
	act_2 -> successors = (List) make_list(sink,NULL,NULL,NULL,NULL);
	sink -> predecessors = (List) make_list(act_2,NULL,NULL,NULL,NULL);

	ITER_END_NODES(act_0) = (List) make_list(act_2,NULL,NULL,NULL,NULL);
	ITER_START_NODES(act_2) = (List) make_list(act_0,NULL,NULL,NULL,NULL);

	source -> next = act_0;
	act_0 -> next = act_1;
	act_1 -> next = act_2;
	act_2 -> next = sink;
	sink -> next = NULL;
                                                                         
        g -> source = source;
        g -> sink = sink;
        source -> matching = sink;
        sink -> matching = source;
                                                                         
	
        fail_unless(action_run(g,act_2->name) == 1,"return value");
	fail_unless(STATE(act_0) == ACT_NONE, "act_0 not none");
	fail_unless(STATE(act_1) == ACT_NONE, "act_1 not none");
	fail_unless(STATE(act_2) == ACT_RUN, "act_2 not run");
}
END_TEST

START_TEST(test_mark_iter_nodes)
{
	Graph g = (Graph) malloc (sizeof(struct graph));
        Node source,sink,act_0,act_1,act_2;
                                                                        
        source = make_node("p",ACT_NONE,PROCESS,0);
        sink = make_node("p",ACT_NONE,PROCESS,4);
        act_0 = make_node("act_0",ACT_READY,ACTION,1);
	act_1 = make_node("act_1",ACT_NONE,ACTION,2);
	act_2 = make_node("act_2",ACT_NONE,ACTION,3);

	source -> successors = (List) make_list(act_0,NULL,NULL,NULL,NULL);
	act_0 -> successors = (List) make_list(act_1,NULL,NULL,NULL,NULL);
	act_0 -> predecessors = (List) make_list(source,act_1,NULL,NULL,NULL);
	act_1 -> successors = (List) make_list(act_2,act_0,NULL,NULL,NULL);
	act_1 -> predecessors = (List) make_list(act_0,NULL,NULL,NULL,NULL);
	act_2 -> predecessors = (List) make_list(act_1,NULL,NULL,NULL,NULL);
	act_2 -> successors = (List) make_list(sink,NULL,NULL,NULL,NULL);
	sink -> predecessors = (List) make_list(act_2,NULL,NULL,NULL,NULL);

	ITER_END_NODES(act_0) = (List) make_list(act_2,NULL,NULL,NULL,NULL);
	ITER_START_NODES(act_2) = (List) make_list(act_1,NULL,NULL,NULL,NULL);

	source -> next = act_0;
	act_0 -> next = act_1;
	act_1 -> next = act_2;
	act_2 -> next = sink;
	sink -> next = NULL;
                                                                         
        g -> source = source;
        g -> sink = sink;
        source -> matching = sink;
        sink -> matching = source;
                                                                         
	mark_iter_nodes(act_0);

	fail_unless(STATE(act_0) == ACT_READY, "act_0 not ready");
	fail_unless(STATE(act_1) == ACT_NONE, "act_1 not none");
	fail_unless(STATE(act_2) == ACT_READY, "act_2 not ready");
}
END_TEST



// Test growing resource list. 
START_TEST(test_insert_resource)
{
    int num_resources = 0, rsize = 2, i, num_expected;
    char rname[32];
   // Pre: a possibly empty resource list exists. 
    peos_resource_t *resource_list = (peos_resource_t *) calloc(rsize, sizeof(peos_resource_t));

    // Action: insert a number of resources into the list. 
    num_expected = (32 * rsize) + 1;
    for (i = 0; i < num_expected; i++) {
	sprintf(rname, "%s_%d", "resource", i);
	insert_resource(rname, &resource_list, &num_resources, &rsize);
    }

    // Post: all resources have been inserted in order. 
    fail_unless(num_resources == num_expected, "num_resources wrong");
    for (i = 0; i < num_expected; i++) {
	sprintf(rname, "%s_%d", "resource", i);
	fail_unless(strcmp(resource_list[i].name, rname) == 0, "resource name wrong");
    }
}	
END_TEST

START_TEST(test_make_resource_list)
{
	int num_resources = 0, rsize = 256;
	char *ptr = "y";
	peos_resource_t *resource_list = (peos_resource_t *) calloc(rsize,sizeof(peos_resource_t));
	
	Tree t1 = make_tree("y",0,NULL,NULL);
	Tree t2 = make_tree("modified",0,NULL,NULL);
	Tree t3 = make_tree(NULL,DOT,t1,t2);
	Tree t4 = make_tree("true",0,NULL,NULL);
	Tree t5 = make_tree(NULL,EQ,t3,t4);

	make_resource_list(t5, &resource_list, &num_resources, &rsize);

	fail_unless(num_resources == 1,"num_resources wrong");
    fail_unless(strcmp(resource_list[0].name,ptr) == 0,"resource name wrong");

}	
END_TEST


// Test growing resource list. 
START_TEST(test_make_resource_list_realloc)
{
    int num_resources = 0, rsize = 2, i, num_expected;
    char rname[32];
    peos_resource_t *resource_list = (peos_resource_t *) calloc(rsize,sizeof(peos_resource_t));
    Tree t[256];

    num_expected = (32 * rsize) + 1;
    for (i = 0; i < num_expected; i++) {
	sprintf(rname, "%s_%d", "resource", i);
	t[i] = make_tree(strdup(rname), 0, NULL, NULL);
    }

    for (i = 0; i < num_expected; i++) {
	make_resource_list(t[i], &resource_list, &num_resources, &rsize);
    }

    fail_unless(num_resources == num_expected, "num_resources wrong");
    for (i = 0; i < num_expected; i++) {
	sprintf(rname, "%s_%d", "resource", i);
	fail_unless(strcmp(resource_list[i].name, rname) == 0, "resource name wrong");
    }
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

	source -> successors = (List) make_list(act_0,NULL,NULL,NULL,NULL);
	source -> predecessors = (List) make_list(NULL,NULL,NULL,NULL,NULL);
	act_0 -> predecessors = (List) make_list(source,NULL,NULL,NULL,NULL);
	act_0 -> successors = (List) make_list(act_1,NULL,NULL,NULL,NULL);
        act_1 -> successors = (List) make_list(sink,NULL,NULL,NULL,NULL);
        act_1 -> predecessors = (List) make_list(act_0,NULL,NULL,NULL,NULL);
	sink -> predecessors = (List) make_list(act_1,NULL,NULL,NULL,NULL);
	sink -> successors = (List) make_list(NULL,NULL,NULL,NULL,NULL);

		 
	
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

		
	initialize_graph(g);

	fail_unless(STATE(act_0) == ACT_READY, "act 0 not ready");
	fail_unless(STATE(act_1) == ACT_NONE, "act 1 not none");
	fail_unless(ORDER(source) == 0, "source order");
	fail_unless(ORDER(act_0) == 1, "act_0 order");
	fail_unless(ORDER(act_1) == 2, "act_1 order");
	fail_unless(ORDER(sink) == 3, "sink order");
	fail_unless(MARKED_0(act_0) == FALSE, "act 0 not marked false");
	fail_unless(MARKED_1(act_0) == FALSE, "act 0 not marked false");
	fail_unless(MARKED_2(act_0) == FALSE, "act 0 not marked false");
	fail_unless(MARKED_3(act_0) == FALSE, "act 0 not marked false");
	fail_unless(MARKED_4(act_0) == FALSE, "act 0 not marked false");
	fail_unless(MARKED_0(act_1) == FALSE, "act 1 not marked false");
	fail_unless(MARKED_1(act_1) == FALSE, "act 0 not marked false");
	fail_unless(MARKED_2(act_1) == FALSE, "act 0 not marked false");
	fail_unless(MARKED_3(act_1) == FALSE, "act 0 not marked false");
	fail_unless(MARKED_4(act_1) == FALSE, "act 0 not marked false");


}
END_TEST


START_TEST(test_annotate_graph)
{
	int i;
	int num_actions,num_other_nodes;
	peos_action_t *actions;
	peos_other_node_t *other_nodes;
	
	Graph g = (Graph) malloc (sizeof (struct graph));
	Node source,sink,act_0,act_1,sel,branch,join,rendezvous;
	num_actions = 2;
	actions = (peos_action_t *) calloc(num_actions, sizeof(peos_action_t));

	for(i = 0; i < num_actions ; i++)
	{
		sprintf(actions[i].name,"act_%d",i);
		actions[i].state = ACT_RUN;
	}

        num_other_nodes = 2;
        other_nodes = (peos_other_node_t *) calloc(num_other_nodes, sizeof(peos_other_node_t));
                                                                       
          sprintf(other_nodes[0].name,"sel");
	  sprintf(other_nodes[1].name,"br");
          other_nodes[0].state = ACT_RUN;
	  other_nodes[1].state = ACT_READY;
     
	source = make_node("p",ACT_NONE,PROCESS,0);
	sink = make_node("p",ACT_NONE,PROCESS,0);
	act_0 = make_node("act_0",ACT_NONE,ACTION,0);
	act_1 = make_node("act_1",ACT_NONE,ACTION,0);
	sel = make_node("sel",ACT_NONE,SELECTION,0);
	join = make_node("sel",ACT_NONE,JOIN,0);
	branch = make_node("br",ACT_NONE,BRANCH,0);
	rendezvous = make_node("br",ACT_NONE,RENDEZVOUS,0);

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

	fail_unless(annotate_graph(g,actions,num_actions,other_nodes,num_other_nodes) == 1, "return value");
	fail_unless(STATE(act_0) == ACT_RUN, "act 0 state not run");
	fail_unless(STATE(act_1) == ACT_RUN, "act 1 state not run");
	fail_unless(STATE(sel) == ACT_RUN, "sel not run");
	fail_unless(STATE(join) == ACT_RUN, "join not run");
	fail_unless(STATE(branch) == ACT_READY, "branch not ready");
       fail_unless(STATE(rendezvous) == ACT_READY, "rendezvous not run");

}
END_TEST




START_TEST(test_set_act_state_graph_ready)
{
	Graph g = (Graph) malloc (sizeof(struct graph));
	Node source,sink,act_0,act_1;

	source = make_node("p",ACT_NONE,PROCESS,0);
	sink = make_node("p",ACT_NONE,PROCESS,3);
	act_0 = make_node("act_0",ACT_NONE,ACTION,1);
	act_1 = make_node("act_1",ACT_NONE,ACTION,2);


	g -> source = source;
	g -> sink = sink;

	source -> next = act_0;
	act_0 -> next = act_1;
	act_1 -> next = sink;
	sink -> next = NULL;


    fail_unless(set_act_state_graph(g,"act_0",ACT_READY) == VM_CONTINUE, "return value");
    fail_unless(STATE(act_0) == ACT_READY, "act 0 state not changed");

}
END_TEST


START_TEST(test_set_act_state_graph_suspend)
{
	Graph g = (Graph) malloc (sizeof(struct graph));
	Node source,sink,act_0,act_1;

	source = make_node("p",ACT_NONE,PROCESS,0);
	sink = make_node("p",ACT_NONE,PROCESS,3);
	act_0 = make_node("act_0",ACT_RUN,ACTION,1);
	act_1 = make_node("act_1",ACT_RUN,ACTION,2);


	g -> source = source;
	g -> sink = sink;

	source -> next = act_0;
	act_0 -> next = act_1;
	act_1 -> next = sink;
	sink -> next = NULL;



    fail_unless(set_act_state_graph(g,"act_0",ACT_SUSPEND) == VM_CONTINUE, "return value");
    fail_unless(STATE(act_0) == ACT_SUSPEND, "act 0 state not changed");

}
END_TEST


START_TEST(test_set_act_state_graph_none)
{
	Graph g = (Graph) malloc (sizeof(struct graph));
	Node source,sink,act_0,act_1;

	source = make_node("p",ACT_NONE,PROCESS,0);
	sink = make_node("p",ACT_NONE,PROCESS,3);
	act_0 = make_node("act_0",ACT_RUN,ACTION,1);
	act_1 = make_node("act_1",ACT_RUN,ACTION,2);


	g -> source = source;
	g -> sink = sink;

	source -> next = act_0;
	act_0 -> next = act_1;
	act_1 -> next = sink;
	sink -> next = NULL;


    fail_unless(set_act_state_graph(g,"act_0",ACT_NONE) == VM_CONTINUE, "return value");
    fail_unless(STATE(act_0) == ACT_NONE, "act 0 state not changed");

}
END_TEST



START_TEST(test_set_act_state_graph_abort)
{
	Graph g = (Graph) malloc (sizeof(struct graph));
	Node source,sink,act_0,act_1;

	source = make_node("p",ACT_NONE,PROCESS,0);
	sink = make_node("p",ACT_NONE,PROCESS,3);
	act_0 = make_node("act_0",ACT_RUN,ACTION,1);
	act_1 = make_node("act_1",ACT_RUN,ACTION,2);


	g -> source = source;
	g -> sink = sink;

	source -> next = act_0;
	act_0 -> next = act_1;
	act_1 -> next = sink;
	sink -> next = NULL;


    fail_unless(set_act_state_graph(g,"act_0",ACT_ABORT) == VM_CONTINUE, "return value");
    fail_unless(STATE(act_0) == ACT_ABORT, "act 0 state not changed");

}
END_TEST

START_TEST(test_set_act_state_graph_new)
{
	Graph g = (Graph) malloc (sizeof(struct graph));
	Node source,sink,act_0,act_1;

	source = make_node("p",ACT_NONE,PROCESS,0);
	sink = make_node("p",ACT_NONE,PROCESS,3);
	act_0 = make_node("act_0",ACT_RUN,ACTION,1);
	act_1 = make_node("act_1",ACT_RUN,ACTION,2);


	g -> source = source;
	g -> sink = sink;

	source -> next = act_0;
	act_0 -> next = act_1;
	act_1 -> next = sink;
	sink -> next = NULL;


    fail_unless(set_act_state_graph(g,"act_0",ACT_NEW) == VM_CONTINUE, "return value");
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
    tcase_add_test(tc, test_find_node_error);

    tc = tcase_create("makegraph");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_makegraph_nofile);
    tcase_add_test(tc,test_makegraph_error);

    tc = tcase_create("mark_successors");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_mark_successors);

    tc = tcase_create("make resource list");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_insert_resource);
    tcase_add_test(tc,test_make_resource_list);
    tcase_add_test(tc,test_make_resource_list_realloc);
 
    tc = tcase_create("action done");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_action_done);
    tcase_add_test(tc,test_propogate_join_done);

    tc = tcase_create("set_rendezvous_state");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_set_rendezvous_state);
   
    tc = tcase_create("action run");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_action_run);
    tcase_add_test(tc,test_action_run_selection);
    tcase_add_test(tc,test_action_run_selection_recursive);
    tcase_add_test(tc,test_action_run_iteration);

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

