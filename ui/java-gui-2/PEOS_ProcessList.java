//
// PEOS_ProcessList.java
//
import java.util.*;

public class PEOS_ProcessList extends Vector
{
	String			_lastCommand;
	PEOS_Process	_lastProc;

    public PEOS_ProcessList() 
    {
		_lastProc = null;
   }

	void setLastExec(String cmd, PEOS_Process proc)
	{
		_lastCommand = cmd;
		_lastProc = proc;
	}

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

	//
	// determine whether the proc is current selected
	// 

	int determState(PEOS_Process proc)
	{
		int ret = PEOS_Interface.STATE_OK;
		PEOS_Process oldProc = findProcess(proc);
		if (oldProc == null)
		{
			if (proc.isRunning())
			{
				proc.setStatus(PEOS_Process.ERR_EXTRARUNNING);
				ret = PEOS_Interface.STATE_ERROR;
			}
		}
		else
		{
			if (proc.isRunning() && 
				proc.isSame(_lastProc) && (_lastCommand.compareTo("run") == 0))
				proc.setState(PEOS_Process.STS_CURRENTRUNNING);
			else if (proc.isAvailable() && proc.isSame(_lastProc))
				proc.setState(PEOS_Process.STS_CURRENTAVAIL);
			removeElement(oldProc);
		}
		return ret;
	}

//
//  for now branch is counted as finished
//
	PEOS_Process postProcessing(Vector avails, Vector runs, Vector errs)
	{
		PEOS_Process finishedProc = null;
		PEOS_Process proc;
/*		try
		{
			while (true)
			{
				proc = (PEOS_Process) lastElement();
				if (proc.isSame(_lastProc) && (_lastCommand.compareTo("done") == 0))
					finishedProc = proc;	// it also could be branch.
				else if (proc.isRunning())
				{
					proc.setStatus(PEOS_Process.ERR_RUN_DROPPED);
					errs.addElement(proc);
				}
				removeElement(proc);
			}
		}
		catch (NoSuchElementException e)
		{
		}
*/
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