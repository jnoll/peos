import junit.framework.*;

public class testACJU extends TestCase{

	private static displayPO dpo;
	private static ActionMap map;

	public static Test suite()
	{
        	return new TestSuite(testACJU.class);
	}

	public void testNext()
	{
		//boolean goodTimes = true;
		LinkNode tester;
		map.setToFirstAction(1);
		for (int i=0; i<17; i++)
		{
			tester=map.getNextActionDetails(1);
			assertTrue(tester != null);
			//if (tester == null)
			//	goodTimes=false;

		}
		//assertTrue(goodTimes);
	}


	public void testNextCheck()
	{
	        assertTrue(map.isNextOk(1) == false);
	}

	public void testNextAtLimit()
	{
	       assertTrue(map.getNextActionDetails(1) == null);
	}

	public void testPrev()
	{
		LinkNode tester;
		for (int i=0; i<17; i++)
		{
			tester = map.getPrevActionDetails(1);
			assertTrue(tester != null);
		}
	}

	public void testPrevCheck()
	{
		for (int i=0; i<17; i++)
		{
			map.getPrevActionDetails(1);
		}
 		assertTrue(map.isPrevOk(1) == false);

	}

	public void testPrevAtLimit()
	{

		for (int i=0; i<17; i++)
		{
			map.getPrevActionDetails(1);
		}
		assertTrue(map.getPrevActionDetails(1) == null);
	}

        public void testSetToFirst()
        {
            map.setToFirstAction(1);
            assertEquals(map.getCurrentAction(1).getAttribute("name"),"overview");
        
        }
        
        public void testBuildActionList()
        {
            assertEquals("<TABLE border=1><TR align=center><TD>State</TD><TD>Action</TD></TR><tr><td>"
                +"Ready</td><td><a href=\"[1]overview\">overview</a></td></tr></TABLE>",map.buildActionList());
        }
	public void testParseActionString()
	{
		String tester="(0)hello";
		String result[]= map.parsePid(tester);
                assertEquals("0", result[0]);
                assertEquals("hello", result[1]);
	}
	protected void setUp()
	{

		try {

			dpo = new displayPO("testfiles/blah.xml");
			dpo.convertDOM(1);
			map = dpo.getActions();

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
