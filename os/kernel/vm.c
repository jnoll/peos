/*****************************************************************
 * File:        vm.c
 * Author:      Crystal Zhu 
 * Date:        11/17/1999 
 * Description: Virtual machine. 
 *****************************************************************/   

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "vm.h"

/* Global variables. */
int PC = 0, SP = INT_MAX, A = 0; /* A is a register */
int *stack;
char **inst_array;		/* Process instructions. */
int num_inst = 0;
vm_vbinding_t *variables;	/* Variable bindings. */
int num_variables;
vm_syscall_param_t *parameters;
char op[256];

/* Forward declarations. */
vm_exit_code nextOPCode(char*);
int pop();
void add_var(char* name, int value);
int dereference(char* string);

int system_call(char *inst)
{
    int ret = VM_SYSCALL;
    char *op;
    op = strtok(inst, " ");	/* skip 'call' */
    assert (strcmp(op, "call") == 0);

    op = strtok(0, " ");
    if (strncmp(op, "set", strlen("set")) == 0 ||
	strncmp(op, "wait", strlen("wait")) == 0) {
	char * token;

	if (strncmp(op, "set", strlen("set")) == 0) {
	    parameters->call = VM_SET;
	} else if (strncmp(op, "wait", strlen("wait")) == 0) {
	    parameters->call = VM_WAIT;
	}

	token = strtok(0, " \n\t"); /* state */

	if (strcmp(token, "new") == 0) {
	    parameters->act.state = ACT_NEW;
	} else if (strcmp(token, "ready") == 0) {
	    parameters->act.state = ACT_READY;
	} else if (strcmp(token, "active") == 0) {
	    parameters->act.state = ACT_RUN;
	} else if (strcmp(token, "done") == 0) {
	    parameters->act.state = ACT_DONE;
	} else if (strcmp(token, "suspend") == 0) {
	    parameters->act.state = ACT_SUSPEND;
	} else if (strcmp(token, "abort") == 0) {
	    parameters->act.state = ACT_ABORT;
	} else if (strcmp(token, "none") == 0) {
	    parameters->act.state = ACT_NONE;
	}

	parameters->act.num_act = 0;
	while ((token = strtok(0, " \n\t"))) {
	    parameters->act.actions[parameters->act.num_act++] = token;
	}
    } else if (strncmp(op, "select", strlen("select")) == 0
	       || strncmp(op, "assert", strlen("assert")) == 0) {
	char * query;
	if (strncmp(op, "select", strlen("select")) == 0) {
	    parameters->call = VM_SELECT;
	} else if (strncmp(op, "assert", strlen("assert")) == 0) {
	    parameters->call = VM_ASSERT;
	}
	query = strtok(0, "\n"); /* query is remainder of line */
	parameters->query = query;
    } else if (strncmp(op, "fork", strlen("fork")) == 0) {
	char *line;
	    parameters->call = VM_FORK;
	    line = strtok(0, " \n\t");
	    parameters->line = atoi(line);
    } else if (strncmp(op, "join", strlen("join")) == 0) {
	    parameters->call = VM_JOIN;
    } else if (strncmp(op, "exit", strlen("exit")) == 0) {
	    parameters->call = VM_EXIT;
    } else if (strncmp(op, "error", strlen("error")) == 0) {
	ret = VM_ERROR;
    }
    return ret;
}

int execute(vm_context_t *context)
{
    int cond = 1; /* condition for the following while loop */
    int pExit = VM_INTERNAL_ERROR;
        
    /* Initialize context. */
    PC = context->PC;
    SP = context->SP;
    A = context->A;
    parameters = &context->parameters;
    stack = context->stack;
    inst_array = context->inst_array;
    num_inst = context->num_inst;
    while (cond) {
        /* fetch next opcode */
        if (nextOPCode(op) == VM_INTERNAL_ERROR) {
	    pExit = VM_INTERNAL_ERROR;
	    break;
	}

        /* interpret opcode */
        if (strncmp(op, "start", 5) == 0) {
        } else if (strncmp(op, "end", 3) == 0) {
            PC--;
            cond = 0;
	    pExit = VM_DONE;
	    fflush(stdout);
        } else if (strncmp(op, "push", 4) == 0) {
            int arg;
            sscanf(op+4, "%d", &arg);
            push(arg);
        } else if (strncmp(op, "pop", 3) == 0) {
		char* token;

            	A = pop();
		token = strtok(op+3," \n");		
		if (token != NULL) {
			add_var(token, A);
		}
        } else if (strncmp(op, "goto", 4) == 0) {
            PC = atoi(op+4);
        } else if (strncmp(op, "jump", 4) == 0) {
            int i;
            char * token;
            A = pop();
            token = strtok(op, " \n\t");
            for (i = 0; i <= A; i++) {
                token = strtok(0, " \n\t");
            }
            PC = atoi(token);
        } else if (strncmp(op, "jzero", 5) == 0) {
            A = pop();
            if (A == 0) {
                char * token;
                token = strtok(op+5, " \n\t");
                PC = atoi(token);
            }
            push(A);
        } else if (strncmp(op, "incr", 4) == 0) {
            A = pop();
            A++;
            push(A);
        } else if (strncmp(op, "decr", 4) == 0) {
            A = pop();
            A--;
            push(A);
        } else if (strncmp(op, "call", 4) == 0) {
	    pExit = system_call(op);
            cond = 0;
        }
    }

        
    /* Save context. */
    context->PC = PC;
    context->SP = SP;
    context->A = A;

    return pExit;
} /* execute */

vm_exit_code nextOPCode(char * op)
{
    if (PC < 0 || PC >= num_inst) {
	return VM_INTERNAL_ERROR;
    }
    if (inst_array[PC]) {
	strcpy(op, inst_array[PC]);	/* XXX Unsafe! */
	PC++;
	return VM_CONTINUE;
    } else {
	return VM_INTERNAL_ERROR;
    }
} /* nextOPCode */

void push(int arg)
{
    stack[++SP] = arg; 
    assert(SP < MAX_STACK_SIZE);
} /* push */

int pop()
{
    assert(SP >= 0 );
    return stack[SP--];
} /* pop */

int dereference(char* string)
{
    return 0;
} /* dereference */

/* Add a variable to global variable space. */
void add_var(char* name, int value)
{
} /* add_var */


#ifdef UNIT_TEST
#include "test_vm.c"
#endif
