package PMLCommands;

import java.io.*;
import java.net.*;
import java.util.*;

import Utils.*;
import PML.*;
import Kernel.*;

///////////////////////////////////////////////////////////////////////////////
// Class Description:                                                        //
//                                                                           //
// This class provides the interface to the PML Kernel Commands.  It is used //
// by the ui to communicate with the PML Kernel.                             //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
public class PMLCommand {
 
  /////////////////////////////////////////////////////////////////////////////
  // Creates a PML Model.                                                    //
  /////////////////////////////////////////////////////////////////////////////
  static public void CreateModel(String model) throws IOException
  {
    CreateKernel createModel = new CreateKernel(model);

    createModel.Execute();
  }

  /////////////////////////////////////////////////////////////////////////////
  // Tells the PML Kernel that a user has completed an action.               //
  /////////////////////////////////////////////////////////////////////////////
  static public void DoneAction(PMLAction action) throws IOException
  {
    // Create an action kernel command and execute it.
    ActionDoneKernel run = new ActionDoneKernel(action);

    run.Execute();
  }

  /////////////////////////////////////////////////////////////////////////////
  // Get the actions for a process.                                          //
  /////////////////////////////////////////////////////////////////////////////
  static public PMLActionList GetActions(String processName, String processID) 
      throws IOException
  {
    // Perform and available command
    AvailableKernel available = new AvailableKernel();
    PMLProcessList availableProc = null;
    availableProc = available.Execute();

    // Find the process in the list returned from available.
    PMLProcess process = availableProc.Find(processName, processID);

    // return the actions for the process.
    return process.GetActions();
  }

  /////////////////////////////////////////////////////////////////////////////
  // Get the models which the user can create.                               //
  /////////////////////////////////////////////////////////////////////////////
  static public LinkedList GetModels() 
      throws IOException
  {

    // Perform a list command
    ListKernel listModels = new ListKernel();
    LinkedList models = listModels.Execute();

    // return the available models
    return models;
  }

  /////////////////////////////////////////////////////////////////////////////
  // Get all processes including available and running.  Running processes   //
  // are processes which have an action running.                             //
  /////////////////////////////////////////////////////////////////////////////
  static public PMLProcessList GetProcesses() throws IOException
  {
    // Perform an available command.
    AvailableKernel available = new AvailableKernel();
    PMLProcessList processes = null;
    processes = available.Execute();

    // Perform an running command.
    RunningKernel runningProcCmd = new RunningKernel();
    PMLProcessList runningProc = runningProcCmd.Execute();


    // Merge the running process list with the available processes
    for (Iterator i = runningProc.iterator(); i.hasNext();) {
      PMLProcess nextProcess = (PMLProcess) i.next();

      PMLProcess foundProcess = processes.Find(nextProcess);
      if (foundProcess == null) {
        // Did not find the process in the set.  Add it to list
        processes.add(nextProcess);
      } else {
        // Found it add its actions to the foundProcess
        foundProcess.add(nextProcess.GetActions());
      }
    }

    return processes;
  }

  /////////////////////////////////////////////////////////////////////////////
  // Get processes which have an action running.
  /////////////////////////////////////////////////////////////////////////////
  static public PMLProcessList GetRunningProc() throws IOException
  {
    // Perform run command.
    RunningKernel runningProcCmd = new RunningKernel();
    PMLProcessList runningProc = null;
    runningProc = runningProcCmd.Execute();

    return runningProc;
  }

  /////////////////////////////////////////////////////////////////////////////
  // Login to the PML Kernel                                                 //
  /////////////////////////////////////////////////////////////////////////////
  static public void Login(String userName, String password, String peosHost,
                           int peosPort) 
    throws IOException
  {
    LoginKernel login = new LoginKernel();

    login.Execute(userName, password, peosHost, peosPort);
  }

  /////////////////////////////////////////////////////////////////////////////
  // Send Logout to the PML Kernel                                           //
  /////////////////////////////////////////////////////////////////////////////
  static public void Logout() throws IOException
  {
       LogoutKernel logout = new LogoutKernel();

       logout.Execute();
  }

  /////////////////////////////////////////////////////////////////////////////
  // Run an action.                                                          //
  /////////////////////////////////////////////////////////////////////////////
  static public void RunAction(PMLAction action) throws IOException
  {
    RunActionKernel run = new RunActionKernel(action);

    run.Execute();
  }
}
