import java.io.*;
import junit.framework.*;

public class testCRJU extends TestCase{

	private static displayPO test1;       
        private static int pid;
        
	public static Test suite()
	{
		return new TestSuite(testCRJU.class);
	}
        public static void setupTest134()
        {
                try{
                    pid=SetupPath.load("testfiles/oldtimec.pml");                    
                }
                catch(IOException e)
                {
                    System.err.println(e);                    
                }
        }
	public void testGetResourceList1() 
        // process has resources but none need changing
	{
		String[] output=null;//{"time_sheet","good_times"};
		try{
			
			test1=new displayPO("proc_table.dat.xml");
			test1.convertDOM(pid);
			String[] result = test1.getResourceList(pid);
			assertEquals(output,result);
			//assertEquals(output[1],result[1]);
			
		}
		catch(Exception e)
		{
                        fail("Exception caught!");
			System.out.println(e);
		}
	}
	public void testGetResourceList2()
        //process has no resources
	{
		String[] output=null;
		try{
                        int pid=SetupPath.load("testfiles/oldtimed.pml");
			
			displayPO test2=new displayPO("proc_table.dat.xml");
			test2.convertDOM(pid);
			String[] result = test2.getResourceList(pid);
			assertEquals(output,result);
			
		}
		catch(IOException e)
		{
			System.out.println(e);
		}
	}
        public void testGetResourceList3() 
        // process has 2 resources, one needs changing
	{
		String[] output={"time_sheet"};//{"time_sheet","good_times"};
		try{
			int pid = SetupPath.load("testfiles/oldtimec.pml");
			
			test1=new displayPO("proc_table.dat.xml");
			test1.convertDOM(pid);
                        test1.bindResource("time_sheet", "whatever",pid);
			String[] result = test1.getResourceList(pid);
			assertEquals(output[0],result[0]);
			//assertEquals(output[1],result[1]);
			
		}
		catch(Exception e)
		{
                        fail("Exception caught!");
			System.out.println(e);
		}
	}
        public void testGetResourceList4() 
        // process has 2 resources, both need changing
	{
		String[] output={"time_sheet", "good_times"};//{"time_sheet","good_times"};
		try{
			int pid=SetupPath.load("testfiles/oldtimec.pml");
			test1=new displayPO("proc_table.dat.xml");
			test1.convertDOM(pid);
                        test1.bindResource("time_sheet", "whatever",pid);
                        test1.bindResource("good_times", "whatever",pid);
			String[] result = test1.getResourceList(pid);
			assertEquals(output[0],result[0]);
			assertEquals(output[1],result[1]);
			
		}
		catch(Exception e)
		{
                        fail("Exception caught!");
			System.out.println(e);
		}
	}
        
        
	public static void main(String args[])
	{
                setupTest134();
		junit.textui.TestRunner.run(suite());
	}
}
