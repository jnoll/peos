#include <curses.h>
#include <cdk/cdk.h>
#include <os/kernel/events.h>

extern CDKSCREEN *cdkscreen;
extern WINDOW *cursesWin;


char **list_actions(peos_action_t *alist, int num_actions)
{

    char **result = NULL, buf[BUFSIZ];
    int i;

    result = (char **)calloc(num_actions + 1, sizeof(char *));
    for (i = 0; i < num_actions; i++) {
	switch (alist[i].state) {
	case ACT_NONE:
	case ACT_NEW:
	    snprintf(buf, sizeof(buf), "%s", alist[i].name);
	    break;
	case ACT_PENDING:
	case ACT_DONE:
	    snprintf(buf, sizeof(buf), "</D>%s<!D> (done)", alist[i].name);
	    break;
	case ACT_DEAD: 
	case ACT_ABORT:
	    snprintf(buf, sizeof(buf), "%s", alist[i].name);
	    break;

	    /* Ready actions. */
	case ACT_BLOCKED:
	    snprintf(buf, sizeof(buf), "</U>%s<!U> (blocked)", alist[i].name);
	    break;
	case ACT_READY:
	    snprintf(buf, sizeof(buf), "</U>%s<!U> (ready)", alist[i].name);
	    break;
	case ACT_AVAILABLE:
	    snprintf(buf, sizeof(buf), "</U>%s<!U> (avail)", alist[i].name);
	    break;

	    /* Running actions. */
	case ACT_SUSPEND:
	    snprintf(buf, sizeof(buf), "</K>%s<!K> (suspend)", alist[i].name);
	    break;
	case ACT_RUN:
	    snprintf(buf, sizeof(buf), "</K>%s<!K> (active)", alist[i].name);
	    break;
	}
	result[i] = strdup(buf); 
    }
    result[i] = NULL;

    return result;
}


void display_action(int pid, int index)
{
    peos_action_t *alist, action;
    char *msg[5], temp[256], title[BUFSIZ];
    CDKVIEWER *view;
    char *buttons[40], *d_buttons[] = {"OK", "Cancel"};
    char *info[5];
    char *loginName;
    char *script, **script_lines;
    int selection, b, num_actions;


    while (1) {
	if ((alist = peos_list_actions(pid, &num_actions)) == NULL) {
	    exit(0);
	}

	action = alist[index];

	b = 0;
	switch (action.state) {
	case ACT_READY:
	case ACT_AVAILABLE:
	case ACT_BLOCKED:
	    buttons[b++] = "Perform";
	    break;
	case ACT_RUN:
	    buttons[b++] = "Finish";
	    buttons[b++] = "Suspend";
	    break;
	case ACT_SUSPEND:
	    buttons[b++] = "Finish";
	    buttons[b++] = "Resume";
	    break;
	default:
	    break;
	}
	buttons[b++] = "Back";

	/* Create the view. */
	view = newCDKViewer (cdkscreen,
			     CENTER, CENTER, 
			     0, 0,
			     buttons, b, 
			     A_REVERSE,
			     TRUE, FALSE);

	/* Could we create the viewer widget? */
	if (view == 0) {
	    /* Exit CDK. */
	    destroyCDKScreen (cdkscreen);
	    endCDK();

	    /* Print out a message and exit. */
	    printf ("Oops. Can't seem to create viewer. Is the window too small?\n");
	    exit (0);
	}


	/* Set up the viewer title, and the contents to the widget. */
	script = peos_get_script(action.pid, action.name);
	script_lines = CDKsplitString(script, '\n');
	sprintf (title, "<C>Action:</K>%20s<!K>", action.name);
	setCDKViewer (view, title, script_lines, CDKcountStrings(script_lines), A_REVERSE, TRUE, TRUE, TRUE);

	/* Activate the viewer widget. */
	selection = activateCDKViewer (view, 0);


	/* Check how the person exited from the widget.*/
	if (view->exitType == vESCAPE_HIT) {
	    /* Return to action list. */
	    destroyCDKViewer(view);
	    refreshCDKScreen(cdkscreen);
	    break;
	} else if (view->exitType == vNORMAL) {
	    int i = 0;
	    int status;
	    peos_event event;

	    if (selection == b-1) {
		/* Back */
		destroyCDKViewer(view);
		refreshCDKScreen(cdkscreen);
		break;
	    } else if (selection == 0) {
		/* Perform or Finish */
		switch (action.state) {
		case ACT_RUN:
		case ACT_SUSPEND:
		    event = PEOS_EVENT_FINISH;
		    sprintf (temp, "<C>Completing action </U>%s<!U>", action.name);
		    msg[i++] = copyChar (temp);
		    msg[i++] = "<C>Select OK to return to worklist.";
		    msg[i++] = "<C>and select another action.";
		    break;
		case ACT_READY:
		case ACT_BLOCKED:
		case ACT_AVAILABLE:
		    event = PEOS_EVENT_START;
		    sprintf (temp, "<C>Performing action %s", action.name);
		    msg[i++] = copyChar (temp);
		    msg[i++] = "<C>Select OK to return to instructions.";
		    break;
		}

		if ((popupDialog (cdkscreen, msg, i, d_buttons, 2) == 0)) {
		    if ((status = peos_notify(action.pid, action.name, event)) == VM_ERROR 
			|| status == VM_INTERNAL_ERROR) {
			printf("process executed an illegal instruction and has been terminated\n");
		    } 
		    if (event == PEOS_EVENT_FINISH) {
			break;	/* Return to action list. */
		    } 
		}
	    } else if (selection == 1 && action.state == ACT_RUN) {
		if (action.state == ACT_RUN) {
		    /* Suspend */
		    event = PEOS_EVENT_SUSPEND;
		    sprintf (temp, "<C>Suspending Action </U>%s<!U>", action.name);
		    msg[i++] = copyChar (temp);
		    msg[i++] = "<C>Select OK to return to worklist.";
		    msg[i++] = "<C>and select another action.";
		} else {
		    /* Resumed */
		    event = PEOS_EVENT_START;
		    sprintf (temp, "<C>Resuming action </U>%s<!U>", action.name);
		    msg[i++] = copyChar (temp);
		    msg[i++] = "<C>Select OK to return to instructions.";
		}

		if ((popupDialog (cdkscreen, msg, i, d_buttons, 2) == 0)) {
		    if ((status = peos_notify(action.pid, action.name, event)) == VM_ERROR 
			|| status == VM_INTERNAL_ERROR) {
			printf("process executed an illegal instruction and has been terminated\n");
			exit(1);
		    } 
		}
	    }
	}

	/* Clean up. */
	destroyCDKViewer(view);
	refreshCDKScreen(cdkscreen);
    }
}

scroll_to_available(CDKSCROLL *scrollList, peos_action_t *alist, int num_actions)
{
    int i;

    /* Scroll forward to first available action. */
    for (i = 0; i < num_actions; i++) {
	if (alist[i].state == ACT_BLOCKED ||
	    alist[i].state == ACT_AVAILABLE ||
	    alist[i].state == ACT_READY ||
	    alist[i].state == ACT_SUSPEND) {
	    break;
	}
	injectCDKScroll(scrollList, KEY_DOWN);
    }
}

display_action_list(int pid)
{
    CDKSCROLL *scrollList = 0;
    char *title = "<C></K>Peos Enactment Interface<!K>\n<C>Select an action to view or perform\n<C>Ready actions are </U>emphasized<!U>, Active actions are </K>Bold<!K>\n";
    char *msg[5];
    char **action_names;
    int selection, num_actions, i;
    peos_action_t *alist;


    /* Get list of actions. */
    if ((alist = peos_list_actions(pid, &num_actions)) != NULL) {
	action_names = list_actions(alist, num_actions);
    }

    /* Create the scrolling list. */
    scrollList = newCDKScroll (cdkscreen, 
			       CENTER, CENTER,
			       RIGHT,
			       0, 0,
			       title, 
			       action_names, num_actions,
			       NONUMBERS,
			       A_REVERSE,
			       TRUE, TRUE);

    while (1) {
	/* Redraw the CDK screen. */
	refreshCDKScreen (cdkscreen);	
	/* Is the scrolling list null? */
	if (scrollList == 0) {
	    /* Exit CDK. */
	    destroyCDKScreen (cdkscreen);
	    endCDK();

	    /* Print out a message and exit. */
	    printf ("Oops. Could not make scrolling list. Is the window too small?\n");
	    exit (1);
	}

	/* Activate the scrolling list. */
	scroll_to_available(scrollList, alist, num_actions);
	selection = activateCDKScroll (scrollList, 0);

	/* Determine how the widget was exited. */
	if (scrollList->exitType == vESCAPE_HIT) {
	    break;
	} else if (scrollList->exitType == vNORMAL) {
	    /* Clean up before displaying action. */
	    CDKfreeStrings (action_names);
	    display_action(pid, selection);
	}

	/* Get new list of actions. */
	if ((alist = peos_list_actions(pid, &num_actions)) != NULL) {
	    action_names = list_actions(alist, num_actions);
	}

	setCDKScrollItems(scrollList, action_names, num_actions, NONUMBERS);
    }

    CDKfreeStrings (action_names);
    destroyCDKScroll (scrollList);
}


