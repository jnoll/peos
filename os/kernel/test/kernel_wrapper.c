/* -*-C-*-
 * $ID$
 * Create and run processes, with tcl list style output.
 * John Noll, Santa Clara University.
 */
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "events.h"

extern char *act_state_name(vm_act_state state);
/* The following mechanism lifted from the GNU readline documentation. */
typedef int Function ();
typedef struct {
    char *name;
    Function *impl;
    char *doc;
} COMMAND;

int list_models()
{
    int i;
    char **result = (char **) peos_list_models();

    printf("{");
    for (i = 0; result && result[i]; i++) {
	printf(" %s", result[i]);
    }
    printf(" }\n");

    return 0;
}

int list_instances()
{
    int i;
    char ** result = peos_list_instances(result);

    printf("{");
    for (i = 0; i <= PEOS_MAX_PID; i++) {
	printf("{%d %s}", i, result[i]);
    }
    printf(" }\n");

    return 0;
}


int print_action(peos_action_t *action) 
{
    printf(" {%d %s %s}", action->pid, action->name, 
	   act_state_name(action->state));
    fflush(stdout);
    return 0;
}

int list_actions(int argc, char *argv[])
{
    peos_action_t *alist;
    int i, num_act, pid;

    if (argc < 2) {
	printf("error: no pid specified\n");
	return -1;
    }

    printf("{");
    pid = atoi(argv[1]);
    if((alist = peos_list_actions(pid,  &num_act)) != NULL) {
	for (i = 0; i < num_act; i++) {
	    print_action(&(alist[i]));
	}
    }
    printf(" }\n");

    return 0;
}


int create_process(int argc, char *argv[])
{
    int pid;
    char *model;
    int num_resources;
    peos_resource_t *resources;

    if (argc < 2) {
	perror("error: no model specified\n");
	return -1;
    }

    model = argv[1];

    resources = (peos_resource_t *) 
	peos_get_resource_list(model, &num_resources);

    if (resources == NULL) {
	perror("error: retrieving resource names\n");
	return -1;
    }

    if ((pid = peos_run(model, resources, num_resources)) < 0) {
	perror("error: couldn't create process");
	return -1;
    } else {
	printf("pid %d\n", pid);
	return 0;
    }
}


peos_event state_of(char *event)
{
    if (strcasecmp(event, "start") == 0) {
	return PEOS_EVENT_START;
    } else if (strcasecmp(event, "finish") == 0) {
	return PEOS_EVENT_FINISH;
    } else if (strcasecmp(event, "suspend") == 0) {
	return PEOS_EVENT_SUSPEND;
    } else if (strcasecmp(event, "abort") == 0) {
	return PEOS_EVENT_ABORT;
    } else {
	return -1;		/* error */
    }
}

int notify_event(int argc, char *argv[])
{
    char *action;
    int pid; 
    peos_event event;

    vm_exit_code status;

    if (argc < 3) {
	printf("error:  pid/action not specified\n");
	return -1;
    }

    pid = atoi(argv[1]);
    action = argv[2];
    event = state_of(argv[3]);
    
    if ((status = peos_notify(pid, action, event)) == VM_ERROR 
	|| status == VM_INTERNAL_ERROR) {
	printf("error: notify %d %s %s\n", pid, action, argv[3]);
	return -1;
    }
    return 0;
}

int quit()
{
    exit(0);
}

int print_commands(int argc, char *argv[]);

COMMAND cmds[] = {
    { "models", list_models, "list models" },
    { "actions", list_actions, "list actions" },
    { "create", create_process, "create an instance of a process" },
    { "notify", notify_event, "submit an event associated with action" },
    { "help", print_commands, "list commands" },
    { "quit", quit, "quit shell" },
    { NULL, NULL, "end of commands"},
};

int print_commands (int argc, char *argv[])
{
    int i;
     
    printf("commands:");
    for (i = 0; cmds[i].name; i++) {
	printf(" %s", cmds[i].name);
    }
    printf("\n");
    return 1;
}

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


/* taken from parse.c, from Matt Strathmann and Ryan Becker's COEN 174
   project (winter 2002); original author unknown. */

int parse_line(char *buf, char **argv) 
{
    int argc = 0;

    /* strip out initial whitespace */
    while (*buf && isspace(*buf))
        buf++;

    /* find each argument, null terminate, and place ptr into argv array */
    while (*buf) {
        *argv++ = buf;
        argc++;

        while (*buf && !isspace(*buf))
            buf++;

        if (*buf) *buf++ = '\0';

	/* skip whitespace between arguments */
        while (*buf && isspace(*buf)) buf++;
    }
    *argv = (char *) 0;
    return argc;
}

int
main(int argc, char *argv[])
{
    int arg_c;
    char *cmd, *line = NULL;
    char *arg_v[256];
    COMMAND *cmd_func;

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
	    arg_c = parse_line(line, arg_v);
	    cmd = arg_v[0];

	    if (!(cmd_func = find_command(cmd))) {
		printf("no such command\n");
		print_commands(0, NULL);
	    } else {
		(*(cmd_func->impl))(arg_c, arg_v);
	    }
	}
    }    
}



