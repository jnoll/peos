package Kernel;

import java.io.*;
import java.net.*;

import Utils.*;

///////////////////////////////////////////////////////////////////////////////
// Class Description:                                                        //
//                                                                           //
// This is an abstract base class for the kernel commands.  The kernel       //
// commands communicate with the PML Kernel via a socket.  This class        //
// performs all the necessary initialization of the socket interface.        //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
public class KernelCommand {

  /////////////////////////////////////////////////////////////////////////////
  // Class Description:                                                      //
  // This is an internal class which the kernel commands use to parse a      //
  // messsage received from the PML kernel into status and message body.     //
  // It assumes the message is in the form "status-message body"             //
  // For example:                                                            //
  // 100-milestone.001 checkout_peos type action mode manual requires ...    //
  //                                                                         //
  /////////////////////////////////////////////////////////////////////////////
  public class PMLMessage {

    public PMLMessage(String message)
    {
      // Get the status code from the message.
      String statusString = message.substring(0, 3);

      // Convert the status code to a number.
      StatusCode = (Integer.valueOf(statusString)).intValue();

      // If the message is less than 4 in length then there is no
      // message body.
      if ( message.length() > 5) {
        Content = message.substring(4);
      } else {
        Content = null;
      }

    }

    public int StatusCode;
    public String Content;
  
  }

  /////////////////////////////////////////////////////////////////////////////
  // This function cleans up the socket interface to the PML Kernel.         //
  /////////////////////////////////////////////////////////////////////////////
  protected static void DeInitialize () throws IOException
  {
    SocketWriter.close();
    SocketReader.close();
    KernelSocket.close();
  }

  /////////////////////////////////////////////////////////////////////////////
  // This function initializes the socket interface to the PML Kernel.       //
  /////////////////////////////////////////////////////////////////////////////
  protected static void Initialize() throws PMLException
  {
    int peosPort; 
    String peosHost = null;

    try {
      // Get the host name that the peos server is running on.
      peosHost = PMLConfig.GetConfig().GetStringValue("PEOS_Server"); 
      // Get the port name that the peos server is running on.
      peosPort = PMLConfig.GetConfig().GetIntValue("PEOS_PortValue");

      // Create the socket to communicate with the PEOS server.
      KernelSocket = new Socket(peosHost, peosPort);

      // Create writer and reader classes for the socket.
      SocketWriter = new PrintWriter(KernelSocket.getOutputStream(), true);
      SocketReader = new BufferedReader( new 
                           InputStreamReader(KernelSocket.getInputStream()));
    } 

    // Process any exceptions that occur during socket intialization.
    catch (UnknownHostException e) {
      String errorMessage = new String("Don't know about host:" + peosHost);
      throw new PMLException(errorMessage);
    } 

    catch (PMLException e) {
      throw e;
    }

    catch (IOException e) {
      String errorMessage = new String(
        "Couldn't get I/O for the connection:" + peosHost);
      throw new PMLException(errorMessage);
    }
  }

  // Socket to communicate with PML Kernel.
  protected static Socket KernelSocket = null;

  // Writer class which allows Kernel commands to write to PML Kernel.
  protected static PrintWriter SocketWriter = null;

  // Reader class which allows Kernel commands to receive message from
  // the PML Kernel.
  protected static BufferedReader SocketReader = null;

  // Status codes which can be received from the PML Kernel.
  protected static final int GoodStatus = 100;
  protected static final int ErrorStatus = 500;
}
