import org.w3c.dom.*;
import junit.framework.*;

public class testSFASJU extends TestCase{

	public static displayPO test;
	public static ActionMap testAM;

	public static Test suite()
	{
		return new TestSuite(testSFASJU.class);
	}

	public void testDFNV()
	{
		Element testElem=testAM.getActionByName(0,"Fill_name");
		assertTrue(test.doesFinishNeedValue(testElem)==0);
		////if (test.doesFinishNeedValue(testElem)==0)
		//	return true;
		//return false;
	}

	public void testDSNV()
	{
		Element testElem=testAM.getActionByName(0,"Fill_name");
		assertTrue(test.doesStartNeedValue(testElem)==1);
		//if (test.doesStartNeedValue(testElem)==1)
		//	return true;
		//return false;
	}

	public void testStartSet()
	{
		Element testElem=testAM.getActionByName(0,"Turn_it_in");
		test.startSetValue(testElem, "whatever",0);
		test.start(testElem,0);
		testElem=testAM.getActionByName(0,"Turn_it_in");
		assertTrue(test.doesStartNeedValue(testElem)==0);
		//if (test.doesStartNeedValue(testElem)==0)
		//	return true;
		//return false;
	}

	public void testStart()
	{
		Element testElem=testAM.getActionByName(0,"Fill_name");
							                         						assertTrue(testElem.getAttribute("state").equals("BLOCKED"));
		//if (!testElem.getAttribute("state").equals("BLOCKED"))
			//return false;
		test.start(testElem,0);
		testElem=testAM.getActionByName(0,"Fill_name");
		assertTrue(testElem.getAttribute("state").equals("RUN"));
		//if (!testElem.getAttribute("state").equals("RUN"))
		//	return false;
		//return true;
	}

	public void testFinish()
	{
		Element testElem=testAM.getActionByName(0,"Fill_name");
		assertTrue(testElem.getAttribute("state").equals("RUN"));
		//if (!testElem.getAttribute("state").equals("RUN"))
		//	return false;
		test.finish(testElem,0);
		testElem=testAM.getActionByName(0,"Fill_name");
		assertTrue(testElem.getAttribute("state").equals("DONE"));
		//if (!testElem.getAttribute("state").equals("DONE"))
		//	return false;
		//return true;
	}

	public void testAbort()
	{
		Element testElem=testAM.getActionByName(0,"Fill_hours");
		test.start(testElem,0);
		testElem = testAM.getActionByName(0,"Fill_hours");
		assertTrue(testElem.getAttribute("state").equals("RUN"));
		//if (!testElem.getAttribute("state").equals("RUN"))
		//	return false;
		test.abort(testElem,0);
                testElem = testAM.getActionByName(0,"Fill_hours");
		assertTrue(testElem.getAttribute("state").equals("NONE"));
		//if (!testElem.getAttribute("state").equals("NONE"))
		//	return false;
		//return true;
	}

	public void testSuspend()
	{
		Element testElem=testAM.getActionByName(0,"Fill_hours");
		test.start(testElem,0);
		testElem=testAM.getActionByName(0,"Fill_hours");
		assertTrue(testElem.getAttribute("state").equals("RUN"));
		//if (!testElem.getAttribute("state").equals("RUN"))
		//	return false;
		test.suspend(testElem,0);
		testElem=testAM.getActionByName(0,"Fill_hours");
		assertTrue(testElem.getAttribute("state").equals("SUSPEND"));
		//if (!testElem.getAttribute("state").equals("SUSPEND"))
		//	return false;
		//return true;
	}

	protected void setUp()
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
	}
	
	public static void main(String[] args)
	{
		junit.textui.TestRunner.run(suite());
	}
}
