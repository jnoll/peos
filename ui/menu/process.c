#include <curses.h>
#include <cdk/cdk.h>
#include <os/kernel/events.h>

extern CDKSCREEN *cdkscreen;
extern WINDOW *cursesWin;

handle_process_list_selection(EObjectType type, void *target, void *instances,
			      chtype input)
{
    CDKSCROLL *scroll = (CDKSCROLL *) target;

    switch (input) {
    case KEY_RETURN : case KEY_TAB : case KEY_ENTER :
	display_action_list(scroll->currentItem);
	/* Clean up. */
	break;
    default:
	break;
    }
}

display_process_list()
{
    int i;
    CDKSCROLL *scrollList = 0;
    char *msg[5];
    int selection;
    char **instances = peos_list_instances(instances);


    /* Create the scrolling list. */
    scrollList = newCDKScroll (cdkscreen, 
			       CENTER, CENTER,
			       RIGHT,
			       0, 0,
			       "<C></K>PEOS Enactment Interface<!K>\n<C>Select a process instance to view",
			       instances, PEOS_MAX_PID,
			       TRUE,
			       A_REVERSE,
			       TRUE, TRUE);

    /* Is the scrolling list null? */
    if (scrollList == 0) {
	/* Exit CDK. */
	destroyCDKScreen (cdkscreen);
	endCDK();

	/* Print out a message and exit. */
	printf ("Oops. Could not make scrolling list. Is the window too small?\n");
	exit (1);
    }

    while (1) {
	refreshCDKScreen (cdkscreen);
	/* Activate the scrolling list. */
	selection = activateCDKScroll (scrollList, 0);

	/* Determine how the widget was exited. */
	if (scrollList->exitType == vESCAPE_HIT) {
	    break;
	} else if (scrollList->exitType == vNORMAL) {
	    display_action_list(selection);
	}
    }

    CDKfreeStrings(instances);
    destroyCDKScroll(scrollList);
}

display_model_list()
{
    int i, selection;
    char **models = (char **) peos_list_models();
    CDKSCROLL *scrollList = 0;

    /* Create the scrolling list. */
    scrollList = newCDKScroll (cdkscreen, 
			       CENTER, CENTER,
			       RIGHT,
			       0, 0,
			       "<C></K>PEOS Enactment Interface<!K>\n<C>Select a model to instantiate",
			       models, CDKcountStrings(models),
			       TRUE,
			       A_REVERSE,
			       TRUE, TRUE);

    /* Is the scrolling list null? */
    if (scrollList == 0) {
	/* Exit CDK. */
	destroyCDKScreen (cdkscreen);
	endCDK();

	/* Print out a message and exit. */
	printf ("Oops. Could not make scrolling list. Is the window too small?\n");
	exit (1);
    }

    refreshCDKScreen (cdkscreen);
    /* Activate the scrolling list. */
    selection = activateCDKScroll (scrollList, 0);

    /* Determine how the widget was exited. */
    if (scrollList->exitType == vNORMAL) {
	create_process(models[selection]);
    }

    CDKfreeStrings(models);
    destroyCDKScroll(scrollList);
}

create_process(char *model)
{
    int pid;
    int i;
    int num_resources;
    peos_resource_t *resources;
    char *msg[1];
    char tmp[BUFSIZ];
    msg[0] = tmp;
     resources = (peos_resource_t *) peos_get_resource_list(model, &num_resources);



    if (resources == NULL) {
        printf("error getting resources\n");
	return;
    }

    for(i = 0; i < num_resources; i++) {
	strncpy(resources[i].value, "%unbound", 256); /* XXX macro needed! */
    }

    if ((pid = peos_run(model,resources,num_resources)) < 0) {
	sprintf(tmp, "couldn't create process", pid);
	popupLabel(cdkscreen, msg, 1);
    } else {
	sprintf(tmp, "Created pid = %d\n", pid);
	popupLabel(cdkscreen, msg, 1);
    }
    
}
