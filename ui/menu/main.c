/* -*-C-*-
*****************************************************************************
*
* File:         $RCSFile: main.c$
* Version:      $Id: main.c,v 1.2 2004/01/13 20:50:51 jnoll Exp $ ($Name:  $)
* Description:  Menu driven interface for PEOS.
* Author:       John Noll, Santa Clara University
* Created:      Fri Jun 27 19:35:11 2003
* Modified:     Tue Jan 13 08:20:24 2004 (John Noll, SCU) jnoll@carbon.cudenver.edu
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

#define DEFAULT_HINT "B)ack to previous screen, Q)uit, ? for help"
CDKSCREEN *cdkscreen = 0;
WINDOW *cursesWin = 0;
CDKLABEL *hint = 0;

void quit()
{
    destroyCDKScreen (cdkscreen);
    delwin (cursesWin);

    /* Shutdown CDK. */
    endCDK();

    /* Exit cleanly. */
    exit (0);
}

void display_msg(char *txt)
{
    char **msg = CDKsplitString(txt, '\n');
    
    popupLabel (cdkscreen, msg, CDKcountStrings(msg)); 
    CDKfreeStrings(msg);
}

int handle_default_keys(EObjectType cdktype GCC_UNUSED,
		       void *target GCC_UNUSED,
		       void *clientData GCC_UNUSED, chtype key)
{
   boolean escape = FALSE;
   chtype actual;

   switch (key) {
   case KEY_LEFT:
   case 'b':
   case 'B':
       escape = TRUE;
       break;
   case 'q':
       quit();
       break;
   case '?':
       display_msg("Type 'B' or 'b' to go to previous screen\n'q' to quit\n'?' for help.\nHit any key to continue.");
       escape = FALSE;
       break;
   default: 
       escape = FALSE;
       break;
   }

   return escape;
}

void bind_default_keys(void *object)
{
    bindCDKObject (0, object, KEY_LEFT, handle_default_keys, NULL);
    bindCDKObject (0, object, 'b', handle_default_keys, NULL);
    bindCDKObject (0, object, 'B', handle_default_keys, NULL);
    bindCDKObject (0, object, 'q', handle_default_keys, NULL);
    bindCDKObject (0, object, '?', handle_default_keys, NULL);
}


void display_hint(char *line1, char *line2)
{
    char *msg[3];
    int i, width;
    boolean box = FALSE;


    if (hint == 0) {
	width = getmaxx(cdkscreen->window);
	msg[0] = (char *)calloc(width + 1, sizeof(char));
	memset(msg[0], ' ', width);
	msg[width] = '\0';
	msg[1] = msg[0];
	hint = newCDKLabel(cdkscreen,
			   CENTER, BOTTOM,
			   msg, 2,
			   box, FALSE);
    }

    i = 0;
    msg[i++] = line1;
    msg[i++] = line2 ? line2 : DEFAULT_HINT;
    eraseCDKLabel(hint);
    setCDKLabel(hint, msg, i, box);
    drawCDKLabel(hint, box);
}

display_main_menu()
{
    CDKSCROLL *peosMenu;
    char *menulist[MAX_MENU_ITEMS];
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
			    CENTER, TOP,
			    RIGHT,
			    -2, 0,
			    "<C></K>PEOS Enactment Interface<!K>",
			    menulist, entry,
			    TRUE,
			    A_REVERSE,
			    TRUE, TRUE);
    bind_default_keys(peosMenu);

/* Loop until we are done. */
    while (1) {
/* Draw the CDK screen. */
	refreshCDKScreen (cdkscreen);

/* Activate the menu */
	display_hint("</K><RET><!K> Selects an operation.", 
		     "Q)uit, ? for help");
	selection= activateCDKScroll(peosMenu, 0);
	switch (selection) {
	case 0:			/* View process instances. */
	    display_process_list();
	    break;
	case 1:			/* Create process instance. */
	    display_model_list();
	    break;
	case 2:		/* View worklist */
	    display_msg("View Worklist");
	    break;
	case 3:		/* View->action */
	    display_msg("Help");
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
