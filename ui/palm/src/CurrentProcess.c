#include "CurrentProcess.h"
#include "../rsc/AppResources.h"
#ifdef STUB
#include "StubKernel.h"
#else
#include <events.h>
#include <process_table.h>
#endif

#include <stdlib.h>
#include <StringMgr.h>
#include <Form.h>
#include <Table.h>


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
/*
void InitTheTable(TableType *table, Char **itemLabels)
{
     UInt16 row;
     UInt16 rowsInTable;

     rowsInTable = TblGetNumberOfRows (table); // this was set by constructor
     for (row = 0; row < rowsInTable; row++)
     {
          // set the cell styles
          TblSetItemStyle (table, row, 0, labelTableItem);
          TblSetItemStyle (table, row, 1, labelTableItem);
          //if row is less than # of actions previously calculated
          //then set usable and unitialize
          if (row<numActions) 
          {
          	TblSetRowUsable (table, row, true); // so it can be clicked upon
          	TblSetRowSelectable (table, row, true);
			TblSetRowStaticHeight (table, row, true);
     	  	// fill in the data for each cell
	        TblSetItemPtr(table, row, 1, itemLabels[row]);
	        TblSetItemPtr(table, row, 0, "FIN");
          }
          //if row # equal to or above # of actions => hide the row
		  else TblSetRowUsable (table, row, false);
          
     }

     // make the columns usable so they're drawn and can be clicked etc.
     TblSetColumnUsable (table, 0, true);
     TblSetColumnUsable (table, 1, true);
     TblSetColumnSpacing(table, 0, 3);
	 TblSetColumnSpacing(table, 1, 3);   
}
*/

Boolean CurrentProcessHandler (EventType* pEvent)
{
	Boolean 	handled = false;
	FormType* 	pForm;
	ControlType* ctl;
	ListType *	list;
	TableType * table;


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
		//now moved to availprocesses and using peos_create_instance
		//		currentPid = peos_run (selection, NULL, 0);
		
		//use returned pid to DISPLAY list of actions
		//also pass numActions to be updated */
		currentActions = (peos_action_t *) peos_list_actions (currentPid, &numActions);
		//get a char** array of action names
		listElements2 = list_actions (currentActions, numActions);
		//populate the list with action names
		
		LstSetListChoices (list, listElements2, numActions);
		LstSetSelection (list, -1);
		
		table= FrmGetObjectPtr (pForm, FrmGetObjectIndex (pForm, actionsTable));
		//InitTheTable(table, listElements2);
		
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

char * SetUpScript()
{
	char *script;

	UInt32 stringSize, i;
	script = (char *) MemPtrNew(stringSize = (2+StrLen ("Name: ") + StrLen ("Script: ") + StrLen(currentActions[currentActionNumber].script)+StrLen (currentActions[currentActionNumber].name)));
	for (i=0; i < stringSize; i++)
	{
		script[i]='\0';
	}
	StrCat (script, "Name: ");
	StrCat (script, currentActions[currentActionNumber].name);
	StrCat (script, "\n");
	StrCat (script, "Script: ");
	StrCat (script, currentActions[currentActionNumber].script);
	return script;
}

char * script;

void fieldSetup (FieldType *fieldPtr, char * script)
{
	FldFreeMemory(fieldPtr);  // initialize everything, just in case.
	FldSetMaxChars(fieldPtr, StrLen(script));
	FldSetTextPtr(fieldPtr, script);
	FldRecalculateField(fieldPtr, true);
	
}


Boolean CurrentActionHandler (EventType* pEvent)
{
	Boolean 	handled = false;
	FormType  *pForm;
	FieldType *fieldPtr;
	ScrollBarType *bar;
	//only used for movable code chunks	MemHandle mem;
	Int16 maxScrollPos, value, min, max, pageSize;
	UInt16 scrollPos, textHeight, fieldHeight;
	vm_act_state currentActionState;
	vm_exit_code exit_code;	
	
	
	switch (pEvent->eType)
	{
		case frmOpenEvent:
			pForm = FrmGetActiveForm();		

			//get action state and act on the form title and FINISH button accordingly
			currentActionState=get_act_state(currentActions[currentActionNumber].name, currentActions, numActions);
			if (currentActionState == ACT_DONE || currentActionState == ACT_PENDING)
			{
				FrmHideObject (pForm, FrmGetObjectIndex (pForm, FINISH_BUTTON));
				FrmCopyTitle (pForm, "Finished Action");
			}
			else 
			{
				FrmCopyTitle (pForm, "Unfin. Action");
				FrmShowObject (pForm, FrmGetObjectIndex (pForm, FINISH_BUTTON));
			}
			
			if (currentActionNumber==0) 			 FrmHideObject (pForm, FrmGetObjectIndex (pForm, PREVIOUS_BUTTON));
			if (currentActionNumber==(numActions-1)) FrmHideObject (pForm, FrmGetObjectIndex (pForm, NEXT_BUTTON));
			
			if (script !=NULL) MemPtrFree(script);
			script = SetUpScript();

   		    fieldPtr = (FieldType *) FrmGetObjectPtr(pForm, FrmGetObjectIndex(pForm, 1901));
		    fieldSetup (fieldPtr, script);
		    
		    //////////////scrollbar stuff
			bar = (ScrollBarType *) FrmGetObjectPtr (pForm, FrmGetObjectIndex (pForm, 123));
			FldGetScrollValues(fieldPtr, &scrollPos, &textHeight, &fieldHeight);
			// Calculate the maximum scroll position:
			if(textHeight > fieldHeight) maxScrollPos = textHeight-fieldHeight;
			else maxScrollPos = 0;

			// Set the scrollbar's position
			// FieldHeight-1 gives an overlap of 1 line when page scrolling.
			SclSetScrollBar(bar, 0, 0, maxScrollPos, fieldHeight-1);
			////////////////////////
			
			
			FrmDrawForm(pForm);		
			handled = true;
			break;
		
		case sclRepeatEvent:
			pForm = FrmGetActiveForm();	
			fieldPtr = (FieldType *) FrmGetObjectPtr(pForm, FrmGetObjectIndex(pForm, 1901));	
			bar = (ScrollBarType *) FrmGetObjectPtr (pForm, FrmGetObjectIndex (pForm, 123));
			
			SclGetScrollBar(bar, &value, &min, &max, &pageSize);
			FldGetScrollValues(fieldPtr, &scrollPos, &textHeight, &fieldHeight);

			// Scroll the field according to the scrollbar's position
			if		(scrollPos > value)	   FldScrollField (fieldPtr, scrollPos-value, winUp);
			else if (scrollPos < value)    FldScrollField (fieldPtr, value-scrollPos, winDown);

		    handled =true;
			break;	
				
			
		case fldChangedEvent:
			pForm = FrmGetActiveForm();	
			fieldPtr = (FieldType *) FrmGetObjectPtr(pForm, FrmGetObjectIndex(pForm, 1901));	
			bar = (ScrollBarType *) FrmGetObjectPtr (pForm, FrmGetObjectIndex (pForm, 123));
			
			FldGetScrollValues(fieldPtr, &scrollPos, &textHeight, &fieldHeight);
			// Calculate the maximum scroll position:
			if(textHeight > fieldHeight) maxScrollPos = textHeight-fieldHeight;
			else maxScrollPos = 0;

			// Set the scrollbar's position
			// FieldHeight-1 gives an overlap of 1 line when page scrolling.
			SclSetScrollBar(bar, scrollPos, 0, maxScrollPos, fieldHeight-1);
					
			handled = true;	
			break;
		
		
		//i dont think this works.....
		/*case fldHeightChangedEvent:
			pForm = FrmGetActiveForm();	
			fieldPtr = (FieldType *) FrmGetObjectPtr(pForm, FrmGetObjectIndex(pForm, 1901));	
			bar = (ScrollBarType *) FrmGetObjectPtr (pForm, FrmGetObjectIndex (pForm, 123));
			
			FldGetScrollValues(fieldPtr, &scrollPos, &textHeight, &fieldHeight);
			// Calculate the maximum scroll position:
			if(textHeight > fieldHeight) maxScrollPos = textHeight-fieldHeight;
			else maxScrollPos = 0;

			// Set the scrollbar's position
			// FieldHeight-1 gives an overlap of 1 line when page scrolling.
			SclSetScrollBar(bar, 0, 0, maxScrollPos, fieldHeight-1);
		*/
		
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
				case PREVIOUS_BUTTON: //PREV
					currentActionNumber--;
					//FrmSetTitle (pForm, currentActions[currentActionNumber].name);
					
					//check if action is a finished action
					//update form title and FINISH button visibility accordingly
					currentActionState=get_act_state(currentActions[currentActionNumber].name, currentActions, numActions);
					if (currentActionState == ACT_DONE || currentActionState == ACT_PENDING)
					{
						FrmHideObject (pForm, FrmGetObjectIndex (pForm, FINISH_BUTTON));
						FrmCopyTitle (pForm, "Finished Action");
					}
					else 
					{
						FrmCopyTitle (pForm, "Unfin. Action");
						FrmShowObject (pForm, FrmGetObjectIndex (pForm, FINISH_BUTTON));
					}
					
					//if at the first action, HIDE the PREV button
					if (currentActionNumber<=0) FrmHideObject (pForm, FrmGetObjectIndex (pForm, PREVIOUS_BUTTON));	
					FrmShowObject (pForm, FrmGetObjectIndex (pForm, NEXT_BUTTON));

					if (script !=NULL) MemPtrFree(script);
					script = SetUpScript();

		   			fieldPtr = (FieldType *) FrmGetObjectPtr(pForm, FrmGetObjectIndex(pForm, 1901));
					fieldSetup (fieldPtr, script);			
					break;
				
				case NEXT_BUTTON: //NEXT
					currentActionNumber++;
					//FrmSetTitle (pForm, currentActions[currentActionNumber].name);	
					
					//check if action is a finished action
					//update form title and FINISH button visibility accordingly
					currentActionState=get_act_state(currentActions[currentActionNumber].name, currentActions, numActions);
					if (currentActionState == ACT_DONE || currentActionState == ACT_PENDING)
					{
						FrmHideObject (pForm, FrmGetObjectIndex (pForm, FINISH_BUTTON));
						FrmCopyTitle (pForm, "Finished Action");
					}
					else 
					{
						FrmCopyTitle (pForm, "Unfin. Action");
						FrmShowObject (pForm, FrmGetObjectIndex (pForm, FINISH_BUTTON));
					}
					
					//if at the last action, HIDE the NEXT button
					if (currentActionNumber>=(numActions-1)) FrmHideObject (pForm, FrmGetObjectIndex (pForm, NEXT_BUTTON));
					FrmShowObject (pForm, FrmGetObjectIndex (pForm, PREVIOUS_BUTTON));

					if (script != NULL)
						MemPtrFree(script);
					script = SetUpScript();

					fieldPtr = (FieldType *) FrmGetObjectPtr(pForm, FrmGetObjectIndex(pForm, 1901));
					fieldSetup (fieldPtr, script);
					break;
				
				case FINISH_BUTTON: //FINISH
					//peos notify changes state to finish if its last action
					//i think normal return code is VM_DONE		
					exit_code = peos_notify (currentPid, currentActions[currentActionNumber].name, PEOS_EVENT_FINISH);
					//currentActionState=get_act_state(currentActions[currentActionNumber].name, currentActions, numActions);
					if (exit_code==VM_DONE || exit_code==VM_CONTINUE)
					{
						FrmHideObject (pForm, FrmGetObjectIndex (pForm, FINISH_BUTTON));
						FrmCopyTitle (pForm, "Finished Action");
					}
					else 
					{
						FrmCopyTitle (pForm, "Unfin. Action");
						FrmShowObject (pForm, FrmGetObjectIndex (pForm, FINISH_BUTTON));
					}
				    
				    
				    break;
				
					
				default: break;
			}
			
			
			pForm = FrmGetActiveForm();	
			fieldPtr = (FieldType *) FrmGetObjectPtr(pForm, FrmGetObjectIndex(pForm, 1901));	
			bar = (ScrollBarType *) FrmGetObjectPtr (pForm, FrmGetObjectIndex (pForm, 123));
			
			FldGetScrollValues(fieldPtr, &scrollPos, &textHeight, &fieldHeight);
			// Calculate the maximum scroll position:
			if(textHeight > fieldHeight) maxScrollPos = textHeight-fieldHeight;
			else maxScrollPos = 0;

			// Set the scrollbar's position
			// FieldHeight-1 gives an overlap of 1 line when page scrolling.
			SclSetScrollBar(bar, 0, 0, maxScrollPos, fieldHeight-1);
			
			
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
			
		
		default: break;
		
	}
	
	return handled;	
}
