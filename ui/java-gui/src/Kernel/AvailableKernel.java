package Kernel;

import java.io.*;
import java.net.*;
import java.util.*;

import Utils.*;
import PML.*;

///////////////////////////////////////////////////////////////////////////////
// Class Description:                                                        //
//                                                                           //
// This class is used to perform to send an available command to the PML     //
// Kernel.  It parses the information returned from the kernel into a        //
// a list of processes.                                                      //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
public class AvailableKernel extends KernelCommand {

  public PMLProcessList Execute() throws IOException
  {
    PMLProcessList processes = new PMLProcessList();

    String messageRcvd;
    int statusCode = ErrorStatus;

    // Write the command string to the socket.
    SocketWriter.println("available");

    // Parse data received from available
    while ( (messageRcvd = SocketReader.readLine()) != null ) {

      PMLMessage messageLine = new PMLMessage(messageRcvd);
      statusCode = messageLine.StatusCode;

      // If status is not good or message length is less than 4
      // then processing for the available command is complete.
      if (statusCode != GoodStatus || messageLine.Content == null) {
        break;
      }
          
      StringTokenizer t = new StringTokenizer(messageLine.Content, " ");

      // Get the process name and id string.
      String nameAndIdStr = t.nextToken();
      StringTokenizer nameAndID = new StringTokenizer(nameAndIdStr, ".");

      // Parse the process name and process id
      String processName = nameAndID.nextToken();
      String processID = nameAndID.nextToken();

      // See if the process is already in the process list.  The available
      // sends back a message for each action available in the process.
      PMLProcess availableProcess = processes.Find(processName, processID);

      if (availableProcess == null) {
        // Did not find the process in the set.  Add a new one to
        // the list.
        availableProcess = new PMLProcess(processName, processID);
        processes.add(availableProcess);
      }
 
      // Get the action string.  The action string should start after the
      // first space in the message.
      int actionStrIndex = messageLine.Content.indexOf(" ");
      String actionStr = messageLine.Content.substring(actionStrIndex + 1);
      // Create an action using the action string from the message.
      PMLAction availableAction = new PMLAction(actionStr, availableProcess);

      // Add the action to the process
      availableProcess.add(availableAction);

    }

    if (statusCode != GoodStatus) {
      throw new PMLException("Available Failed");
    }

      
    return processes;
  }
}
