package GUIComponents;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import java.util.*;
import java.io.*;

import PML.*;
import PMLCommands.*;

/*
 * Class Description: 
 */
class SelectAction extends AbstractAction
{  
  /*
   * Constructors 
   */
  public SelectAction(PMLController cntrl, JList actionList)
  {  
     MainController = cntrl;
     ActionListComp = actionList;
  }

  public void SetActionList(PMLActionList actions)
  {
    MyActions = actions;
  }

  public void actionPerformed(ActionEvent evt)
  {  
    String actionName = (String)ActionListComp.getSelectedValue();

    PMLAction action = MyActions.Find(actionName);

    try {
      // Have to make sure Action is not already running
      PMLProcessList runningProcesses = PMLCommand.GetRunningProc();

      boolean foundAction = false;
      for (Iterator i = runningProcesses.iterator(); 
           i.hasNext() && foundAction == false;) {
        PMLProcess nextProcess = (PMLProcess) i.next();
        if ( nextProcess.GetActions().Find(action) != null ) {
          foundAction = true;
        }

      } 

      if (!foundAction) {
        PMLCommand.RunAction(action);
      }
      MainController.ActionSelected(action);
    }

    catch (IOException e) {
     System.out.println("Error: " + e);
    }

  }

  private PMLController MainController;
  private PMLActionList MyActions;
  private JList ActionListComp;
}
