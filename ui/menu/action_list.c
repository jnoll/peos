#include <curses.h>
#include <cdk/cdk.h>
#include <os/kernel/events.h>

extern CDKSCREEN *cdkscreen;
extern WINDOW *cursesWin;
extern void display_hint(char *l1, char *l2);
extern void display_action(int pid, int index);
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


scroll_to_available(CDKSCROLL *scrollList, peos_action_t *alist, int num_actions)
{
    int i;

    /* Look for first active action. */
    for (i = 0; i < num_actions; i++) {
	if (alist[i].state == ACT_RUN ||
	    alist[i].state == ACT_SUSPEND) {
	    break;
	}
    }
    if (i == num_actions) {
    /* Look for first available action. */
	for (i = 0; i < num_actions; i++) {
	    if (alist[i].state == ACT_BLOCKED ||
		alist[i].state == ACT_AVAILABLE ||
		alist[i].state == ACT_READY) {
		break;
	    }
	}
    }

    while (i--) {
	injectCDKScroll(scrollList, KEY_DOWN);
    }
}

display_action_list(int pid, char *model)
{
    CDKSCROLL *scrollList = 0;
    char title[BUFSIZ];
    peos_action_t *alist;
    char **action_names;
    int selection, num_actions, i;


    /* Get list of actions. */
    if ((alist = peos_list_actions(pid, &num_actions)) != NULL) {
	action_names = list_actions(alist, num_actions);
    }

    sprintf(title, "<C>Actions for process %d:</K>%s<!K>", pid, model);
    /* Create the scrolling list. */
    scrollList = newCDKScroll (cdkscreen, 
			       CENTER, TOP,
			       RIGHT,
			       -2, 0,
			       title, 
			       action_names, num_actions,
			       NONUMBERS,
			       A_REVERSE,
			       TRUE, TRUE);
    if (scrollList == 0) {
	/* Exit CDK. */
	destroyCDKScreen (cdkscreen);
	endCDK();

	/* Print out a message and exit. */
	printf ("Oops. Could not make scrolling list. Is the window too small?\n");
	exit (1);
    }

    bind_default_keys(scrollList);


    while (1) {
	/* Redraw the CDK screen. */
	refreshCDKScreen (cdkscreen); 

	/* Activate the scrolling list. */
	scroll_to_available(scrollList, alist, num_actions);
	display_hint("</K><RET><!K> to select highlighted action.  </U>Emphasized<!U> = ready, </K>bold<!K> = active", NULL);

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


