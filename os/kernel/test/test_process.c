#include <check.h>
#include <stdlib.h>
#include <unistd.h>		/* unlink() */
#include <sys/stat.h>		/* mkdir() */
#include <sys/types.h>		/* mkdir() */
#include "process.h"
#include "test_util.h"

/* Globals. */
char *instance_dir = NULL;
int execute_status = VM_DONE;	/* What execute() should return. */
peos_context_t process_table[PEOS_MAX_PID+1];

/* Stubs. */

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
    return &(process_table[0]);	/* XXX This is a single threaded system now. */
}

int execute(vm_context_t *context) 
{
    return execute_status;
}

int load_instructions(char *file, char ***inst, int *num_inst,
			 peos_action_t **actions, int *num_actions)
{
    return stub_load_instructions(file, inst, num_inst, actions, num_actions);
}

int init_context(vm_context_t *context, int pc) 
{
    return stub_init_context(context, pc);
}




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



/* This is really a silly test, since create instance just calls functions 
   mostly from process_table.c. */
START_TEST(test_create_instance)
{
    int pid;
    char *model = TEST_PROC_NAME;
    peos_context_t *context;

    memset(process_table, 0, PEOS_MAX_PID + 1);

    fail_unless((pid = peos_create_instance(model)) == 0, 
		"failed to create instance");
    context = &(process_table[0]);
    fail_unless(context->status == PEOS_READY, "process status");
    /* The instruction array is set by load_instructions. */
    fail_unless(context->vm_context.inst_array != NULL, "inst_array null");
    fail_unless(context->vm_context.num_inst == test_proc_size, "num_inst wrong");
    fail_unless(context->actions != NULL, "actions null");
    fail_unless(context->num_actions == test_proc_num_act, "num_actions wrong");
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
    sprintf(context->vm_context.parameters.act.actions[0], "act_0");
    context->vm_context.parameters.act.num_act = 1;

    /* Action. */
    status = handle_action_change(0, "act_0", ACT_DONE);
    
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
    sprintf(context->vm_context.parameters.act.actions[0], "act_0");
    context->vm_context.parameters.act.num_act = 1;

    /* Action. */
    status = handle_action_change(0, "act_0", ACT_DONE);
    
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
    sprintf(context->vm_context.parameters.act.actions[0], "act_1");
    context->vm_context.parameters.act.num_act = 1;

    /* Action. */
    status = handle_action_change(0, "act_1", ACT_DONE);
    
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
    sprintf(context->vm_context.parameters.act.actions[0], "act_0");
    context->vm_context.parameters.act.num_act = 1;

    /* Action. */
    status = handle_action_change(0, "act_0", ACT_DONE);
    
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
    sprintf(context->vm_context.parameters.act.actions[0], "act_1");
    context->vm_context.parameters.act.num_act = 1;

    /* Action. */
    status = handle_action_change(0, "act_0", ACT_DONE);
    
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
    sprintf(context->vm_context.parameters.act.actions[0], "act_0");
    context->vm_context.parameters.act.num_act = 1;

    /* Action. */
    status = handle_action_change(0, "act_0", ACT_DONE);
    
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
     * the process is waiting for the action to become RUNNING.
     */
    context->num_actions = 10;
    context->actions = make_actions(context->num_actions, ACT_NONE);
    context->actions[0].state = ACT_READY;
    context->vm_context.SP = 0;
    context->vm_context.stack[context->vm_context.SP] = 0;

    context->vm_context.parameters.call = VM_WAIT;
    context->vm_context.parameters.act.state = ACT_RUN;
    sprintf(context->vm_context.parameters.act.actions[0], "act_0");
    context->vm_context.parameters.act.num_act = 1;

    /* Action. */
    status = handle_action_change(0, "act_0", ACT_RUN);
    
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
    sprintf(context->vm_context.parameters.act.actions[0], "act_0");
    context->vm_context.parameters.act.num_act = 1;

    /* Action. */
    status = handle_action_change(0, "act_0", ACT_RUN);
    
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
    sprintf(context->vm_context.parameters.act.actions[0], "act_0");
    context->vm_context.parameters.act.num_act = 1;

    /* Action. */
    status = handle_action_change(0, "act_0", ACT_RUN);
    
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
    sprintf(context->vm_context.parameters.act.actions[0], "act_1");
    context->vm_context.parameters.act.num_act = 1;

    /* Action. */
    status = handle_action_change(0, "act_1", ACT_RUN);
    
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
    sprintf(context->vm_context.parameters.act.actions[0], "act_0");
    context->vm_context.parameters.act.num_act = 1;

    /* Action. */
    status = handle_action_change(0, "act_0", ACT_RUN);
    
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
    sprintf(context->vm_context.parameters.act.actions[0], "act_1");
    context->vm_context.parameters.act.num_act = 1;

    /* Action. */
    status = handle_action_change(0, "act_0", ACT_RUN);
    
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
    peos_context_t *context;
    /* Pre: a model context exists. */
    context = &(process_table[0]);
    setup_context(&(context->vm_context), pc, sp, accum, num_var, p1);

    /* Post: VM_DONE, for stubbed execute(); */
    fail_unless(peos_resume(0) == VM_DONE, NULL);
    fail_unless(context->status & PEOS_DONE, "process status");
}
END_TEST

START_TEST(test_resume_bound1)
{
    int num_var = 0, pc = 3, sp = 0, accum = -1;
    peos_context_t *context;

    /* Pre: a model context exists. */
    context = &(process_table[0]);
    setup_context(&(context->vm_context), pc, sp, accum, num_var, p1);

    /* Post: VM_DONE, for stubbed execute(); */
    fail_unless(peos_resume(-1) == VM_INTERNAL_ERROR, NULL);
}
END_TEST

START_TEST(test_resume_bound2)
{
    int num_var = 0, pc = 3, sp = 0, accum = -1;
    peos_context_t *context;

    /* Pre: a model context exists. */
    context = &(process_table[0]);
    setup_context(&(context->vm_context), pc, sp, accum, num_var, p1);

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
    TCase *tc;

    parse_args(argc, argv);

    tc = tcase_create("io");
    tcase_add_test(tc, test_find_model_file_default);
    tcase_add_test(tc, test_find_model_file);

    tc = tcase_create("create_instance");
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
