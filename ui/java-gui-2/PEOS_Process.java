//
// PEOS_Process.java
//
import java.util.*;

//public class PEOS_Process extends Object
public class PEOS_Process
{
	// values for _status
	public static final int ERR_OK				= 0;
	public static final int ERR_ILLEGAL_TYPE	= 1;
	public static final int ERR_EXTRARUNNING	= 2;
	public static final int ERR_RUN_DROPPED		= 3;
	public static final int ERR_DUPLICATED		= 4;

	// values for _state
	public static final int STS_NONE = 0;
	public static final int STS_RUNNING = 1;
	public static final int STS_CURRENTRUNNING = 2;
	public static final int STS_AVAILABLE = 3;
	public static final int STS_CURRENTAVAIL = 4;
	public static final int STS_DONE = 5;

	public static final int PML_OPTION_LONG = 1;
	public static final int PML_OPTION_SHORT = 2;

	private String[] errorMessage = {"OK", 
		"Illegal instruction.", 
		"The process wasn't on available list",
		"The process was on running list and it wasn't being executed.",
		"The process is already on ruuning list."};
		 
    public PEOS_Process() 
    {
        _state = STS_NONE;
		_status = ERR_OK;
    }

	void setState(int newState)
	{
		_state = newState;
	}

	int	getState()
	{
		return _state;
	}

	void setStatus(int newStatus)
	{
		_status = newStatus;
	}

	int	getStatus()
	{
		return _status;
	}

	String getErrorMsg()
	{
		return errorMessage[_status];
	}

	boolean isRunning()
	{
		return ((_state == STS_RUNNING) || (_state == STS_CURRENTRUNNING)); 
	}

	boolean isAvailable()
	{
		return ((_state == STS_AVAILABLE) || (_state == STS_CURRENTAVAIL));
	}

	boolean isTypeAction()
	{
		return (_type.compareTo("action") == 0);
	}

	public String getContextof(String token) //such as tool requires...
	{
	    String ret = null;
	    int idx, idx2;
	    boolean bFound = false;
	    try
	    {
            if ((idx = PMLScript.indexOf(token))!= -1)
            {
                idx2 = PMLScript.indexOf('{', idx+1);
                if (idx2 != -1)
                {
	                idx = PMLScript.indexOf('}', idx2+1);
	                bFound = (idx != -1) ? true : false;
	            }
                else
                {
                    idx2 = PMLScript.indexOf('(', idx+1);
                     if (idx2 != -1)
                     {
	                    idx = PMLScript.indexOf(')', idx2+1);
	                     bFound = (idx != -1) ? true : false;
	                 }
	            }
                if (bFound)                   
	                ret = new String(PMLScript.substring(idx2+1, idx));
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
	
	public boolean parse(String PMLStr)
	{
		boolean bRet = true;

	    PMLStr.trim();
	    PMLStr.toLowerCase();
	    StringTokenizer st = new StringTokenizer(PMLStr, " \t");

	    String aStr;
		
	    _processID = new String(st.nextToken());
	    _task = new String(st.nextToken());
//		from st.nextToken() to end save to PMLScript;
		PMLScript = new String(PMLStr); // for now

	    aStr = new String(st.nextToken());
	    if (aStr.compareTo("type") == 0)
	    {
	        _type =  new String(st.nextToken());
	        aStr  = st.nextToken();
	        if (aStr.compareTo("mode") == 0)
	        {
			    _mode  =  new String(st.nextToken());

/*				if (_type.compareTo("action") == 0)
				{    
					_requires = getContextof("requires");
					_provides = getContextof("provides");
					_agent = getContextof("agent");
					_script = getContextof("script");
					_tool = getContextof("tool");		
				}*/
			}
            else if (aStr.compareTo("children") == 0)
			    _children = getContextof("children");
			else
			    bRet = false;
        }
        else
            bRet = false;
		return bRet;
	}
//
// ?? not done yet
	public String getPMLString()
	{
		return PMLScript;
	}

    public int compareProcID(PEOS_Process proc)
    {
		if (proc == null)
			return 1;
		else
			return _processID.compareTo(proc._processID);
    }

    public boolean isSame(String procID, String taskID)
    {
		if ((procID == null) || (taskID == null))
			return false;
		else
			return ((_processID.compareTo(procID) ==0) &&
				(_task.compareTo(taskID) ==0));
	}
    
    public boolean isSame(PEOS_Process proc)
    {
		if (proc == null)
			return false;
		else
			return ((_processID.compareTo(proc._processID) ==0) &&
				(_task.compareTo(proc._task) ==0));
	}

//
// This struct is extracted from mailUI
//
	public String	_processID;
	public String	_task;
	public String	_type;              // "select", "sequence", ("iteract"?) or "action"
	// for action
	public String	_mode;              // for "mode", "manual" or "executable"
	// for non-action
    public String   _children;          // value for "children", {"xx1" "xx2"}

/*	public String	_requires;          // for "requires", {"xx1" "xx2"}
	public String	_provides;          // for "provides", {"xx1" "xx2"}
	public String	_agent;             // for "agent", {"xx1" "xx2"}
	public String	_script;            // for "script", {"xxxxx"}
	public String	_tool;              // for "tool", {"xxxxx"}	*/

	private String  PMLScript;			// saved 
	private String	_parentProcID;      // for branch, not in use
	private int		_state;				// run, suspend, done, avail.
	private int		_status;			// ok, error.
}

