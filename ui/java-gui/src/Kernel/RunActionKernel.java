package Kernel;

import java.io.*;
import java.net.*;

import Utils.*;
import PML.*;

///////////////////////////////////////////////////////////////////////////////
// Class Description:                                                        //
//                                                                           //
// This class is used to send a run action command to the PML Kernel.        //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
public class RunActionKernel extends KernelCommand {

  public RunActionKernel(PMLAction action)
  {
    ActionToRun = action;
  }

  public void Execute() throws IOException
  {

    // Get the process associated with this action.
    PMLProcess processForAction = ActionToRun.GetProcess();

    // Create the process string which is the process name concatenated
    // with the process id.
    String processStr = processForAction.GetName() + "." + 
                 processForAction.GetId();

    // Write the command string to the socket.
    SocketWriter.println("run " + processStr + " " + ActionToRun.GetName());

    String messageRcvd = new String(SocketReader.readLine());

    PMLMessage runStatus = new PMLMessage(messageRcvd);

    // Check the status of the run command.
    if (runStatus.StatusCode != GoodStatus) {
      throw new PMLException("Run Action Failed");
    }

  }

  private PMLAction ActionToRun;
}
