#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <check.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "vm.h"
#include "process.h"
#include "events.h"
#include "test_util.h"

/* Stub (bogus) globals. */


/* Globals. */
char *instance_dir = NULL;
int create_instance_result = 1;
int handle_action_result = VM_DONE;
int resume_result = VM_DONE;

/* Stubs. */
#ifdef NOTUSED
int load_instructions(char *file, char ***inst, int *num_inst,
			 peos_action_t **actions, int *num_actions)
{
    char buf[BUFSIZ];

    sprintf(buf, "%s/p1.txt", instance_dir);
    if (strcmp(file, buf) == 0) {
	*inst = p1;
	*num_inst = p1_size;
	return 1;
    }
    return 0;
}

int init_context(vm_context_t *context)
{
    return context->num_inst;
}

/* 
 * Only the file name is actually looked at.  The contents are never
 * examined, so nothing really needs to be saved.
 */
int save_context(vm_context_t *c, peos_action_t *a, int n, char *file)
{
    FILE *f = fopen(file, "w");
    if (f) {
	fprintf(f, "%s\n", p1_context);
	return fclose(f);
    }
    return -1;
}


#endif
int peos_resume(int pid)
{
    return resume_result;
}

int peos_create_instance(char *model)
{
    return create_instance_result;
}

vm_exit_code handle_action_change(char *act, vm_act_state state) 
{
    return handle_action_result;
}


START_TEST(test_list_models)
{
    int i, failed = 0;
    char *model_dir = "test_models";
    char **models = NULL;

    /* Pre: COMPILER_DIR points to directory with models. */
    setenv("COMPILER_DIR", model_dir, 1);
    mkdir("test_models", S_IRUSR|S_IWUSR|S_IXUSR);
    for (i = 0; i < 10; i++) {
	FILE *modelf;
	char filename[256];

	sprintf(filename, "%s/%d.txt", model_dir, i);
	modelf = fopen(filename, "w");
	fprintf(modelf, "%s", big_txt);
	fclose(modelf);
    }

    /* Action. */
    models = peos_list_models();

    /* Post: A list of all model file names in directory. */
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

    if (!failed) {
	char cmd[BUFSIZ];

	sprintf(cmd, "rm -r %s", model_dir);
	system(cmd);
    }
}
END_TEST

START_TEST(test_run_process)
{
    char *model = "p1", *instance_dir = "test_instances";
    char cmd[BUFSIZ];
    int i;

    /* Pre: model file exists, PEOS_DIR set.  Because
     * load_instructions() is a stub, the model file doesn't actually
     * have to exist.  The instances dir does have to exist, however,
     * because the contents will be searched.
     */

    mkdir("test_instances", S_IRUSR|S_IWUSR|S_IXUSR);
    setenv("PEOS_DIR", instance_dir, 1);
    setenv("COMPILER_DIR", instance_dir, 1);
    
    for (i = 0; i < 10; i++) {
	struct stat b;

	fail_unless(peos_run(model, 0) != 0, 
		    "failed to create instance");

#ifdef NOTUSED
	/* Post: context exists. */
	if (stat(iname, &b) != 0) {
	    fail("no context file");
	}
#endif
    }

    sprintf(cmd, "rm -r %s", instance_dir);
    system(cmd);
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



    tc = tcase_create("run");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_run_process);

    sr = srunner_create(s);

    srunner_set_fork_status(sr, fork_status);

    srunner_run_all(sr, verbosity);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    suite_free(s);
    if (nf == 0) {
	/* Clean up. */
	/* system("rm -r test_instances");*/
	return EXIT_SUCCESS;
    } else {
	/* Leave evidence. */
	return EXIT_FAILURE;
    }
}
