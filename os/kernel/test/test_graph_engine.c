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


/* Globals. */
int lineno = 1;
FILE *yyin ;
Graph program;
char *filename = "some file";
int cur_node = 1;
Node node_list[10];
int lengths[10];
peos_context_t process_table[PEOS_MAX_PID+1];



/* stubs */

Item ListIndex(List l , int index)
{
	return node_list[cur_node++];
}

int ListSize(List l)
{
	return lengths[cur_node];
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
	fail_unless(strcmp(n->name,"act_0") == 0, "node not found");

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

START_TEST(test_mark_successors)
{
	cur_node = 1;
 node_list[0] = make_node("p",ACT_NONE,PROCESS);
 node_list[1] = make_node("s", ACT_NONE,SELECTION);
 node_list[2] = make_node("a",ACT_NONE,ACTION);
 node_list[3] = make_node("b",ACT_NONE,ACTION);
 node_list[4] = make_node("j",ACT_NONE,JOIN);
 node_list[5] = make_node("p",ACT_RUN,PROCESS);
 node_list[6] = NULL;

 lengths[0] = 1;
 lengths[1] = 2;
 lengths[2] = 1;
 lengths[3] = 1;
 lengths[4] = 1;
 lengths[5] = 0;

 mark_successors(node_list[1],ACT_RUN);

 // Post

   fail_unless(STATE(node_list[2]) == ACT_RUN,"action not ready");
   fail_unless(STATE(node_list[3]) == ACT_RUN, "action not ready");


}
END_TEST

START_TEST(test_action_done)
{

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

	cur_node = 2;

	node_list[0] = source;
	node_list[1] = act_0;
	node_list[2] = act_1;
	node_list[3] = sink;
	node_list[4] = NULL;

	lengths[0] = 1;
	lengths[1] = 1;
	lengths[2] = 1;
	lengths[3] = 1;
	lengths[4] = 1;

	fail_unless(action_done(g,act_name) == 1,"return value");
	fail_unless(STATE(act_0) == ACT_DONE,"action not done");
	fail_unless(STATE(act_1) == ACT_READY,"next action not ready");

}
END_TEST


START_TEST(test_action_run)
{
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

	cur_node = 2;

	node_list[0] = source;
	node_list[1] = act_0;
	node_list[2] = act_1;
	node_list[3] = sink;
	node_list[4] = NULL;

	lengths[0] = 1;
	lengths[1] = 1;
	lengths[2] = 1;
	lengths[3] = 1;
	lengths[4] = 1;

        fail_unless(action_run(g,act_0->name) == 1,"return value");

        fail_unless(STATE(act_0) == ACT_RUN, "action state not run");

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


        source -> next = act_0;
	act_0 -> next = act_1;
	act_1 -> next = sink;
	sink -> next = NULL;


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
	Node source,sink,act_0,act_1;
	peos_context_t *context = &(process_table[0]);
	context->num_actions = 2;
	context->actions = (peos_action_t *) calloc(context->num_actions, sizeof(peos_action_t));

	for(i = 0; i < context->num_actions ; i++)
	{
		sprintf(context->actions[i].name,"act_%d",i);
		context->actions[i].state = ACT_RUN;
	}
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

	fail_unless(annotate_graph(g,context) == 1, "return value");
	fail_unless(STATE(act_0) == ACT_RUN, "act 0 state not run");
	fail_unless(STATE(act_1) == ACT_RUN, "act 1 state not run");

}
END_TEST

START_TEST(test_update_context)
{
	int i;
	Graph g = (Graph) malloc (sizeof (struct graph));
	Node source,sink,act_0,act_1;
	peos_context_t *context = &(process_table[0]);

	context -> num_actions = 2;
	context -> actions = (peos_action_t *) calloc(context->num_actions, sizeof(peos_action_t));

	for(i = 0; i < context -> num_actions ; i++)
	{
		sprintf(context->actions[i].name,"act_%d",i);
		context->actions[i].state = ACT_NONE;
	}

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

        fail_unless(update_context(g,context) == 1, "return value");
	fail_unless(context->actions[0].state == ACT_RUN, "act 0 state not run");
	fail_unless(context->actions[1].state == ACT_RUN, "act 1 state not run");


}
END_TEST

START_TEST(test_set_act_state_graph_run)
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

	cur_node = 2;

	node_list[0] = source;
	node_list[1] = act_0;
	node_list[2] = act_1;
	node_list[3] = sink;
	node_list[4] = NULL;

	lengths[0] = 1;
	lengths[1] = 1;
	lengths[2] = 1;
	lengths[3] = 1;
	lengths[4] = 1;

    fail_unless(set_act_state_graph(g,"act_0",ACT_RUN) == 1, "return value");
    fail_unless(STATE(act_0) == ACT_RUN, "act 0 state not changed");

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

	cur_node = 2;

	node_list[0] = source;
	node_list[1] = act_0;
	node_list[2] = act_1;
	node_list[3] = sink;
	node_list[4] = NULL;

	lengths[0] = 1;
	lengths[1] = 1;
	lengths[2] = 1;
	lengths[3] = 1;
	lengths[4] = 1;

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

	cur_node = 2;

	node_list[0] = source;
	node_list[1] = act_0;
	node_list[2] = act_1;
	node_list[3] = sink;
	node_list[4] = NULL;

	lengths[0] = 1;
	lengths[1] = 1;
	lengths[2] = 1;
	lengths[3] = 1;
	lengths[4] = 1;

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

	cur_node = 2;

	node_list[0] = source;
	node_list[1] = act_0;
	node_list[2] = act_1;
	node_list[3] = sink;
	node_list[4] = NULL;

	lengths[0] = 1;
	lengths[1] = 1;
	lengths[2] = 1;
	lengths[3] = 1;
	lengths[4] = 1;

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

	cur_node = 2;

	node_list[0] = source;
	node_list[1] = act_0;
	node_list[2] = act_1;
	node_list[3] = sink;
	node_list[4] = NULL;

	lengths[0] = 1;
	lengths[1] = 1;
	lengths[2] = 1;
	lengths[3] = 1;
	lengths[4] = 1;

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

	cur_node = 2;

	node_list[0] = source;
	node_list[1] = act_0;
	node_list[2] = act_1;
	node_list[3] = sink;
	node_list[4] = NULL;

	lengths[0] = 1;
	lengths[1] = 1;
	lengths[2] = 1;
	lengths[3] = 1;
	lengths[4] = 1;

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

    tc = tcase_create("makegraph");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_makegraph);

    tc = tcase_create("mark_successors");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_mark_successors);

    tc = tcase_create("action done");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_action_done);

    tc = tcase_create("action run");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_action_run);

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
    tcase_add_test(tc,test_set_act_state_graph_run);
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

