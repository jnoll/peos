import org.w3c.dom.*;

public class testSFAS{

	public static displayPO test;
	public static ActionMap testAM;
	public static boolean testFinish()
	{
		Element testElem=testAM.getActionByName(0,"Fill_name");
		if (!testElem.getAttribute("state").equals("RUN"))
			return false;
		test.finish(testElem,0);
		testElem=testAM.getActionByName(0,"Fill_name");
		if (!testElem.getAttribute("state").equals("DONE"))
			return false;
		return true;	
	}
	public static boolean testDFNV()
	{
		Element testElem=testAM.getActionByName(0,"Fill_name");
		if (test.doesFinishNeedValue(testElem)==0)
			return true;
		return false;
	}
	public static boolean testDSNV()
	{
		Element testElem=testAM.getActionByName(0,"Fill_name");
		if (test.doesStartNeedValue(testElem)==1)
			return true;
		return false;
	}
	public static boolean testStart()
	{
		Element testElem=testAM.getActionByName(0,"Fill_name");
		if (!testElem.getAttribute("state").equals("BLOCKED"))
			return false;
		test.start(testElem,0);
		testElem=testAM.getActionByName(0,"Fill_name");
		if (!testElem.getAttribute("state").equals("RUN"))
			return false;
		return true;	
	}
	public static boolean testStartSet()
	{
		Element testElem=testAM.getActionByName(0,"Turn_it_in");
		test.startSetValue(testElem, "whatever",0);
		test.start(testElem,0);
		testElem=testAM.getActionByName(0,"Turn_it_in");
		if (test.doesStartNeedValue(testElem)==0)
			return true;
		return false;	
	}

	public static boolean testAbort()
	{
		Element testElem=testAM.getActionByName(0,"Fill_hours");
		test.start(testElem,0);
		testElem = testAM.getActionByName(0,"Fill_hours");
		if (!testElem.getAttribute("state").equals("RUN"))
			return false;
		test.abort(testElem,0);
		testElem = testAM.getActionByName(0,"Fill_hours");
		if (!testElem.getAttribute("state").equals("NONE"))
			return false;
		return true;	
	}
	public static boolean testSuspend()
	{
		Element testElem=testAM.getActionByName(0,"Fill_hours");
		test.start(testElem,0);
		testElem=testAM.getActionByName(0,"Fill_hours");
		if (!testElem.getAttribute("state").equals("RUN"))
			return false;
		test.suspend(testElem,0);
		testElem=testAM.getActionByName(0,"Fill_hours");
		if (!testElem.getAttribute("state").equals("SUSPEND"))
			return false;
		return true;	
	}
	public static void main(String args[])
	{
		try{
			test = new displayPO("proc_table.dat.xml");
			test.convertDOM(0);
			testAM=test.getActions();
		}
		catch(Exception e)
		{
			System.err.println(e);
		}
		int pass=0, fail=0;
		if (testDFNV())
			pass++;
		else {System.out.println("DFNV failed"); fail++;}
		if (testDSNV())
			pass++;
		else {System.out.println("DSNV failed"); fail++;}
		if (testStartSet())
			pass++;
		else {System.out.println("startSV failed"); fail++;}
		if (testStart())
			pass++;
		else {System.out.println("start failed"); fail++;}
		if (testFinish())
			pass++;
		else {System.out.println("finish failed"); fail++;}
		if (testAbort())
			pass++;
		else {System.out.println("Abort failed"); fail++;}
		if (testSuspend())
			pass++;
		else {System.out.println("suspend failed"); fail++;}
		System.out.println("Buttons pass: " + pass + " fail: "
		+ fail);	
		
	}
}
