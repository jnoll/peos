package Kernel;

import java.io.*;
import java.net.*;

import Utils.*;

///////////////////////////////////////////////////////////////////////////////
// Class Description:                                                        //
//                                                                           //
// This class is used to send a logout command to the PML Kernel.            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
public class LogoutKernel extends KernelCommand {

  public void Execute() throws IOException
  {
    // Write the command string to the socket.
    SocketWriter.println("logout");

    // deinialize the socket. 
    DeInitialize();

  }
}
