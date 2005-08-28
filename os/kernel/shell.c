/* -*-C-*-
*****************************************************************************
*
* File:         $RCSfile: shell.c,v $
* Version:      $Id: shell.c,v 1.34 2005/08/28 06:17:13 ksuwanna Exp $ ($Name:  $)
* Description:  Command line shell for kernel.
* Author:       John Noll, Santa Clara University
* Created:      Mon Mar  3 20:25:13 2003
* Modified:     Sun Nov 16 23:02:46 2003 (John Noll, SCU) jnoll@carbon.cudenver.edu
* Language:     C
* Package:      N/A
* Status:       $State: Exp $
*
* (c) Copyright 2003, Santa Clara University, all rights reserved.
*
*****************************************************************************
*/
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <fcntl.h>
#include "events.h"
#include "process_table.h"
#include "pmlheaders.h"
#include "graph_engine.h"
#include "process.h"

extern char *act_state_name(vm_act_state state);

/* The following mechanism lifted from the GNU readline documentation. */
typedef struct 
{
    char *name;
    VFunction *impl;
    char *doc;
} COMMAND;

void list_models()
{
    int i;
    char **result = (char **) peos_list_models();

    for (i = 0; result && result[i]; i++) {
        printf("\t%s\n", result[i]);
    }
}

void list_instances()
{
    int i;
    char ** result = peos_list_instances(result);
    for (i = 0; i <= PEOS_MAX_PID; i++) {
	printf("%d %s\n", i, result[i]);
    }
}

void list_actions()
{
    peos_action_t *alist;
    int i=0;
    int j;
    
    char ** result = peos_list_instances();
	
    printf("process action (status)\n");
    while((i <= PEOS_MAX_PID)) {
	int num_actions;    
	if (result[i] != NULL) {
            alist = peos_list_actions(i,&num_actions);
	    if(alist) {
	        for(j = 0; j < num_actions; j++) {
	            printf("%d  %s  %s\n",i,alist[j].name,
			   act_state_name(alist[j].state));
	        }
	    }
	    i++;
	}
    }
}    
  
   

void list(int argc, char *argv[])    
{
    char *type;

    if (argc < 2) {
        printf("usage: %s models|processes|actions\n", argv[0]);
	return;
    }
    type = argv[1];

    if (strncmp(type, "m", strlen("m")) == 0) {
	list_models();
    } 
    else if (strncmp(type, "p", strlen("p")) == 0) {
	list_instances();
    } 
    else if (strncmp(type, "a", strlen("a")) == 0) {
	list_actions();
    } 
    else {
	printf("usage: list models|processes|actions\n");
    }
}

void create_process(int argc, char *argv[])
{
    int pid;
    char *model;
    int num_resources;
    peos_resource_t *resources;

    if (argc < 2) {
	printf("usage: %s model\n", argv[0]);
	return;
    }

    model = argv[1];

    /* This is new code I have inserted. */

     resources = (peos_resource_t *) peos_get_resource_list(model,&num_resources);


    if (resources == NULL) {
        printf("error getting resources\n");
	return;
    }
    
    printf("Executing %s:\n", model);
    
    // note the change in interface of peos_run
    if ((pid = peos_run(model,resources,num_resources)) < 0) {
	error_msg("couldn't create process");
    } 
    else {
	printf("Created pid = %d\n", pid);
    }
}

void list_resource_binding(int argc, char *argv[])
{
    int i, pid, num_resources = 0, num_provided_resources = 0;
    char *action;
    peos_resource_t *req_resources;
    peos_resource_t *pro_resources;

    if (argc < 3) {
	printf("usage: %s pid action\n", argv[0]);
	return;
    }

    pid = atoi(argv[1]);
    action = argv[2];

    req_resources = (peos_resource_t *) 
	peos_get_resource_list_action_requires(pid, action, &num_resources);

    pro_resources = (peos_resource_t *) 
	peos_get_resource_list_action_provides(pid, action, &num_provided_resources);


    if ((req_resources == NULL) || (pro_resources == NULL)){
        printf("error getting resources\n");
        return;
    }
    printf("req_resources=%d\npro_resources=%d\n",num_resources,num_provided_resources);
    printf("Required resources\n");
    for(i = 0; i < num_resources; i++) {
        printf("%s=%s=%s\n", req_resources[i].name, req_resources[i].value,req_resources[i].qualifier) ;
    }

    printf("Provided resources\n");
    for(i = 0; i < num_provided_resources; i++) {
        printf("%s=%s=%s\n", pro_resources[i].name, pro_resources[i].value,pro_resources[i].qualifier) ;
    }

    
    free(req_resources);
    free(pro_resources);
}

void list_resources(int argc, char *argv[])
{
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
    model_file = (char *)find_model_file(model);
    if (model_file == NULL) {
        printf("error: model file %s not found\n", model);
        return;
    }
    resources = (peos_resource_t *) peos_get_resource_list(model_file,&num_resources);


    if (resources == NULL) {
        printf("error getting resources\n");
        return;
    }
    for(i = 0; i < num_resources; i++) {
        printf("%s=%s\n", resources[i].name, resources[i].value) ;
    }

    free(resources);
}

void run_action(int argc, char *argv[])
{
    char *action, *script;
    int pid; 
    vm_exit_code status;

    if (argc < 3) {
	printf("usage: %s pid action\n", argv[0]);
	return;
    }

    pid = atoi(argv[1]);
    action = argv[2];
    printf("Performing action %s\n", action);
    if ((status = peos_notify(pid, action, PEOS_EVENT_START)) == VM_ERROR 
	|| status == VM_INTERNAL_ERROR) {
	printf("process executed an illegal instruction and has been terminated\n");
    } 
    else {
	script = (char *) peos_get_script(pid, action);
	if (script) {
	    printf(script);
	    printf("\n");
	} 
	else {
	    printf("Error : Script Not Found");	
	}
    }
            
}

void finish_action(int argc, char *argv[])
{
    char *action;
    int pid; 
    vm_exit_code status;

    if (argc < 3) {
	printf("usage: %s pid action\n", argv[0]);
	return;
    }
    pid = atoi(argv[1]);
    action = argv[2];
    printf("Performing action %s\n", action);
    if ((status = peos_notify(pid, action,PEOS_EVENT_FINISH)) == VM_ERROR 
	|| status == VM_INTERNAL_ERROR) {
	printf("process executed an illegal instruction and has been terminated\n");
    }
}


void suspend_action(int argc, char *argv[])
{
    char *action;
    int pid;
    vm_exit_code status;

    if (argc < 3) {
        printf("usage: %s pid action\n", argv[0]);
	return;
    }
    pid = atoi(argv[1]);
    action = argv[2];
    if ((status = peos_notify(pid, action,PEOS_EVENT_SUSPEND)) == VM_ERROR 
	|| status == VM_INTERNAL_ERROR) {
	printf("process encountered an illegal event and has been terminated\n");
    }
}


void abort_action(int argc, char *argv[])
{
    char *action;
    int pid;
    vm_exit_code status;

    if (argc < 3) {
        printf("usage: %s pid action\n", argv[0]);
	return;
    }
    pid = atoi(argv[1]);
    action = argv[2];
    if ((status = peos_notify(pid, action,PEOS_EVENT_ABORT)) == VM_ERROR 
	|| status == VM_INTERNAL_ERROR) {
	printf("process encountered an illegal event and has been terminated\n");
    }
}


void resource_change(int argc, char *argv[])
{
    char *action;
    int pid;
    vm_exit_code status;

    if (argc < 3) {
        printf("usage: %s pid action\n", argv[0]);
	return;
    }
    pid = atoi(argv[1]);

    action = argv[2];
    if ((status = peos_notify(pid, action,PEOS_EVENT_RESOURCE_CHANGE)) == VM_ERROR 
	|| status == VM_INTERNAL_ERROR) {
	printf("process encountered an illegal event and has been terminated\n");
    }
}

    
void quit()
{

    exit(0);
}

COMMAND cmds[] = {
    { "list", list, "list actions/models/instances" },
    { "models", list_models, "list models" },
    { "actions", list_actions, "list actions" },
    { "do", run_action, "perform a ready action" },
    { "resource_list", list_resources, "list resources associated with model" },
    { "resources", list_resource_binding, "list resources associated with model" },
    { "run", run_action, "perform a ready action" },
    { "change", resource_change, "resource change notify"},
    { "suspend", suspend_action, "suspend action" },
    { "abort", abort_action, "abort action"},
    { "select", run_action, "perform a ready action" },
    { "finish", finish_action, "finish a running action" },
    { "done", finish_action, "finish a running action" },
    { "exec", create_process, "create an instance of a process" },
    { "create", create_process, "create an instance of a process" },
    { "resource_bindings", list_resource_binding, "list resource details" },
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

void print_commands ()
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
		print_commands();
	    } 
	    else {
		(*(cmd_func->impl))(arg_c, arg_v);
	    }
	}
    }    
}



