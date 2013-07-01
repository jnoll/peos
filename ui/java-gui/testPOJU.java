import java.io.*;
import junit.framework.*;

public class testPOJU extends TestCase{

	private static	displayPO test1;

	public static Test suite()
	{
		return new TestSuite(testPOJU.class);
	}

	public void testOutput()
	{
		String line1, line2;
		try{
			SetupPath.load("testfiles/oldtimec.pml");
			test1=new displayPO("proc_table.dat.xml");
			test1.convertDOM(0);
			ActionMap testA=test1.getActions();
			testA.setToFirstAction(0);
			assertTrue(testA.printCurrentName(0).equals("Fill_name") == true);
			//if (testA.printCurrentName(0).equals("Fill_name") == false)
				//return false;
			while (testA.getNextActionDetails(0) !=null);
			assertTrue(testA.printCurrentName(0).equals("Turn_it_in") == true);
			//if (testA.printCurrentName(0).equals("Turn_it_in") == false)
				//return false;
			//return true;
		}
		catch(IOException e)
		{
			System.out.println(e);
			//return false;
		}
	}

	public void testGS1()
	{
		try{
			ActionMap map = test1.getActions();
			map.reset(0);

			String result=test1.getScript(map.getCurrentAction(0),0);
			assertTrue(result == null);
			//if (result == null)
			//	return true;
			//else
				//return false;
		}
		catch(Exception e)
		{
			System.out.println(e);
		}
		//return false;
	}


	public void testGS2()
	{
		try{
			ActionMap map=test1.getActions();
			map.setToFirstAction(0);
			String result = test1.getScript(map.getCurrentAction(0),0);
			assertTrue(result != null);
			//if (result == null)
			//	return false;
			//else return true;
		}
		catch(Exception e)
		{
			System.out.println(e);
		}
		//return false;
	}

	public void testGS3()
	{
		try{
			ActionMap map=test1.getActions();
			map.setToFirstAction(0);
			String result = test1.getScript(map.getNextActionDetails(0).getElement(),0);
			assertTrue(result.equals("Fill in the hours in the time_sheet"));
			//if (result.equals("Fill in the hours in the time_sheet"))
			//	return true;
			//else return false;
		}
		catch(Exception e)
		{
			System.out.println(e);
		}
		//return false;
	}

	public void testGRN()
	{
		ActionMap map = test1.getActions();
		map.setToFirstAction(0);
		String[] result = test1.getRRName(map.getNextActionDetails(0).getElement());
		assertTrue(result[0].equals("time_sheet"));
		//if (result.equals("time_sheet"))
		//	return true;
		//else
		//	return false;

	}

	public void testPS1()
	{
		ActionMap map = test1.getActions();
		map.setToFirstAction(0);
		String output="Fill in your name and other details in the time_sheet. Make sure you have good_times";
		assertTrue(output.equals(test1.getScript(map.getCurrentAction(0),0)));
		//if (output.equals(test1.getScript(map.getCurrentAction(0),0)))
		//	return true;
		//else return false;
	}

	public void testPS2()
	{
		try{
			ActionMap map = test1.getActions();
			map.setToFirstAction(0);			                       
                        test1.bindResource("time_sheet","whatever",0);
                        map.setToFirstAction(0);
                        test1.start(map.getCurrentAction(0),0);
			String output="Fill in your name and other details in the <a href=\"kfmclient exec whatever\">time_sheet</a>. Make sure you have good_times";
				assertTrue(output.equals(test1.getScript(map.getActionByName(0,"Fill_name"),0)));                                                                  }
		catch(Exception e)
		{
			System.out.println(e);
                        fail();
		}
	}

	public void testRRC()
	{
		ActionMap map = test1.getActions();
		ResourceMap rMap = test1.getResources();
		rMap.reset(0);
		map.setToFirstAction(0);
		assertTrue(test1.isResValid(rMap.getCurrentResource(0)));
		//if(test1.isResValid(rMap.getCurrentResource(0)))
		//	return true;
		//else
		//	return false;
	}

	public void testPRC()
	{
		ActionMap map = test1.getActions();
		ResourceMap rMap = test1.getResources();
		rMap.nextResource(0);
		map.setToFirstAction(0);
		assertTrue(test1.isResValid(rMap.getCurrentResource(0)) == false);
		//if(test1.isResValid(rMap.getCurrentResource(0)))
		//	return false;
		//else
			//return true;
	}
        public void testChkPid1()
        {
            try{
                assertTrue(test1.checkForPid(0));
            }
            catch(Exception e)
            {         
                fail("Exception caught in testChkPid1");
            }
        }
        public void testChkPid2()
        {
            try{
                assertFalse(test1.checkForPid(1));
            }
            catch(Exception e)
            {         
                fail("Exception caught in testChkPid1");
            }
        }
	public static void main(String args[])
	{
		junit.textui.TestRunner.run(suite());
	}
}
