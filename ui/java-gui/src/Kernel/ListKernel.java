package Kernel;

import java.io.*;
import java.net.*;
import java.util.*;

import Utils.*;
import PML.*;

///////////////////////////////////////////////////////////////////////////////
// Class Description:                                                        //
//                                                                           //
// This class is used to send a list command the PML Kernel.  It will parse  //
// the information received back from the PML Kernel into a list of strings. //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
public class ListKernel extends KernelCommand {

  public LinkedList Execute() throws IOException
  {

    LinkedList models = new LinkedList();
    String messageRcvd;
    int statusCode = ErrorStatus;

    // Write the command string to the socket. 
    SocketWriter.println("list");

    // Parse data received from the list command.
    while ( (messageRcvd = SocketReader.readLine()) != null ) {

      PMLMessage messageLine = new PMLMessage(messageRcvd);
      statusCode = messageLine.StatusCode;

      // If status is not good or message length is less than 4
      // then processing for the list command is complete.
      if (statusCode != GoodStatus || messageLine.Content == null) {
        break;
      }
       
      // Add the model name to the list. 
      models.add(messageLine.Content);

      if (statusCode != GoodStatus) {
        throw new PMLException("List Failed");
      }
    }

    // Return a list of model names.
    return models;
  }
}
