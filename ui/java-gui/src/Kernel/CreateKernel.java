package Kernel;

import java.io.*;
import java.net.*;

import Utils.*;
import PML.*;

///////////////////////////////////////////////////////////////////////////////
// Class Description:                                                        //
//                                                                           //
// This class is used to send a create command to the PML Kernel.  It will   //
// create a new instance of a model.                                         //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
public class CreateKernel extends KernelCommand {

  public CreateKernel(String model)
  {
    ModelName = model;
  }

  public void Execute() throws IOException
  {
    // Write the command string to the socket.
    SocketWriter.println("create " + ModelName);

    String messageRcvd = new String(SocketReader.readLine());

    PMLMessage createStatus = new PMLMessage(messageRcvd);

    // Check the status of the create command.
    if (createStatus.StatusCode != GoodStatus) {
      throw new PMLException("Create Failed");
    }

  }

  private String ModelName;
}
