//
// PEOS_Interface.java
//

import java.io.*;
import java.net.*;
import java.util.*;

class PEOSInterfaceException extends Exception 
{
    PEOSInterfaceException(String s) 
    {
	    super(s);
    }
}

class PEOS_connection
{
	private Socket	_socket;
    private DataInputStream _sin;
    private PrintStream _sout;
    public PEOS_connection(String host, int port)
        throws java.net.UnknownHostException, java.io.IOException
    {
        _socket = new Socket(host, port);  
		_sin = new DataInputStream(_socket.getInputStream());
		_sout = new PrintStream(_socket.getOutputStream());
    }

	public void close()
        throws java.io.IOException
	{
        if (_socket != null)
        {
            _socket.close();
            _socket = null;
        }
	}
	
	public String[] exec(String command)
	    throws java.io.IOException, PEOSInterfaceException
	{
	    String[] result = null;
	    if (_socket == null)
	    {
			throw new PEOSInterfaceException(new String("No connection to PEOS engine"));
	    }
	    String SockCommand = new String(command);
	    SockCommand += new String("\n");
		_sout.println(SockCommand);
		String line, status;
		boolean bNext = true;
		for (int i = 0; bNext;)
		{
			line = _sin.readLine();
			int len = line.length();
			if (len > 2)
			{
			    status = line.substring(0,2);
			    if (status.compareTo("100") == 0)
			    {
			        if (len > 4)
			        {
				        result[i++] = new String(line.substring(4));
                    }				        
				    if ((len > 3) && (line.indexOf('-') != 3))
				    {
				        bNext = false;
				    }
			    }
                else if (status.compareTo("500") == 0)
			    {
			        String Err = new String("Engine error: ");
			        if (len > 4)
			        {
			            Err += new String(line.substring(4,len-1));
			        }
			        else
			        {
			            Err += new String("no error messages available.");
			        }
		            throw new PEOSInterfaceException(Err);
                }
			}
		}
		return result;
	}
}

public class PEOS_Interface
{
    PEOS_connection _connection;
    Hashtable       _modelList;
    Vector          _runningProcs;
    Vector          _availableProcs;

    String[] commands = {"run", "done", "suspend"};
    
    String[] getCommands()
    {
        return commands;
    }
    
	public PEOS_Interface(String host, int port)
	    throws java.net.UnknownHostException, java.io.IOException
	{
		_connection = new PEOS_connection(host, port);
		_modelList = null;
		_runningProcs = null;
		_availableProcs = null;
	}

	public void PEOS_login(String name, String password)
	    throws java.io.IOException, PEOSInterfaceException
	{
	    String command = new String("login ");
	    command += name;
	    command += new String(" ");
	    command += password;
	    _connection.exec(command);
	}

	public Vector getModelList()
	    throws java.io.IOException, PEOSInterfaceException
	{
	    Vector  RetList = null;
	    String Command = new String("list");
		String[] result = _connection.exec(Command);
		{
		    RetList = new Vector(result.length);
		    if (_modelList == null)
		    {
		        _modelList = new Hashtable(result.length);
		    }
		    // if not on list, add it in. If no longer in use
		    // make a note. All proc should be ?? (not implemented)
		    
			for (int i = 0; i < result.length; i++)
			{
			    if (!_modelList.containsKey(result[i]))
				{
				    String model = new String(result[i]);
				    Integer val = new Integer(0);
				    _modelList.put(model, val);
				}
				RetList.addElement(new String(result[i]));
			}
		}
		return RetList;
	}
	
    public Vector PEOS_execModel(String name)
	    throws java.io.IOException, PEOSInterfaceException
	{
	    if (!_modelList.containsKey(name))
	    {   
	        String Err = new String(name);
	        Err += new String(" is not a valid model.");
	        throw new PEOSInterfaceException(Err);
	    }
	    String command = new  String("create ");
	    command += name;
		_connection.exec(command);
		return getProcState();
	} 

	public Vector PEOS_execTask(int cmdID, String processID, 
		String TaskID)
	    throws java.io.IOException, PEOSInterfaceException
	{
	    if (cmdID >= commands.length)
	    {
	        String Err = new String("Not a valid command.");
	        throw new PEOSInterfaceException(Err);
	    }
	    String Command= new String(commands[cmdID]);
	    Command += new String(" ");
	    Command += processID;
	    Command += new String(" ");
	    Command += TaskID; 
        _connection.exec(Command);
        return getProcState();
	}
	
	void ParseStr(String input, String token, String dest)
	{
	    int idx, idx2;
        if ((idx = input.indexOf(token))!= -1)
        {
            idx2 = input.indexOf('{', idx+1);
	        idx = input.indexOf('}', idx2+1);
	        dest = input.substring(idx2+1, idx);
        }
	}
	
	PEOS_Process ParseResult(String input)
	{
	    input.trim();
	    PEOS_Process proc = new PEOS_Process();
	    int idx = input.indexOf('{');
	    String part1;
        if (idx == -1)
            part1 = input;
        else
            part1 = input.substring(0, idx);
	    StringTokenizer st = new StringTokenizer(input, " \t");
	    proc._processID = st.nextToken();
	    proc._task = st.nextToken();
	    proc._type = st.nextToken();
	    proc._typeValue = st.nextToken();
	    proc._mode  = st.nextToken();
	    proc._modeValue  = st.nextToken();
    	if (idx != -1)
	    {
	        ParseStr(input, "requires", proc._requires);
	        ParseStr(input, "provides", proc._provides);
	        ParseStr(input, "agent", proc._agent);
	        ParseStr(input, "script", proc._script);
        }	        
        return proc;
	}
	
	//
	// for now, we copy the new result. In the future
	// we are going to validate the result.
	void parseAvailableResult(String[] result)
	{
        _availableProcs.removeAllElements();
        for (int i = 0; i < result.length; i++)
        {
            _availableProcs.addElement(ParseResult(result[i]));
	    }
	}
	
	void parseRunningResult(String[] result)
	{
        _availableProcs.removeAllElements();
        for (int i = 0; i < result.length; i++)
        {
            _availableProcs.addElement(ParseResult(result[i]));
	    }
	}
	
	Vector getProcState()
	    throws java.io.IOException, PEOSInterfaceException
	{
	    Vector RetList = new Vector();
	    String command = new String("available");
		String[] result = _connection.exec(command);
	    if (result.length > 0)
	    {
	        if (_availableProcs == null)
	        {
	            _availableProcs = new Vector(result.length);
                for (int i = 0; i < result.length; i++)
                    _availableProcs.addElement(ParseResult(result[i]));
            }
            else
                parseAvailableResult(result);
        }
        command = "running";
		result = _connection.exec(command);
	    if (result.length > 0)
	    {
	        if (_runningProcs == null)
	        {
	            _runningProcs = new Vector(result.length);
                for (int i = 0; i < result.length; i++)
                    _runningProcs.addElement(ParseResult(result[i]));
            }
            else
                parseRunningResult(result);
	    }
	    
	    for (int i = 0; i < _availableProcs.size(); i++)
	    {
	        PEOS_Process proc = (PEOS_Process)_availableProcs.elementAt(i);
	        if (proc.isValid())
	            RetList.addElement(new PEOS_Process(proc));
	    }
	    for (int i = 0; i < _runningProcs.size(); i++)
	    {
	        PEOS_Process proc = (PEOS_Process) _runningProcs.elementAt(i); 
	        if (proc.isValid())
	            RetList.addElement(new PEOS_Process(proc));
	    }
	    return RetList;
	}
}