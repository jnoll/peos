#include "CurrentProcess.h"
#include "../rsc/AppResources.h"
#ifdef STUB
#include "StubKernel.h"
#endif
#include <stdlib.h>
#include <StringMgr.h>
#include <Form.h>

#ifdef STUB
#else
#include <events.h>
#include <process_table.h>
#endif

extern char * selection;
char * actionSelection;
peos_action_t * currentActions;
int itemSelected;
int numActions;
int currentActionNumber;
int currentPid;

int fromNextForm;
//extracts names of actions from the array of peos_action_t structs 
//and puts those names into char** array which can be later displayed as items in the list
//arguments: peos_action_t array, number of actions
//returns: char** aray of action names
char ** list_actions (peos_action_t * currentActions, int numActions)
{
	UInt16 i;
	char ** list;	

	//take peos_action_t * and convert it to char **
	list = (char**) malloc (numActions*4);
	for (i=0; i<numActions; i++) 
	{
		(char*) list[i]=(char*) malloc (256);
		(char*) list[i]=currentActions[i].name;
	}
	return list;
}
//
//
Boolean CurrentProcessHandler (EventType* pEvent)
{
	Boolean 	handled = false;
	FormType* 	pForm;
	ControlType* ctl;
	ListType *	list;


	//RectangleType theRect;
	
	char ** listElements2;

	switch (pEvent->eType)
	{
	case frmOpenEvent:		
		//form opened - display first action of the process that was selected
		//in the AvailableProcessesForm or StartedProcessesForm
		pForm = FrmGetActiveForm();		
		FrmCopyTitle (pForm, selection);
		list = FrmGetObjectPtr (pForm, FrmGetObjectIndex (pForm, ActionsList));				
		//CREATE process - for now pass NULL to resources, and 0 for number of resources
//		currentPid = peos_run (selection, NULL, 0);
		
		//use returned pid to DISPLAY list of actions
		//also pass numActions to be updated
		currentActions = (peos_action_t *) peos_list_actions (currentPid, &numActions);
		//get a char** array of action names
		listElements2 = list_actions (currentActions, numActions);
		//populate the list with action names
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
		itemSelected = LstGetSelection (list);
		currentActionNumber = itemSelected;
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
	//only used for movable code chunks	MemHandle mem;
	char * script;
	//char * script2;
	
	
	
	switch (pEvent->eType)
	{
		case frmOpenEvent:
			pForm = FrmGetActiveForm();		
			FrmCopyTitle (pForm, actionSelection);
			
			if (currentActionNumber==0) FrmHideObject (pForm, FrmGetObjectIndex (pForm, 1902));
			if (currentActionNumber==(numActions-1)) FrmHideObject (pForm, FrmGetObjectIndex (pForm, 1903));
			
			//testing purposes - stub out
			script = (char *) MemPtrNew(1+StrLen (currentActions[currentActionNumber].script));
			StrCopy(script, currentActions[currentActionNumber].script);
			
			//end testing
   		    fieldPtr = (FieldType *) FrmGetObjectPtr(pForm, FrmGetObjectIndex(pForm, 1901));
		    FldFreeMemory(fieldPtr);  // initialize everything, just in case.
		    FldSetMaxChars(fieldPtr, StrLen(script));
	        FldSetTextPtr(fieldPtr, script);
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
				fromNextForm = 1;
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
					currentActionNumber--;
					//if moved back before the first action - go back to list of actions
					/*if (currentActionNumber<0)
					{
						//currentActionNumber++;
						//FrmHideObject (pForm, FrmGetObjectIndex (pForm, 1902));
						pForm = FrmInitForm (CurrentProcessForm);
						FrmGotoForm (CurrentProcessForm);
						FrmDeleteForm (pForm);
						handled = true;
					}
					else
					{*/
						FrmCopyTitle (pForm, currentActions[currentActionNumber].name);
						if (currentActionNumber<=0) 
						{
							//currentActionNumber++;
							FrmHideObject (pForm, FrmGetObjectIndex (pForm, 1902));	
						}
						FrmShowObject (pForm, FrmGetObjectIndex (pForm, 1903));
						//if (script!=NULL) MemPtrFree (script);
						script = (char *) MemPtrNew(1+StrLen(currentActions[currentActionNumber].script));
						StrCopy(script, currentActions[currentActionNumber].script);

		   			    fieldPtr = (FieldType *) FrmGetObjectPtr(pForm, FrmGetObjectIndex(pForm, 1901));
					    FldFreeMemory(fieldPtr);  // clear the field from prev data
				    	FldSetMaxChars(fieldPtr, StrLen(script));
		    		    FldSetTextPtr(fieldPtr, script);
					    FldRecalculateField(fieldPtr, true);					
					//}
					break;
				
				case 1903: //next
					currentActionNumber++;
					//if incremented beyond the last action
					/*if (currentActionNumber>=numActions)
					{
						//currentActionNumber--;
						//FrmHideObject (pForm, FrmGetObjectIndex (pForm, 1903));
						pForm = FrmInitForm(CurrentProcessForm);			
						FrmGotoForm (CurrentProcessForm);
						FrmDeleteForm(pForm);
						handled = true;
					}
					else
					{*/
						FrmCopyTitle (pForm, currentActions[currentActionNumber].name);	
						if (currentActionNumber>=(numActions-1))
						{
							//currentActionNumber--;
							FrmHideObject (pForm, FrmGetObjectIndex (pForm, 1903));
						}
						FrmShowObject (pForm, FrmGetObjectIndex (pForm, 1902));
						//if (script!=NULL) MemPtrFree (script);
						script = (char *) MemPtrNew(1+StrLen(currentActions[currentActionNumber].script));
						StrCopy(script, currentActions[currentActionNumber].script);
				
				   		fieldPtr = (FieldType *) FrmGetObjectPtr(pForm, FrmGetObjectIndex(pForm, 1901));
					    FldFreeMemory(fieldPtr);  // clear the field from prev data
					    FldSetMaxChars(fieldPtr, StrLen(script));
	    			    FldSetTextPtr(fieldPtr, script);
					    FldRecalculateField(fieldPtr, true);
					//}					    	
				    //peos notify changes state to finish if its last action
				    //dont worry abt this yet...........
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
