/* 
 * Unit tests for vm.c.
 */
#include <stdio.h>
#include <check.h>

#include "vm.h"
#include "process.h"
#include "test_util.h"

/* Stub (bogus) globals. */

/* process.c */
peos_action_t *actions = NULL;
int num_actions;

void
set_context(vm_context_t *context, int pc)
{
    context->PC = pc;
    context->A = -1;
    context->SP = 0;
    PC = SP = -1;		/* Meaningless value to test context mech. */
}

int
push_ctx(vm_context_t *context, int val)
{
    return (context->stack[++context->SP] = val);
}

int
load_inst(vm_context_t *context)
{
    int i = 0, size = INST_ARRAY_INCR;

    inst_array = (char **) calloc(size, sizeof(char *));

    inst_array[i++] = "a type action mode manual requires { a } provides { a }";
    inst_array[i++] = "b type action mode manual requires { a } provides { a }";
    inst_array[i++] = "c type action mode manual requires { a } provides { a }";
    set_context(context, i);
    PC = i;
    inst_array[i++] = "start";
    inst_array[i++] = "call select &&id==$a";
    inst_array[i++] = "pop ";
    inst_array[i++] = "call set ready a";
    inst_array[i++] = "jzero 35";
    inst_array[i++] = "pop";
    inst_array[i++] = "call wait done a";
    inst_array[i++] = "jzero 35";
    inst_array[i++] = "pop";
    inst_array[i++] = "call select &&id==$a";
    inst_array[i++] = "pop ";
    inst_array[i++] = "call select &&id==$a";
    inst_array[i++] = "pop ";
    inst_array[i++] = "call set ready b";
    inst_array[i++] = "jzero 35";
    inst_array[i++] = "pop";
    inst_array[i++] = "call wait done b";
    inst_array[i++] = "jzero 35";
    inst_array[i++] = "pop";
    inst_array[i++] = "call select &&id==$a";
    inst_array[i++] = "pop ";
    inst_array[i++] = "call select &&id==$a";
    inst_array[i++] = "pop ";
    inst_array[i++] = "call set ready c";
    inst_array[i++] = "jzero 35";
    inst_array[i++] = "pop";
    inst_array[i++] = "call wait done c";
    inst_array[i++] = "jzero 35";
    inst_array[i++] = "pop";
    inst_array[i++] = "call select &&id==$a";
    inst_array[i++] = "pop ";
    inst_array[i++] = "end";
    inst_array[i++] = "call error";
    inst_array[i] = NULL;
    num_inst = i;

    context->inst_array = inst_array;
    context->num_inst = num_inst;
    
    return i;
}

START_TEST(test_nextOPCode)
{
    vm_context_t context;
    char opbuf[BUFSIZ]; /* BUF_SIZ is overkill, to be sure. */

    int i = 0, j;
    
    /* Preconditions: process loaded into opcode array, PC set. */
    i = load_inst(&context);

    for (j = PC; j < i; j++) {
        /* Action: return opcode at PC, increment PC. */
	if (nextOPCode(opbuf) == VM_INTERNAL_ERROR) {
	    fail("no opcode returned");
	    break;
	}
        /* Post: actual == inst_arry[PC]; PC = PC+1. */    
	if (strcmp(inst_array[j], opbuf) != 0) {
	    fail("instruction compare");
	    break;
	}
    }
    fail_unless (j == i, NULL);
}
END_TEST

START_TEST(test_addvar)
{
}
END_TEST

START_TEST(test_dereference)
{
}
END_TEST

START_TEST(test_push)
{
}
END_TEST

START_TEST(test_pop)
{
}
END_TEST

static int load_trivial(vm_context_t *context)
{
    int i = 0, size = INST_ARRAY_INCR;

    context->inst_array = (char **) calloc(size, sizeof(char *));

    set_context(context, i);
    context->inst_array[i++] = "start";
    context->inst_array[i++] = "end";
    context->num_inst = i;

    return i;
}

static int load_incr(vm_context_t *context)
{
    int i = 0, size = INST_ARRAY_INCR;

    context->inst_array = (char **) calloc(size, sizeof(char *));

    set_context(context, i);
    context->inst_array[i++] = "start";
    context->inst_array[i++] = "push 0";
    context->inst_array[i++] = "incr";
    context->inst_array[i++] = "pop";
    context->inst_array[i++] = "end";
    context->num_inst = i;

    return i;
}


static int load_decr(vm_context_t *context)
{
    int i = 0, size = INST_ARRAY_INCR;

    context->inst_array = (char **) calloc(size, sizeof(char *));

    set_context(context, i);
    context->inst_array[i++] = "start";
    context->inst_array[i++] = "push 2";
    context->inst_array[i++] = "decr";
    context->inst_array[i++] = "pop";
    context->inst_array[i++] = "end";
    context->num_inst = i;

    return i;
}

static int load_goto(vm_context_t *context)
{
    int i = 0, size = INST_ARRAY_INCR;

    context->inst_array = (char **) calloc(size, sizeof(char *));

    set_context(context, i);
    context->inst_array[i++] = "start";
    context->inst_array[i++] = "push 0";
    context->inst_array[i++] = "goto 4";
    context->inst_array[i++] = "incr";
    context->inst_array[i++] = "incr";
    context->inst_array[i++] = "end";
    context->num_inst = i;

    return i;
}

static int load_jzero(vm_context_t *context)
{
    int i = 0, size = INST_ARRAY_INCR;

    context->inst_array = (char **) calloc(size, sizeof(char *));

    set_context(context, i);
    context->inst_array[i++] = "start";
    context->inst_array[i++] = "push 0";
    context->inst_array[i++] = "jzero 4";
    context->inst_array[i++] = "incr";
    context->inst_array[i++] = "incr";
    context->inst_array[i++] = "end";
    context->num_inst = i;

    return i;
}

static int load_jump(vm_context_t *context)
{
    int i = 0, size = INST_ARRAY_INCR;

    context->inst_array = (char **) calloc(size, sizeof(char *));

    set_context(context, i);
    context->inst_array[i++] = "start";
    context->inst_array[i++] = "push 0";
    context->inst_array[i++] = "push 0";
    context->inst_array[i++] = "jump 5";
    context->inst_array[i++] = "incr";
    context->inst_array[i++] = "incr";
    context->inst_array[i++] = "end";
    context->num_inst = i;

    return i;
}

static int load_call(char *call, vm_context_t *context)
{
    int i = 0, size = INST_ARRAY_INCR;
    static char op_buf[BUFSIZ];

    sprintf(op_buf, "call %s", call);
    context->inst_array = (char **) calloc(size, sizeof(char *));

    set_context(context, i);
    context->inst_array[i++] = "start";
    context->inst_array[i++] = "push 0";
    context->inst_array[i++] = op_buf;
    context->inst_array[i++] = "jzero 5";
    context->inst_array[i++] = "push 2";
    context->inst_array[i++] = "pop"; /* inst 5: loads AC w/TOS, which will
                                    be 2 if there are no errors. */
    context->inst_array[i++] = "end";
    context->num_inst = i;

    return i;
}



START_TEST(test_execute1)
{
    vm_context_t context;
    vm_exit_code result; 

    /* Preconditions: inst_array loaded; PC, SP set; */
    load_trivial(&context);
    /* Action. */
    mark_point();
    result = execute(&context);
    /* Postcondition: return == DONE. */
    fail_unless(VM_DONE == result, "trivial");
}
END_TEST


START_TEST(test_execute2)
{
    vm_context_t context;
    vm_exit_code result; 

    load_incr(&context);
    mark_point();
    result = execute(&context);
    /* Postcondition: return == DONE. */
    fail_unless(VM_DONE == result, "incr: exit status");
    fail_unless(1 == A, "incr: AC contents");
}
END_TEST


START_TEST(test_execute3)
{
    vm_context_t context;
    vm_exit_code result; 

    load_decr(&context);
    mark_point();
    result = execute(&context);
    /* Postcondition: return == DONE. */
    fail_unless(VM_DONE == result, "decr: exit status");
    fail_unless(1 == A, "decr: AC contents");
}
END_TEST

START_TEST(test_execute4)
{
    vm_context_t context;
    vm_exit_code result; 

    load_goto(&context);
    mark_point();
    result = execute(&context);
    /* Postcondition: return == DONE. */
    fail_unless(VM_DONE == result, "goto: exit status");
    fail_unless(1 == A, "goto: AC contents");
}
END_TEST


START_TEST(test_execute5)
{
    vm_context_t context;
    vm_exit_code result; 

    load_jzero(&context);
    mark_point();
    result = execute(&context);
    /* Postcondition: return == DONE. */
    fail_unless(VM_DONE == result, "jzero: exit status");
    fail_unless(1 == A, "jzero: AC contents");
}
END_TEST

START_TEST(test_execute6)
{
    vm_context_t context;
    vm_exit_code result; 

    load_jump(&context);
    mark_point();
    result = execute(&context);
    /* Postcondition: return == DONE. */
    fail_unless(VM_DONE == result, "jump: exit status");
    fail_unless(1 == A, "jump: AC contents");
}
END_TEST


START_TEST(test_execute7)
{
    vm_context_t context;
    vm_exit_code result; 

    load_call("set ready act1", &context);
    mark_point();
    result = execute(&context);
    fail_unless(VM_SYSCALL == result, "set: exit status");
    fail_unless(-1 == A, "set: AC contents");
    fail_unless(VM_SET == context.parameters.call, "set: opcode");
    fail_unless(ACT_READY == context.parameters.act.state,
		    "set: act_state");
    fail_unless(1 == context.parameters.act.num_act,
		    "set: num_act");
    /* Run again. */
    push_ctx(&context, 1);
    mark_point();
    result = execute(&context);
    fail_unless(VM_DONE == result, "set: exit status");
    fail_unless(2 == A, "set: AC contents");
}
END_TEST

START_TEST(test_execute8)
{
    vm_context_t context;
    vm_exit_code result; 

    load_call("select ID == file:///etc/resolv.conf", &context);
    mark_point();
    result = execute(&context);
    fail_unless(VM_SYSCALL == result, "select:: exit status");
    fail_unless(-1 == A, "select: AC contents");
    fail_unless(VM_SELECT == context.parameters.call, "select: opcode");
    if (strcmp("ID == file:///etc/resolv.conf",  context.parameters.query) != 0) {
		fail("select: query");
    }
    /* Run again. */
    push_ctx(&context, 1);
    mark_point();
    result = execute(&context);
    fail_unless(VM_DONE == result, "select exit status");
    fail_unless(2 == A, "select: AC contents");
}
END_TEST

START_TEST(test_execute9)
{
    vm_context_t context;
    vm_exit_code result; 

    load_call("assert ID == file:///etc/resolv.conf", &context);
    result = execute(&context);
    fail_unless(VM_SYSCALL == result, "assert: exit status");
    fail_unless(-1 == A, "assert: AC contents");
    fail_unless(VM_ASSERT == context.parameters.call, "assert: opcode");
    if (strcmp("ID == file:///etc/resolv.conf",  context.parameters.query) != 0) {
	fail("select: query");
    }
    push_ctx(&context, 1);
    mark_point();
    result = execute(&context);
    fail_unless(VM_DONE == result, "assert: exit status");
    fail_unless(2 == A, "assert: AC contents");
}
END_TEST

START_TEST(test_execute10)
{
    vm_context_t context;
    vm_exit_code result; 

    load_call("wait done act1", &context);
    mark_point();
    result = execute(&context);
    fail_unless(VM_SYSCALL == result, "wait: exit status");
    fail_unless(-1 == A, "wait: AC contents");
    fail_unless(VM_WAIT == context.parameters.call, "wait: opcode");
    fail_unless(ACT_DONE == context.parameters.act.state, "set: act_state");

    push_ctx(&context, 1);
    mark_point();
    result = execute(&context);
    fail_unless(VM_DONE == result, "wait: exit status");
    fail_unless(2 == A, "wait: AC contents");
}
END_TEST

START_TEST(test_execute11)
{
    vm_context_t context;
    vm_exit_code result; 

    load_call("fork 1", &context);
    mark_point();
    result = execute(&context);
    fail_unless(VM_SYSCALL == result, "fork: exit status");
    fail_unless(-1 == A, "fork: AC contents");
    fail_unless(VM_FORK == context.parameters.call, "fork: opcode");
    fail_unless(1 == context.parameters.line, "fork: line");
    /* Run again. */
    push_ctx(&context, 2);
    mark_point();
    result = execute(&context);
    fail_unless(VM_DONE == result, "fork: exit status");
    fail_unless(2 == A, "fork: AC contents");
}
END_TEST

START_TEST(test_execute12)
{
    vm_context_t context;
    vm_exit_code result; 

    load_call("join", &context);
    mark_point();
    result = execute(&context);
    fail_unless(VM_SYSCALL == result, "join: exit status");
    fail_unless(-1 == A, "join: AC contents");
    fail_unless(VM_JOIN == context.parameters.call, "join: opcode");
    /* Run again. */
    push_ctx(&context, 2);
    mark_point();
    result = execute(&context);
    fail_unless(VM_DONE == result, "join: exit status");
    fail_unless(2 == A, "join: AC contents");
}
END_TEST

START_TEST(test_execute13)
{
    vm_context_t context;
    vm_exit_code result; 

    load_call("exit", &context);
    mark_point();
    result = execute(&context);
    fail_unless(VM_SYSCALL == result, "exit: exit status");
    fail_unless(-1 == A, "exit: AC contents");
    fail_unless(VM_EXIT == context.parameters.call, "exit: opcode");

    /* Run again. */
    push_ctx(&context, 1);
    mark_point();
    result = execute(&context);
    fail_unless(VM_DONE == result, "exit: exit status");
    fail_unless(2 == A, "exit: AC contents");
}
END_TEST

/* Test goto beyond end of instructions. */
START_TEST(test_goto_outbound)
{
    int i = 0, size = INST_ARRAY_INCR;
    vm_context_t context;
    vm_exit_code result; 

    context.inst_array = (char **) calloc(size, sizeof(char *));

    i = 0;
    set_context(&context, i);
    context.inst_array[i++] = "start";
    context.inst_array[i++] = "push 6";
    context.inst_array[i++] = "goto 5";
    context.inst_array[i++] = "incr";
    context.inst_array[i++] = "end";

    context.num_inst = i;

    mark_point();
    result = execute(&context);
    /* Postcondition: error.. */
    fail_unless(VM_INTERNAL_ERROR == result, "goto: exit status");
    fail_unless(6 == stack[SP], "goto: TOS");

    free(context.inst_array);
}
END_TEST



int main(int argc, char *argv[])
{
    int nf;
    SRunner *sr;
    Suite *s = suite_create("vm");
    TCase *tc;

    parse_args(argc, argv);

    tc = tcase_create("vm_util");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_nextOPCode);
    tcase_add_test(tc, test_addvar);
    tcase_add_test(tc, test_dereference);
    tcase_add_test(tc, test_push);
    tcase_add_test(tc, test_pop);

    tc = tcase_create("execute");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_execute1);
    tcase_add_test(tc, test_execute2);
    tcase_add_test(tc, test_execute3);
    tcase_add_test(tc, test_execute4);
    tcase_add_test(tc, test_execute5);
    tcase_add_test(tc, test_execute6);
    tcase_add_test(tc, test_execute7);
    tcase_add_test(tc, test_execute8);
    tcase_add_test(tc, test_execute9);
    tcase_add_test(tc, test_execute10);
    tcase_add_test(tc, test_execute11);
    tcase_add_test(tc, test_execute12);
    tcase_add_test(tc, test_execute13);
    tcase_add_test(tc, test_goto_outbound);

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

