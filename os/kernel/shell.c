/* -*-C-*-
*****************************************************************************
*
* File:         $RCSfile: shell.c,v $
* Version:      $Id: shell.c,v 1.13 2003/09/12 21:10:09 jshah1 Exp $ ($Name:  $)
* Description:  Command line shell for kernel.
* Author:       John Noll, Santa Clara University
* Created:      Mon Mar  3 20:25:13 2003
* Modified:     Sun Jul  6 16:51:47 2003 (John Noll, SCU) jnoll@carbon.cudenver.edu
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
#include "process_table.h"
#include "pmlheaders.h"



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
    int i, n;
    char ** result = peos_list_instances(result);
    for (i = 0; i <= PEOS_MAX_PID; i++) {
	printf("%d %s\n", i, result[i]);
    }
}

print_action(peos_action_t *action, char *state) 
{
    peos_resource_t *resources;
    int num_resources;
    int i;

   peos_resource_t  *resources_req,*resources_pro;  
   int num_req_resources,num_pro_resources;

    resources = (peos_resource_t *) get_resource_list_action(action->pid,action->name,&num_resources);

/* get the required resources */    
    resources_req = (peos_resource_t *) get_resource_list_action_requires(action -> pid,action -> name,&num_req_resources);


/* get the provided resources */    
    resources_pro = (peos_resource_t *) get_resource_list_action_provides(action->pid, action -> name, &num_pro_resources);

   
    printf("  %2d    %-6s (%s) %s  resources:", action->pid, action->name, state, action->script ? action->script : "no script");
     
    if(num_resources == 0) printf("no resources");
    for(i = 0; i < num_resources; i++)
    {
    printf(" %s",resources[i].name);
    if(i != num_resources-1) printf(",");
    }
    printf("\n");

/* print the required resources */    
    printf("Required Resources: ");
    if(num_req_resources == 0) printf("no resources required");
    for(i = 0; i < num_req_resources; i++)
      {
       printf(" %s",resources_req[i].name);
       if(i != num_req_resources-1) printf(",");
      }
      printf("\n");

/* print the provided resources */      
      printf("Provided Resources: ");
      if(num_pro_resources == 0) printf("no resources provided");
      for(i = 0; i < num_pro_resources; i++)
        {
         printf(" %s",resources_pro[i].name);
         if(i != num_pro_resources-1) printf(",");
        }
         printf("\n");
    
}


list_actions()
{
    peos_action_t **alist;
    int i;
    printf("process action (status)\n");
    alist = peos_list_actions(ACT_RUN);
    if (alist && alist[0]) {
	for (i = 0; alist[i]; i++) {
	    print_action(alist[i], "active");
	}
    }
    alist = peos_list_actions(ACT_SUSPEND);
    if (alist && alist[0]) {
	for (i = 0; alist[i]; i++) {
	    print_action(alist[i], "suspended");
	}
    }
    alist = peos_list_actions(ACT_READY);
    if (alist && alist[0]) {
	for (i = 0; alist[i]; i++) {
	    print_action(alist[i], "ready");
	}
    }
}

list(int argc, char *argv[])    
{
    char *type;

    if (argc < 2) {
	printf("usage: %s models|processes|actions\n", argv[0]);
	return;
    }
    type = argv[1];

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

create_process(int argc, char *argv[])
{
    int pid;
    int i;
    char *model;
    char *model_file;
    int num_resources;
    peos_resource_t *resources;

    if (argc < 2) {
	printf("usage: %s model\n", argv[0]);
	return;
    }

    model = argv[1];

    // This is new code I have inserted

     model_file = (char *)find_model_file(model);
	if (model_file == NULL)
	{
		printf("error: model file not found\n");
		return;
	}
	
	resources = (peos_resource_t *) get_resource_list(model_file,&num_resources);

	/* 
	 * replace the code below to get resource values from user
	 * 
	 */

	if (resources == NULL)
	{
		printf("error getting resources\n");
		return;
	}
	for(i = 0; i < num_resources; i++)
	{
		//sprintf(resources[i].value,"value_%d",i);
		printf("\nEnter binding for resource %s : ",resources[i].name);
		scanf("%s", &resources[i].value);

	}

	
    
    printf("Executing %s:\n", model);
    
    // note the change in interface of peos_run
    if ((pid = peos_run(model,resources,num_resources)) < 0) {
	error_msg("couldn't create process");
    } else {
	printf("Created pid = %d\n", pid);
    }
}

run_action(int argc, char *argv[])
{
    char *action, *pid_string, *script;
    int pid; 
    vm_exit_code status;

    if (argc < 3) {
	printf("usage: %s pid action\n", argv[0]);
	return;
    }

    pid = atoi(argv[1]);
    action = argv[2];
    printf("Performing action %s\n", action);
    if ((status = peos_run_action(pid, action)) == VM_ERROR 
	|| status == VM_INTERNAL_ERROR) {
	printf("process executed an illegal instruction and has been terminated\n");
    } else {
	script = (char *) get_field(pid, action, ACT_SCRIPT);
	if (script) {
	    printf(script);
	    printf("\n");
	} else {
	    printf("(no script)\n");
	}
    }
}

finish_action(int argc, char *argv[])
{
    char *action, *pid_string;
    int pid; 
    vm_exit_code status;

    if (argc < 3) {
	printf("usage: %s pid action\n", argv[0]);
	return;
    }
    pid = atoi(argv[1]);
    action = argv[2];
    printf("Performing action %s\n", action);
    if ((status = peos_finish_action(pid, action)) == VM_ERROR 
	|| status == VM_INTERNAL_ERROR) {
	printf("process executed an illegal instruction and has been terminated\n");
    }
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

/* taken from parse.c, from Matt Strathmann and Ryan Becker's COEN 174
   project; original author unknown. */

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
    int i, arg_c;
    char *cmd, *line = NULL;
    char *arg_v[256];
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
	arg_c = parse_line(line, arg_v);
	cmd = arg_v[0];
	if (!(cmd_func = find_command(cmd))) {
	    printf("no such command\n");
	    print_commands();
	} else {
	(*(cmd_func->impl))(arg_c, arg_v);
	}
    }    
}



