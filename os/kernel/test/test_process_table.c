#include <unistd.h>		/* unlink() */
#include <sys/stat.h>		/* mkdir() */
#include <sys/types.h>		/* mkdir() */
#include <check.h>
#include "process_table.h"
#include "test_util.h"

/* Globals. */
char *instance_dir = NULL;

/* Look for model file in current dir. */
START_TEST(test_find_model_file_default)
{
    char *model_file, *model= "model.txt", buf[BUFSIZ];
    FILE *f;

    /* Pre: model file exists. */
    unsetenv("PEOS_DIR");
    unsetenv("COMPILER_DIR");
    f = fopen(model, "w");
    if (f) {
	fprintf(f, "I am the example model file.\n");
	fclose(f);
    } else {
	fail("open expected.txt failed");
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
    unlink("model.txt");
}
END_TEST

/* Look for model file in COMPILER_DIR */
START_TEST(test_find_model_file)
{
    char *model_file, *model= "model.txt", buf[BUFSIZ];
    FILE *f;

    /* Pre: model file exists. */
    setenv("COMPILER_DIR", ".", 1);
    setenv("PEOS_DIR", ".", 1);

    f = fopen(model, "w");
    if (f) {
	fprintf(f, "I am the example model file.\n");
	fclose(f);
    } else {
	fail("open expected.txt failed");
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
    unlink("model.txt");
}
END_TEST

START_TEST(test_load_instructions)
{
    int i, num_inst, num_actions;
    char **inst_array;		/* Process instructions. */
    peos_action_t *actions;
    FILE *out;

    /* Pre: a model exists. */
    out = fopen("expected.txt", "w");
    if (out == NULL) {
	fail("open expected.txt failed");
	return;
    }
    fprintf(out, big_txt);
    fclose(out);

    /* Action. */
    fail_unless(load_instructions("expected.txt", 
				   &inst_array, 
				   &num_inst,
				   &actions, 
				   &num_actions) == 49, "load failed");

    /* Post: model loaded - each entry matches a line in the file. */
    fail_unless(num_actions == 49, "num_actions wrong");
    fail_unless(num_inst == 542, "num_inst wrong");

    out = fopen("actual.txt", "w");
    for (i = 0; i < num_actions; i++) {
	fprintf(out, "%d %s type action mode manual\n", i, actions[i].name);
    }
    for (; i <  num_inst; i++) {
	fprintf(out, "%d %s", i, inst_array[i]);
    }
    fclose(out);

    if (0 != system("diff -w expected.txt actual.txt > /dev/null")) {
	fail("diff expected.txt actual.txt shows differences");
    } else {
	unlink("expected.txt");
	unlink("actual.txt");
    }
}
END_TEST


START_TEST(test_save_proc_table)
{
    int i,  pid = 0;
    char expected[BUFSIZ], actual[BUFSIZ];
    FILE *f;
    /* Pre: process table initialized. */
    peos_context_t *context = &(process_table[0]);

    f = fopen("expected_proc_table.dat", "w");
    sprintf(context->model, "test");
    fprintf(f, "pid: %d\nmodel: %s\n", pid, context->model);
    init_context(&(context->vm_context), 1);
    fprintf(f, "PC: %d\nSP: %d\nA: %d\n", context->vm_context.PC, 
	    context->vm_context.SP, context->vm_context.A);
    fprintf(f, "stack:");
    for (i = 0; i < MAX_STACK_SIZE; i++) {
	context->vm_context.stack[i] = 9;
	fprintf(f, " %d", context->vm_context.stack[i]);
    }
    fprintf(f, "\n");
    context->vm_context.num_variables = 3;
    fprintf(f, "variables: ");
    fprintf(f, "%d ", context->vm_context.num_variables);
    context->vm_context.variables = (vm_vbinding_t *)calloc(context->vm_context.num_variables, sizeof(vm_vbinding_t));

    for (i = 0; i < context->vm_context.num_variables; i++) {
	sprintf(context->vm_context.variables[i].name, "var");
	context->vm_context.variables[i].value = 8;
	fprintf(f, " %s %d", context->vm_context.variables[i].name,
		context->vm_context.variables[i].value);
    }
    fprintf(f, "\n");
    fprintf(f, "parameters:");
    context->vm_context.parameters.call = VM_SET;
    fprintf(f, " %d", context->vm_context.parameters.call);
    context->vm_context.parameters.act.state = ACT_READY;
    fprintf(f, " %d", context->vm_context.parameters.act.state);

    context->vm_context.parameters.act.num_act = 3;
    fprintf(f, " %d", context->vm_context.parameters.act.num_act);
    for (i = 0; i < context->vm_context.parameters.act.num_act; i++) {
	sprintf(context->vm_context.parameters.act.actions[i], "an_action_param");
	fprintf(f, " %s", context->vm_context.parameters.act.actions[i]);
    }
    fprintf(f, "\n"); 
    context->num_actions = 3;
    fprintf(f, "actions: "); 
    fprintf(f, "%d ", context->num_actions);
    context->actions = (peos_action_t *)calloc(context->num_actions, sizeof(peos_action_t));
    for (i = 0; i < context->num_actions; i++) {
	strcpy(context->actions[i].name, "an_action");
	context->actions[i].state = ACT_NONE;
	fprintf(f, " %s %d", context->actions[i].name, context->actions[i].state); 
    }
    fprintf(f, "\n\n"); 
    fprintf(f, "current_process: 0\n");
    fclose(f);
    num_proc = 1;
    mark_point();

    f = fopen("expected_proc_table.dat", "r");
    fgets(expected, BUFSIZ, f);
    fclose(f);
    mark_point();

    /* Action */
    save_proc_table("proc_table.dat");
    mark_point();

    /* Post: process table file exists and contains proc table. */
    f = fopen("proc_table.dat", "r");
    fgets(actual, BUFSIZ, f);
    fclose(f);
    mark_point();
    fail_unless (strcmp(actual, expected) == 0, "proc table contents differ");
    unlink("proc_table.dat");
    unlink("expected_proc_table.dat");
}
END_TEST


START_TEST(test_load_proc_table) 
{
    int i,  pid = 0;
    char expected[BUFSIZ], actual[BUFSIZ];
    peos_context_t ctx;
    FILE *f;
    char *p_txt = \
"0 an_action0 type action mode manual requires { a } provides { a }\n\
1 an_action1 type action mode manual requires { a } provides { a }\n\
2 an_action2 type action mode manual requires { a } provides { a }\n\
3 start\n\
4 end\n";

    /* Pre: saved process table file. */
    peos_context_t *context = &(ctx);

    f = fopen("proc_table.dat", "w");
    sprintf(context->model, "test");
    fprintf(f, "pid: %d\nmodel: %s\n", pid, context->model);
    init_context(&(context->vm_context), 3);
    fprintf(f, "PC: %d\nSP: %d\nA: %d\n", context->vm_context.PC, 
	    context->vm_context.SP, context->vm_context.A);
    fprintf(f, "stack:");
    for (i = 0; i < MAX_STACK_SIZE; i++) {
	context->vm_context.stack[i] = 9;
	fprintf(f, " %d", context->vm_context.stack[i]);
    }
    fprintf(f, "\n");
    context->vm_context.num_variables = 3;
    fprintf(f, "variables: ");
    fprintf(f, "%d ", context->vm_context.num_variables);
    context->vm_context.variables = (vm_vbinding_t *)calloc(context->vm_context.num_variables, sizeof(vm_vbinding_t));

    for (i = 0; i < context->vm_context.num_variables; i++) {
	sprintf(context->vm_context.variables[i].name, "var");
	context->vm_context.variables[i].value = 8;
	fprintf(f, " %s %d", context->vm_context.variables[i].name,
		context->vm_context.variables[i].value);
    }
    fprintf(f, "\n");
    fprintf(f, "parameters:");
    context->vm_context.parameters.call = VM_SET;
    fprintf(f, " %d", context->vm_context.parameters.call);
    context->vm_context.parameters.act.state = ACT_READY;
    fprintf(f, " %d", context->vm_context.parameters.act.state);

    context->vm_context.parameters.act.num_act = 3;
    fprintf(f, " %d", context->vm_context.parameters.act.num_act);
    for (i = 0; i < context->vm_context.parameters.act.num_act; i++) {
	sprintf(context->vm_context.parameters.act.actions[i], 
		"an_action%d", i);
	fprintf(f, " %s", context->vm_context.parameters.act.actions[i]);
    }
    fprintf(f, "\n"); 
    context->num_actions = 3;
    fprintf(f, "actions: "); 
    fprintf(f, "%d ", context->num_actions);
    context->actions = (peos_action_t *)calloc(context->num_actions, sizeof(peos_action_t));
    for (i = 0; i < context->num_actions; i++) {
	sprintf(context->actions[i].name, "an_action%d", i);
	context->actions[i].state = ACT_NONE;
	fprintf(f, " %s %d", context->actions[i].name, context->actions[i].state); 
    }
    fprintf(f, "\n\n"); 
    fprintf(f, "current_process: 0\n");
    fclose(f);
    mark_point();
    memset(process_table, 0, (PEOS_MAX_PID+1) * sizeof(peos_context_t));
    mark_point();

    /* Load_proc_table requires a real model file to load actions from. */
    f = fopen("test.txt", "w");
    fprintf(f, "%s", p_txt);
    fclose(f);

    /* Action */
    load_proc_table("proc_table.dat");
    mark_point();

    /* Post: process table reflects file contents; other globals
       initialized. */
    fail_unless(strcmp(context->model, "test") == 0, "model name");
    fail_unless(context->vm_context.PC == 3, "PC");
    fail_unless(context->vm_context.SP == 0, "SP");
    fail_unless(context->vm_context.A == -1, "A");
    for (i = 0; i < MAX_STACK_SIZE; i++) {
	fail_unless(context->vm_context.stack[i] == 9, "stack");
    }
    fail_unless(context->vm_context.num_variables == 3, "num var");
    for (i = 0; i < context->vm_context.num_variables; i++) {
	fail_unless(strcmp(context->vm_context.variables[i].name, "var") == 0,
		    "var name");
	fail_unless(context->vm_context.variables[i].value == 8, "var value");
    }
    fail_unless(context->vm_context.parameters.call == VM_SET, "param call");
    fail_unless(context->vm_context.parameters.act.state == ACT_READY, "param act state");
    fail_unless(context->vm_context.parameters.act.num_act == 3, "param num act");
    for (i = 0; i < context->vm_context.parameters.act.num_act; i++) {
	fail_unless(strncmp(context->vm_context.parameters.act.actions[i], "an_action", strlen("an_action")) == 0, "param act name");
    }

    fail_unless(context->num_actions == 3, "num actions");
    for (i = 0; i < context->num_actions; i++) {
	fail_unless(strncmp(context->actions[i].name, "an_action", strlen("an_action")) == 0, "act name");
	fail_unless(context->actions[i].state == ACT_NONE, "act state");
    }
    fail_unless(strcmp(actual, expected) == 0, "proc table contents differ");
    fail_unless(current_process == &(process_table[0]), "current process wrong");
    fail_unless(num_proc == 1, "num_proc wrong");
    unlink("proc_table.dat");
}
END_TEST

START_TEST(test_init_context)
{
    peos_context_t context;
    /* Pre: none. */
    context.vm_context.inst_array = p1;
    context.vm_context.num_inst = p1_size;

    /* Action. */
    init_context(&context.vm_context, 9);

    /* Post: PC, A, SP initialized. */
    fail_unless(context.vm_context.PC == 9, "PC wrong");
    fail_unless(context.vm_context.A == -1, "A wrong");
    fail_unless(context.vm_context.SP == 0, "SP wrong");
}
END_TEST

void
setup_create_instance()
{
    char cmd[BUFSIZ];
    FILE *model;
    instance_dir = "test_instances";

    sprintf(cmd, "[-d %s] && rm -r %s", instance_dir, instance_dir);

    /* Pre: model file exists, PEOS_DIR set. */

    mkdir(instance_dir, S_IRUSR|S_IWUSR|S_IXUSR);
    setenv("PEOS_DIR", instance_dir, 1);
    setenv("COMPILER_DIR", "test_instances", 1);
    sprintf(cmd, "%s/%s", instance_dir, "big.txt");
    if ((model = fopen(cmd, "w")) != NULL) {
	fprintf(model, "%s", big_txt);
	fclose(model);
    }
}

void
teardown_create_instance()
{
    char cmd[BUFSIZ];
    unsetenv("PEOS_DIR");
    unsetenv("COMPILER_DIR");
    sprintf(cmd, "rm -r %s", instance_dir);
    system(cmd);
}


START_TEST(test_create_instance)
{
    int i, pid;
    char *model = "big";
    peos_context_t *context;

    memset(process_table, 0, PEOS_MAX_PID + 1);
    num_proc = 0;
    for (i = 0; i <= PEOS_MAX_PID; i++) {
	fail_unless((pid = peos_create_instance(model)) == i, 
		    "failed to create instance");
	context = &(process_table[i]);
	fail_unless(context->vm_context.PC == 49, "PC wrong");
	fail_unless(context->vm_context.inst_array != NULL, "inst_array null");
	fail_unless(context->vm_context.num_inst == 542, "num_inst wrong");
	fail_unless(context->vm_context.A == -1, "A wrong");
	fail_unless(context->vm_context.SP == 0, "SP wrong");
	fail_unless(context->vm_context.stack[0] == 0, "");
	fail_unless(context->actions != NULL, "actions null");
	fail_unless(context->num_actions == 49, "num_actions wrong");
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


int
main(int argc, char *argv[])
{
    int nf;
    SRunner *sr;
    Suite *s = suite_create("process_table");
    TCase *tc = tcase_create("io");

    parse_args(argc, argv);

    suite_add_tcase(s, tc);

    tcase_add_test(tc, test_find_model_file_default);
    tcase_add_test(tc, test_find_model_file);
    tcase_add_test(tc, test_load_instructions);
    tcase_add_test(tc, test_save_proc_table);
    tcase_add_test(tc, test_load_proc_table);

    tc = tcase_create("context");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_init_context);

    tc = tcase_create("create_instance");
    suite_add_tcase(s, tc);
    tcase_add_unchecked_fixture(tc, setup_create_instance,
			      teardown_create_instance);
    tcase_add_test(tc, test_create_instance);
    tcase_add_test(tc, test_create_instance_noexist);

    sr = srunner_create(s);

    srunner_set_fork_status(sr, fork_status);

    srunner_run_all(sr, verbosity);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    suite_free(s);
    return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
