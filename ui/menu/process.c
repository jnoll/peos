#include <curses.h>
#include <cdk/cdk.h>
#include <os/kernel/events.h>

extern CDKSCREEN *cdkscreen;
extern WINDOW *cursesWin;
extern void display_hint(char *l1, char *l2);
extern void display_msg(char *txt);

display_process_list()
{
    CDKSCROLL *scrollList = 0;
    CDKLABEL *hint;
    char *msg[5];
    int i, selection;
    char **instances = peos_list_instances(instances);

    for (i = 0; i <= PEOS_MAX_PID; i++) {
	if (instances[i] == NULL || strlen(instances[i]) == 0) break;
    }

    /* Create the scrolling list. */
    scrollList = newCDKScroll (cdkscreen, 
			       CENTER, TOP,
			       RIGHT,
			       -2, 0,
			       "<C></K>Available Process Models<!K>",
			       instances, i,
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

    bind_default_keys(scrollList);

    while (1) {
	refreshCDKScreen (cdkscreen);
	display_hint("</K><RET><!K> Selects a process to view.", NULL);
	/* Activate the scrolling list. */
	selection = activateCDKScroll (scrollList, 0);

	/* Determine how the widget was exited. */
	if (scrollList->exitType == vESCAPE_HIT) {
	    break;
	} else if (scrollList->exitType == vNORMAL) {
	    display_action_list(selection, instances[scrollList->currentItem]);
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
			       "<C></K>Create Process<!K>",
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

    bind_default_keys(scrollList);

    refreshCDKScreen (cdkscreen);
    /* Activate the scrolling list. */
    display_hint("</K><RET><!K> selects a model to instantiate.", NULL);
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
    int i, num_resources, pid;
    char tmp[BUFSIZ];
    peos_resource_t *resources = peos_get_resource_list(model, &num_resources);


    if (resources == NULL) {
        printf("error getting resources\n");
	return;
    }

    for(i = 0; i < num_resources; i++) {
	strncpy(resources[i].value, "%unbound", 256); /* XXX macro needed! */
    }

    if ((pid = peos_run(model,resources,num_resources)) < 0) {
	display_msg("couldn't create process");
    } else {
	sprintf(tmp, "Created pid = %d\n", pid);
	display_msg(tmp);
    }
    
}
