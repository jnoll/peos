//
// PEOS_Process.java
//

//public class PEOS_Process extends Object
public class PEOS_Process
{
    public PEOS_Process() 
    {
        _execState = 0;
        _diagCode = 0;
    }
    
    public PEOS_Process(PEOS_Process Right) 
    {
        
    }
    
    public int compareTo(PEOS_Process Right)
    {
         return _processID.compareTo(Right._processID);
    }
    
    public boolean isValid()
    {
        return (_diagCode == 0);        // for now
    }
//
// This struct is extracted from mailUI
//
	public String	_processID;
	public String	_task;
	public String	_type;              // ??
	public String	_typeValue;         // used in mailUI
	public String	_mode;              // ??
	public String	_modeValue;         // used in mailUI
	public String	_requires;          // stripped {},strings delim by " "
	public String	_provides;          // stripped {},strings delim by " "
	public String	_agent;             // stripped {},strings delim by " "
	public String	_script;            // stripped {},strings delim by " "

	public String	_parentProcID;
	public int		_execState;			// run, suspend, done, avail.
	public int		_diagCode;	        // not for this version	
}

