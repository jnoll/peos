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
		
	}

	public void testDSNV()
	{
		Element testElem=testAM.getActionByName(0,"Fill_name");
		assertTrue(test.doesStartNeedValue(testElem)==1);
	}


        public void testBindResources()
        {
            Element testElem=testAM.getActionByName(0,"Turn_it_in");         
            assertTrue(test.doesStartNeedValue(testElem) == 1);
            test.bindResource("time_sheet","my_head_is_a_giant_bucket", 0);
            testElem=testAM.getActionByName(0,"Turn_it_in");            
            assertTrue(test.doesStartNeedValue(testElem) == 0);            
        }

	public void testStart()
	{
		Element testElem=testAM.getActionByName(0,"Fill_name");
                assertTrue(testElem.getAttribute("state").equals("BLOCKED"));                                                                                                                                
		test.start(testElem,0);
		testElem=testAM.getActionByName(0,"Fill_name");
		assertTrue(testElem.getAttribute("state").equals("RUN"));
	}

	public void testFinish()
	{
		Element testElem=testAM.getActionByName(0,"Fill_name");
		assertTrue(testElem.getAttribute("state").equals("RUN"));
		test.finish(testElem,0);
		testElem=testAM.getActionByName(0,"Fill_name");
		assertTrue(testElem.getAttribute("state").equals("DONE"));
	}

	public void testAbort()
	{
		Element testElem=testAM.getActionByName(0,"Fill_hours");
		test.start(testElem,0);
		testElem = testAM.getActionByName(0,"Fill_hours");
		assertTrue(testElem.getAttribute("state").equals("RUN"));
		test.abort(testElem,0);
                testElem = testAM.getActionByName(0,"Fill_hours");
		assertTrue(testElem.getAttribute("state").equals("NONE"));
	}

	public void testSuspend()
	{
		Element testElem=testAM.getActionByName(0,"Fill_hours");
		test.start(testElem,0);
		testElem=testAM.getActionByName(0,"Fill_hours");
		assertTrue(testElem.getAttribute("state").equals("RUN"));
		test.suspend(testElem,0);
		testElem=testAM.getActionByName(0,"Fill_hours");
		assertTrue(testElem.getAttribute("state").equals("SUSPEND"));
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
