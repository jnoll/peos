#include <curses.h>
#include <cdk/cdk.h>
#include <os/kernel/events.h>

extern CDKSCREEN *cdkscreen;
extern WINDOW *cursesWin;
extern void display_hint(char *l1, char *l2);
void bind_default_keys(void *object);

int bind_ready_state(CDKSWINDOW *view, peos_action_t *action);
int bind_run_state(CDKSWINDOW *view, peos_action_t *action);
int bind_suspend_state(CDKSWINDOW *view, peos_action_t *action);

handle_action_keys(EObjectType cdktype GCC_UNUSED,
		   void *target,
		   void *client_data, chtype key)
{
    char *msg[5], *hint, temp[256], title[BUFSIZ];
    char *d_buttons[] = {"OK", "Cancel"};
    int i = 0;
    int status;
    boolean escape = FALSE;
    peos_event event;
    peos_action_t *action = (peos_action_t *)client_data;

    switch (key) {
    case 'F': case 'f': case 'P': case 'p':
	switch (key) {
	case 'F': case 'f':
	    if (action->state != ACT_RUN && action->state != ACT_SUSPEND) {
		display_msg("ERROR: action not active/suspended; can't finish");
		quit();
	    }
	    event = PEOS_EVENT_FINISH;
	    sprintf (temp, "<C>Completing action </U>%s<!U>", action->name);
	    i = 0;
	    msg[i++] = copyChar (temp);
	    msg[i++] = "<C>Select OK to return to worklist.";
	    msg[i++] = "<C>and select another action->";
	    break;

	case 'P': case 'p':
	    if (action->state != ACT_BLOCKED && action->state != ACT_READY
		&& action->state != ACT_AVAILABLE) {
		display_msg("ERROR: action not available; can't perform");
		quit();
	    }
	    event = PEOS_EVENT_START;
	    sprintf (temp, "<C>Performing action </U>%s<!U>", action->name);
	    i = 0;
	    msg[i++] = copyChar (temp);
	    msg[i++] = "<C>Select OK to return to instructions.";
	}

	if ((popupDialog (cdkscreen, msg, i, d_buttons, 2) == 0)) {
	    if ((status = peos_notify(action->pid, action->name, event)) == VM_ERROR 
		|| status == VM_INTERNAL_ERROR) 
		{
		    display_msg("process executed an illegal instruction and has been terminated\n");
		    quit();
		} 
	    if (event == PEOS_EVENT_FINISH) {
		escape = TRUE;	/* Return to action list. */
	    } else {
		/* Have to reset state here, because action state is not
		   retrieved from kernel again. */
		action->state = ACT_RUN;
		/* Reset key bindings for new state. */
		bind_run_state(target, action);
	    }
	}
	break;

    case 'S': case 's': case 'R': case 'r':
	switch (key) {
	case 'S': case 's': 
	    /* Suspend */
	    if (action->state != ACT_RUN) {
		display_msg("ERROR: action not active; can't suspend.");
		quit();
	    }

	    event = PEOS_EVENT_SUSPEND;
	    sprintf(temp, "<C>Suspending Action </U>%s<!U>", action->name);
	    i = 0;
	    msg[i++] = copyChar (temp);
	    msg[i++] = "<C>Select OK to return to worklist.";
	    msg[i++] = "<C>and select another action.";
	    escape = TRUE;
	    break;

	case 'R': case 'r':
	    /* Resume */
	    if (action->state != ACT_SUSPEND) {
		display_msg("ERROR: action not suspended; can't resume.");
		quit();
	    }
	    event = PEOS_EVENT_START;
	    sprintf(temp, "<C>Resuming action </U>%s<!U>", action->name);
	    i = 0;
	    msg[i++] = copyChar (temp);
	    msg[i++] = "<C>Select OK to return to instructions.";
	    break;
	}

	if ((popupDialog(cdkscreen, msg, i, d_buttons, 2) == 0)) {
	    if ((status = peos_notify(action->pid, action->name, event)) == VM_ERROR 
		|| status == VM_INTERNAL_ERROR) {
		printf("process executed an illegal instruction and has been terminated\n");
		quit();
	    } else {
		if (event == PEOS_EVENT_SUSPEND) {
		    bind_suspend_state(target, action);
		} else {
		    action->state = ACT_RUN;
		    bind_run_state(target, action);
		}
	    }
	}
    } 
    return escape;
}


bind_ready_state(CDKSWINDOW *view, peos_action_t *action)
{
    cleanCDKObjectBindings(vSWINDOW, (void *)view);
    bindCDKObject (0, view, 'P', handle_action_keys, (void *)action); 
    bindCDKObject (0, view, 'p', handle_action_keys, (void *)action); 
    bind_default_keys(view);
    display_hint("</K>P<!K>)erform this action", NULL);

}

bind_suspend_state(CDKSWINDOW *view, peos_action_t *action)
{
    cleanCDKObjectBindings(vSWINDOW, (void *)view);

    bindCDKObject(0, view, 'F', handle_action_keys, (void *)action); 
    bindCDKObject(0, view, 'f', handle_action_keys, (void *)action); 
    bindCDKObject(0, view, 'R', handle_action_keys, (void *)action); 
    bindCDKObject(0, view, 'r', handle_action_keys, (void *)action); 
    bind_default_keys(view);
    display_hint("</K>F<!K>)inish this action; </K>R<!K>)esume this action.",
		 NULL);
}

/* Bind keys and return hint string. */
bind_run_state(CDKSWINDOW *view, peos_action_t *action)
{
    cleanCDKObjectBindings(vSWINDOW, (void *)view);

    bindCDKObject(0, view, 'F', handle_action_keys, (void *)action); 
    bindCDKObject(0, view, 'f', handle_action_keys, (void *)action); 
    bindCDKObject(0, view, 'S', handle_action_keys, (void *)action); 
    bindCDKObject(0, view, 's', handle_action_keys, (void *)action); 
    bind_default_keys(view);
    display_hint("</K>F<!K>)inish this action; </K>S<!K>)uspend this action to do another",
		 NULL);
}


char *lookup_resource(char *name,  peos_resource_t *resources, int num)
{
    int i;
    for (i = 0; i < num; i++) {
	if (strcmp(name, resources[i].name) == 0) {
	    return resources[i].value;
	}
    }
    return NULL;
}
/*
 * Write script to file, replacing substrings like  $resource_name  with
 * bound value.
 */
int format_script(char *script, peos_resource_t *resources, int num_resources,
		  FILE *out)
{
    char *start, *end, *value, *data = strdup(script);;

    end = start = data;
    while (end) {
	/* Look for begin character. */
	start = strsep(&end, "$");
	/* Write out leading text. */
	fwrite(start, sizeof(char), strlen(start), out);
	/* Look for end of token. */
	if (end && (start = strsep(&end, " \t\n~!@#$%^&*()-+={}[]\\|\":;,.<>?/"))) {
	    if ((value = lookup_resource(start, resources, num_resources)) != NULL) {
		fprintf(out, value);
	    } else {
		/* Not found? write resource name. */
		fwrite(start, sizeof(char), end - start, out);
		fprintf(out, "(unbound)");
	    }
	}
    }
}

void display_action(int pid, int index)
{
    peos_action_t *alist, action;
    char  title[BUFSIZ],  cmd[BUFSIZ], *script;
    int tempfd;
    FILE *tempfile;
    int i, num_actions, num_resources;
    peos_resource_t *resources;

    sprintf(title, "peos_actXXXXXX");
    tempfd = mkstemp(title);
    tempfile = fdopen(tempfd, "w");

    if ((alist = peos_list_actions(pid, &num_actions)) == NULL) {
	display_msg("Unable to retrieve action list.");
	quit();
    }

    action = alist[index];
	
    /* Create the view. */
    fprintf (tempfile, "<html><head><title>%20s</title></head>\n",
	     action.name);
    fprintf (tempfile, "<body><h1>Action: %20s</h1>\n", 
	     action.name);

    /* Get the script and install as window text. */
    script = peos_get_script(action.pid, action.name);
    resources = peos_get_resource_list_action(action.pid, action.name, &num_resources);
    for (i = 0; i < num_resources; i++) {
	if ((strlen(resources[i].value) == 0) 
	    || (strcmp(resources[i].value, "%unbound") == 0)) 
	{
	    strcpy(resources[i].value, 
		   "http://www.cse.scu.edu/~jnoll/286/projects");
	}
    }
    if (script) {
	format_script(script, resources, num_resources, tempfile);
	fprintf (tempfile, "</body>\n</html>"); 


	fclose(tempfile);

	/* Activate the viewer widget. */
	sprintf(cmd, "lynx -force_html %s", title);
	system(cmd);
    }
    refreshCDKScreen (cdkscreen);
}
