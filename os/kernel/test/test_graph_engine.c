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


/* Globals. */

char *login_name = "jnoll";
peos_context_t process_table[PEOS_MAX_PID+1];
Graph global_graph = NULL;
int requires_state[5];
int provides_state[5];
int requires_index = 0;
int provides_index = 0;

/* Stubs. */
extern Node make_node(char *name, vm_act_state, int type,int order);
extern Tree make_tree(char *,int, Tree,Tree);
extern Tree make_dot_tree(char *name, char* attr);
extern Tree make_op_tree(Tree left, int op, char* value);
extern Tree make_con_tree(Tree left, Tree right, int op, char* value);
extern List make_list(Item i1, Item i2, Item i3, Item i4, Item i5);


peos_context_t *peos_get_context(int pid)
{
	process_table[0].process_graph = global_graph;
	return pid == 0 ? &(process_table[pid]) : NULL;
}

char *act_state_name(vm_act_state state)
{
    return "READY";
}

int is_requires_true(int pid, char *act_name)
{
	int value = requires_state[++requires_index];
    return value;
}

int is_provides_true(int pid, char *act_name)
{
	int value = provides_state[++provides_index];
   return value;
}

void log_event(char *msg)
{
    return;
}

int delete_entry(int pid)
{
    return 1;
}

START_TEST(test_handle_resource_event_1)
{
    
    Graph g = (Graph) malloc(sizeof(struct graph));
    
    Node source,sink,act_0,act_1;
    source = make_node("p",ACT_NONE,PROCESS,0);
    sink = make_node("p",ACT_NONE,PROCESS,3);

    act_0 = make_node("act_0",ACT_BLOCKED,ACTION,1);
    act_1 = make_node("act_1",ACT_NONE,ACTION,2);

    requires_index = 0;
    requires_state[1] = TRUE;

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

    global_graph = g;

    fail_unless(handle_resource_event(0,"act_0",REQUIRES_TRUE) == VM_CONTINUE, "Return Value");
    
    fail_unless(STATE(act_0) == ACT_READY, "act_0 not ready");
    fail_unless(STATE(act_1) == ACT_NONE, "act_1 not none");
    
}
END_TEST
    
		    

START_TEST(test_handle_resource_event_2)
{
    
    Graph g = (Graph) malloc(sizeof(struct graph));
    
    Node source,sink,act_0,act_1;
    source = make_node("p",ACT_NONE,PROCESS,0);
    sink = make_node("p",ACT_NONE,PROCESS,3);

    act_0 = make_node("act_0",ACT_READY,ACTION,1);
    act_1 = make_node("act_1",ACT_NONE,ACTION,2);

    requires_index = 0;
    provides_index = 0;
    requires_state[1] = FALSE;
    provides_state[1] = TRUE;

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

    global_graph = g;


    fail_unless(handle_resource_event(0,"act_0",PROVIDES_TRUE) == VM_CONTINUE, "Return Value");
    
    fail_unless(STATE(act_0) == ACT_READY, "act_0  done");
    fail_unless(STATE(act_1) == ACT_NONE, "act_1 not none");
    
}
END_TEST


START_TEST(test_handle_resource_event_3)
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

    global_graph = g;

    fail_unless(handle_resource_event(0,"act_0",REQUIRES_TRUE) == VM_CONTINUE, "Return Value");
    
    fail_unless(STATE(act_0) == ACT_AVAILABLE, "act_0 not available");
    fail_unless(STATE(act_1) == ACT_NONE, "act_1 not none");
}
END_TEST


START_TEST(test_handle_resource_change_1)
{
    
    Graph g = (Graph) malloc(sizeof(struct graph));
    
    Node source,sink,act_0,act_1;
    source = make_node("p",ACT_NONE,PROCESS,0);
    sink = make_node("p",ACT_NONE,PROCESS,3);

    act_0 = make_node("act_0",ACT_PENDING,ACTION,1);
    act_1 = make_node("act_1",ACT_BLOCKED,ACTION,2);

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

    requires_index = 0;
    provides_index = 0;

    requires_state[1] = TRUE;
    provides_state[1] = TRUE;

    requires_state[2] = TRUE;
    requires_state[3] = TRUE;
    provides_state[2] = TRUE;
    provides_state[3] = FALSE;

    global_graph = g;

    
    fail_unless(handle_resource_change(0) == VM_CONTINUE, "Return Value");
    fail_unless(STATE(act_0) == ACT_DONE, "act_0 not done ");
    fail_unless(STATE(act_1) == ACT_READY, "act_1 not ready");
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
	
	requires_index = 0;
	
	requires_state[1] = TRUE;
	requires_state[2] = TRUE;

        s -> matching = j;
        j -> matching = s;
	s -> successors = (List) make_list(a,b,NULL,NULL,NULL);

	SUPER_NODES(a) = (List) make_list(s,NULL,NULL,NULL,NULL);
	SUPER_NODES(b) = (List) make_list(s,NULL,NULL,NULL,NULL);
	
        mark_successors(s,ACT_READY);

	/* Post */
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

    requires_index = 0;
    provides_index = 0;
    requires_state[1] = TRUE;
    requires_state[2] = TRUE;
    provides_state[1] = TRUE;
    

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

START_TEST(test_action_done_selection)
{

   Graph g =(Graph) malloc (sizeof(struct graph));

   Node source,sink,act_0,act_1,sel,join;

   source = make_node("process",ACT_NONE,PROCESS,0);
   sink = make_node("process",ACT_NONE,PROCESS,5);
   act_0 = make_node("act_0",ACT_RUN,ACTION,2);
   act_1 = make_node("act_1",ACT_NONE,ACTION,3);
   sel = make_node("sel",ACT_RUN,SELECTION,1);
   join = make_node("sel",ACT_RUN,JOIN,4);

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

   SUPER_NODES(act_0) = (List) make_list(sel,NULL,NULL,NULL,NULL);
   
   SUPER_NODES(act_1) = (List) make_list(sel,NULL,NULL,NULL,NULL);

   requires_index = 0;
   requires_state[1] = TRUE;
   provides_state[1] = FALSE;
   
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

  
  fail_unless(action_done(g,act_0->name) == VM_CONTINUE,"return value");
  fail_unless(STATE(act_0) == ACT_PENDING, "action 0  not pending");
  fail_unless(STATE(sink) != ACT_DONE, "process done");
  fail_unless(STATE(sel) != ACT_PENDING, "selection not pending");
  fail_unless(STATE(join) != ACT_PENDING, "join not pending");
  
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


START_TEST(test_action_2runs)
{

    Graph g = (Graph) malloc(sizeof(struct graph));
    
    Node source,sink,act_0,act_1;
    source = make_node("p",ACT_NONE,PROCESS,0);
    sink = make_node("p",ACT_NONE,PROCESS,3);

    act_0 = make_node("act_0",ACT_RUN,ACTION,1);
    act_1 = make_node("act_1",ACT_READY,ACTION,2);

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
    
    
    fail_unless(action_run(g,"act_1") == 1,"return value");
    fail_unless(STATE(source) != ACT_DONE, "source done");
    fail_unless(STATE(sink) != ACT_DONE, "sink done");
    fail_unless(STATE(act_0) == ACT_SUSPEND,"action 0 not suspend");
    fail_unless(STATE(act_1) == ACT_RUN,"act_1  not run");

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

   SUPER_NODES(act_0) = (List) make_list(sel,NULL,NULL,NULL,NULL);
   
   SUPER_NODES(act_1) = (List) make_list(sel,NULL,NULL,NULL,NULL);
   
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


START_TEST(test_action_run_iter_sel)
{
   Graph g =(Graph) malloc (sizeof(struct graph));
   Node source,sink,act_0,act_1,act_2,sel,join;

   source = make_node("process",ACT_NONE,PROCESS,0);
   sink = make_node("process",ACT_NONE,PROCESS,6);
   act_0 = make_node("act_0",ACT_READY,ACTION,2);
   act_1 = make_node("act_1",ACT_READY,ACTION,3);
   act_2 = make_node("act_2",ACT_READY,ACTION,5);
   sel = make_node("sel",ACT_NONE,SELECTION,1);
   join = make_node("sel",ACT_NONE,JOIN,4);

   source -> successors = (List) make_list(sel,NULL,NULL,NULL,NULL);
   sel -> successors = (List) make_list(act_0,act_1,NULL,NULL,NULL);
   sel -> predecessors = (List) make_list(source,join,NULL,NULL,NULL);
   act_0 -> predecessors = (List) make_list(sel,NULL,NULL,NULL,NULL);
   act_1 -> predecessors = (List) make_list(sel,NULL,NULL,NULL,NULL);
   act_0 -> successors = (List) make_list(join,NULL,NULL,NULL,NULL);
   act_1 -> successors = (List) make_list(join,NULL,NULL,NULL,NULL);

   join -> predecessors = (List) make_list(act_0,act_1,NULL,NULL,NULL);
   join -> successors = (List) make_list(sel,act_2,NULL,NULL,NULL);
   act_2 -> successors = (List) make_list(sink,NULL,NULL,NULL,NULL);
   act_2 -> predecessors = (List) make_list(join,NULL,NULL,NULL,NULL);
   sink -> predecessors = (List) make_list(act_2,NULL,NULL,NULL,NULL);

   ITER_START_NODES(act_2) = (List) make_list(sel,NULL,NULL,NULL,NULL);
   ITER_END_NODES(sel) = (List) make_list(act_2,NULL,NULL,NULL,NULL);
   SUPER_NODES(act_0) = (List) make_list(sel,NULL,NULL,NULL,NULL);
   SUPER_NODES(act_1) = (List) make_list(sel,NULL,NULL,NULL,NULL);
   
   g -> source = source;
   g -> sink = sink;
   
   source -> next = sel;
   sel -> next = act_0;
   act_0 -> next = act_1;
   act_1 -> next = join;
   join -> next = act_2;
   act_2 -> next = sink;
   sink -> next = NULL;

   sel -> matching = join;
   join -> matching = sel;

  fail_unless(action_run(g,act_2->name) == 1,"return value");
  fail_unless(STATE(act_0) == ACT_NONE, "action 0  not none");
  fail_unless(STATE(act_1) == ACT_NONE, "action 1 not set to none");
  fail_unless(STATE(sel) == ACT_NONE, "selection not none");
  fail_unless(STATE(join) == ACT_NONE, "join not none");
  fail_unless(STATE(act_2) == ACT_RUN, "act_2 not run");
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

  SUPER_NODES(act_0) = (List) make_list(sel,sel1,NULL,NULL,NULL);
  SUPER_NODES(act_1) = (List) make_list(sel,sel1,NULL,NULL,NULL); 
  SUPER_NODES(act_2) = (List) make_list(sel1,NULL,NULL,NULL,NULL);
         
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
							      

  /* Action  */
   propogate_join_done(join,ACT_DONE); 

   /* Post */
   
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


START_TEST(test_set_process_state_iter)
{
	Graph g = (Graph) malloc (sizeof(struct graph));
        Node source,sink,act_0,act_1;
                                                                        
        source = make_node("p",ACT_NONE,PROCESS,0);
        sink = make_node("p",ACT_NONE,PROCESS,4);
        act_0 = make_node("act_0",ACT_READY,ACTION,1);
	act_1 = make_node("act_1",ACT_NONE,ACTION,2);

	source -> successors = (List) make_list(act_0,NULL,NULL,NULL,NULL);
	act_0 -> successors = (List) make_list(act_1,NULL,NULL,NULL,NULL);
	act_0 -> predecessors = (List) make_list(source,act_1,NULL,NULL,NULL);
	act_1 -> successors = (List) make_list(sink,act_0,NULL,NULL,NULL);
	act_1 -> predecessors = (List) make_list(act_0,NULL,NULL,NULL,NULL);
	sink -> predecessors = (List) make_list(act_1,NULL,NULL,NULL,NULL);

	ITER_END_NODES(act_0) = (List) make_list(sink,NULL,NULL,NULL,NULL);
	ITER_START_NODES(sink) = (List) make_list(act_0,NULL,NULL,NULL,NULL);

	source -> next = act_0;
	act_0 -> next = act_1;
	act_1 -> next = sink;
	sink -> next = NULL;
                                                                         
        g -> source = source;
        g -> sink = sink;
        source -> matching = sink;
        sink -> matching = source;

	set_process_state(g);
	
        fail_unless(STATE(source) != ACT_DONE, "process source done");
	fail_unless(STATE(sink) != ACT_DONE, "process sink done");
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


START_TEST(test_action_done_iteration)
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

	requires_index = 0;
	provides_index = 0;

	requires_state[1] = 1;
	requires_state[2] = 1;
	provides_state[1] = 1;
	
                                                                         
	
        fail_unless(action_done(g,act_0->name) == VM_CONTINUE,"return value");
	fail_unless(STATE(act_0) == ACT_DONE, "act_0 not done");
	fail_unless(STATE(act_1) == ACT_READY, "act_1 not ready");
	fail_unless(STATE(act_2) == ACT_NONE, "act_2 not none");
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

	requires_index = 0;
	requires_state[1] = TRUE;

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
	
	requires_index = 0;
        requires_state[1] = FALSE;	
	
	initialize_graph(g,1);

	fail_unless(STATE(act_0) == ACT_BLOCKED, "act 0 not blocked");
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
	fail_unless(PID(source) = 1, "wrong pid");
	fail_unless(PID(sink) = 1, "wrong pid");
	fail_unless(PID(act_0) = 1, "wrong pid");
	fail_unless(PID(act_1) = 1, "wrong pid");	


}
END_TEST


START_TEST(test_set_node_state_ready)
{
 
   Node n = make_node("act",ACT_NONE,ACTION,0);

   requires_index = 0;
   provides_index = 0;
   requires_state[1] = TRUE;
   provides_state[1] = TRUE;

   fail_unless(set_node_state(n, ACT_READY) == ACT_READY, "return value");
   fail_unless(STATE(n) == ACT_READY, "act not ready");
}
END_TEST
		   

START_TEST(test_set_node_state_ready_1)
{
 
   Node n = make_node("act",ACT_NONE,ACTION,0);

   requires_index = 0;
   provides_index = 0;

   requires_state[1] = TRUE;
   provides_state[1] = FALSE;
 
   fail_unless(set_node_state(n, ACT_READY) == ACT_READY, "return value");
   fail_unless(STATE(n) == ACT_READY, "act not ready");
}
END_TEST
   

START_TEST(test_set_node_state_ready_2)
{
 
   Node n = make_node("act",ACT_NONE,ACTION,0);

   requires_index = 0;
   provides_index = 0;
   requires_state[1] = FALSE;
   provides_state[1] = FALSE;

   fail_unless(set_node_state(n, ACT_READY) == ACT_BLOCKED, "return value");
   fail_unless(STATE(n) == ACT_BLOCKED, "act not blocked");
}
END_TEST


START_TEST(test_set_node_state_done)
{
 
   Node n = make_node("act",ACT_NONE,ACTION,0);

   provides_index = 0;
   provides_state[1] = TRUE;

   fail_unless(set_node_state(n, ACT_DONE) == ACT_DONE, "return value");
   fail_unless(STATE(n) == ACT_DONE, "act not done");
}
END_TEST


START_TEST(test_set_node_state_done_1)
{
 
   Node n = make_node("act",ACT_NONE,ACTION,0);

   provides_index = 0;
   provides_state[1] = FALSE;
   
   fail_unless(set_node_state(n, ACT_DONE) == ACT_PENDING, "return value");
   fail_unless(STATE(n) == ACT_PENDING, "act not pending");
}
END_TEST


START_TEST(test_set_node_state_none)
{
 
   Node n = make_node("act",ACT_READY,ACTION,0);

   fail_unless(set_node_state(n, ACT_NONE) == ACT_NONE, "return value");
   fail_unless(STATE(n) == ACT_NONE, "act not none");
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

	requires_index = 0;
	requires_state[1] = TRUE;


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
	PID(act_0) = 0;
	PID(act_1) = 0;

	g -> source = source;
	g -> sink = sink;

	source -> next = act_0;
	act_0 -> next = act_1;
	act_1 -> next = sink;
	sink -> next = NULL;

	global_graph = g;

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

    tc = tcase_create("mark_successors");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_mark_successors);
 
    tc = tcase_create("action done");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_action_done);
    tcase_add_test(tc, test_action_done_selection);
    tcase_add_test(tc,test_propogate_join_done);
    tcase_add_test(tc,test_action_done_iteration);

    tc = tcase_create("set_rendezvous_state");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_set_rendezvous_state);
   
    tc = tcase_create("action run");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_action_run);
    tcase_add_test(tc,test_action_run_selection);
    tcase_add_test(tc,test_action_run_selection_recursive);
    tcase_add_test(tc,test_action_run_iteration);
    tcase_add_test(tc,test_action_run_iter_sel);
    tcase_add_test(tc,test_action_2runs);

    tc = tcase_create("set_process_state");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_set_process_state);
    tcase_add_test(tc,test_set_process_state_iter);
    
    

    tc = tcase_create("make_iter_nodes");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_mark_iter_nodes);

    tc = tcase_create("test node state");
    suite_add_tcase(s,tc);
    tcase_add_test(tc, test_set_node_state_ready);
    tcase_add_test(tc, test_set_node_state_ready_1);
    tcase_add_test(tc, test_set_node_state_ready_2);
    tcase_add_test(tc, test_set_node_state_done);
    tcase_add_test(tc, test_set_node_state_done_1);
    tcase_add_test(tc, test_set_node_state_none);

    tc = tcase_create("test handle resource event");
    suite_add_tcase(s,tc);
    tcase_add_test(tc, test_handle_resource_event_1);
    tcase_add_test(tc, test_handle_resource_event_2);
    tcase_add_test(tc, test_handle_resource_event_3);


    tc = tcase_create("test handle resource change");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_handle_resource_change_1);
    
    
    tc = tcase_create("initialize graph");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_initialize_graph);

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
    return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

