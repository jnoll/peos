#include <check.h>
#include <stdlib.h>
#include <unistd.h>		/* unlink() */
#include <sys/stat.h>		/* mkdir() */
#include <sys/types.h>		/* mkdir() */
#include "process.h"
#include "test_util.h"



/* Globals. */
char *instance_dir = NULL;
peos_context_t process_table[PEOS_MAX_PID+1];

/* Stubs. */


vm_exit_code handle_action_change_graph(int pid, char *action, vm_act_state state)
{
	return VM_DONE;
}
	

int peos_get_pid(peos_context_t *context) 
{
    return context == &(process_table[0]) ? 0 : -1;
}

peos_context_t *peos_get_context(int pid)
{
    return pid == 0 ? &(process_table[pid]) : NULL ;
}

peos_context_t *find_free_entry()
{
    return &(process_table[0]);
}


int load_actions(char *file, peos_action_t **actions, int *num_actions, peos_other_node_t **other_nodes, int *num_other_nodes)
{
    return stub_load_actions(file, actions, num_actions,other_nodes,num_other_nodes);
}





/* Look for model file in current dir. */
START_TEST(test_find_model_file_default)
{
    char *model_file, *model= "model.pml", buf[BUFSIZ];
    FILE *f;

    /* Pre: model file exists. */
    unsetenv("PEOS_DIR");
    unsetenv("COMPILER_DIR");
    f = fopen(model, "w");
    if (f) {
	fprintf(f, "I am the example model file.\n");
	fclose(f);
    } else {
	fail("open expected.pml failed");
    }
    
    /* Action: find it. */
    model_file = find_model_file(model);

    /* Post: model file found. */
    fail_unless(model_file != NULL,
		"find_model_file failed");
    sprintf(buf, "./%s", model);
    fail_unless(strcmp(model_file, buf) == 0,
		"actual model_file not correct");
    free(model_file);
    unlink("model.pml");
}
END_TEST

/* Look for model file in COMPILER_DIR */
START_TEST(test_find_model_file)
{
    char *model_file, *model= "model.pml", buf[BUFSIZ];
    FILE *f;

    /* Pre: model file exists. */
    setenv("COMPILER_DIR", ".", 1);
    setenv("PEOS_DIR", ".", 1);

    f = fopen(model, "w");
    if (f) {
	fprintf(f, "I am the example model file.\n");
	fclose(f);
    } else {
	fail("open expected.pml failed");
    }
    
    /* Action: find it. */
    model_file = find_model_file(model);
    /* Post: model file found. */
    fail_unless(model_file != NULL,
		"find_model_file failed");
    sprintf(buf, "./%s", model);
    fail_unless(strcmp(model_file, buf) == 0,
		"actual model_file not correct");
    free(model_file);
    unlink("model.pml");
}
END_TEST



/* This is really a silly test, since create instance just calls functions 
   mostly from process_table.c. */
START_TEST(test_create_instance)
{
    int pid, i;
    char *model = "test_sample_1.pml";
    peos_context_t *context;

    memset(process_table, 0, PEOS_MAX_PID + 1);

    fail_unless((pid = peos_create_instance(model)) == 0, 
		"failed to create instance");
    context = &(process_table[0]);
    fail_unless(context->status == PEOS_READY, "process status");
    fail_unless(context->actions != NULL, "actions null");
    fail_unless(context->num_actions == 2, "num_actions wrong");
  //  strcpy(context->actions[1].name,"act_1");
    for (i = 0; i < context->num_actions; i++) {
	char buf[256];
	sprintf(buf, "act_%d", i);
	fail_unless(strcmp(context->actions[i].name, buf) == 0, "act name");
	fail_unless(context->actions[i].pid == pid, "act pid");
    }
}
END_TEST


/* Try to create an instance of a non-existent model. */
START_TEST(test_create_instance_noexist)
{
    char *model = "no";

    fail_unless(peos_create_instance(model) == -1,
		"created from non-existent model.");
}
END_TEST

void setup_get_field_test(int act_num, char *script)
{
    /* Pre: a process is loaded; some action is RUNNING,
     * the process is waiting for the action to become DONE.
     */
    peos_context_t *context = &process_table[0];
    context->num_actions = 10;
    context->actions = make_actions(context->num_actions, ACT_NONE);
    context->actions[act_num].script = script;
}

/* Action field accessor. */
START_TEST(test_get_field_first)
{
    char *expected = "This is the script.", *actual;
    setup_get_field_test(0, expected);
    actual = get_field(0, "act_0", ACT_SCRIPT);
    fail_unless(strcmp(actual, expected) == 0, "script contents");
}
END_TEST

/* Action field accessor. */
START_TEST(test_get_field_last)
{
    char *expected = "This is the script.", *actual;
    setup_get_field_test(9, expected);
    actual = get_field(0, "act_9", ACT_SCRIPT);
    fail_unless(strcmp(actual, expected) == 0, "script contents");
}
END_TEST

/* Action field accessor. */
START_TEST(test_get_field_none)
{
    char *expected = "This is the script.", *actual;
    setup_get_field_test(9, expected);
    actual = get_field(0, "No_action", ACT_SCRIPT);
    fail_unless(actual == NULL, "script contents");
}
END_TEST


int
main(int argc, char *argv[])
{
    int nf;
    SRunner *sr;
    Suite *s = suite_create("process");
    TCase *tc;

    parse_args(argc, argv);

    tc = tcase_create("io");
    tcase_add_test(tc, test_find_model_file_default);
    tcase_add_test(tc, test_find_model_file);

    tc = tcase_create("create_instance");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_create_instance);
    tcase_add_test(tc, test_create_instance_noexist);

    tc = tcase_create("action accessors");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_get_field_first);
    tcase_add_test(tc, test_get_field_last);
    tcase_add_test(tc, test_get_field_none);

    sr = srunner_create(s);

    srunner_set_fork_status(sr, fork_status);

    srunner_run_all(sr, verbosity);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    suite_free(s);
    return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
