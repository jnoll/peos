//Available processes
//
#include "AvailProcesses.h"
#include "../rsc/AppResources.h"
#include <stdlib.h>
#include <StringMgr.h>
//#include <Clipboard.h>

char * selection;

/*static void AvailProcessListDraw (ListType * list, RectangleType * bounds)
{
	//for 11 files
	char ** listElements = malloc (sizeof (char*) * 11);
	(char*) listElements [0] = "process1";
	listElements = malloc (sizeof (3));
	WinDrawChars (listElements [0], StrLen (listElements [0]), 
				bounds->topLeft.x, bounds->topLeft.y);
}
*/

Boolean CurrentProcessHandler (EventType* pEvent)
{
	Boolean 	handled = false;
	FormType* 	pForm;
	ControlType* ctl;
	ListType *	list;
	RectangleType theRect;

	
	//MemHandle text;
	
	switch (pEvent->eType)
	{
	case frmOpenEvent:		
		//form opened - display first action of the process that was selected
		//in the AvailableProcessesForm or StartedProcessesForm
		pForm = FrmGetActiveForm();
		FrmDrawForm(pForm);
		
		//for testing purposes only!!! - see if we can use global variable set in different form
		ctl = FrmGetObjectPtr (pForm, FrmGetObjectIndex (pForm, PreviousActionButton));
		CtlSetLabel (ctl, selection);
		//end test
		FrmCopyTitle (pForm, selection);
				
		//end test	
		handled = true;
		break;
	
	case menuEvent:
		switch (pEvent->data.menu.itemID)
		{
		case 1601:
			pForm = FrmInitForm(1100);
			FrmDoDialog(pForm);					// Display the About Box.
			FrmDeleteForm(pForm);
			handled = true;
			break;
		case 1602:
			pForm = FrmInitForm (MainForm);
			FrmGotoForm (MainForm);
			FrmDeleteForm (pForm);
			handled=true;
			break;
		//available processes form
		case 1603:
			pForm = FrmInitForm(AvailableProcessesForm);			
			FrmGotoForm (AvailableProcessesForm);
			FrmDeleteForm(pForm);
			handled = true;
			break;
		//started processes form
		case 1604:
			pForm = FrmInitForm(1500);	
			FrmGotoForm (1500);
			FrmDeleteForm(pForm);
			handled = true;
			break;
		default: break;
		}

	case lstSelectEvent:
		//remember selection
		pForm = FrmGetActiveForm();
		list = FrmGetObjectPtr (pForm, FrmGetObjectIndex (pForm, ActionsList));
		selection = LstGetSelectionText (list, LstGetSelection (list));
			
		ctl = FrmGetObjectPtr (pForm, FrmGetObjectIndex (pForm, 1402));
		CtlSetLabel (ctl, selection);	
		break;
		
	case ctlSelectEvent:
			switch (pEvent->data.ctlSelect.controlID)
			{
			case ActionButton:  //to to specific action view
				pForm = FrmGetActiveForm();
				list = FrmGetObjectPtr (pForm, FrmGetObjectIndex (pForm, ActionsList));
				if (LstGetSelection (list)==noListSelection)
				{
					FrmCustomAlert (CheckSelection, "an action", NULL, NULL);	
				}
				else 
				{
					//replace with code that switches to current action form
					/*pForm = FrmInitForm(CurrentProcessForm);			
					FrmGotoForm (CurrentProcessForm);
					FrmDeleteForm(pForm);
					handled = true;
					*/
				}
				break;
			}
			break;
	default: break;
	}
	
return handled;	
	
}

/*
char ** list_models (UInt16 * size)
{
	UInt16 i;
	char ** list = peos_list_models ();
	for (i=0; list[i]; i++)
	{
		*size=i;
	}
	return list;
}
*/

Boolean AvailableProcessesHandler (EventType* pEvent)
{
	Boolean 	handled = false;
	FormType* 	pForm;
	ListType*   list;
	ControlType* ctl;
	UInt16 numChoices=0;
	char ** listElements2;
	
		
	
		switch (pEvent->eType) {
		case lstSelectEvent:
			//remember selection
			pForm = FrmGetActiveForm();
			list = FrmGetObjectPtr (pForm, FrmGetObjectIndex (pForm, 1001));
			selection = LstGetSelectionText (list, LstGetSelection (list));
			
			ctl = FrmGetObjectPtr (pForm, FrmGetObjectIndex (pForm, 1201));
			CtlSetLabel (ctl, selection);	
			break;
			
			
		/*case lstSelectEvent:
			ctl = FrmGetObjectPtr (pForm, FrmGetObjectIndex (pForm, 1201));
			CtlSetLabel (ctl, selection);			
			break;
			*/
					
		case ctlSelectEvent:
			switch (pEvent->data.ctlSelect.controlID)
			{
			case 1201:  //create and go to process
				pForm = FrmGetActiveForm();
				list = FrmGetObjectPtr (pForm, FrmGetObjectIndex (pForm, 1001));
				if (LstGetSelection (list)==noListSelection)
				{
					//alert
					FrmCustomAlert (CheckSelection, "a process", NULL, NULL);	
				}
				else 
				{
					pForm = FrmInitForm(CurrentProcessForm);			
					FrmGotoForm (CurrentProcessForm);
					FrmDeleteForm(pForm);
					handled = true;
				}
				break;
			}
			break;
			
		case frmOpenEvent:	
			pForm = FrmGetActiveForm();
			list = FrmGetObjectPtr (pForm, FrmGetObjectIndex (pForm, 1001));

			//test 1:
			listElements2 = (char**) malloc (4*2);
			(char*) listElements2 [0] = "process1";
			(char*) listElements2 [1] = "proc";
			numChoices = ( 8 / sizeof (listElements2[0]));
			
			//test2:  real
			//listElements2 = list_models (&numChoices);
			
			LstSetListChoices (list, listElements2, numChoices);
			LstSetSelection (list, -1);

			//this may be a useful function later on
			//SysFormPointerArrayToStrings (listElements2, 1)
			FrmDrawForm(pForm);
			handled = true;
			break;
		
		case frmCloseEvent:
			
			//free (listElements2[0]);
			//free (listElements2[1]);
			//free (listElements2);

			break;
		
		default:
			break;
	}
	
	//free (listElements2);
	return handled;	
}