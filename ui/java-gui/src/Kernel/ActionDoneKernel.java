package Kernel;

import java.io.*;
import java.net.*;

import Utils.*;
import PML.*;

///////////////////////////////////////////////////////////////////////////////
// Class Description:                                                        //
//                                                                           //
// This class is used to send an action done command to the PML              //
// Kernel.                                                                   //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
public class ActionDoneKernel extends KernelCommand {

  public ActionDoneKernel(PMLAction action)
  {
    DoneAction = action;
  }

  public void Execute() throws IOException
  {

    // Get the process associated with this action.
    PMLProcess processForAction = DoneAction.GetProcess();

    // Create the process string which is the process name concatenated
    // with the process id.
    String processStr = processForAction.GetName() + "." + 
                 processForAction.GetId();

    // Write the command string to the socket.
    SocketWriter.println("done " + processStr + " " + DoneAction.GetName());

    // Get the message returned from the done command.
    String messageRcvd = new String(SocketReader.readLine());

    PMLMessage doneStatus = new PMLMessage(messageRcvd);

    // Check the status of the done command.
    if (doneStatus.StatusCode != GoodStatus) {
      throw new PMLException("Done Action Failed");
    }


  }

  // Action to perform done on.
  private PMLAction DoneAction;
}
