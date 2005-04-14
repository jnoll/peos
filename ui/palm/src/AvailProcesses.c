//Available processes
//
#include "AvailProcesses.h"
#include "../rsc/AppResources.h"
#include <stdlib.h>
#include <StringMgr.h>

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

char ** list_models (UInt16 * size)
{
	UInt16 i;	
	//get the list of available processes that can be created 
	char ** list = (char**) peos_list_models ();
	/*char ** listElements2 = (char**) malloc (4*3);
	(char*) listElements2 [0] = "process1";
	(char*) listElements2 [1] = "process2";
	//malloc last element to null
	(char*) listElements2 [2] = NULL;
	list = listElements2;*/
	for (i=0; list[i]; i++)
	{
		*size=i;
	}
	*size = i;
	
	//if (i==10000)
	//	peos_list_models();
	//return list of process models, size of list is already updated 
	return list;
	//return listElements2;
}

Boolean AvailableProcessesHandler (EventType* pEvent)
{
	Boolean 	handled = false;
	FormType* 	pForm;
	ListType*   list;
	ControlType* ctl;
	UInt16 numChoices=0;
	char ** listElements2;
	
		
	
		switch (pEvent->eType) {
		case menuEvent:
			
			switch (pEvent->data.menu.itemID)
			{
			case 1701:
				pForm = FrmInitForm(1100);
				FrmDoDialog(pForm);					// Display the About Box.
				FrmDeleteForm(pForm);
				handled = true;
				break;
			case 1702:
				pForm = FrmInitForm (MainForm);
				FrmGotoForm (MainForm);
				FrmDeleteForm (pForm);
				handled=true;
				break;
		//started processes form
			case 1703:
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
			list = FrmGetObjectPtr (pForm, FrmGetObjectIndex (pForm, 1001));
			selection = LstGetSelectionText (list, LstGetSelection (list));
			
			ctl = FrmGetObjectPtr (pForm, FrmGetObjectIndex (pForm, 1201));
			CtlSetLabel (ctl, selection);	
			break;	
					
		case ctlSelectEvent:
			switch (pEvent->data.ctlSelect.controlID)
			{
			case 1201:  //create and go to process
				pForm = FrmGetActiveForm();
				list = FrmGetObjectPtr (pForm, FrmGetObjectIndex (pForm, 1001));
				if (LstGetSelection (list)==noListSelection)
				{
					//alert
					FrmCustomAlert (CheckSelection, "an available process", NULL, NULL);	
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
			//pass numChoices to update, get list of Elements		
			listElements2 = list_models (&numChoices);
			//populate the list
			LstSetListChoices (list, listElements2, numChoices);
			LstSetSelection (list, -1);
			//this may be a useful function later on
			//SysFormPointerArrayToStrings (listElements2, 1)
			FrmDrawForm(pForm);
			handled = true;
			break;
		
		case frmCloseEvent:
			//MemPtrFree((MemPtr) FldGetTextPtr(fieldPtr));
			//free (listElements2[0]);
			//free (listElements2[1]);
			//free (listElements2);

			break;
		
		default:
			break;
	}
	return handled;	
}