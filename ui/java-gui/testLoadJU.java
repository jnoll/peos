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
			int result=SetupPath.load("/ver/123/whatever.pml");
			assertTrue(result==-2);
		}
		catch(IOException err)
		{
	
		}
	}
	public void testGoodPath()
	{
		try{
			int s1=SetupPath.load("./testfiles/oldtime.pml");
			assertTrue(s1>=0);
		}
		catch(IOException err)
		{
	
		}
	}
	
	public void testGoodPathBadFile()
	{
		try{	
			int s1=SetupPath.load("./testfiles/timesheet.pml");
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

