//
// PEOS_Interface.java
//
package engineInterface;

import java.io.*;
import java.util.*;

/**
 * Class PEOS_Interface is the class of the package.
 * 
 * @author Ming Rutar
 */

public class PEOS_Interface
{
	// 
	// PEOS task commands
	//
	public final static String COMMAND_RUN     = new String("run");
	public final static String COMMAND_DONE    = new String("done");
	public final static String COMMAND_SUSPEND = new String("suspend");
	public final static String COMMAND_RESUME  = new String("resume");

	public final static String TL_AVAILABLE = new String("available");
	public final static String TL_RUNNING   = new String("running");
	public final static String TL_FINISHED  = new String("finished");
	public final static String TL_ERROR     = new String("error");

	protected final static int STATE_ERROR   = -1;
	protected final static int STATE_OK      = 0;		// ok, no auto exec
	protected final static int STATE_OK_EXEC = 1;		// ok, extra executions.
	
    private	PEOS_connection _connection;
	private	PEOS_ProcessList _allProc;
    private	Hashtable       _modelList;

    private	Vector          _runningProcs;
    private	Vector          _availableProcs;
	private	Vector			_errorProcs;
	private	PEOS_Process	_doneProc;

	private	boolean			_fDiagnosis;	// if set, it checks for misbehavior
											// it could overcome itself.
    private boolean			 _bLogin;
    

	/**
	 * Constructor
	 * 
	 * @param host - the remote host name or IP address
	 * @param port - the IP address of remote server.
	 */
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

	/**
	 * Set/Get diagnosis flag. If the flag is set, it will report for 
	 * misbehaviors which otherwise it overcomes itseft silently. 
	 * 
	 */
	public void setDiagnosis(boolean bVal)
	{
		_fDiagnosis = bVal;
	}

	public boolean getDiagnosis()
	{
		return _fDiagnosis;
	}
	
	/**
	 * Disconnect from remote site.
	 * 
	 */
	public void disconnect()
    {
        try
        {
		    if (_bLogin)
		        PEOS_logout();
            _connection.close();
            _bLogin = false;
        }
        catch (Exception e)
        {
        }
    }
    
	/**
	 * Log into remote PEOS engine server.
	 * 
	 * @param name  user name
	 * @param password password
	 *
	 * @throws java.io.IOException, PEOSInterfaceException
	 */
	public void PEOS_login(String name, String password)
	    throws java.io.IOException, PEOSInterfaceException
	{
	    String command = new String("login ");
	    command += name;
	    command += new String(" ");
	    command += password;
		_connection.setLoginString(command);
	    _connection.exec(command);
        _bLogin = true;
	}

	/**
	 * Logout from remote PEOS engine server.
	 * 
	 * @throws java.io.IOException, PEOSInterfaceException
	 */
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

	/**
	 * Get list of models from server.
	 * 
	 * @throws java.io.IOException, PEOSInterfaceException
	 */
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
	
 	/**
	 * Get list of models from server.
	 * 
     * @param name the model name 
	 * @return Hashtable that contains list of available, running, error and done
	 *         processes.
	 * 
	 * @throws java.io.IOException, PEOSInterfaceException
	 */
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

 	/**
	 * Execute a command
	 * 
     * @param cmd the command name 
     * @param processID the process ID
     * @param taskID the task ID
	 * @return Hashtable that contains list of available, running, error and done
	 *         processes.
	 * 
	 * @throws java.io.IOException, PEOSInterfaceException
	 */
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
	
	/**
	 * Get all processes
	 * 
	 * @return Hashtable that contains list of available, running, error and done
	 *         processes.
	 * 
	 * @throws java.io.IOException, PEOSInterfaceException
	 */
	public Hashtable getProcState()
	    throws java.io.IOException, PEOSInterfaceException
	{
	    String availCmd = new String("available");
	    String runCmd = new String("running");

		_doneProc = null;

		PEOS_Process    proc;
		boolean bContinue = true;
		Vector result = null;
		while (bContinue)
		{
			_errorProcs.removeAllElements();
			_availableProcs.removeAllElements();
			result = _connection.exec(availCmd);
			if (result.size() > 0)
			{
				int ret = STATE_OK;
				for (int i = 0; i < result.size(); i++)
				{
					proc = new PEOS_Process(PEOS_Process.STS_AVAILABLE);
					if (proc.parse((String) result.elementAt(i)))
					{
						ret = validationProc(proc);
						if (ret == STATE_OK_EXEC)		// retrieve new available list
							break;
						else if (ret == STATE_ERROR)	// the proc also shows on the list
							_errorProcs.addElement(proc);	

						_allProc.determState(proc);		
						_availableProcs.addElement(proc);
					}
				}
				if (ret == STATE_OK_EXEC)
					continue;
		    }

			bContinue = false;
			_runningProcs.removeAllElements();
			result = _connection.exec(runCmd);
			if (result.size() > 0)
			{
				for (int i = 0; i < result.size(); i++)
				{
					proc = new PEOS_Process(PEOS_Process.STS_RUNNING);
					if (proc.parse((String) result.elementAt(i)))
					{
    					for (int iAv = 0; iAv < _availableProcs.size(); iAv++)
						{
							if (proc.isSame((PEOS_Process)_availableProcs.elementAt(iAv)))
							{
								proc.setStatus(PEOS_Process.ERR_DUPLICATED);
								_errorProcs.addElement(proc);	
								break;
							}
						}	
						if (_allProc.determState(proc) != STATE_ERROR)
							_runningProcs.addElement(proc);
						else
							_errorProcs.addElement(proc);	
					}
				}
			}

		}

		_doneProc = _allProc.postProcessing(_availableProcs, _runningProcs,_errorProcs);
		
		Hashtable RetVal = new Hashtable();
		if (_runningProcs.size() > 0)
			RetVal.put(TL_RUNNING, _runningProcs);
		if (_availableProcs.size() > 0)
			RetVal.put(TL_AVAILABLE, _availableProcs);
		if (_doneProc != null)
			RetVal.put(TL_FINISHED, _doneProc);
		if (_errorProcs.size() > 0)
			RetVal.put(TL_ERROR, _errorProcs);

	    return RetVal;
	}

	/**
	 * Validate a process
	 * 
	 * @param proc a process.
	 * @return STATE_OK indicates ok, STATE_OK_EXEC indicates ok, but retrieve the 
	 *  list agian. STATE_ERROR something is wrong
	 * 
	 */
	private int validationProc(PEOS_Process proc)
	{
		int ret = STATE_OK;

		if (!proc.isTypeAction()) 
		{
			if (_fDiagnosis)
			{
				proc.setStatus(PEOS_Process.ERR_ILLEGAL_TYPE);
				ret = STATE_ERROR;
			}
			else
			{
				String task = new String(proc._processID);
				task += new String(" ");
				task += proc._task;
				try
				{
					_connection.exec(new String("run ") + task);
					_connection.exec(new String("done ") + task);
				}
				catch (Exception e)
				{
				}
				finally
				{
					ret = STATE_OK_EXEC;
				}
			}
		}
		return ret;
	}
}

 
