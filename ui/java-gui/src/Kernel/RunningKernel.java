package Kernel;

import java.io.*;
import java.net.*;
import java.util.*;

import Utils.*;
import PML.*;

///////////////////////////////////////////////////////////////////////////////
// Class Description:                                                        //
//                                                                           //
// This class is used to send a running command to the PML Kernel.  It will  //
// parse the information received back from the PML Kernel into a list of    //
// processes.           
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
public class RunningKernel extends KernelCommand {

  public PMLProcessList Execute() throws IOException
  {
    PMLProcessList processes = new PMLProcessList();


    String messageRcvd;
    int statusCode = ErrorStatus;

    // Write the command string to the socket.
    SocketWriter.println("running");

    // Parse data received from running.
    while ( (messageRcvd = SocketReader.readLine()) != null ) {

      PMLMessage messageLine = new PMLMessage(messageRcvd);
      statusCode = messageLine.StatusCode;

      // If status is not good or message length is less than 4
      // then processing for the running command is complete.
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

      // See if the process is already in the process list.  The running 
      // command sends back a message for each action running in the 
      // process.
      PMLProcess runningProcess = processes.Find(processName, processID);

      if (runningProcess == null) {
        // Did not find the process in the set.  Add a new one to
        // the list.
        runningProcess = new PMLProcess(processName, processID);
        processes.add(runningProcess);
      }
 
      // Get the action string.  The action string should start after the
      // first space in the message.
      int actionStrIndex = messageLine.Content.indexOf(" ");
      String actionStr = messageLine.Content.substring(actionStrIndex + 1);
      PMLAction runningAction = new PMLAction(actionStr, runningProcess);

      // Add the action to the process
      runningProcess.add(runningAction);

    }

    if (statusCode != GoodStatus) {
      throw new PMLException("Running Failed");
    }

    return processes;
  }
}
