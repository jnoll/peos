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


int stub_load_actions(char *file,Graph *process_graph);

vm_exit_code handle_action_change_graph(int pid, char *action, vm_act_state state)
{
	return VM_CONTINUE;
}
	
int delete_entry(int pid)
{
    return 1;
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
  


int load_actions(char *file, Graph *process_graph)
{
    return stub_load_actions(file, process_graph);
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

/*
START_TEST(test_handle_action_change_run)
{
  int nbytes,abytes;
  FILE *file;
  char expected[BUFSIZ],actual[BUFSIZ];
  char times[20];
  struct tm *current_info;
  time_t current;
  char msg[256];

  time(&current);
  current_info = localtime(&current);
  current = mktime(current_info);
  strftime(times,25,"%b %d %Y %H:%M",localtime(&current));
  file = fopen("expected_event.log", "a");
  strcpy(msg,"jnoll RUN act_0 1");
  fprintf(file, "%s %s\n", times,msg);
  fclose(file);
                                                                                  mark_point();
									                                                                             
  file = fopen("expected_event.log","r");
  memset(expected,0,BUFSIZ);
  nbytes = fread(expected,sizeof(char),BUFSIZ,file);
  fclose(file);
  mark_point();
                                                                                  fail_unless(handle_action_change(1,"act_0",ACT_RUN) == VM_CONTINUE, "failed to change act state");
  file = fopen("event.log", "r");
  memset(actual,0,BUFSIZ);
  abytes = fread(actual,sizeof(char),BUFSIZ,file);
  fail_unless(abytes == nbytes, "file size");
  fclose(file);
  mark_point();
  fprintf(stderr,"\n%s%s",actual,expected);
  
   
  
  fail_unless(strcmp(actual,expected) == 0, "event.log differs");
  // unlink("expected_event.log");
  // unlink("event.log");

}
END_TEST
*/									     


/* This is really a silly test, since create instance just calls functions 
   mostly from process_table.c. */
START_TEST(test_create_instance)
{
    int pid;
    char *model = "test_sample_1.pml";
    peos_context_t *context;
    
    int num_resources = 2;
    peos_resource_t *resources = (peos_resource_t *) calloc(num_resources,sizeof(peos_resource_t));

    memset(process_table, 0, PEOS_MAX_PID + 1);

   fail_unless((pid = peos_create_instance(model,resources,num_resources)) == 0, "failed to create instance");
    context = &(process_table[0]);
    fail_unless(context->process_graph != NULL,"graph null");
    fail_unless(context->status == PEOS_READY, "process status");
    fail_unless(context->num_resources == 2,"num_resources wrong");
    fail_unless(context->resources != NULL,"resources null");
}
END_TEST


START_TEST(test_log_event)
{
    int nbytes,abytes;
    FILE *file;
    char expected[BUFSIZ],actual[BUFSIZ];
    char times[256];
    struct tm *current_info;
    time_t current;
    char *msg = "jnoll RUN act_0 1";
    /* setup */
    time(&current);
    current_info = localtime(&current);
    current = mktime(current_info);
    strftime(times,256,"%b %d %Y %H:%M",localtime(&current));
    sprintf(expected, "%s %s\n", times, msg);
    
    unlink("event.log");	/* start with empty log. */

    mark_point();
  
    /* action */
    log_event(msg);

    /* post */
    file = fopen("event.log", "r");
    memset(actual,0,BUFSIZ);
    nbytes = strlen(expected);
    abytes = fread(actual,sizeof(char),BUFSIZ,file);
    fail_unless(abytes == nbytes, "file size");
    fclose(file);
    mark_point();
    fail_unless(strcmp(actual,expected) == 0, "event.log differs");

    unlink("event.log");
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
    suite_add_tcase(s,tc);
    tcase_add_test(tc, test_find_model_file_default);
    tcase_add_test(tc, test_find_model_file);

    tc = tcase_create("create_instance");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_create_instance);

  /*  tc = tcase_create("handle action change");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_handle_action_change_run);
*/

    tc = tcase_create("log_event");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_log_event);


    sr = srunner_create(s);

    srunner_set_fork_status(sr, fork_status);

    srunner_run_all(sr, verbosity);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    suite_free(s);
    return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
