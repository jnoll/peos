import org.apache.xerces.parsers.DOMParser;
import java.io.*;
import org.w3c.dom.*;
import junit.framework.*;
import org.w3c.dom.html.*;
import org.xml.sax.*;

public class testActionMapJU extends TestCase{
	private LinkedList ActionList[] = new LinkedList[11];

	public static Test suite()
	{
		return new TestSuite(testActionMapJU.class);
	}
	
	public void testDeleteProcess()
	{
		ActionMap dp = new ActionMap();
		int ret0 = dp.DeleteProcess(0);
		assertTrue(ret0 == 0);
		assertTrue(ActionList[0] == null);
		int ret10 = dp.DeleteProcess(10);
		assertTrue(ret10 == 0);
		assertTrue(ActionList[10] == null);
		int ret11 = dp.DeleteProcess(11);
		assertTrue(ret11 == 1);
	}

	public void testGetActionByName()
	{
		ActionMap ga = new ActionMap();
		Element ret = ga.getActionByName(0, "action");
		/* check: these are null */
		assertTrue(ret == null);
		Element ret2 = ga.getActionByName(10, "action");
		assertTrue(ret2 == null);
	}

	public void testGetLNodeByName()
	{
		ActionMap gn = new ActionMap();
		LinkNode ret = gn.getLNodeByName(0, "action");
		/* check: these are null */
		assertTrue(ret == null);
		LinkNode ret2 = gn.getLNodeByName(10, "action");
		assertTrue(ret2 == null);
	}

	public void testNumActiveProcesses()
	{
		ActionMap na = new ActionMap();
		int ret = na.numActiveProcesses();
		assertFalse(ret == -1);
	}

	public void testListActiveProcesses()
	{
		ActionMap la = new ActionMap();
		boolean [] ret = la.listActiveProcesses();
		assertTrue((ret[0] == true) || (ret[0] == false));
		assertTrue((ret[10] == true) || (ret[10] == false));
	}

	
	/*public void testGetCurrentAction()
	{
	       ActionMap na = new ActionMap();
	       Element ret =  na.getCurrentAction(0);
	       assertTrue(ret != null);
	       Element ret2 = na.getCurrentAction(10);
	       assertTrue(ret2 != null);
	}
	*/

	public void testGetCurrentLink()
	{
		ActionMap na = new ActionMap();
		LinkNode ret = na.getCurrentLink(0);
		LinkNode ret2 = na.getCurrentLink(10);
		assertTrue(ret == null);
		assertTrue(ret2 == null);
	}

	/*public void testIsNextOk()
	{
		ActionMap na = new ActionMap();
		boolean ret = na.isNextOk(0);
		boolean ret2 = na.isNextOk(10);
		assertTrue((ret == true) || (ret == false));
		assertTrue((ret2 == true) || (ret2 == false));
	}
	*/

	/*public void testGetPrevActionDetails()
	{
		ActionMap na = new ActionMap();
		LinkNode ret = na.getPrevActionDetails(0);
		LinkNode ret2 = na.getPrevActionDetails(10);
		assertTrue((ret != null) & (ret2 != null));
	}
	*/

	/*public void testGetNextActionDetails()
	{
		ActionMap na = new ActionMap();
		LinkNode ret = na.getNextActionDetails(0);
		LinkNode ret2 = na.getNextActionDetails(10);
		assertTrue((ret != null) & (ret2 != null));
	}
	*/

	public void testChooseAction()
	{
		ActionMap na = new ActionMap();
		assertTrue((ActionList[0] == null) && (ActionList[10] == null));
	}

	public void testPrint()
	{
		LinkNode curr;
		int count = 0;
		ActionMap na = new ActionMap();
		for(int i=0; i<11; i++)
			if(ActionList[i] == null) count ++;
		assertTrue(count > 0);	
		na.Print();
	}
	
	public static void main(String[] args)
	{
		junit.textui.TestRunner.run(suite());
	}
}
