package GUIComponents;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import java.util.*;
import java.io.*;

import PML.*;
import PMLCommands.*;

///////////////////////////////////////////////////////////////////////////////
// Class Description:                                                        //
//                                                                           //
// This class is used to perform an done action command.  It will send a     //
// done action message to PMLCommand.                                        //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
class ActionDone extends AbstractAction
{  
  public ActionDone(PMLActionFrame frame, PMLController mainController, 
                    PMLAction action)
  {  
    FrameForAction = frame;
    MainController = mainController;
    ActionToPerformDoneOn = action;
  }

  /////////////////////////////////////////////////////////////////////////////
  //  This function implements the perform action functionality and will be  //
  //  called when a gui component is activated (i.e. a gui button is pushed).//
  //  The component must register this as a listener for the component. This //
  //  function will send a done message to PMLCommand. It will also          //
  //  send a GetProcess message to PMLCommand.  The processes that are       //
  //  return are used to call UpdateProcesses on the main controller.  The   //
  //  processes need to be updated because the actions for the process could //
  //  change after the done action is sent.                                  //
  /////////////////////////////////////////////////////////////////////////////
  public void actionPerformed(ActionEvent evt)
  {  

    try {
      PMLCommand.DoneAction(ActionToPerformDoneOn);

      PMLProcessList processes = PMLCommand.GetProcesses();

      FrameForAction.setVisible(false);
      FrameForAction.dispose();

      MainController.RemoveActionFrame(FrameForAction);
      MainController.UpdateProcesses(processes);
    }

    catch (IOException e) {
      System.out.println("Error: " + e);
    }

  }

  private PMLController MainController;
  private PMLAction ActionToPerformDoneOn;
  private PMLActionFrame FrameForAction;
}
