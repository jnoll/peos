#include <unistd.h>		/* unlink() */
#include <sys/stat.h>		/* mkdir() */
#include <sys/types.h>		/* mkdir() */
#include <check.h>
#include "process_table.h"

#define PROCESS_TABLE
#include "test_util.h"

/* Globals. */
/* NONE! */

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

START_TEST(test_read_field_data) 
{
    char *field, buf[BUFSIZ], *contents = "these are the field contents";
    int len;

    sprintf(buf, " { %s } \n", contents);
    field = read_field_data(buf, &len);
    fail_unless(len == strlen(contents), "field length");
    fail_unless(strncmp(field, contents, strlen(contents)) == 0, "field contents");
    
}
END_TEST

START_TEST(test_add_act)
{
    int size, num = 0;
    peos_action_t *actions;
    char buf[BUFSIZ], *name = "an_action";
    /* Pre: action array exists, buffer loaded with action. */
    size = 0;
    actions = (peos_action_t *)calloc(size, sizeof(peos_action_t));
    /* The buffer already has line numbers stripped. */
    sprintf(buf, "%s type action mode manual", name);

    /* Action. */
    actions = add_act(actions, buf, num, &size);

    /* Post: a new action at location num. */
    fail_unless(strcmp(actions[num].name, name) == 0, "action name"); 
    fail_unless(actions[num].state == ACT_NONE, "action state");
    fail_unless(size >= num, "array size");
}
END_TEST

START_TEST(test_add_act_realloc)
{
    int i, size, num = 0;
    peos_action_t *actions;
    char buf[BUFSIZ], *name = "an_action";
    /* Pre: action array exists, buffer loaded with action. */
    size = 9;
    num = size + 1;
    actions = (peos_action_t *)calloc(size+1, sizeof(peos_action_t));
    
    for (i = 0;  i < num; i++) {
	/* The buffer already has line numbers stripped. */
	sprintf(buf, "%s type action mode manual", name);

	/* Action. */
	actions = add_act(actions, buf, i, &size);
	/* Post: a new action at location num. */
	fail_unless(strcmp(actions[i].name, name) == 0, "action name"); 
	fail_unless(actions[i].state == ACT_NONE, "action state");
	fail_unless(size >= i, "array size");
    }
}
END_TEST

START_TEST(test_add_act_script)
{
    int size, num = 0;
    peos_action_t *actions;
    char buf[BUFSIZ], *name = "an_action",
	*script = "add enough ice to shaker so that the gin will just barely cover it.";

    /* Pre: action array exists, buffer loaded with action. */
    size = 0;
    actions = (peos_action_t *)calloc(size, sizeof(peos_action_t));
    /* The buffer already has line numbers stripped. */
    sprintf(buf, "%s type action mode manual script { %s }", name, script);

    /* Action. */
    actions = add_act(actions, buf, num, &size);

    /* Post: a new action at location num. */
    fail_unless(strcmp(actions[num].name, name) == 0, "action name"); 
    fail_unless(actions[num].state == ACT_NONE, "action state");
    fail_unless(size >= num, "array size");
    fail_unless(strcmp(actions[num].script, script) == 0, "script");
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
    int i, j, nbytes, abytes;
    char expected[BUFSIZ], actual[BUFSIZ];
    FILE *f;

    /* Pre: process table initialized. */
    peos_context_t *context = &(process_table[0]);

    f = fopen("expected_proc_table.dat", "w");
    for (j = 0; j < PEOS_MAX_PID+1; j++) {
	context = &(process_table[j]);
	sprintf(context->model, "test.cpml");
	fprintf(f, "pid: %d\nmodel: %s\n", j, context->model);
	context->status = PEOS_RUNNING;
	fprintf(f, "status: %d\n", context->status);

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
/*    unlink("proc_table.dat");
      unlink("expected_proc_table.dat");*/
}
END_TEST


START_TEST(test_load_proc_table) 
{
    int i, j;
    char  *model = "test.cpml";
    peos_context_t ctx;
    FILE *f;
    char *p_txt = \
"0 an_action0 type action mode manual requires { a } provides { a } script { test script }\n\
1 an_action1 type action mode manual requires { a } provides { a } script { test script }\n\
2 an_action2 type action mode manual requires { a } provides { a } script { test script }\n\
3 start\n\
4 end\n";

    /* Pre: saved process table file. */
    peos_context_t *context = &(ctx);

    f = fopen("proc_table.dat", "w");
    for (j = 0; j <= PEOS_MAX_PID; j++) {
	sprintf(context->model, model);
	fprintf(f, "pid: %d\nmodel: %s\n", j, context->model);
	context->status = PEOS_RUNNING;
	fprintf(f, "status: %d\n", context->status);
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
	    context->actions[i].script = "test script";
	    fprintf(f, " %s %d", context->actions[i].name, context->actions[i].state); 
	}
	fprintf(f, "\n\n"); 
    }
    fclose(f);
    mark_point();

    memset(process_table, 0, (PEOS_MAX_PID+1) * sizeof(peos_context_t));
    mark_point();

    /* Load_proc_table requires a real model file to load actions from. */
    f = fopen(model, "w");
    fprintf(f, "%s", p_txt);
    fclose(f);

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
	    fail_unless(context->actions[i].pid == j, "action pid");
	    fail_unless(context->actions[i].state == ACT_NONE, "act state");
	    fail_unless(strcmp(context->actions[i].script, "test script") == 0, "act script");
	}
    }

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

    tc = tcase_create("add act");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_read_field_data);
    tcase_add_test(tc, test_add_act);
    tcase_add_test(tc, test_add_act_realloc);
    tcase_add_test(tc, test_add_act_script);



    tc = tcase_create("table io");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_load_instructions);
    tcase_add_test(tc, test_load_proc_table);
    tcase_add_test(tc, test_save_proc_table);

    tc = tcase_create("init context");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_init_context);

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
