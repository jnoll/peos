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

peos_resource_t *peos_get_resource_list_action(int pid,char *action, int *num_resources)
{
peos_resource_t *resources = (peos_resource_t *) calloc(2,sizeof(peos_resource_t));  

*num_resources = 2;	

strcpy(resources[0].name,"a");
strcpy(resources[1].name,"b");

return resources;
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

START_TEST(test_handle_action_change_run)
{

  int nbytes,abytes;
  FILE *file;
  char expected[BUFSIZ],actual[BUFSIZ];
  char times[20];
  struct tm *current_info;
  time_t current;

  time(&current);
  current_info = localtime(&current);
  current = mktime(current_info);
  strftime(times,25,"%b %d %Y %H:%M",localtime(&current));
  file = fopen("expected_event.log", "a");
  fprintf(file, "%s jnoll RUN act_0 1 resource(s): a, b\n", times);
  fclose(file);
								                                                                                     
  mark_point();
									                                                                                 
  file = fopen("expected_event.log","r");
  memset(expected,0,BUFSIZ);
  nbytes = fread(expected,sizeof(char),BUFSIZ,file);
  fclose(file);
  mark_point();
                                                                                              fail_unless(handle_action_change(1,"act_0",ACT_RUN) == VM_DONE, "failed to change act state");
											      file = fopen("event.log", "r");
											     memset(actual,0,BUFSIZ);
   abytes = fread(actual,sizeof(char),BUFSIZ,file);
   fail_unless(abytes == nbytes, "file size");
   fclose(file);
   mark_point();

    fail_unless(strcmp(actual,expected) == 0, "event.log differs");
   unlink("expected_event.log");
   unlink("event.log");

}
END_TEST
										     



/* This is really a silly test, since create instance just calls functions 
   mostly from process_table.c. */
START_TEST(test_create_instance)
{
    int pid, i;
    char *model = "test_sample_1.pml";
    peos_context_t *context;
    
    int num_resources = 2;
    peos_resource_t *resources = (peos_resource_t *) calloc(num_resources,sizeof(peos_resource_t));

    memset(process_table, 0, PEOS_MAX_PID + 1);

    fail_unless((pid = peos_create_instance(model,resources,num_resources)) == 0, 
		"failed to create instance");
    context = &(process_table[0]);
    fail_unless(context->status == PEOS_READY, "process status");
    fail_unless(context->actions != NULL, "actions null");
    fail_unless(context->num_actions == 2, "num_actions wrong");
    fail_unless(context->num_resources == 2,"num_resources wrong");
    fail_unless(context->resources != NULL,"resources null");
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
    peos_resource_t *resources;
    int num_resources;

    fail_unless(peos_create_instance(model,resources,num_resources) == -1,
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

    tc = tcase_create("handle action change");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_handle_action_change_run);


    
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
