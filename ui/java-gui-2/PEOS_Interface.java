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
    public static final int START_LENGTH = 10;
	private Socket	_socket;
    private DataInputStream _sin;
    private PrintStream _sout;
    String  _host = null;
    int     _port = 0;
    
    public PEOS_connection(String host, int port)
        throws java.net.UnknownHostException, java.io.IOException
    {
        _socket = new Socket(host, port);  
		_sin = new DataInputStream(_socket.getInputStream());
		_sout = new PrintStream(_socket.getOutputStream());
		_host = new String (host);
		_port = port;
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

	public Vector exec(String command)
	    throws java.io.IOException, PEOSInterfaceException
	{
	    Vector result = null;
	    if (_socket == null)
	    {
			throw new PEOSInterfaceException(new String("No connection to PEOS engine"));
	    }
		_sout.println(command);
		String line, status;
		boolean bNext = true;
        result = new Vector(START_LENGTH);   //there are something
		for (int i = 0; bNext;)
		{
			line = _sin.readLine();
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
		return result;
	}
}

public class PEOS_Interface
{
	public static final int STATE_ERROR = -1;
	public static final int STATE_OK = 0;			// ok, no auto exec
	public static final int STATE_OK_EXEC = 1;		// ok, extra executions.
	
    PEOS_connection _connection;
    Hashtable       _modelList;

	PEOS_ProcessList _allProc;

    Vector          _runningProcs;
    Vector          _availableProcs;
	Vector			_errorProcs;
	PEOS_Process	_doneProc;

	boolean			_fDiagnosis;
	
    private boolean  _bLogin;
    
	public PEOS_Interface(String host, int port)
	    throws java.net.UnknownHostException, java.io.IOException
	{
		_connection = new PEOS_connection(host, port);
		_modelList = null;
		_allProc = new PEOS_ProcessList();
		_runningProcs = new Vector();
		_availableProcs = new Vector();
		_errorProcs = new Vector();
		_doneProc = null;

 		_fDiagnosis = false;
 		
 		_bLogin = false;
	}

	public void setDiagnosis(boolean bVal)
	{
		_fDiagnosis = bVal;
	}

	public boolean getDiagnosis()
	{
		return _fDiagnosis;
	}
	
	public String getHost() 
	{
	    return _connection._host;
	}
	public String getPort() 
	{
	    String ret = new String();
	    ret.valueOf(_connection._port);
	    return ret;
	}

	public void disconnect()
    {
        try
        {
		    if (_bLogin)
		        PEOS_logout();
            _connection.close();
        }
        catch (Exception e)
        {
        }
    }
    
	public void PEOS_login(String name, String password)
	    throws java.io.IOException, PEOSInterfaceException
	{
	    String command = new String("login ");
	    command += name;
	    command += new String(" ");
	    command += password;
	    _connection.exec(command);
        _bLogin = true;
	}

	public void PEOS_logout()
	    throws java.io.IOException, PEOSInterfaceException
	{
	    if (_bLogin)
	    {
	        String command = new String("logout");
	        _connection.exec(command);
	        _bLogin = false;
        }	     
	}

	public Vector getModelList()
	    throws java.io.IOException, PEOSInterfaceException
	{
	    String Command = new String("list");
		Vector result = _connection.exec(Command);

		if (_modelList == null)
		{
		    _modelList = new Hashtable(result.size());
		}
	    // if not on list, add it in. If no longer in use
        // make a note. All proc should be ?? (not implemented)
		    
		for (int i = 0; i < result.size(); i++)
		{
		    if (!_modelList.containsKey(result.elementAt(i)))
			{
			    String model = new String((String) result.elementAt(i));
			    _modelList.put(model, new Integer(0));
			}
		}
		return result;
	}
	
    public Hashtable PEOS_execModel(String name)
	    throws java.io.IOException, PEOSInterfaceException
	{
	    if (!_modelList.containsKey(name))
	    {   
	        String Err = new String(name);
	        Err += new String(" is not a valid model.");
	        throw new PEOSInterfaceException(Err);
	    }

		int val = ((Integer)_modelList.remove(name)).intValue();
		val++;
		_modelList.put(name, new Integer(val));

	    String command = new  String("create ");
	    command += name;
		_connection.exec(command);
		return getProcState();
	} 

	public Hashtable PEOS_execTask(String cmd, String processID, 
		String taskID)
	    throws java.io.IOException, PEOSInterfaceException
	{
		PEOS_Process proc = _allProc.findProcess(processID, taskID);
		if (proc == null)
	    {   
	        String Err = new String("Process ID=");
			Err  += processID;
			Err += new String(", task ID=");
			Err += taskID;
	        Err += new String(" is not a valid task.");
	        throw new PEOSInterfaceException(Err);
	    }
		
		_allProc.setLastExec(cmd, proc);

	    String Command= new String(cmd);
	    Command += new String(" ");
	    Command += processID;
	    Command += new String(" ");
	    Command += taskID; 
        _connection.exec(Command);
        return getProcState();
	}
	
	//
	// .
	int validationProc(PEOS_Process proc)
	{
		int ret = STATE_OK;
/*			if (proc.isRunning())
		{
			if (!proc.isTypeAction())
			{
				if (!_fDiagnosis)
				{
					String task = new String(proc._processID);
					task += new String(" ");
					task += proc._task;
					try
					{
						_connection.exec(new String("run ") + task);
						_connection.exec(new String("done ") + task);
						ret = STATE_OK_EXEC;
					}
					catch (Exception e)
					{
					}
				}
				else
				{
					proc.setStatus(PEOS_Process.ERR_ILLEGAL_TYPE);
					ret = STATE_ERROR;
				}
			}
		}
		else
		{
		for (int i = 0; i < _runningProcs.size(); i++)
			{
				if (proc.isSame((PEOS_Process)_runningProcs.elementAt(i)))
				{
					proc.setStatus(PEOS_Process.ERR_DUPLICATED);
					ret = STATE_ERROR;
				}
			}	
		}*/
		return ret;
	}

	Hashtable getProcState()
	    throws java.io.IOException, PEOSInterfaceException
	{
	    String availCmd = new String("available");
	    String runCmd = new String("running");

		_doneProc = null;

		PEOS_Process    proc;
		boolean bContinue = true;
		while (bContinue)
		{
			_errorProcs.removeAllElements();
			_runningProcs.removeAllElements();
			Vector result = _connection.exec(runCmd);
			if (result.size() > 0)
			{
				for (int i = 0; i < result.size(); i++)
				{
					proc = new PEOS_Process();
					if (proc.parse((String) result.elementAt(i)))
					{
						proc.setState(PEOS_Process.STS_RUNNING);
						int ret = validationProc(proc);
						if (ret == STATE_OK_EXEC)		// retrieve new available list
							continue;
						if (_allProc.determState(proc) != STATE_ERROR)
							_runningProcs.addElement(proc);
						else
							_errorProcs.addElement(proc);	
					}
				}
			}

			bContinue = false;
			_availableProcs.removeAllElements();
			result = _connection.exec(availCmd);
			if (result.size() > 0)
			{
				for (int i = 0; i < result.size(); i++)
				{
					proc = new PEOS_Process();
					if (proc.parse((String) result.elementAt(i)))
					{
						proc.setState(PEOS_Process.STS_AVAILABLE);
						if (validationProc(proc) == STATE_ERROR)
							_errorProcs.addElement(proc);	 
						else
						{
							_allProc.determState(proc); // no error will return for availbale
							_availableProcs.addElement(proc);
						}
					}
				}
		    }
		}

		_doneProc = _allProc.postProcessing(_availableProcs, _runningProcs,_errorProcs);
		
		Hashtable RetVal = new Hashtable();
		if (_runningProcs.size() > 0)
			RetVal.put(runCmd, _runningProcs);
		if (_availableProcs.size() > 0)
			RetVal.put(availCmd, _availableProcs);
		if (_doneProc != null)
			RetVal.put(new String("done") , _doneProc);
		if (_errorProcs.size() > 0)
			RetVal.put(new String("error") , _errorProcs);

	    return RetVal;
	}
}