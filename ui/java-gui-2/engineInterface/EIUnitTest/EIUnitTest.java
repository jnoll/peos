//
// EIUnitTest.java
//

//
// This is unit test program for engineInterface
//

import java.io.*;
import java.util.*;
import engineInterface.*;

public class EIUnitTest 
{
	static final String DEFAULT_HOST = new String("127.0.0.1"); //			"132.194.43.151");
	static final int DEFAULT_PORT = 14523;
    
	private   Random randVal = new Random(20);

    PEOS_Interface _PEOSInterface = null;

	public EIUnitTest(String user, String host, int port)
	{
		boolean bExit = false;

		try
		{
			_PEOSInterface = new PEOS_Interface(host, port);
			_PEOSInterface.PEOS_login(user, user);
			runTest();
		}
		catch (Exception e) 
		{
			System.err.println(e);
		}
		finally	
		{
			if (_PEOSInterface != null) 
			{
				try
				{
					_PEOSInterface.disconnect();
				}
				catch (Exception ce)
				{
					String err = new String("Exception in finally:")+ce;
					System.err.println(err);
				}
			}
		}
	}
	
	private int getIdxVal(int t)
	{
	    int r = randVal.nextInt();
	    if (r < 0)
	        r = 0 - r;
		return r % t;
	}
	
	Hashtable testRunTask(Vector Objs, String cmd)
	{
		try
		{
			int i = getIdxVal(Objs.size());
			PEOS_Process proc = (PEOS_Process) Objs.elementAt(i);
			String msg = new String("Run Command: ")+cmd;
			msg += new String(" for") + proc.getProcessID() + new String(", ")+proc.getTaskID();
			System.out.println(msg);
			return _PEOSInterface.PEOS_execTask(cmd, proc.getProcessID(), proc.getTaskID());
		}
		catch (Exception e) 
		{
			System.err.println(e);
			return null;
		}
	}	
				
	void runTest()
	{	
		try
		{
			Hashtable retVal = null;
			Vector modelList = _PEOSInterface.getModelList();
			

			boolean bExit = false;
			int iOp = 0;						// create proc
			int iModel, iAvail, iRun;
			while (!bExit)
			{	
				try
				{
					if (iOp == 0)				// create
					{	
						iModel = getIdxVal(modelList.size());
						retVal = _PEOSInterface.PEOS_execModel((String)modelList.elementAt(iModel));
						String msg = new String("Create model: ") + modelList.elementAt(iModel);
						System.out.println(msg);
					}
					if (retVal == null)
						continue;				// try other create;

					if (iOp == 1)				// avail
					{
						if (retVal.containsKey(PEOS_Interface.TL_AVAILABLE))	//avail
							retVal = testRunTask((Vector) retVal.get(PEOS_Interface.TL_AVAILABLE), PEOS_Interface.COMMAND_RUN);
					}
					else if (iOp == 2)				// avail
					{
						if (retVal.containsKey(PEOS_Interface.TL_RUNNING))		//run
							retVal = testRunTask((Vector) retVal.get(PEOS_Interface.TL_RUNNING), PEOS_Interface.COMMAND_DONE);
					}
					if (retVal == null)
						iOp = 0;
					else
						iOp = getIdxVal(3);
				}
				catch (IOException ioe) 
				{
					System.err.println(ioe);
					bExit = true;
				}
				catch (Exception e) 
				{
					System.err.println(e);
				}
			}
		}
		catch (Exception e)
		{
			System.err.println(e);
		}
	}

	public static void usage()
	{
		System.out.println("Usage: java EIUnitTest username [hostname] [port]");
		System.exit(0);
	}

	public static void main(String[] args)
	{
	    String host = DEFAULT_HOST;		
		int port = DEFAULT_PORT;

		if (args.length < 1)
		{
			usage();
		}
		if (args.length == 3)
		{
			try 
			{
				host = args[1];
				port = Integer.parseInt(args[2]);
			}
			catch (NumberFormatException e) 
			{
				usage();
			}
		}
		new EIUnitTest(args[0], host, port);
	}
}
