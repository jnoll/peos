/*
 * File: palmUI.c
 * Author: Mark Mastroieni
 * Date: 11/1/2
 */

#include <stdio.h>
#include <PalmOS.h>
#include "PalmPEOS.h"
#include "PalmUI.h"
#include "PalmEngine.h"

char old_msg[512];
char selection[dmDBNameLength]; //store user's selection from process list
processNode firstPN; // store the first node in the linked list
actionNode firstAN; // store the first node in the action list --for deallocation
instanceNode firstIN;
Int16 messageScrollPos; //store position of scroll bar 
Int16 scriptScrollPos;
static void SetFieldHandle (FieldType *field, MemHandle textH); 
MemHandle newMessageH, newScriptH; // handles to text fields on runAction form
char *newMessage;
char *newScript;
Boolean loadingInstance;

void sendUI(char new_msg[])
{
  WinEraseChars(old_msg, StrLen(old_msg), 10, 300);  
  WinDrawChars(new_msg, StrLen(new_msg), 10, 30);
  StrCopy(old_msg, new_msg);
}

static Boolean AppHandleEvent(EventPtr eventP)
{
  UInt16 formId;
  Boolean handled = false;
  FormType *frmP;
  ListType *lstP;
  actionNode First; /* these are used for RunActionForm */
  actionNode* Current; /* ibid. */
  char ** actionItems; /* ibid. */
  MemHandle A_handle, nextHandle, actionItemsH; /* ibid. - for _actions_ list */
  MemHandle P_handle, nextItemH; /* for accessing the
				    avail. processes list */
  MemHandle I_handle; /* handle to available instances list */

  FieldPtr messageFieldPtr, scriptFieldPtr;

  messageScrollPos = 0; // initially scroll car is at top	
  scriptScrollPos = 0; // initially scroll car is at top	

  if (eventP->eType == frmLoadEvent) {
    // Initialize and activate the form resource.
    formId = eventP->data.frmLoad.formID;
    frmP = FrmInitForm(formId);
    FrmSetActiveForm(frmP);
    handled = true;
  } else if (eventP->eType == frmOpenEvent) {
    //Load the form resource.
    frmP = FrmGetActiveForm();

    formId = eventP->data.frmOpen.formID;
    
    if (formId == NoModelsForm) {
      FormType *activeForm;
      activeForm = FrmGetActiveForm();
      FrmDrawForm(activeForm);
    }
    if (formId == SelectProcessForm) { //if its the SelectProcessForm, need to populate lists
      FormType *activeForm;
      int numChoices = 0;
      processNode init;
      processNode *currentNode;

      /* create process list */
    
	init = listModels();
      	P_handle = init.Next;
      	firstPN = init;
	
      	activeForm = FrmGetActiveForm();
      	lstP = FrmGetObjectPtr(activeForm, FrmGetObjectIndex(activeForm, 
							   ProcessList));
	
      	/* allocate space for processListItems
      	 * start with 10, expand if need be
       	*/
     	listItemsH = MemHandleNew(10*sizeof(char *));
      	processListItems = MemHandleLock(listItemsH);
     
      	//fill array of process to list
      	while (P_handle != NULL) {
		currentNode = MemHandleLock(P_handle);
		processListItems[numChoices] = currentNode->process;
		nextHandle = currentNode->Next;
		P_handle = nextHandle;
		numChoices++;
      	}
     
     	LstSetListChoices(lstP,processListItems,numChoices);

	/* create instance list if necessary */

	{
		int numChoices = 0;
      		instanceNode init;
      		instanceNode *currentNode;
	
		init = listInstances();

      		if(init.Next != NULL) {
			I_handle = init.Next;
     			firstIN = init;
      		} else
			I_handle = NULL;
      
      		lstP = FrmGetObjectPtr(activeForm, FrmGetObjectIndex(activeForm, 
							  InstanceList));
	
      		// allocate space for instanceListItems
      		// start with 10, expand if need be
       
      		instanceListItemsH = MemHandleNew(10*sizeof(char *));
     		instanceListItems = MemHandleLock(instanceListItemsH);
     
      		//fill array of instance to list
      		while (I_handle != NULL) {
			currentNode = MemHandleLock(I_handle);
			instanceListItems[numChoices] = currentNode->name;
			nextHandle = currentNode->Next;
			I_handle = nextHandle;
			numChoices++;
      		}
      
      		LstSetListChoices(lstP,instanceListItems,numChoices);
	}

      	FrmDrawForm(frmP);

    }
    if (formId == RunActionForm) {
      FormType* activeForm;
      int numActions = 0;
      char statusMsg[256]; /* instructions to user... */
      
      FrmDrawForm(frmP);

      /* display the process name/instance# - e.g. 00timesheet */
      StrNCopy(palm_msg, inst_num, 2);
      StrCopy(&palm_msg[2], inst_name);
      
      WinDrawChars(palm_msg, StrLen(palm_msg), 10, 45);


      messageFieldPtr = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, MessageField));
      scriptFieldPtr = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, ScriptField));
      activeForm = FrmGetActiveForm();
      lstP = FrmGetObjectPtr(activeForm, FrmGetObjectIndex(activeForm, 
							   ActionList));

	if(loadingInstance) {
		newMessageH = MemHandleNew(512);
		newScriptH = MemHandleNew(512);
		loadInstance(newMessageH, newScriptH); // calls loadInstructions
	}
	else {
		loadProcess(selection); // calls loadInstructions
      		StrCopy(statusMsg, "Select an available action to RUN it.");
      		newMessageH = MemHandleNew(512);
      		StrCopy(MemHandleLock(newMessageH), statusMsg);
	}

      actionItemsH = MemHandleNew(context.PROC_NACT);
      actionItems = MemHandleLock(actionItemsH);

      First = listActions();
      A_handle = First.Next;
      firstAN = First;

      // populate array of list items
      while(A_handle!=NULL){
	Current = MemHandleLock(A_handle);
	actionItems[numActions++] = Current->readyAction->ActName;

	nextHandle = Current->Next;
	A_handle = nextHandle;
      }
      
      LstSetListChoices(lstP,actionItems,numActions);
      FrmDrawForm(frmP);
      SetFieldHandle(messageFieldPtr,newMessageH);
	if(loadingInstance)
		SetFieldHandle(scriptFieldPtr, newScriptH);

    }   
    handled = true;
  } else if (eventP->eType == lstSelectEvent) {
    FormType *activeForm;
    char act_name[256]; /* a temp holder for the selected *action* name,
			   _if_ we're dealing with the _ActionList_ */
    activeForm = FrmGetActiveForm();
    
    /* discern between ProcessList, ActionList event (which form) */
    if (FrmGetFormId(activeForm) == SelectProcessForm) {
             if (eventP->data.lstSelect.listID == ProcessList)
		{
			loadingInstance = false;
      			lstP = FrmGetObjectPtr(activeForm, 
			FrmGetObjectIndex(activeForm, ProcessList));
      			StrCopy(selection,LstGetSelectionText(lstP, LstGetSelection(lstP)));
			StrCopy(inst_name, selection); // set instance name
			StrCopy(inst_num, getInstanceNum());
			
		}
		else if(eventP->data.lstSelect.listID == InstanceList)
		{
			loadingInstance = true;
			lstP = FrmGetObjectPtr(activeForm,
				FrmGetObjectIndex(activeForm, InstanceList));
			StrCopy(selection, LstGetSelectionText(lstP, LstGetSelection(lstP)));
			StrNCopy(inst_num, selection, 2);
			inst_num[3] = '\0';
			StrCopy(inst_name, &selection[2]);
		}
 
      deListInstances(firstIN);
      FrmGotoForm(RunActionForm);     
    } else if(FrmGetFormId(activeForm) == RunActionForm) {
      int numActions = 0;
      char statusMsg[256];
      char script[512];

      lstP = FrmGetObjectPtr(activeForm, 
			     FrmGetObjectIndex(activeForm, ActionList));
      StrCopy(act_name, LstGetSelectionText(lstP, LstGetSelection(lstP)));
      
      /* tell the Engine which action was clicked */
      selectAction(act_name, statusMsg, script); 
      deListActions(firstAN); //free up the memory...
      
      /* repopulate the list, re-draw the form */
      actionItemsH = MemHandleNew(context.PROC_NACT*sizeof(char *));
      actionItems = MemHandleLock(actionItemsH);
      
      First = listActions(); //gets all READY/RUNNING actions
      A_handle = First.Next;
      firstAN = First;

      if(A_handle==NULL) {  //process is done or waiting 
	deleteInstance();
	FrmGotoForm(SelectProcessForm);
      }
      // populate array of list items
      while(A_handle!=NULL){
	Current = MemHandleLock(A_handle);
	actionItems[numActions++] = Current->readyAction->ActName;
	nextHandle = Current->Next;
	A_handle = nextHandle;
      }

      LstSetListChoices(lstP,actionItems,numActions);
      frmP = FrmGetActiveForm(); /* we were getting an error w/o this... */
      FrmDrawForm(frmP); /* re-draw the form */

      /* let the user know what to do... */
      messageFieldPtr = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, MessageField));
      newMessageH = MemHandleNew(512);
      newMessage = MemHandleLock(newMessageH);
      StrCopy( newMessage, statusMsg);
      SetFieldHandle( messageFieldPtr, newMessageH);

      scriptFieldPtr = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, ScriptField));
      newScriptH = MemHandleNew(512);
      newScript = MemHandleLock(newScriptH);
      StrCopy(newScript, script);
      SetFieldHandle( scriptFieldPtr, newScriptH);

    } else {  /* error case */
      WinDrawChars("Error in lstSelectEvent:RunAction", 
		   StrLen("Error in lstSelectEvent:RunAction"), 10, 30);
    }
    handled = true;
  } else if (eventP->eType == ctlSelectEvent) {
    handled = true;
  } else if (eventP->eType == menuEvent) {

    if (eventP->data.menu.itemID == MenuSuspendProcess) {
      MenuEraseStatus(0); //Clear the menu status
      saveState(newMessage, newScript);
	removeState(); 
    }

    freeArgs();
    deListActions(firstAN);

    handled = true;
    FrmGotoForm(SelectProcessForm);
  } else if (eventP->eType == sclExitEvent) {
	 
    FormType *frmP;
    ScrollBarType *scrollP;
    FieldType *fld;
    UInt16 newScrollPos, textHeight, fieldHeight, linesToScroll;
    Int16 scrollVal, min, max, pageSize;
    WinDirectionType dir;
    char temp[100];
    char temp2[100];

    frmP = FrmGetActiveForm();

    switch (eventP->data.sclExit.scrollBarID) 
      {
      case MessageScroll:
	scrollP = FrmGetObjectPtr( frmP, FrmGetObjectIndex(frmP, MessageScroll));
	fld = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, MessageField));
	FldGetScrollValues(fld, &messageScrollPos, &textHeight, &fieldHeight);
	    
	if(eventP->data.sclExit.value > messageScrollPos)
	  dir = winDown;
	else if (eventP->data.sclExit.value < messageScrollPos)
	  dir = winUp;

	messageScrollPos = eventP->data.sclExit.value;
	FldScrollField(fld, 1,dir);
	break;
	
      case ScriptScroll:
	scrollP = FrmGetObjectPtr( frmP, FrmGetObjectIndex(frmP, ScriptScroll));
	fld = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, ScriptField));
	FldGetScrollValues(fld, &scriptScrollPos, &textHeight, &fieldHeight);
			
	if(eventP->data.sclExit.value > scriptScrollPos)
	  dir = winDown;
	else if (eventP->data.sclExit.value < scriptScrollPos)
	  dir = winUp;

	scriptScrollPos = eventP->data.sclExit.value;
	FldScrollField(fld, 1,dir);
	break;
						
      }	
			
  } else if (eventP->eType == menuEvent) {
    
    if (eventP->data.menu.itemID == MenuSuspendProcess) {
      MenuEraseStatus(0); //Clear the menu status
      saveState(newMessage, newScript); 
    } else if (eventP->data.menu.itemID == MenuExitProcess) {
      /* exit the process without saving state... */
      MenuEraseStatus(0); //Clear the menu status
      //removeState(" "); /* DEBUG: do we need this???? */
    } else {
      /* do nothing... */
      MenuEraseStatus(0);
    }
    
    handled = true;
    FrmGotoForm(SelectProcessForm);
  } else if (eventP->eType == appStopEvent) {
    // Unload the form resource.
    frmP = FrmGetActiveForm();

	deListProcesses(firstPN); 

	/* If we're leaving PalmPEOS from the RunActionForm
     	* we need to "suspend" the process - Engine's saveState() */
    if(FrmGetFormId(frmP) == RunActionForm) {
      saveState(newMessage, newScript);
	removeState();
    }


    FrmEraseForm(frmP);
    FrmDeleteForm(frmP);
    handled = true;
  }
  return (handled);
}

static void SetFieldHandle (FieldType *field, MemHandle textH) 
{
  MemHandle oldTextH;

  // Retrieve the field's old text handle
  oldTextH = FldGetTextHandle(field);
 
  // Set the new text handle for the field
  FldSetTextHandle(field, textH);
  FldDrawField(field);
  
  // free old text handle
  if (oldTextH) {
    MemHandleUnlock(oldTextH);
    MemHandleFree(oldTextH);
  }
}

UInt32 PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
  EventType event;
  Err err = 0;
  UInt16 cdNo;
  LocalID lid;
  DmSearchStateType srchst;
  
  switch (cmd) {
  case sysAppLaunchCmdNormalLaunch:
 
    /* -  with reloadable context files, check for the
     * context files, *then* check for corresponding
     * model files (instr. array will have to be re-loaded
     * from the model file for the context re-load to work
     * -  display a second list with suspended processes? 
     */
    if(DmGetNextDatabaseByTypeCreator
       (true,&srchst,'TEXt','PEOS',false,&cdNo,&lid) 
       == dmErrCantFind) /* no matches */
      FrmGotoForm(NoModelsForm);
    else
      FrmGotoForm(SelectProcessForm);
    
    do {
      UInt16 MenuError;
      
      EvtGetEvent(&event, evtWaitForever);
      
      if (! SysHandleEvent(&event))
	if (! MenuHandleEvent(0, &event, &MenuError))
	  if (! AppHandleEvent(&event))
	    FrmDispatchEvent(&event);
      
    } while (event.eType != appStopEvent);
    break;
    
  default:
    break;
  }
  
  return(err);
} /* PilotMain */
