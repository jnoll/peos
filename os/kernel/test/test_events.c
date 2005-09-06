#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <check.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "action.h"
#include "process.h"
#include "events.h"
#include "test_util.h"

/* Globals. */
char *instance_dir = NULL;
char *login_name = "jnoll";
peos_resource_t* global_resources;
int global_num_resources;

/* Stubs. */
peos_context_t *peos_get_context(int pid)
{
    static peos_context_t context;
    return &context;
}

int load_process_table() 
{
    return 1;
}

int save_process_table()
{
    return 1;
}

vm_act_state get_act_state_graph(int pid, char *act_name)
{
    return ACT_READY;
}

char *act_state_name(vm_act_state s)
{
    return "action state name here";
}

char *find_model_file(char *model)
{
    return TEST_PROC_NAME;
}

void log_event(char *msg)
{
    return;
}

int delete_entry(int pid)
{
    return 1;
}

vm_exit_code handle_action_change(int pid, char *act_name, vm_act_state state)
{
    return VM_CONTINUE;
}

vm_exit_code update_process_state(int pid)
{
    return VM_CONTINUE;
}

char *get_script(int pid, char *action_name)
{
    return "script";
}

peos_resource_t *get_resource_list(char *model, int *num_res)
{
    peos_resource_t *resources;	
    return resources;
}

peos_resource_t *get_resource_list_action(int pid,char *act_name, int *num_res)
{
    *num_res = global_num_resources;
    return global_resources;
}

peos_resource_t *get_resource_list_action_requires(int pid,char *act_name, int *num_res)
{
    *num_res = global_num_resources;
    return global_resources;
}


peos_resource_t *get_resource_list_action_provides(int pid,char *act_name, int *num_res)
{
    *num_res = global_num_resources;
    return global_resources;
}

int peos_create_instance(char *model,peos_resource_t *resources,int num_resources)
{
    return 1;
}

int set_resource_binding(int pid, char *resource_name, char *resource_value)
{
    FILE* f;
    if (pid == 600) {//for test_peos_bind_resource_file
        f = fopen("test_bind_resource_file.dat", "a");
        fprintf(f, "%s %s\n", resource_name, resource_value);
        fclose(f);
    }
    return 1;
}

char *get_resource_binding(int pid, char *resource_name)
{
    return "binding";
}


char *get_resource_qualifier(int pid, char *resource_name)
{
    return "qualifier";
}

int eval_resource_list(peos_resource_t** resources, int num_resources) {
    return 1;
}

int fill_resource_list_value(peos_resource_t* source, int num_source, peos_resource_t** destination, int num_destination) {
    int i;
    peos_resource_t* res = *destination;
    for (i = 0; i < num_destination; i++) {
        strcat(res[i].value, "_eval");
    }
    return 1;
}

char* find_file(char* file) {
    return file;
}

START_TEST(test_trim)
{
    char* str = malloc(256);
    strcpy(str, "a");
    fail_unless(trim(str) == 1, "test_trim failed");
    fail_unless(strcmp(str, "a") == 0, "test_trim failed");
    strcpy(str, "abc");
    fail_unless(trim(str) == 3, "test_trim failed");
    fail_unless(strcmp(str, "abc") == 0, "test_trim failed");
    strcpy(str, " abc");
    fail_unless(trim(str) == 3, "test_trim failed");
    fail_unless(strcmp(str, "abc") == 0, "test_trim failed");
    strcpy(str, "abc ");
    fail_unless(trim(str) == 3, "test_trim failed");
    fail_unless(strcmp(str, "abc") == 0, "test_trim failed");
    strcpy(str, " abc ");
    fail_unless(trim(str) == 3, "test_trim failed");
    fail_unless(strcmp(str, "abc") == 0, "test_trim failed");
    strcpy(str, " a b c ");
    fail_unless(trim(str) == 5, "test_trim failed");
    fail_unless(strcmp(str, "a b c") == 0, "test_trim failed");
    strcpy(str, "  abc  \n");
    fail_unless(trim(str) == 3, "test_trim failed");
    fail_unless(strcmp(str, "abc") == 0, "test_trim failed");
    free(str);
}
END_TEST

START_TEST(test_peos_bind_resource_file)
{
    FILE* f;
    char line[600];
    //one resource line
    system("echo res0:val0 > test_bind_resource_file.res");
    peos_bind_resource_file(600, "test_bind_resource_file.res");
    f = fopen("test_bind_resource_file.dat", "r");
    fgets(line, 600, f);
    fail_unless(strcmp(line, "res0 val0\n") == 0, "test_peos_bind_resource_file failed");
    fclose(f);
    system("rm test_bind_resource_file.dat");
    
    //three resource lines
    system("echo res1:val1 >> test_bind_resource_file.res");
    system("echo res2:val2 >> test_bind_resource_file.res");
    peos_bind_resource_file(600, "test_bind_resource_file.res");
    f = fopen("test_bind_resource_file.dat", "r");
    fgets(line, 600, f);
    fail_unless(strcmp(line, "res0 val0\n") == 0, "test_peos_bind_resource_file failed");
    fgets(line, 600, f);
    fail_unless(strcmp(line, "res1 val1\n") == 0, "test_peos_bind_resource_file failed");
    fgets(line, 600, f);
    fail_unless(strcmp(line, "res2 val2\n") == 0, "test_peos_bind_resource_file failed");
    fclose(f);
    system("rm test_bind_resource_file.*");
    
    //two resource lines with empty line in between
    system("echo res0:val0 > test_bind_resource_file.res");
    system("echo >> test_bind_resource_file.res");
    system("echo res1:val1 >> test_bind_resource_file.res");
    peos_bind_resource_file(600, "test_bind_resource_file.res");
    f = fopen("test_bind_resource_file.dat", "r");
    fgets(line, 600, f);
    fail_unless(strcmp(line, "res0 val0\n") == 0, "test_peos_bind_resource_file failed");
    fgets(line, 600, f);
    fail_unless(strcmp(line, "res1 val1\n") == 0, "test_peos_bind_resource_file failed");
    fclose(f);
    system("rm test_bind_resource_file.*");
    
    //resource line with whitespace
    system("echo 'res0 :val0 ' > test_bind_resource_file.res");
    system("echo ' res1: val1' >> test_bind_resource_file.res");
    system("echo ' res2 : val2 ' >> test_bind_resource_file.res");
    system("echo '  res3  :  val3  ' >> test_bind_resource_file.res");
    peos_bind_resource_file(600, "test_bind_resource_file.res");
    f = fopen("test_bind_resource_file.dat", "r");
    fgets(line, 600, f);
    fail_unless(strcmp(line, "res0 val0\n") == 0, "test_peos_bind_resource_file failed");
    fgets(line, 600, f);
    fail_unless(strcmp(line, "res1 val1\n") == 0, "test_peos_bind_resource_file failed");
    fgets(line, 600, f);
    fail_unless(strcmp(line, "res2 val2\n") == 0, "test_peos_bind_resource_file failed");
    fgets(line, 600, f);
    fail_unless(strcmp(line, "res3 val3\n") == 0, "test_peos_bind_resource_file failed");
    fclose(f);
    system("rm test_bind_resource_file.*");
}
END_TEST

START_TEST(test_list_models)
{
    int i, j, failed = 0;
    char *model_dir = "test_models";
    char **models = NULL;

    /* Pre: COMPILER_DIR points to directory with models. */
    setenv("COMPILER_DIR", model_dir, 1);
    mkdir("test_models", S_IRUSR|S_IWUSR|S_IXUSR);
    for (i = 0; i < 10; i++) {
	FILE *modelf;
	char filename[256];

	sprintf(filename, "%s/%d.pml", model_dir, i);
	modelf = fopen(filename, "w");
	fprintf(modelf, "%s", "process s { action a {} }");
	fclose(modelf);
    }

    /* Action. */
    models = peos_list_models();

    /* Post: A list of all model file names in directory. */
    /* *** old section 
    for (i = 0; i < 10; i++) {
	char filename[256];

	sprintf(filename, "%d",  i);
	if (strcmp(models[i], filename) != 0) {
	    char msg[BUFSIZ];
	    sprintf(msg, "model file %s not found (%s)", filename, models[i]);
	    fail(msg);
	    failed = 1;
	}
    }
    */
    /* if the filenames are not sorted in increasing order
       there are n^2 possibilities
    */
    for (i = 0; i < 10; i++) {
	char filename[256];
	sprintf(filename, "%d",  i);
	for(j = 0; j < 10; j++) 
  		if (!strcmp(models[j], filename)) break;
 	if (j>=10){
		char msg[BUFSIZ];
		sprintf(msg, "model file %s not found (%s)", filename, models[i]);
		fail(msg);
		failed = 1;
	}
    }
    if (!failed) {
	char cmd[BUFSIZ];

	sprintf(cmd, "rm -r %s", model_dir);
	system(cmd);
    }
}
END_TEST

START_TEST(test_run_process)
{
    char *model = TEST_PROC_NAME;
    peos_resource_t *resources;
    int num_resources;

    /* Pre: Because load_actions() and find_model_file are stubs,
     * there are no actual pre conditions.
     */
	fail_unless(peos_run(model,resources, num_resources) != 0, "failed to create instance");
}
END_TEST

START_TEST(test_peos_eval_resource_list)
{
    peos_resource_t* resources = (peos_resource_t *) calloc(3, sizeof(peos_resource_t));
    strcpy(resources[0].value, "val0");
    strcpy(resources[1].value, "val1");
    strcpy(resources[2].value, "val2");
    
    peos_eval_resource_list(0, &resources, 3);
    
    fail_unless(strcmp(resources[0].value, "val0_eval") == 0, "fail evaluate resource list");
    fail_unless(strcmp(resources[1].value, "val1_eval") == 0, "fail evaluate resource list");
    fail_unless(strcmp(resources[2].value, "val2_eval") == 0, "fail evaluate resource list");
}
END_TEST

START_TEST(test_peos_get_resource_list_action)
{
    int num_resources;
    peos_resource_t* resources;
    global_num_resources = 3;
    global_resources = (peos_resource_t *) calloc(3, sizeof(peos_resource_t));
    
    strcpy(global_resources[0].value, "val0");
    strcpy(global_resources[1].value, "val1");
    strcpy(global_resources[2].value, "val2");
    
    resources = peos_get_resource_list_action(0, &resources, &num_resources);
    
    fail_unless(num_resources == 3, "fail get action resource list");
    fail_unless(strcmp(resources[0].value, "val0_eval") == 0, "fail get action resource list");
    fail_unless(strcmp(resources[1].value, "val1_eval") == 0, "fail get action resource list");
    fail_unless(strcmp(resources[2].value, "val2_eval") == 0, "fail get action resource list");
}
END_TEST
        
START_TEST(test_peos_get_resource_list_action_requires)
{
    int num_resources;
    peos_resource_t* resources;
    global_num_resources = 3;
    global_resources = (peos_resource_t *) calloc(3, sizeof(peos_resource_t));
    
    strcpy(global_resources[0].value, "val0");
    strcpy(global_resources[1].value, "val1");
    strcpy(global_resources[2].value, "val2");
    
    resources = peos_get_resource_list_action_requires(0, &resources, &num_resources);
    
    fail_unless(num_resources == 3, "fail get action requires resource list");
    fail_unless(strcmp(resources[0].value, "val0_eval") == 0, "fail get action requires resource list");
    fail_unless(strcmp(resources[1].value, "val1_eval") == 0, "fail get action requires resource list");
    fail_unless(strcmp(resources[2].value, "val2_eval") == 0, "fail get action requires resource list");
}
END_TEST
        
START_TEST(test_peos_get_resource_list_action_provides)
{
    int num_resources;
    peos_resource_t* resources;
    global_num_resources = 3;
    global_resources = (peos_resource_t *) calloc(3, sizeof(peos_resource_t));
    
    strcpy(global_resources[0].value, "val0");
    strcpy(global_resources[1].value, "val1");
    strcpy(global_resources[2].value, "val2");
    
    resources = peos_get_resource_list_action_provides(0, &resources, &num_resources);
    
    fail_unless(num_resources == 3, "fail get action provides resource list");
    fail_unless(strcmp(resources[0].value, "val0_eval") == 0, "fail get action provides resource list");
    fail_unless(strcmp(resources[1].value, "val1_eval") == 0, "fail get action provides resource list");
    fail_unless(strcmp(resources[2].value, "val2_eval") == 0, "fail get action provides resource list");
}
END_TEST

int
main(int argc, char *argv[])
{
    int nf;
    SRunner *sr;
    Suite *s = suite_create("events");
    TCase *tc;

    parse_args(argc, argv);
    
    tc = tcase_create("list_models");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_list_models);

    tc = tcase_create("trim");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_trim);
    
    tc = tcase_create("bind_resource_file");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_peos_bind_resource_file);

    tc = tcase_create("run");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_run_process);

    tc = tcase_create("resource_list");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_peos_eval_resource_list);
    tcase_add_test(tc, test_peos_get_resource_list_action);
    tcase_add_test(tc, test_peos_get_resource_list_action_requires);
    tcase_add_test(tc, test_peos_get_resource_list_action_provides);
    
    sr = srunner_create(s);

    srunner_set_fork_status(sr, fork_status);

    srunner_run_all(sr, verbosity);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    suite_free(s);
    return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
