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
int free_entry = 0;
char *login_name = "jnoll";
/* Stubs. */
peos_context_t *peos_get_context(int pid)
{
	return pid == 0 ? &(process_table[pid]) : NULL;
}

int delete_entry(int pid)
{
    return 1;
}

int get_lock(int filedes)
{
    return 1;
}

int release_lock(int filedes)
{
    return 1;
}

int peos_get_pid(peos_context_t *context) 
{
    return context - process_table;
}

peos_context_t *find_free_entry()
{
    return &(process_table[free_entry]);
}

peos_resource_t *peos_get_resource_list_action(int pid,char *action, int *num_resources)
{
peos_resource_t *resources = (peos_resource_t *) calloc(2,sizeof(peos_resource_t));  

*num_resources = 2;	

strcpy(resources[0].name,"a");
strcpy(resources[1].name,"b");

return resources;
}
  
void initialize_graph(Graph g, int pid)
{
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


/* Look for model file in absolute path */
START_TEST(test_find_model_file_absolute)
{
    char *model_file, *cwd, *model= "model.pml", buf[BUFSIZ];
    FILE *f;

    /* Pre: model file exists. */
    setenv("COMPILER_DIR", ".", 1);
    setenv("PEOS_DIR", ".", 1);
    cwd = getcwd(NULL, 0);
    sprintf(buf, "%s/%s", cwd, model);

    f = fopen(model, "w");
    if (f) {
	fprintf(f, "I am the example model file.\n");
	fclose(f);
    } else {
	fail("open expected.pml failed");
    }
    
    /* Action: find it. */
    model_file = find_model_file(buf);
    /* Post: model file found. */
    fail_unless(model_file != NULL,
		"find_model_file failed");

    fail_unless(strcmp(model_file, buf) == 0,
		"actual model_file not correct");
    free(cwd);
    free(model_file);
    unlink("model.pml");
}
END_TEST

#ifdef FIXME
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
   unlink("expected_event.log");
   unlink("event.log");

}
END_TEST
#endif


/* This is really a silly test, since create instance just calls functions 
   mostly from process_table.c. */
START_TEST(test_create_instance)
{
    int pid = 0, new_pid;
    char *model = "test_sample_1.pml";
    peos_context_t *context;
    const int num_resources = 2;
    peos_resource_t resources[num_resources];

    /* pre: process table exists, resources exist. */
    memset(process_table, 0, PEOS_MAX_PID + 1);
    memset(resources, 'x', sizeof(resources));

    /* action */
    fail_unless((new_pid = peos_create_instance(model,resources,num_resources)) == pid, "failed to create instance");

    /* post: correct entry initialized correctly. */
    context = &(process_table[pid]);
    fail_unless(context->pid == pid,"wrong pid");
    fail_unless(context->process_graph != NULL,"graph null");
    fail_unless(context->status == PEOS_READY, "process status");
    fail_unless(context->num_resources == num_resources,"num_resources wrong");
    fail_unless(context->resources != NULL,"resources null");
    fail_unless(memcmp(context->resources, resources, sizeof(resources)) == 0,
		"resources wrong");
}
END_TEST

START_TEST(test_create_instance_multi)
{
    int pid = 1;
    char *model = "test_sample_1.pml";
    peos_context_t *context;
    const int num_resources = 2;
    peos_resource_t resources[num_resources];


    /* pre: entry 0 is in use. */
    free_entry = pid;
    memset(process_table, 0, PEOS_MAX_PID + 1);
    memset(resources, 'x', sizeof(resources));

    /* action */
    fail_unless(peos_create_instance(model,resources,num_resources) == pid,
		"failed to create instance");

    /* post: context at 1 initialized. */
    context = &(process_table[pid]);
    fail_unless(context->pid == pid,"wrong pid");
    fail_unless(context->process_graph != NULL,"graph null");
    fail_unless(context->status == PEOS_READY, "process status");
    fail_unless(context->num_resources == num_resources,"num_resources wrong");
    fail_unless(context->resources != NULL,"resources null");
    fail_unless(memcmp(context->resources, resources, sizeof(resources)) == 0,
		"resources wrong");
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
    tcase_add_test(tc, test_find_model_file_absolute);

    tc = tcase_create("create_instance");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_create_instance);
    tcase_add_test(tc, test_create_instance_multi);

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
