//
// PEOS_connection.java
//
package engineInterface;

import java.io.*;
import java.net.*;
import java.util.*;

/**
 * Class PEOS_connection is low level communication layer. 
 * It is an internal class in package It:
 *
 * 1. communicates with remote server through socket.
 * 2. check status of results, and strip out status code.
 *
 * @author Ming Rutar
 */

class PEOS_connection extends Object
{
    private static final int DELAY_TIME = 100;         // delay 20 millis
	private Socket			_socket;
    private BufferedReader	_sin;
    private PrintStream		_sout;
    
	protected String  _host = null;
    protected int     _port = 0;
    private	  String  _connString = null;

	/**
	 * Constructors. Open a socket.
	 *
     * @param host the remote server name or IP address.
     * @param port the port at remote server.
	 *
	 * @throws java.net.UnknownHostException and java.io.IOException
	 */
    public PEOS_connection(String host, int port)
        throws java.net.UnknownHostException, java.io.IOException
    {
		_host = new String (host);
		_port = port;
		connection();
    }

    public void connection()
        throws java.net.UnknownHostException, java.io.IOException
    {
        _socket = new Socket(_host, _port);  
		_sin = new BufferedReader(new InputStreamReader(_socket.getInputStream()));
		_sout = new PrintStream(_socket.getOutputStream());
    }

	/**
	 * Thsi is used for the remote reset error.
	 *
     * @param command the login string.
	 */
	public void setLoginString(String command)
	{
		_connString = command;
	}

	/**
	 * Close the socket
	 *
	 * @throws java.io.IOException
	 */
	public void close()
        throws java.io.IOException
	{
        if (_socket != null)
        {
            _socket.close();
            _socket = null;
        }
	}

	/**
	 * Execute a PEOS command. Validate the result. Strip out the status code.
	 *
     * @param command the command.
	 * 
	 * @throws java.io.IOException, PEOSInterfaceException
	 */
	public Vector exec(String command)
	    throws java.io.IOException, PEOSInterfaceException
	{
	    Vector result = null;
	    if (_socket == null)
	    {
			throw new PEOSInterfaceException(new String("No connection to PEOS engine"));
	    }
		int retries = 0;
		boolean bExit = false;
		String err = null;
		while (!bExit)
		{
			try
			{
				_sout.println(command);
				String line, status;
				boolean bNext = true;
				result = new Vector();			 //there are something
			    Thread.sleep(DELAY_TIME);   
				
				for (int i = 0; bNext;)
				{
					line = _sin.readLine();
					retries = 0;
					if (line == null)
					    break;
					int len = line.length();
					if (len > 2)
					{
						status = line.substring(0,3);
						if (status.compareTo("100") == 0)
						{
							bNext = false;
							if (len > 4)
							{
								result.addElement(new String(line.substring(4, len)));
								if (line.indexOf('-') == 3)  //more lines
								{
									bNext = true;
								}
							}				        
						}
						else if (status.compareTo("500") == 0)
						{
							String Err = new String("Engine error: ");
							if (len > 4)
							{
								Err += new String(line.substring(4,len));
							}
							else
							{
								Err += new String("no error messages available.");
							}
							throw new PEOSInterfaceException(Err);
						}
					}
				}
				bExit = true;
			}
			catch (Exception e)
			{
			    err = null;
				bExit = true;
				close();
				if (retries < 1)
				{
					retries++;
					try
					{
						reconnection();
					    bExit = false;
					}
					catch (Exception ex)
					{
						err = new String("Exception in reconnection: ")+ex;
					}
				}
				else
				    err = new String("Caught socket exception: ")+e;
			}
			finally
			{
			    if (err != null)
					throw new PEOSInterfaceException(err);
			}
		}
		return result;
	}

	private void reconnection()
        throws java.net.UnknownHostException, java.io.IOException, PEOSInterfaceException
	{
		connection();
		_sout.println(_connString);
		String line, status;
		boolean bNext = true;
		line = _sin.readLine();		// one return line
		status = line.substring(0,3);
		if (status.compareTo("500") == 0)
			throw new PEOSInterfaceException(new String("Cannot relogin."));
	}
}

