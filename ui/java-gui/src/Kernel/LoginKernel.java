package Kernel;

import java.io.*;
import java.net.*;

import Utils.*;

///////////////////////////////////////////////////////////////////////////////
// Class Description:                                                        //
//                                                                           //
// This class is used to send a login command to the PML Kernel.             //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
public class LoginKernel extends KernelCommand {

  public void Execute(String userName, String password, String peosHost,
                      int peosPort) 
    throws IOException
  {
    // Initialize the socket.
    Initialize(peosHost, peosPort);

    // Write the command string to the socket.
    SocketWriter.println("login " + userName + " " + password);

    String messageRcvd = new String(SocketReader.readLine());

    PMLMessage loginStatus = new PMLMessage(messageRcvd);

    // Check the status of the login command.
    if (loginStatus.StatusCode != GoodStatus) {
      throw new PMLException("Login Failed");
    }

  }
}
