import java.io.*;
import junit.framework.*;


public class testLoadJU extends TestCase{
	public static Test suite()
	{
		return new TestSuite(testLoadJU.class);
	}
	public void testBadPath()
	{
		try{
			LoadProcess proc1=new LoadProcess("/ver/123/whatever.pml");
			int result=proc1.tryToLoad();
			assertTrue(result==-2);
		}
		catch(IOException err)
		{
	
		}
	}
	public void testGoodPath()
	{
		try{
			LoadProcess proc1=new LoadProcess("./testfiles/oldtime.pml");
			int s1=proc1.tryToLoad();
			assertTrue(s1>=0);
		}
		catch(IOException err)
		{
	
		}
	}
	
	public void testGoodPathBadFile()
	{
		try{
			LoadProcess proc1=new LoadProcess("./testfiles/timesheet.pml");
			int s1=proc1.tryToLoad();
			assertTrue(s1==-1);
		}
		catch(IOException err)
		{
		}
	}
	
	
	public static void main(String[] args)
	{
		junit.textui.TestRunner.run(suite());
	}
}

