//
// PEOS_ProcessList.java
//

package engineInterface;

import java.util.*;

/**
 * Class PEOS_ProcessList contains information of last execution.
 * It is an internal class in the package. It:
 *
 * 1. offers process search mechinism
 * 2. validates a process.
 * 3. determines the state of a process.
 * 
 * @author Ming Rutar
 */

class PEOS_ProcessList extends Vector
{
	String			_lastCommand;
	PEOS_Process	_lastProc;

	/**
	 * Constructors	
	 */

    public PEOS_ProcessList() 
    {
		_lastCommand = null;
		_lastProc = null;
	}

	/**
	 * Set lats commnad and process
	 *
     * @param cmd the command.
     * @param proc the process.
	 */
	void setLastExec(String cmd, PEOS_Process proc)
	{
		_lastCommand = cmd;
		_lastProc = proc;
	}

	/**
	 * Search a process by process ID and task ID.
	 *
     * @param processID the process ID.
     * @param taskID the task ID.
	 */
	PEOS_Process findProcess(String processID, String taskID)
	{
		PEOS_Process proc;
		for (int i = 0; i < size(); i++)
		{
			proc = (PEOS_Process) elementAt(i);
			if (proc.isSame(processID, taskID))
				return proc;
		}
		return null;
	}

	/**
	 * Find the process in the list of last execution
	 *
     * @param beFound the input process.
	 */
	PEOS_Process findProcess(PEOS_Process beFound)
	{
		PEOS_Process proc;
		for (int i = 0; i < size(); i++)
		{
			proc = (PEOS_Process) elementAt(i);
			if (proc.isSame(beFound))
				return proc;
		}
		return null;
	}

	/**
	 * Find the first process has the same process ID.
	 *
     * @param prodID the process ID.
	 */
	PEOS_Process findProcess(String prodID)
	{
		PEOS_Process proc;
		for (int i = 0; i < size(); i++)
		{
			proc = (PEOS_Process) elementAt(i);
			if (prodID.compareTo(proc._processID) == 0)
				return proc;
		}
		return null;
	}

	/**
	 *  Validates a process and determines the state of a process.
	 *
     * @param proc a process.
	 */
	int determState(PEOS_Process proc)
	{
		int ret = PEOS_Interface.STATE_OK;
		PEOS_Process oldProc = null;
		if (proc.isRunning())
		{
		    oldProc = findProcess(proc);
		    if ((oldProc == null) && (_lastProc != null))
		    {
				proc.setStatus(PEOS_Process.ERR_EXTRARUNNING);
				ret = PEOS_Interface.STATE_ERROR;
		    }
		    else if (proc.isSame(_lastProc) && (_lastCommand.compareTo("run") == 0))
				proc.setState(PEOS_Process.STS_CURRENTRUNNING);
        }		    
		else
		{
		    oldProc = findProcess(proc._processID);
		    if (_lastProc != null)
		    {
			    if (proc._processID.compareTo(_lastProc._processID) == 0)
				    proc.setState(PEOS_Process.STS_CURRENTAVAIL);
		    }
		}
		if (oldProc != null)
			removeElement(oldProc);
		return ret;
	}

	/**
	 *  Reset the list and further validate processes. It reurns the finished
	 *  or branched process if it be found, null otherwise.
	 *
     *  @param avails the list of available processes.
     *  @param runs the list of running processes.
     *  @param errs the list of error processes.
	 */
	PEOS_Process postProcessing(Vector avails, Vector runs, Vector errs)
	{
		PEOS_Process finishedProc = null;
		PEOS_Process proc;
		
		for (int i = 0; i < size(); i++)
		{
			proc = (PEOS_Process) elementAt(i);
			if (proc.isSame(_lastProc) && (_lastCommand.compareTo("done") == 0))
				finishedProc = proc;	// it also could be branch.
			else if (proc.isRunning())
			{
				proc.setStatus(PEOS_Process.ERR_RUN_DROPPED);
				errs.addElement(proc);
			}
        }
		removeAllElements();		// should be empty already
			
		int iRun = 0;
		int iAvail = 0;

		if ((runs.size() > 0) && (avails.size() > 0))
		{
			boolean bContinue = true;
			while (bContinue)
			{
				proc = (PEOS_Process) runs.elementAt(iRun);
				while ((proc.compareProcID((PEOS_Process)avails.elementAt(iAvail)) >= 0) && bContinue)
				{
					addElement(avails.elementAt(iAvail++));
					if (iAvail == avails.size())
					{
					    bContinue = false;
					    break;
					}
				}
				if (bContinue)
				{
					proc = (PEOS_Process)avails.elementAt(iAvail);
					while ((proc.compareProcID((PEOS_Process)runs.elementAt(iRun)) >= 0) && bContinue)
					{
						addElement(runs.elementAt(iRun++));
						if (iRun == runs.size())
						{
							bContinue = false;
                            break;
                        }
					}
				}
			}
		}
		if (runs.size() == iRun)
		{
			for (int i = iAvail; i < avails.size(); i++)
				addElement(avails.elementAt(i));
		}
		else if (avails.size() == iAvail)
		{
			for (int i = iRun; i < runs.size(); i++)
				addElement(runs.elementAt(i));
		}
		return finishedProc;
	}
}