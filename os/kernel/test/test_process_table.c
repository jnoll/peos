#include <unistd.h>		/* unlink() */
#include <sys/stat.h>		/* mkdir() */
#include <sys/types.h>		/* mkdir() */
#include <check.h>
#include "process_table.h"

#define PROCESS_TABLE
#include "test_util.h"

/* Globals. */
/* NONE! */

/* stubs */

extern Graph stub_makegraph(char *file);

Graph makegraph(char *file)
{
   return stub_makegraph(file);
}

void GraphDestroy(Graph g)
{
	return;
}



START_TEST(test_get_pid)
{
    int pid;
    peos_context_t *context = &(process_table[0]);
    pid = peos_get_pid(context);
    fail_unless(pid == 0, "pid");
}
END_TEST

START_TEST(test_get_pid_last)
{
    int pid;
    peos_context_t *context = &(process_table[PEOS_MAX_PID]);
    pid = peos_get_pid(context);
    fail_unless(pid == PEOS_MAX_PID, "pid");
}
END_TEST


START_TEST(test_load_actions)
{
 
    int s;
    peos_context_t *context = &(process_table[0]);
    
    /* Pre: a model exists. */
    /* Action. */
    
  
	  s = load_actions("test_sample_1.pml", &(context->actions),&(context->num_actions),&(context->other_nodes),&(context->num_other_nodes));
   
	  /* Post: model loaded - each entry matches a line in the file. */
   fail_unless(context->num_actions == 2, "num_actions wrong");
    fail_unless(strcmp(context->actions[0].name,"act_0") == 0, "action name wrong");
    fail_unless(strcmp(context->actions[1].name,"act_1") == 0, "action name wrong");
    fail_unless(context->num_other_nodes == 1, "num_other_nodes wrong");
    fail_unless(strcmp(context->other_nodes[0].name,"sel") == 0, "sel name wrong");

}
END_TEST

START_TEST(test_peos_set_resource_value)
{
	int i;
	peos_resource_t *resources = (peos_resource_t *) calloc(3,sizeof(peos_resource_t));
	peos_context_t *context = &(process_table[0]);

	context -> pid = 0;
	context -> resources = resources;
	context -> num_resources = 3;

	for(i = 0; i < 3; i++)
	{
	sprintf(resources[i].name, "r%d",i);
	sprintf(resources[i].value, "val");
	}

	fail_unless(peos_set_resource_value(0, "r2", "new_val") == 1,"return value");

	fail_unless(strcmp(context->resources[0].value, "val") == 0, "r0 value");
	fail_unless(strcmp(context->resources[1].value, "val") == 0, "r1 value");
	fail_unless(strcmp(context->resources[2].value, "new_val") == 0, "r2 value");
	
}
END_TEST;


START_TEST(test_peos_set_resource_value_no_resource)
{
   int i;
   peos_resource_t *resources = (peos_resource_t *) calloc(3,sizeof(peos_resource_t));
   peos_context_t *context = &(process_table[0]);
  
   context -> pid = 0;
   context -> resources = resources;
   context -> num_resources = 3;
                                                                        
   for(i = 0; i < 3; i++)
     {
       sprintf(resources[i].name, "r%d",i);
       sprintf(resources[i].value, "val");
     }

     fail_unless(peos_set_resource_value(0, "r4", "new_val") == -1," unknown resource bound");

}
END_TEST

START_TEST(test_save_proc_table)
{
    int i, j, nbytes, abytes;
    char expected[BUFSIZ], actual[BUFSIZ];
    FILE *f;

    /* Pre: process table initialized. */
    peos_context_t *context = &(process_table[0]);

    f = fopen("expected_proc_table.dat", "w");
    for (j = 0; j < PEOS_MAX_PID+1; j++) {
	context = &(process_table[j]);
	sprintf(context->model, "test.pml");
	fprintf(f, "pid: %d\nmodel: %s\n", j, context->model);
	context->status = PEOS_RUNNING;
	fprintf(f, "status: %d\n", context->status);
	context->num_actions = 3;
	fprintf(f, "actions: "); 
	fprintf(f, "%d ", context->num_actions);
	context->actions = (peos_action_t *)calloc(context->num_actions, sizeof(peos_action_t));
	for (i = 0; i < context->num_actions; i++) {
	    strcpy(context->actions[i].name, "an_action");
	    context->actions[i].state = ACT_NONE;
	    fprintf(f, " %s %d", context->actions[i].name, context->actions[i].state); 
	}
        fprintf(f, "\n");
        context->num_other_nodes = 3;
        fprintf(f, "other_nodes: ");
	fprintf(f, "%d ", context->num_other_nodes);
	context->other_nodes = (peos_other_node_t *)calloc(context->num_other_nodes, sizeof(peos_other_node_t));
	for (i = 0; i < context->num_other_nodes; i++) {
	    strcpy(context->other_nodes[i].name, "some_other_node");
	    context->other_nodes[i].state = ACT_NONE;
	   fprintf(f, " %s %d", context->other_nodes[i].name, context->other_nodes[i].state);
	}

        fprintf(f, "\n");
	context->num_resources = 2;
        fprintf(f, "resources: ");
        fprintf(f, "%d ", context->num_resources);
        context->resources = (peos_resource_t *)calloc(context->num_resources, sizeof(peos_resource_t));
        for (i = 0; i < context->num_resources; i++) {
          strcpy(context->resources[i].name, "some_resource");
            strcpy(context->resources[i].value,"some_value");
            fprintf(f, " %s %s", context->resources[i].name, context->resources[i].value);
          }
	
	fprintf(f, "\n\n"); 
    }
    fclose(f);

    mark_point();

    f = fopen("expected_proc_table.dat", "r");
    memset(expected, 0, BUFSIZ);
    nbytes = fread(expected, sizeof(char), BUFSIZ, f);
    fclose(f);
    mark_point();

    /* Action */
    save_proc_table("proc_table.dat");
    mark_point();

    /* Post: process table file exists and contains proc table. */
    f = fopen("proc_table.dat", "r");
    memset(actual, 0, BUFSIZ);
    abytes = fread(actual, sizeof(char), BUFSIZ, f);
    fail_unless(abytes == nbytes, "file size");
    fclose(f);
    mark_point();

    fail_unless (strcmp(actual, expected) == 0, "proc table contents differ");
 
       unlink("proc_table.dat");
      unlink("expected_proc_table.dat");
}
END_TEST

START_TEST(test_load_proc_table) 
{
    int i, j;
    char  *model = "test_sample_5.pml";
    peos_context_t ctx;
    FILE *f;

    /* Pre: saved process table file. */
    peos_context_t *context = &(ctx);

    f = fopen("proc_table.dat", "w");
    for (j = 0; j <= PEOS_MAX_PID; j++) {
	sprintf(context->model, model);
	fprintf(f, "pid: %d\nmodel: %s\n", j, context->model);
	context->status = PEOS_RUNNING;
	fprintf(f, "status: %d\n", context->status);
	context->num_actions = 2;
	fprintf(f, "actions: "); 
	fprintf(f, "%d ", context->num_actions);
	context->actions = (peos_action_t *)calloc(context->num_actions, sizeof(peos_action_t));
	for (i = 0; i < context->num_actions; i++) {
	    sprintf(context->actions[i].name, "act_%d", i);
	    context->actions[i].state = ACT_NONE;
	    context->actions[i].script = "test script";
	    fprintf(f, " %s %d", context->actions[i].name, context->actions[i].state); 
	}

        
	fprintf(f,"\n");

	context->num_other_nodes = 1;
        fprintf(f, "other_nodes: ");
        fprintf(f, "%d ", context->num_other_nodes);
        context->other_nodes = (peos_other_node_t *)calloc(context->num_other_nodes, sizeof(peos_other_node_t));
        for (i = 0; i < context->num_other_nodes; i++) {
            sprintf(context->other_nodes[i].name, "sel");
            context->other_nodes[i].state = ACT_NONE;
            fprintf(f, " %s %d", context->other_nodes[i].name, context->other_nodes[i].state);
        }
	

	fprintf(f,"\n");
	
        context->num_resources = 2;
        fprintf(f, "resources: ");
        fprintf(f, "%d ", context->num_resources);
        context->resources = (peos_resource_t *)calloc(context->num_resources, sizeof(peos_resource_t));
        for (i = 0; i < context->num_resources; i++) {
        strcpy(context->resources[i].name, "some_resource");
        strcpy(context->resources[i].value,"some_value"); 
            fprintf(f, " %s %s", context->resources[i].name, context->resources[i].value);
          }
	
	fprintf(f, "\n\n"); 
    }
    fclose(f);
    mark_point();

    memset(process_table, 0, (PEOS_MAX_PID+1) * sizeof(peos_context_t));
    mark_point();

    /* Load_proc_table requires a real model file to load actions from. */


    /* Action */
    load_proc_table("proc_table.dat");
    mark_point();

    /* Post: process table reflects file contents; other globals
       initialized. */
    for (j = 0; j <= PEOS_MAX_PID; j++) {
	context = &(process_table[j]);
	fail_unless(context->pid == j, "pid");
	fail_unless(strcmp(context->model, model) == 0, "model name");
	fail_unless(context->status == PEOS_RUNNING, "process status");
	fail_unless(context->num_actions == 2, "num actions");
	
	fail_unless(strcmp(context->actions[0].name,"act_0") == 0, "action name wrong");
	fail_unless(strcmp(context->actions[1].name,"act_1") == 0, "action name wrong");
	
	fail_unless(context->num_other_nodes == 1,"num_other_nodes_wrong");
	fail_unless(strcmp(context->other_nodes[0].name,"sel") == 0, "sel name wrong");
	
	for (i = 0; i < context->num_actions; i++) {
	    fail_unless(context->actions[i].pid == j, "action pid");
	    fail_unless(context->actions[i].state == ACT_NONE, "act state");
	    fail_unless(strcmp(context->actions[i].script, "test script") == 0, "act script");
	}

	for (i=0; i < context->num_other_nodes;i++) {
		fail_unless(context->other_nodes[i].pid == j, "other_nodes pid");
		fail_unless(context->other_nodes[i].state == ACT_NONE, "other node state");
	}

	for (i=0; i < context->num_resources;i++) {
          fail_unless(context->resources[i].pid == j, "resources pid");
	  fail_unless(strcmp(context->resources[i].name,"some_resource") == 0, "resource name");
	  fail_unless(strcmp(context->resources[i].value,"some_value") == 0, "resource value");
        }
	
    }

    unlink("proc_table.dat");
}
END_TEST


void setup_list_actions()
{
    int i;

    for (i = 0; i <= PEOS_MAX_PID; i++) {
	process_table[i].actions = make_actions(10, ACT_NONE);
	process_table[i].num_actions = 10;
    }
}

void teardown_list_actions()
{
    int i;
    for (i = 0; i <= PEOS_MAX_PID; i++) {
	free(process_table[i].actions);
    }
}

START_TEST(test_list_actions_0)
{
    peos_action_t **acts = 0;
    int i;

    /* Empty result. */
    /*  Pre: process table is initialized. */

    /* Action. */
    acts = peos_list_actions(ACT_READY);

    /* Post: list of actions in READY state. */
    for (i = 0; acts[i]; i++) {
	fail_unless(acts[i]->state == ACT_READY, NULL);
    }

    fail_unless(i == 0, "action count wrong (there aren't any)"); 
    free(acts);
}
END_TEST

START_TEST(test_list_actions_1)
{
    peos_action_t **acts = 0;
    int i;

    /* One ready, at beginning. */
    /* Pre: action list is initialized. */
    for (i = 0; i <= PEOS_MAX_PID; i++) {
	process_table[i].actions[0].state = ACT_READY;
    }

    /* Action. */
    acts = peos_list_actions(ACT_READY);

    /* Post: one action in READY state. */
    for (i = 0; acts[i]; i++) {
	fail_unless(acts[i]->state == ACT_READY, NULL);
    }
    fail_unless(i == (1 * (PEOS_MAX_PID + 1)), "action count wrong");
    free(acts);

}
END_TEST

START_TEST(test_list_actions_2)
{
    peos_action_t **acts = 0;
    int i;

    /* Two ready, beginning and end. */
    /* Pre: action list is initialized. */
    for (i = 0; i <= PEOS_MAX_PID; i++) {
	process_table[i].actions[0].state = ACT_READY;
	process_table[i].actions[9].state = ACT_READY;
    }

    /* Action. */
    acts = peos_list_actions(ACT_READY);

    /* Post: two actions in READY state. */
    for (i = 0; acts[i]; i++) {
	fail_unless(acts[i]->state == ACT_READY, NULL);
    }
    fail_unless(i == (2 * (PEOS_MAX_PID + 1)), "action count wrong");
    free(acts);
}
END_TEST

START_TEST(test_list_actions_3)
{
    peos_action_t **acts = 0;
    int i;

    /* Three ready, including middle. */
    /* Pre: action list is initialized. */
    for (i = 0; i <= PEOS_MAX_PID; i++) {
	process_table[i].actions[0].state = ACT_READY;
	process_table[i].actions[5].state = ACT_READY;
	process_table[i].actions[9].state = ACT_READY;
    }
    /* Action. */
    acts = peos_list_actions(ACT_READY);

    /* Post: three actions in READY state. */
    for (i = 0; acts[i]; i++) {
	fail_unless(acts[i]->state == ACT_READY, NULL);
    }
    fail_unless(i == (3 * (PEOS_MAX_PID + 1)), "action count wrong");
    free(acts);
}
END_TEST

/* Sets up a full table. */
void setup_find_free_entry()
{
    int i;

    for (i = 0; i < PEOS_MAX_PID + 1; i++) {
	process_table[i].status = PEOS_RUNNING;
    }
}


START_TEST(test_find_free_entry)
{
    int i;
    peos_context_t *context; 
    /* Pre: an empty process table. */
    for (i = 0; i < PEOS_MAX_PID + 1; i++) {
	process_table[i].status = PEOS_NONE;
    }
    context = find_free_entry();

    /* Post: first entry is returned. */
    fail_unless(context == &(process_table[0]), "wrong context returned");
}
END_TEST

START_TEST(test_find_free_entry_full)
{
    peos_context_t *context; 
    /* Pre: an full process table. */
    setup_find_free_entry();
    context = find_free_entry();

    /* Post: first entry is returned. */
    fail_unless(context == NULL, "full: wrong context returned");
}
END_TEST

START_TEST(test_find_free_entry_full2)
{
    int i;
    peos_context_t *context; 
    /* Pre: an full process table. */
    for (i = 0; i < PEOS_MAX_PID + 1; i++) {
	process_table[i].status = PEOS_READY;
    }
    context = find_free_entry();

    /* Post: first entry is returned. */
    fail_unless(context == NULL, "full2: wrong context returned");
}
END_TEST


START_TEST(test_find_free_entry_first)
{
    peos_context_t *context; 
    /* Pre: an full process table. */
    setup_find_free_entry();
    process_table[0].status = PEOS_NONE;

    context = find_free_entry();

    /* Post: first entry is returned. */
    fail_unless(context == &(process_table[0]), "first: wrong context returned");
}
END_TEST

START_TEST(test_find_free_entry_last)
{
    peos_context_t *context; 
    /* Pre: an full process table. */
    setup_find_free_entry();
    process_table[PEOS_MAX_PID].status = PEOS_NONE;

    context = find_free_entry();

    /* Post: first entry is returned. */
    fail_unless(context == &(process_table[PEOS_MAX_PID]), "last: wrong context returned");
}
END_TEST

START_TEST(test_find_free_entry_middle)
{
    peos_context_t *context; 
    /* Pre: an full process table. */
    setup_find_free_entry();
    process_table[PEOS_MAX_PID/2].status = PEOS_NONE;

    context = find_free_entry();

    /* Post: first entry is returned. */
    fail_unless(context == &(process_table[PEOS_MAX_PID/2]), "middle: wrong context returned");
}
END_TEST


int
main(int argc, char *argv[])
{
    int nf;
    SRunner *sr;
    Suite *s = suite_create("process_table");
    TCase *tc;

    parse_args(argc, argv);

    tc = tcase_create("get pid");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_get_pid);
    tcase_add_test(tc, test_get_pid_last);


    tc = tcase_create("table io");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_load_actions);
    tcase_add_test(tc, test_load_proc_table);
    tcase_add_test(tc, test_save_proc_table);

    tc = tcase_create("set resource value");
    suite_add_tcase(s, tc);
    tcase_add_test(tc,test_peos_set_resource_value);
    tcase_add_test(tc,test_peos_set_resource_value_no_resource);

    tc = tcase_create("list actions");
    suite_add_tcase(s, tc);
    tcase_add_unchecked_fixture(tc, setup_list_actions,
			      teardown_list_actions);
    tcase_add_test(tc, test_list_actions_0);
    tcase_add_test(tc, test_list_actions_1);
    tcase_add_test(tc, test_list_actions_2);
    tcase_add_test(tc, test_list_actions_3);

    tc = tcase_create("find free entry");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_find_free_entry);
    tcase_add_test(tc, test_find_free_entry_full);
    tcase_add_test(tc, test_find_free_entry_full2);
    tcase_add_test(tc, test_find_free_entry_first);
    tcase_add_test(tc, test_find_free_entry_last);
    tcase_add_test(tc, test_find_free_entry_middle);

    sr = srunner_create(s);

    srunner_set_fork_status(sr, fork_status);

    srunner_run_all(sr, verbosity);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    suite_free(s);
    return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
