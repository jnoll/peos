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


void display_action(int pid, int index)
{
    peos_action_t *alist, action;
    char *hint, title[BUFSIZ];;
    CDKSWINDOW *view;

    char *info[5];
    char *loginName;
    char *script, **script_lines;
    int selection, b, num_actions;


    while (1) {
	if ((alist = peos_list_actions(pid, &num_actions)) == NULL) {
	    display_msg("Unable to retrieve action list.");
	    quit();
	}

	action = alist[index];

	/* Create the view. */
	sprintf (title, "<C>Action: </K>%20s<!K>", action.name);
	view = newCDKSwindow (cdkscreen,
			      CENTER, TOP,
			      -4, 0,
			      title,
			      1024,
			      TRUE, FALSE);


	switch (action.state) {
	case ACT_READY:
	case ACT_AVAILABLE:
	case ACT_BLOCKED:
	    bind_ready_state(view, &action);
	    break;
	case ACT_RUN:
	    bind_run_state(view, &action);
	    break;
	case ACT_SUSPEND:
	    bind_suspend_state(view, &action);
	    break;
	default:
	    bind_default_keys(view);
	    break;
	}


	/* Could we create the viewer widget? */
	if (view == 0) {
	    /* Exit CDK. */
	    destroyCDKScreen (cdkscreen);
	    endCDK();

	    /* Print out a message and exit. */
	    printf ("Oops. Can't seem to create viewer. Is the window too small?\n");
	    exit (0);
	}


	/* Get the script and install as window text. */
	script = peos_get_script(action.pid, action.name);
	script_lines = CDKsplitString(script, '\n');
	setCDKSwindow(view, script_lines, CDKcountStrings(script_lines), TRUE);

	/* Activate the viewer widget. */
	activateCDKSwindow (view, 0);


	/* Check how the person exited from the widget.*/
	if (view->exitType == vESCAPE_HIT) {
	    /* Return to action list. */
	    destroyCDKViewer(view);
	    refreshCDKScreen(cdkscreen);
	    break;
	}

	/* Clean up. */
	CDKfreeStrings(script_lines);
	destroyCDKViewer(view);
	refreshCDKScreen(cdkscreen);
    }
}
