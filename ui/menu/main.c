/* -*-C-*-
*****************************************************************************
*
* File:         $RCSFile: main.c$
* Version:      $Id: main.c,v 1.1 2004/01/11 20:32:54 jnoll Exp $ ($Name:  $)
* Description:  Menu driven interface for PEOS.
* Author:       John Noll, Santa Clara University
* Created:      Fri Jun 27 19:35:11 2003
* Modified:     Sun Jan 11 10:49:39 2004 (John Noll, SCU) jnoll@carbon.cudenver.edu
* Language:     C
* Package:      N/A
* Status:       $State: Exp $
*
* (c) Copyright 2003, Santa Clara University, all rights reserved.
*
*****************************************************************************
*/

#include <curses.h>
#include <cdk/cdk.h>

#ifdef HAVE_XCURSES
char *XCursesProgramName="peos";
#endif

extern int display_process(int pid);

CDKSCREEN *cdkscreen		= 0;
WINDOW *cursesWin		= 0;

void quit()
{
    destroyCDKScreen (cdkscreen);
    delwin (cursesWin);

    /* Shutdown CDK. */
    endCDK();

    /* Exit cleanly. */
    exit (0);
}


display_main_menu()
{
    CDKSCROLL *peosMenu;
    char *menulist[MAX_MENU_ITEMS];
    char *msg[10];
    int entry, selection;

/* Create the menu lists. */
    entry = 0;
    menulist[entry++] = "List Process Instances"; 
    menulist[entry++] = "Create Process"; 
    menulist[entry++] = "Show Worklist"; 
    menulist[entry++] = "Help";
    menulist[entry++] = "Quit   ";

/* Create the menu. */
    peosMenu = newCDKScroll(cdkscreen, 
			    CENTER, CENTER,
			    RIGHT,
			    0, 0,
			    "<C></K>PEOS Enactment Interface<!K>\n<C>Select an Operation",
			    menulist, entry,
			    TRUE,
			    A_REVERSE,
			    TRUE, TRUE);

/* Define the help key binding. */
/*bindCDKObject (vMENU, peosMenu, '?', helpCB, 0);*/


/* Loop until we are done. */
    while (1) {
/* Draw the CDK screen. */
	refreshCDKScreen (cdkscreen);

/* Activate the menu */
	selection= activateCDKScroll(peosMenu, 0);
	switch (selection) {
	case 0:			/* View process instances. */
	    display_process_list();
	    break;
	case 1:			/* Create process instance. */
	    display_model_list();
	    break;
	case 2:		/* View worklist */
	    msg[0] = "View Worklist";
	    popupLabel (cdkscreen, msg, 1); 
	    break;
	case 3:		/* View->action */
	    msg[0] = "About->Help";
	    popupLabel (cdkscreen, msg, 1);
	    break;
	case 4:
	    destroyCDKScroll(peosMenu);
	    quit();
	    break;
	default:
	    break;
	}
    }
}

int main(int argc, char **argv)
{
   /* Declare variables. */
   CDK_PARAMS params;

   CDKparseParams(argc, argv, &params, CDK_CLI_PARAMS);

   /* Set up CDK. */
   cursesWin = initscr();
   cdkscreen = initCDKScreen (cursesWin);

   /* Set up CDK Colors. */
/*   initCDKColor();*/


   display_main_menu();

   /* Clean up. */
   quit();
}
