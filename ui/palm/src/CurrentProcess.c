#include "CurrentProcess.h"
#include "../rsc/AppResources.h"
#include <stdlib.h>
#include <StringMgr.h>
//#include <Clipboard.h>
extern char * selection;
char * actionSelection;
//function extrats names of actions from the array of peos_action_t structs 
//and puts those names into char** array which can be later displayed as items in the list
char ** list_actions (peos_action_t * currentActions, int numActions)
{
	UInt16 i;
	char ** list;
	char ** listElements = (char**) malloc (4*3);
	(char*) listElements [0] = "act1";
	(char*) listElements [1] = "act2";
	(char*) listElements [2] = NULL;
	list = listElements;
	
	//HERE:
	//write the code that would take peos_action_t * and convert it to char **
	//malloc last element to null
	// char ** list = peos_list_models ();
	
	return listElements;
}


Boolean CurrentProcessHandler (EventType* pEvent)
{
	Boolean 	handled = false;
	FormType* 	pForm;
	ControlType* ctl;
	ListType *	list;
	int currentPid;
	int numActions;
	RectangleType theRect;
	peos_action_t * currentActions;
	char ** listElements2;

	switch (pEvent->eType)
	{
	case frmOpenEvent:		
		//form opened - display first action of the process that was selected
		//in the AvailableProcessesForm or StartedProcessesForm
		pForm = FrmGetActiveForm();		
		FrmCopyTitle (pForm, selection);
		list = FrmGetObjectPtr (pForm, FrmGetObjectIndex (pForm, ActionsList));				
		//create process - for now pass NULL to resources, and 0 for number of resources
		currentPid =  0; //peos_create_instance (selection, NULL, 0);
		//use returned pid to display list of actions	
		//currentActions = peos_list_actions (currentPid, &numActions);

		numActions=2;
		
		listElements2 = list_actions (currentActions, numActions);
		LstSetListChoices (list, listElements2, numActions);
		LstSetSelection (list, -1);
			
		FrmDrawForm(pForm);		
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
		actionSelection = LstGetSelectionText (list, LstGetSelection (list));
			
		ctl = FrmGetObjectPtr (pForm, FrmGetObjectIndex (pForm, ActionButton));
		CtlSetLabel (ctl, actionSelection);	
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
					pForm = FrmInitForm(CurrentActionForm);			
					FrmGotoForm (CurrentActionForm);
					FrmDeleteForm(pForm);
					handled = true;
				}
				break;
			}
			break;
			
		case frmCloseEvent:
			break;
			
		default: break;
	}
	
return handled;		
}


Boolean CurrentActionHandler (EventType* pEvent)
{
	Boolean 	handled = false;
	FormType* 	pForm;
	FieldType *fieldPtr;
	MemHandle mem;
	char * actionScript;
	char * script;
	char * script2;

	
	
	switch (pEvent->eType)
	{
		case frmOpenEvent:
			pForm = FrmGetActiveForm();		
			FrmCopyTitle (pForm, actionSelection);
			
			//testing purposes - stub out
			script = (char *) MemPtrNew(1+StrLen("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaddddddddddddddddddddddddd"));
			StrCopy(script, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaddddddddddddddddddddddddd");
			
			//end testing
			actionScript = (char *) MemPtrNew(1+StrLen(script));
			StrCopy(actionScript, script);
   		    fieldPtr = (FieldType *) FrmGetObjectPtr(pForm, FrmGetObjectIndex(pForm, 1901));
		    FldFreeMemory(fieldPtr);  // initialize everything, just in case.
		    FldSetMaxChars(fieldPtr, StrLen(actionScript));
	        FldSetTextPtr(fieldPtr, actionScript);
		    FldRecalculateField(fieldPtr, true);
			
			
			
			FrmDrawForm(pForm);		
			handled = true;
			break;
			
		case menuEvent:
			switch (pEvent->data.menu.itemID)
			{
			case 2001:
				pForm = FrmInitForm(1100);
				FrmDoDialog(pForm);					// Display the About Box.
				FrmDeleteForm(pForm);
				handled = true;
				break;
			case 2002:
				pForm = FrmInitForm (MainForm);
				FrmGotoForm (MainForm);
				FrmDeleteForm (pForm);
				handled=true;
				break;
			//available processes form
			case 2003:
				pForm = FrmInitForm(AvailableProcessesForm);			
				FrmGotoForm (AvailableProcessesForm);
				FrmDeleteForm(pForm);
				handled = true;
				break;
			//started processes form
			case 2004:
				pForm = FrmInitForm(1500);	
				FrmGotoForm (1500);
				FrmDeleteForm(pForm);
				handled = true;
				break;
			case 2005:
				pForm = FrmInitForm (CurrentProcessForm);
				FrmGotoForm (CurrentProcessForm);
				FrmDeleteForm (pForm);
				handled = true;
				break;
			default: break;
		}
			break;
			
		case ctlSelectEvent:
			pForm = FrmGetActiveForm();	
			switch (pEvent->data.ctlSelect.controlID)
			{
				case 1902: //prev
					FrmCopyTitle (pForm, "prevAction");	
					script2 = (char *) MemPtrNew(1+StrLen("cccccccccccccccccccccccccccccccccc"));
					StrCopy(script2, "cccccccccccccccccccccccccccccccccc");
			

		   		    fieldPtr = (FieldType *) FrmGetObjectPtr(pForm, FrmGetObjectIndex(pForm, 1901));
				    FldFreeMemory(fieldPtr);  // clear the field from prev data
				    FldSetMaxChars(fieldPtr, StrLen(script2));
	    		    FldSetTextPtr(fieldPtr, script2);
				    FldRecalculateField(fieldPtr, true);					
					break;
				
				case 1903: //next
					FrmCopyTitle (pForm, "nextAction");	
					script2 = (char *) MemPtrNew(1+StrLen("cccccccccccccccccccccccccccccccccc"));
					StrCopy(script2, "cccccccccccccccccccccccccccccccccc");
			
		
		   		    fieldPtr = (FieldType *) FrmGetObjectPtr(pForm, FrmGetObjectIndex(pForm, 1901));
				    FldFreeMemory(fieldPtr);  // clear the field from prev data
				    FldSetMaxChars(fieldPtr, StrLen(script2));
	    		    FldSetTextPtr(fieldPtr, script2);
				    FldRecalculateField(fieldPtr, true);
					break;
					
				default: break;
			}
			break;
		
		case frmCloseEvent:
			 pForm = FrmGetActiveForm();
      
		      // Free the buffer we allocated for this field and gave 
		      // it as a pointer. This is only OK since we know the field
		      // is about to be destroyed, and because we know it is 
		      // a pointer-based field.
		      fieldPtr = (FieldType *) FrmGetObjectPtr(pForm, FrmGetObjectIndex(pForm, 1901));
		      MemPtrFree((MemPtr) FldGetTextPtr(fieldPtr));
		      //MemPtrFree( actionScript);
		      //MemPtrFree( script);
      break;
			
			break;
		default:
			break;
		
	}
	
	return handled;	
}