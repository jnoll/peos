#include <check.h>
#include <stdlib.h>
#include <unistd.h>		/* unlink() */
#include <sys/stat.h>
#include <sys/types.h>
#include "process.h"
#include "test_util.h"

/* Globals. */
char *instance_dir = NULL;
int execute_status = VM_DONE;	/* What execute() should return. */

/* Stub (bogus) globals. */

/* Stubs. */

int execute(vm_context_t *context) 
{
    return execute_status;
}

/* action.c */
int find_actions(char *inst_array[], int size)
{
    return size;
}


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

    fail_unless(load_instructions("expected.txt", 
				   &inst_array, 
				   &num_inst,
				   &actions, 
				   &num_actions) == 49, "load failed");
    fail_unless(num_actions == 49, "num_actions wrong");
    fail_unless(num_inst == 542, "num_inst wrong");
    /* Post: model loaded - each entry matches a line in the file. */
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


START_TEST(test_save_context)
{
#ifdef NOTUSED
    int num_var = 13, pc = 19, sp = MAX_STACK_SIZE, accum = 233;
    FILE *expected;
    vm_context_t context;
    /* Pre: a model context exists. */
    setup_context(&context, pc, sp, accum, num_var, p1);

    /* Action. */
    save_context(&context, actions, num_actions, "actual.pid");

    /* Post. */
    expected = fopen("expect.pid", "w");
    fprintf(expected, "%s", p1_context);
    fclose(expected);

    if (system("diff expect.pid actual.pid > /dev/null") != 0) {
	fail("diff");
    } else {
	unlink("expect.pid");
	unlink("actual.pid");
    }
#endif
}
END_TEST

/* Try to save to non-existent directory. */
START_TEST(test_save_context_nodir)
{
    int num_var = 13, pc = 19, sp = MAX_STACK_SIZE, accum = 233;
    vm_context_t context;
    /* Pre: a model context exists. */
    setup_context(&context, pc, sp, accum, num_var, p1);

    /* Action. */
/*    fail_unless(save_context(&context, actions, num_actions, 
      "not_there/actual.pid") == -1, NULL);*/
}
END_TEST

START_TEST(test_load_context) 
{
#ifdef NOTUSED
    vm_context_t context;
    int i, num_var = 0, num_act = 3, pc = 19, sp = MAX_STACK_SIZE, accum = 233;

    /* Pre: a model context exists. */
    setup_context(&context,  pc, sp, accum, num_var, p1);
/*    save_context(&context, actions, num_actions, "test.pid");*/

    free(context.variables);
    context.variables = NULL;

    /* Action. */
/*    load_context(&context, &context.actions, &context.num_actions, "test.pid");*/

    /* Post: model loaded, context set. */
    fail_unless(pc == context.PC, "PC");
    fail_unless(sp == context.SP, "SP");
    fail_unless(accum == context.A, "A");

    mark_point();

    for (i = 0; i < context.SP; i++) {
	fail_unless(i == context.stack[i], "stack contents");
    }

    mark_point();

    for (i = 0; i < num_var; i++) {
	char buf[32];
	sprintf(buf, "%d", i);
	fail_unless(strcmp(buf, context.variables[i].name) == 0,
		    "var name");
	fail_unless(i = context.variables[i].value, "var val");
    }

    mark_point();

    for (i = 0; i < num_act; i++) {
	char buf[32];
	sprintf(buf, "%d", i);
	fail_unless(strcmp(buf, context.actions[i].name) == 0, "act name");
	fail_unless(ACT_NONE == context.actions[i].state, "act state");
    }

    mark_point();
    unlink("test.pid");
#endif
}
END_TEST

START_TEST(test_load_context_nofile) 
{
#ifdef NOTUSED
    vm_context_t context;
    /* Pre: none. */
    /* Action. */
    mark_point();
    fail_unless(load_context(&context, &actions, &num_actions, 
      "Imnotheretest.pid") == -1, NULL);

    /* Post: model not loaded, context not set. */
    fail_unless(-1 == context.PC, "PC");
    fail_unless(0 == context.SP, "SP");
    fail_unless(-1 == context.A, "A");

#endif
    mark_point();
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

    sprintf(cmd, "rm -r %s", instance_dir);
    system(cmd);
}

START_TEST(test_create_instance)
{
    int i, pid;
    char *model = "big";
    peos_context_t *context;
    for (i = 0; i < 10; i++) {
/*	struct stat b;
	char fname[BUFSIZ];
	peos_instance_t iname;*/

	/* XXX in future, pid will be != 0 */
	fail_unless((pid = peos_create_instance(model)) == 0, 
		    "failed to create instance");
	context = &(process_table[0]);
	fail_unless(context->vm_context.PC == 49, "PC wrong");
	fail_unless(context->vm_context.inst_array != NULL, "inst_array null");
	fail_unless(context->vm_context.num_inst == 542, "num_inst wrong");
	fail_unless(context->vm_context.A == -1, "A wrong");
	fail_unless(context->vm_context.SP == 0, "SP wrong");
	fail_unless(context->vm_context.stack[0] == 0, "");
	fail_unless(context->actions != NULL, "actions null");
	fail_unless(context->num_actions == 49, "num_actions wrogn");
#ifdef NOTUSED
	/* 
	 * Post: instance file exists.  Since the context is 
	 * established by load_instructions()/init_context() in 
	 * process.c, we don't have to verify the context.
	 */
	sprintf(fname, "%s/%d", instance_dir, i+1);
	if (stat(fname, &b) != 0) {
	    fail("no context file");
	}
#endif
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


/*
 * Happy path: first action (of 10) is running.
 */ 
START_TEST(test_finish_action)
{
    vm_exit_code status;
    peos_context_t *context = &process_table[0];
    /* Pre: a process is loaded; some action is RUNNING,
     * the process is waiting for the action to become DONE.
     */
    context->num_actions = 10;
    context->actions = make_actions(context->num_actions, ACT_NONE);
    context->actions[0].state = ACT_RUN;
    context->vm_context.SP = 0;
    context->vm_context.stack[context->vm_context.SP] = 0;

    context->vm_context.parameters.call = VM_WAIT;
    context->vm_context.parameters.act.state = ACT_DONE;
    context->vm_context.parameters.act.actions[0] = "act_0";
    context->vm_context.parameters.act.num_act = 1;
    current_process = context;

    /* Action. */
    status = handle_action_change("act_0", ACT_DONE);
    
    /* Post: selected action is DONE; process resumed -> next action
     * is READY.  
     */
    fail_unless(status == VM_DONE, "return value");
    fail_unless(context->actions[0].state == ACT_DONE, "action not DONE");
    fail_unless(context->vm_context.stack[context->vm_context.SP] == 1, "stack value");
    free_actions(context->actions, 10);
}
END_TEST

/*
 * Boundary: only one action, in RUNNING state.
 */ 
START_TEST(test_finish_action_one)
{
    vm_exit_code status;
    peos_context_t *context = &process_table[0];
    /* Pre: a process is loaded; some action is RUNNING,
     * the process is waiting, but not for the finished action.
     */
    context->num_actions = 1;
    context->actions = make_actions(context->num_actions, ACT_NONE);
    context->vm_context.SP = 0;
    context->vm_context.stack[context->vm_context.SP] = 0;

    /* VM is waiting for act_0 to finish.. */
    context->vm_context.parameters.call = VM_WAIT;
    context->vm_context.parameters.act.state = ACT_DONE;
    context->vm_context.parameters.act.actions[0] = "act_0";
    context->vm_context.parameters.act.num_act = 1;
    current_process = context;

    /* Action. */
    status = handle_action_change("act_0", ACT_DONE);
    
    /* Post: selected action is DONE; process resumed -> next action
     * is READY.  
     */
    fail_unless(context->actions[0].state == ACT_DONE, "action not DONE");
    fail_unless(status == VM_DONE, "return value");
    fail_unless(context->vm_context.stack[context->vm_context.SP] == 1, "stack value");
    free_actions(context->actions, 1);
}
END_TEST

/*
 * Boundary: last action (of two) in RUNNING state.
 */ 
START_TEST(test_finish_action_last)
{
    vm_exit_code status;
    peos_context_t *context = &process_table[0];
    /* Pre: a process is loaded; some action is RUNNING,
     * the process is waiting, but not for the finished action.
     */
    context->num_actions = 2;
    context->actions = make_actions(context->num_actions, ACT_NONE);
    context->actions[1].state = ACT_RUN;
    context->vm_context.SP = 0;
    context->vm_context.stack[context->vm_context.SP] = 0;

    /* VM is waiting for act_1 to finish.. */
    context->vm_context.parameters.call = VM_WAIT;
    context->vm_context.parameters.act.state = ACT_DONE;
    context->vm_context.parameters.act.actions[0] = "act_1";
    context->vm_context.parameters.act.num_act = 1;
    current_process = context;

    /* Action. */
    status = handle_action_change("act_1", ACT_DONE);
    
    /* Post: selected action is DONE; process resumed -> next action
     * is READY.  
     */
    fail_unless(context->actions[1].state == ACT_DONE, "action not DONE");
    fail_unless(status == VM_DONE, "return value");
    fail_unless(context->vm_context.stack[context->vm_context.SP] == 1, "stack value");
    free_actions(context->actions, 2);
}
END_TEST

START_TEST(test_finish_action_nowait)
{
    vm_exit_code status;
    peos_context_t *context = &process_table[0];
    /* Pre: a process is loaded; some action is RUNNING,
     * but the process is NOT waiting for anything.
     */
    context->num_actions = 10;
    context->actions = make_actions(context->num_actions, ACT_NONE);
    context->actions[0].state = ACT_RUN;
    context->vm_context.SP = 0;
    context->vm_context.stack[context->vm_context.SP] = 0;

    /* Somehow, the VM set the action to RUN, but didn't call wait. */
    context->vm_context.parameters.call = VM_SET;
    context->vm_context.parameters.act.state = ACT_RUN;
    context->vm_context.parameters.act.actions[0] = "act_0";
    context->vm_context.parameters.act.num_act = 1;
    current_process = context;

    /* Action. */
    status = handle_action_change("act_0", ACT_DONE);
    
    /* Post: selected action is DONE; process resumed -> next action
     * is READY.  
     */
    fail_unless(context->actions[0].state == ACT_DONE, "action not DONE");
    fail_unless(status == VM_ERROR, "return value");
    fail_unless(context->vm_context.stack[context->vm_context.SP] == 0, "stack value");
    free_actions(context->actions, 10);
}
END_TEST

START_TEST(test_finish_action_nowait2)
{
    vm_exit_code status;
    peos_context_t *context = &process_table[0];
    /* Pre: a process is loaded; some action is RUNNING,
     * the process is waiting, but not for the finished action.
     */
    context->num_actions = 10;
    context->actions = make_actions(context->num_actions, ACT_NONE);
    context->actions[0].state = ACT_RUN;
    context->vm_context.SP = 0;
    context->vm_context.stack[context->vm_context.SP] = 0;

    /* VM is waiting for act_1 to finish. */
    context->vm_context.parameters.call = VM_WAIT;
    context->vm_context.parameters.act.state = ACT_DONE;
    context->vm_context.parameters.act.actions[0] = "act_1";
    context->vm_context.parameters.act.num_act = 1;
    current_process = context;

    /* Action. */
    status = handle_action_change("act_0", ACT_DONE);
    
    /* Post: selected action is DONE; process resumed -> next action
     * is READY.  
     */
    fail_unless(context->actions[0].state == ACT_DONE, "action not DONE");
    fail_unless(status == VM_INTERNAL_ERROR, "return value");
    fail_unless(context->vm_context.stack[context->vm_context.SP] == 0, "stack value");
    free_actions(context->actions, 10);
}
END_TEST


START_TEST(test_finish_action_nowait3)
{
    vm_exit_code status;
    peos_context_t *context = &process_table[0];
    /* Pre: a process is loaded; some action is RUNNING,
     * the process is waiting, but not for the finished action.
     */
    
    context->num_actions = 10;
    context->actions = make_actions(context->num_actions, ACT_NONE);
    context->actions[0].state = ACT_RUN;
    context->vm_context.SP = 0;
    context->vm_context.stack[context->vm_context.SP] = 0;


    /* VM is waiting for act_0 to go ACTIVE, not DONE. */
    context->vm_context.parameters.call = VM_WAIT;
    context->vm_context.parameters.act.state = ACT_RUN;
    context->vm_context.parameters.act.actions[0] = "act_0";
    context->vm_context.parameters.act.num_act = 1;
    current_process = context;

    /* Action. */
    status = handle_action_change("act_0", ACT_DONE);
    
    /* Post: selected action is DONE; the process is waiting, but 
     * not for ACT_FINISH -> VM_ERROR.
     */
    fail_unless(context->actions[0].state == ACT_DONE, "action not DONE");
    fail_unless(status == VM_ERROR, "return value");
    fail_unless(context->vm_context.stack[context->vm_context.SP] == 0, "stack value");
    free_actions(context->actions, 10);
}
END_TEST

/*
 * Happy path: first action (of 10) is ready.
 */ 
START_TEST(test_run_action)
{
    vm_exit_code status;
    peos_context_t *context = &process_table[0];
    /* Pre: a process is loaded; some action is READY,
     * the process is waiting for the action to become RUNNIN.
     */
    context->num_actions = 10;
    context->actions = make_actions(context->num_actions, ACT_NONE);
    context->actions[0].state = ACT_READY;
    context->vm_context.SP = 0;
    context->vm_context.stack[context->vm_context.SP] = 0;

    context->vm_context.parameters.call = VM_WAIT;
    context->vm_context.parameters.act.state = ACT_RUN;
    context->vm_context.parameters.act.actions[0] = "act_0";
    context->vm_context.parameters.act.num_act = 1;
    current_process = context;

    /* Action. */
    status = handle_action_change("act_0", ACT_RUN);
    
    /* Post: selected action is DONE; process resumed -> next action
     * is READY.  
     */
    fail_unless(status == VM_DONE, "return value");
    fail_unless(context->actions[0].state == ACT_RUN, "action not RUNning");
    fail_unless(context->vm_context.stack[context->vm_context.SP] == 1, "stack value");
    free_actions(context->actions, 10);
}
END_TEST

/*
 * Happy path 2: first action (of 10) is ready, but VM waiting for
 * DONE.  This is the case for sequences, where compiler generates
 * code that skips the RUN state.
 */ 
START_TEST(test_run_action2)
{
    vm_exit_code status;
    peos_context_t *context = &process_table[0];
    /* Pre: a process is loaded; some action is READY,
     * the process is waiting for the action to become RUNNIN.
     */
    context->num_actions = 10;
    context->actions = make_actions(context->num_actions, ACT_NONE);
    context->actions[0].state = ACT_READY;
    context->vm_context.SP = 0;
    context->vm_context.stack[context->vm_context.SP] = 0;

    context->vm_context.parameters.call = VM_WAIT;
    context->vm_context.parameters.act.state = ACT_DONE;
    context->vm_context.parameters.act.actions[0] = "act_0";
    context->vm_context.parameters.act.num_act = 1;
    current_process = context;

    /* Action. */
    status = handle_action_change("act_0", ACT_RUN);
    
    /* Post: selected action is DONE; process resumed -> next action
     * is READY.  
     */
    fail_unless(status == VM_CONTINUE, "return value");
    fail_unless(context->actions[0].state == ACT_RUN, "action not RUNning");
    fail_unless(context->vm_context.stack[context->vm_context.SP] == 0, "stack value");
    free_actions(context->actions, 10);
}
END_TEST

/*
 * Boundary: only one action, in READY state.
 */ 
START_TEST(test_run_action_one)
{
    vm_exit_code status;
    peos_context_t *context = &process_table[0];
    /* Pre: a process is loaded; some action is READY,
     * the process is waiting, but not for the finished action.
     */
    context->num_actions = 1;
    context->actions = make_actions(context->num_actions, ACT_NONE);
    context->actions[0].state = ACT_READY;
    context->vm_context.SP = 0;
    context->vm_context.stack[context->vm_context.SP] = 0;

    /* VM is waiting for act_0 to finish.. */
    context->vm_context.parameters.call = VM_WAIT;
    context->vm_context.parameters.act.state = ACT_RUN;
    context->vm_context.parameters.act.actions[0] = "act_0";
    context->vm_context.parameters.act.num_act = 1;
    current_process = context;

    /* Action. */
    status = handle_action_change("act_0", ACT_RUN);
    
    /* Post: selected action is DONE; process resumed -> next action
     * is READY.  
     */
    fail_unless(context->actions[0].state == ACT_RUN, "action not DONE");
    fail_unless(status == VM_DONE, "return value");
    fail_unless(context->vm_context.stack[context->vm_context.SP] == 1, "stack value");
    free_actions(context->actions, 1);
}
END_TEST

/*
 * Boundary: last action (of two) in READY state.
 */ 
START_TEST(test_run_action_last)
{
    vm_exit_code status;
    peos_context_t *context = &process_table[0];
    /* Pre: a process is loaded; some action is READY,
     * the process is waiting, but not for the finished action.
     */
    context->num_actions = 2;
    context->actions = make_actions(context->num_actions, ACT_NONE);
    context->actions[1].state = ACT_READY;
    context->vm_context.SP = 0;
    context->vm_context.stack[context->vm_context.SP] = 0;

    /* VM is waiting for act_1 to finish.. */
    context->vm_context.parameters.call = VM_WAIT;
    context->vm_context.parameters.act.state = ACT_RUN;
    context->vm_context.parameters.act.actions[0] = "act_1";
    context->vm_context.parameters.act.num_act = 1;
    current_process = context;

    /* Action. */
    status = handle_action_change("act_1", ACT_RUN);
    
    /* Post: selected action is DONE; process resumed -> next action
     * is READY.  
     */
    fail_unless(context->actions[1].state == ACT_RUN, "action not DONE");
    fail_unless(status == VM_DONE, "return value");
    fail_unless(context->vm_context.stack[context->vm_context.SP] == 1, "stack value");
    free_actions(context->actions, 2);
}
END_TEST

START_TEST(test_run_action_nowait)
{
    vm_exit_code status;
    peos_context_t *context = &process_table[0];
    /* Pre: a process is loaded; some action is READY,
     * but the process is NOT waiting for anything.
     */
    context->num_actions = 10;
    context->actions = make_actions(context->num_actions, ACT_NONE);
    context->actions[0].state = ACT_READY;
    context->vm_context.SP = 0;
    context->vm_context.stack[context->vm_context.SP] = 0;

    /* Somehow, the VM set the action to RUN, but didn't call wait. */
    context->vm_context.parameters.call = VM_SET;
    context->vm_context.parameters.act.state = ACT_RUN;
    context->vm_context.parameters.act.actions[0] = "act_0";
    context->vm_context.parameters.act.num_act = 1;
    current_process = context;

    /* Action. */
    status = handle_action_change("act_0", ACT_RUN);
    
    /* Post: selected action is DONE; process resumed -> next action
     * is READY.  
     */
    fail_unless(context->actions[0].state == ACT_RUN, "action not DONE");
    fail_unless(status == VM_ERROR, "return value");
    fail_unless(context->vm_context.stack[context->vm_context.SP] == 0, "stack value");
    free_actions(context->actions, 10);
}
END_TEST

START_TEST(test_run_action_nowait2)
{
    vm_exit_code status;
    peos_context_t *context = &process_table[0];
    /* Pre: a process is loaded; some action is READY,
     * the process is waiting, but not for the finished action.
     */
    context->num_actions = 10;
    context->actions = make_actions(context->num_actions, ACT_NONE);
    context->actions[0].state = ACT_READY;
    context->vm_context.SP = 0;
    context->vm_context.stack[context->vm_context.SP] = 0;

    /* VM is waiting for act_1 to finish. */
    context->vm_context.parameters.call = VM_WAIT;
    context->vm_context.parameters.act.state = ACT_RUN;
    context->vm_context.parameters.act.actions[0] = "act_1";
    context->vm_context.parameters.act.num_act = 1;
    current_process = context;

    /* Action. */
    status = handle_action_change("act_0", ACT_RUN);
    
    /* Post: selected action is DONE; process resumed -> next action
     * is READY.  
     */
    fail_unless(context->actions[0].state == ACT_RUN, "action not DONE");
    fail_unless(status == VM_INTERNAL_ERROR, "return value");
    fail_unless(context->vm_context.stack[context->vm_context.SP] == 0, "stack value");
    free_actions(context->actions, 10);
}
END_TEST


START_TEST(test_resume)
{
    int num_var = 0, pc = 3, sp = 0, accum = -1;

    /* Pre: a model context exists. */
    setup_context(&(process_table[0].vm_context), pc, sp, accum, num_var, p1);

    /* Post: VM_DONE, for stubbed execute(); */
    fail_unless(peos_resume(0) == VM_DONE, NULL);
}
END_TEST

START_TEST(test_resume_bound1)
{
    int num_var = 0, pc = 3, sp = 0, accum = -1;

    /* Pre: a model context exists. */
    setup_context(&(process_table[0].vm_context), pc, sp, accum, num_var, p1);

    /* Post: VM_DONE, for stubbed execute(); */
    fail_unless(peos_resume(-1) == VM_INTERNAL_ERROR, NULL);
}
END_TEST

START_TEST(test_resume_bound2)
{
    int num_var = 0, pc = 3, sp = 0, accum = -1;

    /* Pre: a model context exists. */
    setup_context(&(process_table[0].vm_context), pc, sp, accum, num_var, p1);

    /* Post: VM_DONE, for stubbed execute(); */
    fail_unless(peos_resume(1) == VM_INTERNAL_ERROR, NULL);
}
END_TEST



int
main(int argc, char *argv[])
{
    int nf;
    SRunner *sr;
    Suite *s = suite_create("process");
    TCase *tc = tcase_create("io");

    parse_args(argc, argv);

    suite_add_tcase(s, tc);

    tcase_add_test(tc, test_load_instructions);
    tcase_add_test(tc, test_save_context);
    tcase_add_test(tc, test_save_context_nodir);
    tcase_add_test(tc, test_load_context);
    tcase_add_test(tc, test_load_context_nofile);

    tc = tcase_create("context");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_init_context);

    tc = tcase_create("create_instance");
    tcase_add_unchecked_fixture(tc, setup_create_instance,
			      teardown_create_instance);
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_create_instance);
    tcase_add_test(tc, test_create_instance_noexist);

    tc = tcase_create("resume");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_resume);
    tcase_add_test(tc, test_resume_bound1);
    tcase_add_test(tc, test_resume_bound2);

    tc = tcase_create("finish_action");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_finish_action);
    tcase_add_test(tc, test_finish_action_one);
    tcase_add_test(tc, test_finish_action_last);
    tcase_add_test(tc, test_finish_action_nowait);
    tcase_add_test(tc, test_finish_action_nowait2);
    tcase_add_test(tc, test_finish_action_nowait3);

    tc = tcase_create("run_action");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_run_action);
    tcase_add_test(tc, test_run_action2);
    tcase_add_test(tc, test_run_action_one);
    tcase_add_test(tc, test_run_action_last);
    tcase_add_test(tc, test_run_action_nowait);
    tcase_add_test(tc, test_run_action_nowait2);


    sr = srunner_create(s);

    srunner_set_fork_status(sr, fork_status);

    srunner_run_all(sr, verbosity);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    suite_free(s);
    return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

