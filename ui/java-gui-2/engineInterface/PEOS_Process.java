//
// PEOS_Process.java
//

package engineInterface;

import java.util.*;

/**
 * Class PEOS_Process is object presentation of a PEOS process. The objects outside
 * the PEOS.engineInterface package can access it read only.
 *
 * 1. parses flatwire string from comm layer into corresponding fields.
 * 2. contains state (run, availbal...) indicator
 * 3. contains status (ok, err..) indicator
 * 4. offers access to its properties.
 * 5. offers a set of methods to perform variaty functions..
 *
 * @author Ming Rutar
 */
public class PEOS_Process extends Object
{
    /**
     * The status indicates a process health after an execution.
	 * The enumeration values are:
	 *
	 *   ERR_OK           - No error.		
	 *   ERR_ILLEGAL_TYPE - Illegal action type, such as sequence, select...
	 *   ERR_EXTRARUNNING - The running process wasn't on last available list.
	 *   ERR_RUN_DROPPED  - The running process disappear from list without execution 
	 *   ERR_DUPLICATED   - The process shows on both available and running list.
	 */
	public static final int ERR_OK				= 0;
	public static final int ERR_ILLEGAL_TYPE	= 1;
	public static final int ERR_EXTRARUNNING	= 2;
	public static final int ERR_RUN_DROPPED		= 3;
	public static final int ERR_DUPLICATED		= 4;

	private String[] errorMessage = {"OK",									// 0
		"Illegal instruction.",												// 1
		"The process wasn't on available list",								// 2
		"The process was on running list and it wasn't being executed.",	// 3
		"The process is already on ruuning list."};							// 4

    /**
     * The state indicates a process state after an execution
	 * The enumeration values:
	 *
 	 *   STS_NONE           - Initial state
  	 *   STS_RUNNING		- The process is run, but in suspended state
  	 *   STS_CURRENTRUNNING	- The process is in running state. One only.
  	 *   STS_AVAILABLE	    - The process is available.	
  	 *   STS_CURRENTAVAIL	- The process just swapped out from running and become available. 	
  	 *   STS_DONE		    - The process is finished or branched. (we cannot distingush branch.) 
	 */
	public static final int STS_NONE = 0;
	public static final int STS_RUNNING = 1;
	public static final int STS_CURRENTRUNNING = 2;
	public static final int STS_AVAILABLE = 3;
	public static final int STS_CURRENTAVAIL = 4;
	public static final int STS_DONE = 5;
	
	protected String	_processID;			// process ID
	protected String	_task;				// task ID
	protected String	_type;              // "select", "sequence", ("iteract"?) or "action"
	protected String	_mode;              // for action, "mode"="manual"!"executable"
    protected String	_children;          // for non-action value for "children", ("xx1" "xx2")

	protected String	_PMLScript;			// the PML script 
	protected String	_systemCommand;		// system command
	protected String	_execScript;		// system command
	protected String	_parentProcID;      // for branch, not in use
	protected int		_state;				// run, suspend, done, avail.
	protected int		_status;			// ok, error.

	//	_agent = getContextof("agent");		// what the agent for??

	/**
	 * Constructors	
	 */
    protected PEOS_Process() 
    {
        _state = STS_NONE;
		_status = ERR_OK;
    }

	/**
	 * Constructors	
	 *
     * @param newState the state of process.
	 */
    protected PEOS_Process(int newState) 
    {
        _state = newState;
		_status = ERR_OK;
    }

    /**
	 * Property Accessing.
	 *
     * @return  process ID property 
     */
	public String getProcessID() 
	{
		return _processID;
	}

    /**
     * @return  task ID property 
     */
	public String getTaskID()
	{
		return _task;
	}

    /**
     * @return  process state
     */
	public int	getState()
	{
		return _state;
	}

    /**
     * @return process status
     */
	public int	getStatus()
	{
		return _status;
	}

    /**
     * @return the status message.
     */
	public String getErrorMsg()
	{
		return errorMessage[_status];
	}

    /**
     * @return the original PML string from engine.
     */
	public String getPMLString()
	{
		return _PMLScript;
	}

    /**
     * @return the line for executing system command, or 
	 * null if no command
     */
	public String getSystemCommand()
	{
		return _systemCommand;
	}

	public String getScript()
	{
	    return null;
//		return _execScript;
	}

	/**
	 * Query methods
	 *
	 * @return true if it is a running process, false otherwise
	 */
	public boolean isRunning()
	{
		return ((_state == STS_RUNNING) || (_state == STS_CURRENTRUNNING)); 
	}

	/**
	 *
	 *  @return true if it it an available process, false otherwise
	 */
	public boolean isAvailable()
	{
		return ((_state == STS_AVAILABLE) || (_state == STS_CURRENTAVAIL));
	}

	/**
	 *
	 * @return true if it is a type action process, false otherwise
	 */
	public boolean isTypeAction()
	{
		return (_type.compareTo("action") == 0);
	}

	/** 
	 * Compareson of processes
	 *
	 * Does the compare have the same process ID.
     * @param proc the compare process.
	 * @return  0 if they are the same, non-zero otherwise
	 */
    public int compareProcID(PEOS_Process proc)
    {
		if (proc == null)
			return 1;
		else
			return _processID.compareTo(proc._processID);
    }

	/**
     * @param procID the ID of compare process.
	 * @return  0 if they are the same, non-zero otherwise
	 */
    public int compareProcID(String procID)
    {
		if (procID == null)
			return 1;
		else
			return _processID.compareTo(procID);
    }
    
	/**
	 * Is it the same process
	 *
     * @param procID the ID of compare process.
     * @param taskID the task ID of compare process.
	 * @return true if they are the same, non-zero otherwise
	 */
    public boolean isSame(String procID, String taskID)
    {
		if ((procID == null) || (taskID == null))
			return false;
		else
			return ((_processID.compareTo(procID) ==0) &&
				(_task.compareTo(taskID) ==0));
	}
    
	/**
     * @param proc the compare process.
	 * @return true if they are the same, non-zero otherwise
	 */
    public boolean isSame(PEOS_Process proc)
    {
		if (proc == null)
			return false;
		else
			return ((_processID.compareTo(proc._processID) ==0) &&
				(_task.compareTo(proc._task) ==0));
	}

	//
	// protected
	//
	protected void setState(int newState)
	{
		_state = newState;
	}

	protected void setProcessID(String procID) 
	{
		_processID = procID;
	}

	protected void setTaskID(String taskID)
	{
		_task = taskID;
	}

	protected void setStatus(int newStatus)
	{
		_status = newStatus;
	}

	private String getContextof(String token) //such as tool requires...
	{
	    String ret = null;
	    int idx, idx2;
	    boolean bFound = false;
	    try
	    {
            if ((idx = _PMLScript.indexOf(token))!= -1)
            {
                idx2 = _PMLScript.indexOf('{', idx+1);
                if (idx2 != -1)
                {
	                idx = _PMLScript.indexOf('}', idx2+1);
	                bFound = (idx != -1) ? true : false;
	            }
                else
                {
                    idx2 = _PMLScript.indexOf('(', idx+1);
                     if (idx2 != -1)
                     {
	                    idx = _PMLScript.indexOf(')', idx2+1);
	                     bFound = (idx != -1) ? true : false;
	                 }
	            }
                if (bFound)     
                {
                    idx2++;
                    while (idx2 < idx)
                    {
                        char aChar = _PMLScript.charAt(idx2);
                        if (Character.isWhitespace(aChar) || (aChar == '"' ))
                            idx2++;
                        else
                            break;
                    }
                    idx--;
                    while (idx > idx2)
                    {
                        char aChar = _PMLScript.charAt(idx);
                        if (Character.isWhitespace(aChar) || (aChar == '"' ))
                            idx--;
                        else
                            break;
                    }
                    ret = new String(_PMLScript.substring(idx2, idx+1));
	            }
            }
        }
        catch (Exception e)
        {
            ret = null;
        }
        finally
        {
            return ret;
        }
	}
	
	protected boolean parse(String PMLStr)
	{
		boolean bRet = true;

	    PMLStr.trim();
	    PMLStr.toLowerCase();
        _PMLScript = new String(PMLStr);
	    StringTokenizer st = new StringTokenizer(PMLStr, " \t");

	    String aStr;
		
	    _processID = new String(st.nextToken());
	    _task = new String(st.nextToken());

	    aStr = new String(st.nextToken());
	    if (aStr.compareTo("type") == 0)
	    {
            int idx = PMLStr.indexOf("type");
	        _type =  new String(st.nextToken());
	        aStr  = st.nextToken();
	        if (aStr.compareTo("mode") == 0)
	        {
			    _mode  =  new String(st.nextToken());

			}
            else if (aStr.compareTo("children") == 0)
			    _children = getContextof("children");
			else
			    bRet = false;
        }
        else
            bRet = false;

		parseTool();
		_execScript = getContextof("_script");
			
		return bRet;
	}

	void parseTool()
	{
		StringBuffer command;
		String aStr = getContextof("tool");
		if (aStr != null)
		{
		    StringTokenizer st = new StringTokenizer(aStr, " \t");
			command = new StringBuffer(st.nextToken());
			boolean bAbort = false;
			while (st.hasMoreTokens() && !bAbort)
			{
				aStr = st.nextToken();
				if (aStr.compareTo("%r") == 0)
					aStr = getContextof(new String("requires"));
				else if (aStr.compareTo("%p") == 0)
					aStr = getContextof(new String("provides"));
				else
					bAbort = true;			// something is wrong. Ignore the rest.

				if (aStr != null)
				{
					command.append(" ");
					command.append(aStr);
				}
				else
					bAbort = true;			// something is wrong. Ignore the rest.
			}
			if (!bAbort)
				_systemCommand = command.toString();
		}
	}
}

