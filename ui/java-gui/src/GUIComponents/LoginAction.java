package GUIComponents;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.io.*;
import java.util.*;

import PML.*;
import PMLCommands.*;

///////////////////////////////////////////////////////////////////////////////
// Class Description:                                                        //
//                                                                           //
// This class performs a login action.  It will send a login message to      //
// PMLCommand.  It is both a window listener and an action listener.  The    //
// window listener functionality allows the login action to be performed     //
// when a window is opened. The action listener functionality provided by    //
// extending AbstractAction allows the login action to be preformed when a   //
// button or menu is selected.
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
class LoginAction extends AbstractAction
  implements WindowListener
{  
  /*
   * Constructors 
   */
  public LoginAction(String name, PMLController cntrl, 
                     LoginDialogBox loginDialog)
  {  
    putValue(Action.NAME, name);
    MainController = cntrl;
    Dialog = loginDialog;
  }

  /////////////////////////////////////////////////////////////////////////////
  //  This function implements the login functionality and will be called    //
  //  when a gui component is activated (i.e. a menu item is selected).  The //
  //  component must register this as a listener for the component.          //
  /////////////////////////////////////////////////////////////////////////////
  public void actionPerformed(ActionEvent evt)
  {  
    PerformLogin();
  }

  /////////////////////////////////////////////////////////////////////////////
  //  This function will display a dialog to the user prompting for user name//
  //  and password. It will then send login message to PMLCommand. It will   //
  //  also send a GetProcess message to PMLCommand.  The processes that are  //
  //  return are used to call LoggedIn on the main controller.               //
  /////////////////////////////////////////////////////////////////////////////
  public void PerformLogin()
  {  
    try {
      ConnectInfo conInfo = new ConnectInfo();

      // Get the user name and password from the user.
      Dialog.showDialog(conInfo);

      // Login.
      PMLCommand.Login(conInfo.userName, conInfo.password);

      // Get list of processes.
      PMLProcessList processes = PMLCommand.GetProcesses();

      MainController.LoggedIn(processes);
    }

    catch (IOException e) {
     MainController.DisplayError(e.getMessage());
    } 
  }

  
  /////////////////////////////////////////////////////////////////////////////
  //  This function implements the login functionality and will be called    //
  //  when a window is opened.  The window must register this as a listener  //
  //  for the window.                                                        //
  /////////////////////////////////////////////////////////////////////////////
  public void windowOpened(WindowEvent e)
  {
    PerformLogin();
  }

  // Other Window listener functions which must be implemented for a window
  // listener.  They are null functions since this class only cares about
  // windowOpended.

  public void windowActivated(WindowEvent e)
  {

  }

  public void windowClosed(WindowEvent e)
  {

  }

  public void windowClosing(WindowEvent e)
  {

  }

  public void windowDeactivated(WindowEvent e)
  {

  }

  public void windowDeiconified(WindowEvent e)
  {

  }

  public void windowIconified(WindowEvent e)
  {

  }

  // Dialog box to prompt user for user name and password.
  private LoginDialogBox Dialog;

  // Main controller for GUI.
  private PMLController MainController;
}
