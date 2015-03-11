#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <check.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "test_util.h"

/* Globals. */
peos_context_t process_table[PEOS_MAX_PID+1];
Graph global_graph = NULL;

/* Stubs. */
extern Node make_node(char *name, vm_act_state, int type,int order);
extern Tree make_tree(char *sval, int ival, Tree left, Tree right);

peos_context_t *peos_get_context(int pid)
{
    process_table[0].process_graph = global_graph;
    return pid == 0 ? &(process_table[pid]) : NULL;
}

/* Test growing resource list. */
START_TEST(test_insert_resource)
{
    int num_resources = 0, rsize = 2, i, num_expected;
    char rname[32];
    /* Pre: a possibly empty resource list exists. */
    peos_resource_t *resource_list = (peos_resource_t *) calloc(rsize, sizeof(peos_resource_t));

    /* Action: insert a number of resources into the list. */
    num_expected = (32 * rsize) + 1;
    for (i = 0; i < num_expected; i++) {
	sprintf(rname, "%s_%d", "resource", i);
	insert_resource(rname, &resource_list, &num_resources, &rsize, "qualifier");
    }

    /* Post: all resources have been inserted in order. */
    fail_unless(num_resources == num_expected, "num_resources wrong");
    for (i = 0; i < num_expected; i++) {
        sprintf(rname, "%s_%d", "resource", i);
        fail_unless(strcmp(resource_list[i].name, rname) == 0, "resource name wrong");
        fail_unless(strcmp(resource_list[i].qualifier, "qualifier") == 0, "qualifier missing");
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
    Tree t4 = make_tree("\"true\"",0,NULL,NULL);
    Tree t5 = make_tree(NULL,EQ,t3,t4);

    make_resource_list(t5, &resource_list, &num_resources, &rsize, "\0");

    fail_unless(num_resources == 1,"num_resources wrong");
    fail_unless(strcmp(resource_list[0].name,ptr) == 0,"resource name wrong");

}	
END_TEST


START_TEST(test_make_resource_list_1)
{
	int num_resources = 0, rsize = 256;
	char *ptr = "y";
	peos_resource_t *resource_list = (peos_resource_t *) calloc(rsize,sizeof(peos_resource_t));
	
	Tree t1 = make_tree("y",0,NULL,NULL);
	Tree t3 = make_tree("(All)",0,NULL,NULL);
	Tree t2 = make_tree(NULL,QUALIFIER,t3,t1);

	make_resource_list(t2, &resource_list, &num_resources, &rsize, "\0");

	fail_unless(num_resources == 1,"num_resources wrong");
        fail_unless(strcmp(resource_list[0].name,ptr) == 0,"resource name wrong");
	fail_unless(strcmp(resource_list[0].qualifier,"(All)") == 0, "qualifier wrong");

}	
END_TEST


START_TEST(test_make_resource_list_2)
{
	int num_resources = 0, rsize = 256;
	char *ptr = "y";
	peos_resource_t *resource_list = (peos_resource_t *) calloc(rsize,sizeof(peos_resource_t));
	
	Tree t1 = make_tree("y",0,NULL,NULL);
	Tree t3 = make_tree("(All)",0,NULL,NULL);
	Tree t2 = make_tree(NULL,QUALIFIER,t3,t1);

	Tree t4 = make_tree("y1",0,NULL,NULL);
	Tree t5 = make_tree("(Any)",0,NULL,NULL);
	Tree t6 = make_tree(NULL,QUALIFIER,t5,t4);

	Tree t7 = make_tree(NULL,AND,t2,t6);

	make_resource_list(t7, &resource_list, &num_resources, &rsize, "\0");

	fail_unless(num_resources == 2,"num_resources wrong");
        fail_unless(strcmp(resource_list[0].name,ptr) == 0,"resource name wrong");
	fail_unless(strcmp(resource_list[0].qualifier,"(All)") == 0, "qualifier wrong");
        fail_unless(strcmp(resource_list[1].name,"y1") == 0,"resource name wrong");
	fail_unless(strcmp(resource_list[1].qualifier,"(Any)") == 0, "qualifier 1 wrong");
}
END_TEST

	
/* Test growing resource list. */
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
	make_resource_list(t[i], &resource_list, &num_resources, &rsize, "\0");
    }

    fail_unless(num_resources == num_expected, "num_resources wrong");
    for (i = 0; i < num_expected; i++) {
	sprintf(rname, "%s_%d", "resource", i);
	fail_unless(strcmp(resource_list[i].name, rname) == 0, "resource name wrong");
    }
}	
END_TEST

START_TEST(test_get_resource_list_action_requires)
{
    peos_resource_t *resources;
    int num_resources = 0;
    Tree t0, t1, t2;
    Node source, sink, act0, act1;

    source = make_node("p",ACT_NONE,PROCESS,0);  // process p { }
    sink = make_node("p",ACT_NONE,PROCESS,3);

    act0 = make_node("act0",ACT_READY,ACTION,1); // action act0 { }
    act0 -> requires = make_tree("res0", 0, NULL, NULL); // requires { res0 }

    act1 = make_node("act1",ACT_NONE,ACTION,2); // action act1 { }
    t1 = make_tree("res1", 0, NULL, NULL);
    t2 = make_tree("res2", 0, NULL, NULL);
    t0 = make_tree(NULL, EQ, t1, t2);
    act1 ->requires = t0;  // requires { res1 == res2 }

    //source -> act0 -> act1 -> sink

    source->next = act0;
    act0->next = act1;
    act1->next = sink;

    act0 -> predecessors = (List) make_list(source, NULL, NULL, NULL, NULL);
    act0 -> successors = (List) make_list(act1, NULL, NULL, NULL, NULL);
    act1 -> predecessors = (List) make_list(act0, NULL, NULL, NULL, NULL);
    act1 -> successors = (List) make_list(sink, NULL, NULL, NULL, NULL);
    sink -> predecessors = (List) make_list(act1, NULL, NULL, NULL, NULL);
    source -> successors = (List) make_list(act0, NULL, NULL, NULL, NULL);
    sink -> next = NULL;

    global_graph = (Graph) malloc(sizeof(struct graph));
    global_graph -> source = source;
    global_graph -> sink = sink;

    resources = get_resource_list_action_requires(0, "act0", &num_resources);
    fail_unless(num_resources == 1, "test_get_resource_list_action_requires");
    fail_unless(strcmp(resources[0].name, "res0") == 0, "test_get_resource_list_action_requires");

    resources = get_resource_list_action_requires(0, "act1", &num_resources);
    fail_unless(num_resources == 2, "test_get_resource_list_action_requires");
    fail_unless(strcmp(resources[0].name, "res1") == 0, "test_get_resource_list_action_requires");
    fail_unless(strcmp(resources[1].name, "res2") == 0, "test_get_resource_list_action_requires");
}
END_TEST

START_TEST(test_get_resource_list_action_provides)
{
    peos_resource_t *resources;
    int num_resources = 0;
    Tree t0, t1, t2;
    Node source, sink, act0, act1;
    
    source = make_node("p",ACT_NONE,PROCESS,0); // process p { }
    sink = make_node("p",ACT_NONE,PROCESS,3);

    act0 = make_node("act0",ACT_READY,ACTION,1); // action act0 { }
    act0 -> provides = make_tree("res0", 0, NULL, NULL); // provides { res0 }

    act1 = make_node("act1",ACT_NONE,ACTION,2); // action act1 { }
    t1 = make_tree("res1", 0, NULL, NULL);
    t2 = make_tree("res2", 0, NULL, NULL);
    t0 = make_tree(NULL, EQ, t1, t2);
    act1 -> provides = t0;  // provides { res1 == res2 }

    //source -> act0 -> act1 -> sink
    
    source->next = act0;
    act0->next = act1;
    act1->next = sink;

    act0 -> predecessors = (List) make_list(source, NULL, NULL, NULL, NULL);
    act0 -> successors = (List) make_list(act1, NULL, NULL, NULL, NULL);
    act1 -> predecessors = (List) make_list(act0, NULL, NULL, NULL, NULL);
    act1 -> successors = (List) make_list(sink, NULL, NULL, NULL, NULL);
    sink -> predecessors = (List) make_list(act1, NULL, NULL, NULL, NULL);
    source -> successors = (List) make_list(act0, NULL, NULL, NULL, NULL);
    sink -> next = NULL;

    global_graph = (Graph) malloc(sizeof(struct graph));
    global_graph -> source = source;
    global_graph -> sink = sink;

    resources = get_resource_list_action_provides(0, "act0", &num_resources);
    fail_unless(num_resources == 1, "test_get_resource_list_action_provides");
    fail_unless(strcmp(resources[0].name, "res0") == 0, "test_get_resource_list_action_provides");

    resources = get_resource_list_action_provides(0, "act1", &num_resources);
    fail_unless(num_resources == 2, "test_get_resource_list_action_provides");
    fail_unless(strcmp(resources[0].name, "res1") == 0, "test_get_resource_list_action_provides");
    fail_unless(strcmp(resources[1].name, "res2") == 0, "test_get_resource_list_action_provides");
}
END_TEST

START_TEST(test_get_resource_list_action)
{
    peos_resource_t *resources;
    int num_resources = 0;
    Tree t0, t1, t2, t3, t4;
    Node source, sink, act0, act1;

    source = make_node("p",ACT_NONE,PROCESS,0);
    sink = make_node("p",ACT_NONE,PROCESS,3);

    act0 = make_node("act0",ACT_READY,ACTION,1);
    act0 -> requires = make_tree("res0", 0, NULL, NULL); // requires { res0 }
    act0 -> provides = make_tree("res1", 0, NULL, NULL); // provides { res1 }

    act1 = make_node("act1",ACT_NONE,ACTION,2);
    act1 -> requires = make_tree("res0", 0, NULL, NULL); // requires { res0 }
    t3 = make_tree("res0", 0, NULL, NULL);
    t4 = make_tree("res1", 0, NULL, NULL);
    t1 = make_tree(NULL, AND, t3, t4);
    t2 = make_tree("res2", 0, NULL, NULL);
    t0 = make_tree(NULL, EQ, t1, t2);
    act1 ->provides = t0;  // provides { (res0 && res1) == res2 }

    //source -> act0 -> act1 -> sink

    source->next = act0;
    act0->next = act1;
    act1->next = sink;

    act0 -> predecessors = (List) make_list(source, NULL, NULL, NULL, NULL);
    act0 -> successors = (List) make_list(act1, NULL, NULL, NULL, NULL);
    act1 -> predecessors = (List) make_list(act0, NULL, NULL, NULL, NULL);
    act1 -> successors = (List) make_list(sink, NULL, NULL, NULL, NULL);
    sink -> predecessors = (List) make_list(act1, NULL, NULL, NULL, NULL);
    source -> successors = (List) make_list(act0, NULL, NULL, NULL, NULL);
    sink -> next = NULL;

    global_graph = (Graph) malloc(sizeof(struct graph));
    global_graph -> source = source;
    global_graph -> sink = sink;

    resources = get_resource_list_action(0, "act0", &num_resources);
    fail_unless(num_resources == 2, "test_get_resource_list_action");
    fail_unless(strcmp(resources[0].name, "res0") == 0, "test_get_resource_list_action");
    fail_unless(strcmp(resources[1].name, "res1") == 0, "test_get_resource_list_action");
    mark_point();
    resources = get_resource_list_action(0, "act1", &num_resources);
    mark_point();
    fail_unless(num_resources == 3, "test_get_resource_list_action");
    mark_point();
    fail_unless(strcmp(resources[0].name, "res0") == 0, "test_get_resource_list_action");
    mark_point();
    fail_unless(strcmp(resources[1].name, "res1") == 0, "test_get_resource_list_action");
    mark_point();
    fail_unless(strcmp(resources[2].name, "res2") == 0, "test_get_resource_list_action");
    mark_point();
}
END_TEST

START_TEST(test_get_resource_list)
{
    peos_resource_t *resources;
    int num_resources;
    FILE *fp;
    fp = fopen("temp.pml", "w");
    fprintf(fp, "process p { action act0 {  requires { res0 } provides { res1 && res2 }} action act1 { requires { res2 && res3} }}\n");
    fclose(fp);
    resources = get_resource_list("temp.pml", &num_resources);
    system("rm temp.pml");
    fail_unless(num_resources == 4, "test_get_resource_list");
    fail_unless(strcmp(resources[0].name, "res0") == 0, "test_get_resource_list");
    fail_unless(strcmp(resources[1].name, "res1") == 0, "test_get_resource_list");
    fail_unless(strcmp(resources[2].name, "res2") == 0, "test_get_resource_list");
    fail_unless(strcmp(resources[3].name, "res3") == 0, "test_get_resource_list");
}
END_TEST
        
START_TEST(test_fill_resource_list_value_0)
{
    peos_resource_t* source = (peos_resource_t *) calloc(3, sizeof(peos_resource_t));
    peos_resource_t* destination = (peos_resource_t *) calloc(3, sizeof(peos_resource_t));

    strcpy(source[0].name, "res0");
    strcpy(source[0].value, "val0");
    strcpy(source[1].name, "res1");
    strcpy(source[1].value, "val1");
    strcpy(source[2].name, "res2");
    strcpy(source[2].value, "val2");
    
    strcpy(destination[0].name, "res0");
    strcpy(destination[1].name, "res1");
    strcpy(destination[2].name, "res2");
    
    fill_resource_list_value(source, 3, &destination, 3);
    
    fail_unless(strcmp(destination[0].value, "val0") == 0, "test_fill_resource_list_value");
    fail_unless(strcmp(destination[1].value, "val1") == 0, "test_fill_resource_list_value");
    fail_unless(strcmp(destination[2].value, "val2") == 0, "test_fill_resource_list_value");
    
}
END_TEST
        
START_TEST(test_fill_resource_list_value_1)
{
    peos_resource_t* source = (peos_resource_t *) calloc(2, sizeof(peos_resource_t));
    peos_resource_t* destination = (peos_resource_t *) calloc(3, sizeof(peos_resource_t));

    strcpy(source[0].name, "res0");
    strcpy(source[0].value, "val0");
    strcpy(source[1].name, "res1");
    strcpy(source[1].value, "val1");
    
    strcpy(destination[0].name, "res0");
    strcpy(destination[1].name, "res1");
    strcpy(destination[2].name, "res2");
    
    fill_resource_list_value(source, 3, &destination, 3);
    
    fail_unless(strcmp(destination[0].value, "val0") == 0, "test_fill_resource_list_value");
    fail_unless(strcmp(destination[1].value, "val1") == 0, "test_fill_resource_list_value");
    fail_unless(strcmp(destination[2].value, "") == 0, "test_fill_resource_list_value");
    
}
END_TEST
        
START_TEST(test_fill_resource_list_value_2)
{
    peos_resource_t* source = (peos_resource_t *) calloc(3, sizeof(peos_resource_t));
    peos_resource_t* destination = (peos_resource_t *) calloc(2, sizeof(peos_resource_t));

    strcpy(source[0].name, "res0");
    strcpy(source[0].value, "val0");
    strcpy(source[1].name, "res1");
    strcpy(source[1].value, "val1");
    strcpy(source[2].name, "res2");
    strcpy(source[2].value, "val2");
    
    strcpy(destination[0].name, "res0");
    strcpy(destination[1].name, "res1");
    
    fill_resource_list_value(source, 3, &destination, 3);
    
    fail_unless(strcmp(destination[0].value, "val0") == 0, "test_fill_resource_list_value");
    fail_unless(strcmp(destination[1].value, "val1") == 0, "test_fill_resource_list_value");
    
}
END_TEST

int main(int argc, char *argv[])
{
    int nf;
    SRunner *sr;
    Suite *s = suite_create("resources");
    TCase *tc;

    parse_args(argc, argv);
    tc = tcase_create("make resource list");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_insert_resource);
    tcase_add_test(tc,test_make_resource_list);
    tcase_add_test(tc,test_make_resource_list_1);
    tcase_add_test(tc,test_make_resource_list_realloc);
    tcase_add_test(tc,test_make_resource_list_2);
    tcase_add_test(tc,test_get_resource_list_action_requires);
    tcase_add_test(tc,test_get_resource_list_action_provides);
    tcase_add_test(tc,test_get_resource_list_action);
    tcase_add_test(tc,test_get_resource_list);
    tcase_add_test(tc,test_fill_resource_list_value_0);
    tcase_add_test(tc,test_fill_resource_list_value_1);
    tcase_add_test(tc,test_fill_resource_list_value_2);
    
    sr = srunner_create(s);

    srunner_set_fork_status(sr, fork_status);

    srunner_run_all(sr, verbosity);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    /*suite_free(s);*/
    return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
