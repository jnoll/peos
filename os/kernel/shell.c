/* -*-C-*-
*****************************************************************************
*
* File:         $RCSFile: shell.c$
* Version:      $Id: shell.c,v 1.2 2003/06/29 22:39:14 jnoll Exp $ ($Name:  $)
* Description:  Command line shell for kernel.
* Author:       John Noll, Santa Clara University
* Created:      Mon Mar  3 20:25:13 2003
* Modified:     Sun Jun 29 15:22:49 2003 (John Noll, SCU) jnoll@carbon.cudenver.edu
* Language:     C
* Package:      N/A
* Status:       $State: Exp $
*
* (c) Copyright 2003, Santa Clara University, all rights reserved.
*
*****************************************************************************
*/
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "vm.h"
#include "events.h"

/* The following mechanism lifted from the GNU readline documentation. */
typedef struct {
    char *name;
    Function *impl;
    char *doc;
} COMMAND;

list_models()
{
    int i;
    char **result = (char **) peos_list_models();

    for (i = 0; result && result[i]; i++) {
	printf("\t%s\n", result[i]);
    }
}

list_instances()
{
}

list_actions()
{
    peos_action_t *alist;
    int i;

    alist = peos_list_actions(ACT_RUN);
    if (alist && alist[0].name[0]) {
	for (i = 0; alist[i].name[0]; i++) {
	    printf("%s (active)\n", alist[i].name);
	}
    }
    alist = peos_list_actions(ACT_SUSPEND);
    if (alist && alist[0].name[0]) {
	for (i = 0; alist[i].name[0]; i++) {
	    printf("%s (suspended)\n", alist[i].name);
	}
    }
    alist = peos_list_actions(ACT_READY);
    if (alist && alist[0].name[0]) {
	for (i = 0; alist[i].name[0]; i++) {
	    printf("%s (ready)\n", alist[i].name);
	}
    }
}

list(char *line)    
{
    char *type = strtok(line, " \t\n");
    if (strncmp(type, "m", strlen("m")) == 0) {
	list_models();
    } else if (strncmp(type, "p", strlen("p")) == 0) {
	list_instances();
    } else if (strncmp(type, "a", strlen("a")) == 0) {
	list_actions();
    } else {
	printf("usage: list models|processes|actions\n");
    }
}

create_process(char *model)
{
    int pid;

    printf("Executing %s:\n", model);
    if ((pid = peos_run(model, 0)) < 0) {
	error_msg("couldn't create process");
    } else {
	printf("Created pid = %d\n", pid);
    }
}

run_action(char *action)
{
    vm_exit_code status;
    printf("Performing action %s\n", action);
    if ((status = peos_run_action(action)) == VM_ERROR 
	|| status == VM_INTERNAL_ERROR) {
	printf("process executed an illegal instruction and has been terminated\n");
    }
}

finish_action(char *action)
{
    peos_finish_action(action);
}

quit()
{
    save_proc_table("proc_table.dat");
    exit(0);
}

COMMAND cmds[] = {
    { "list", list, "list actions/models/instances" },
    { "models", list_models, "list models" },
    { "actions", list_actions, "list actions" },
    { "do", run_action, "perform a ready action" },
    { "run", run_action, "perform a ready action" },
    { "select", run_action, "perform a ready action" },
    { "finish", finish_action, "finish a running action" },
    { "done", finish_action, "finish a running action" },
    { "exec", create_process, "create an instance of a process" },
    { "create", create_process, "create an instance of a process" },
    { "quit", quit, "quit shell" },
    { NULL, NULL, "end of commands"},
};

COMMAND *find_command (char *name)
{
    int i;
     
    if (name) {
	for (i = 0; cmds[i].name; i++) {
	    if (strcmp (name, cmds[i].name) == 0) {
		return (&cmds[i]);
	    }
	}
    }

    return ((COMMAND *)NULL);
}

print_commands ()
{
    int i;
     
    printf("commands:");
    for (i = 0; cmds[i].name; i++) {
	printf(" %s", cmds[i].name);
    }
    printf("\n");
}

int
main(int argc, char *argv[])
{
    int i;
    char *cmd, *line = NULL;
    COMMAND *cmd_func;

    load_proc_table("proc_table.dat");
    while (1) {
	/* If the buffer has already been allocated, return the memory
	   to the free pool. */
	if (line) {
	    free (line);
	    line = (char *)NULL;
	}
     
	/* Get a line from the user. */
	line = readline ("> ");
     
	/* If the line has any text in it, save it on the history. */
	if (line && *line) {
	    add_history (line);
	}
	cmd = strtok(line, " \t\n");
	if (!(cmd_func = find_command(cmd))) {
	    printf("no such command\n");
	    print_commands();
	} else {
	(*(cmd_func->impl))(line + strlen(cmd) + 1);
	}
    }    
}



