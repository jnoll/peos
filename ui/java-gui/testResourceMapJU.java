import org.apache.xerces.parsers.DOMParser;
import java.io.*;
import org.w3c.dom.*;
import junit.framework.*;
import org.w3c.dom.html.*;
import org.xml.sax.*;

public class testResourceMapJU extends TestCase{
	private LinkedList ResourceList[] = new LinkedList[11];

	public static Test suite()
	{
		return new TestSuite(testResourceMapJU.class);
	}
	
	public void testDeleteProcess()
	{
		ResourceMap dp = new ResourceMap();
		int ret0 = dp.DeleteProcess(0);
		assertTrue(ret0 == 0);
		assertTrue(ResourceList[0] == null);
		int ret10 = dp.DeleteProcess(10);
		assertTrue(ret10 == 0);
		assertTrue(ResourceList[10] == null);
		int ret11 = dp.DeleteProcess(11);
		assertTrue(ret11 == 1);
	}

	public void testGetMap()
	{
		ResourceMap na = new ResourceMap();
			String[] test = na.getMap(0);
			String[] test2 = na.getMap(10);
			assertTrue(test == null);
			assertTrue(test2 == null);
	}

	public void testNumOfResources()
	{
		ResourceMap na = new ResourceMap();
		int ret = na.numberOfResources(0);
		int ret2 = na.numberOfResources(10);
		assertTrue((ret >= 0) && (ret <= 11));
		assertTrue((ret2 >= 0) && (ret2 <= 11));
	}
	
	public void testGetCurRes()
	{
		ResourceMap la = new ResourceMap();
		Element ret = la.getCurrentResource(0);
		Element ret2 = la.getCurrentResource(10);
		assertTrue((ret == null) && (ret2 == null));
	}
	
	/*public void testIsNextOk()
	{
		ResourceMap na = new ResourceMap();
		boolean ret = na.isNextOk(0);
		boolean ret2 = na.isNextOk(10);
		assertTrue((ret == true) || (ret == false));
		assertTrue((ret2 == true) || (ret2 == false));
	}

	public void testNextResource()
	{
		ResourceMap na = new ResourceMap();
		na.nextResource(0);
		na.nextResource(10);
	  	boolean ret = na.isNextOk(0);
		boolean ret2 = na.isNextOk(10);
		assertTrue((ret == true) || (ret == false));
		assertTrue((ret2 == true) || (ret2 == false));
	}
	*/
	
	public void testPrint()
	{
		LinkNode curr;
		int count = 0;
		ResourceMap na = new ResourceMap();
		for(int i=0; i<11; i++)
			if(ResourceList[i] == null) count ++;
		assertTrue(count > 0);	
		na.Print();
	}
	
	public static void main(String[] args)
	{
		junit.textui.TestRunner.run(suite());
	}
}
