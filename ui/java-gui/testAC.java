public class testAC{

	private static displayPO dpo;
	private static ActionMap map; 
	public static boolean testNextCheck()
	{
		if (!map.isNextOk(1))
			return true;
		return false;
	}
	public static boolean testNextAtLimit()
	{
		if (map.getNextActionDetails(1) == null)
			return true;

		else return false;
	}
	public static boolean testPrevAtLimit()
	{
		if (map.getPrevActionDetails(1) == null)
			return true;

		else return false;
	}
	public static boolean testNext()
	{
		LinkNode tester;
		map.setToFirstAction(1);
		for (int i=0; i<17; i++)
		{
			tester=map.getNextActionDetails(1);
			if (tester == null)
				return false;
		}
		return true;
	}
	public static boolean testPrev()
	{
		LinkNode tester;
		for (int i=0; i<17; i++)
		{
			tester = map.getPrevActionDetails(1);
			if(tester == null)
				return false;
		}
		return true;
	}
	public static boolean testPrevCheck()
	{
		if (!map.isPrevOk(1))
			return true;
		return false;
	}
	public static void main(String args[])
	{
		int pass=0, fail=0;
		try {
			dpo = new displayPO("testfiles/blah.xml");
			dpo.convertDOM(1);
			map = dpo.getActions();
			if (testNext()) pass++;
				else {System.out.println("1"); fail++;}
			if (testNextCheck()) pass++;
				else {System.out.println("2"); fail++;}
			if (testNextAtLimit()) pass++;
				else {System.out.println("3"); fail++;}
			if (testPrev()) pass++;
				else {System.out.println("4"); fail++;}
			if (testPrevCheck()) pass++;
				else {System.out.println("5"); fail++;}
			if (testPrevAtLimit()) pass++;
				else {System.out.println("6"); fail++;}



			System.out.println("ActionMap pass: " 
			+ pass + " fail: " + fail);
		}
		catch(Exception e)
		{
			System.err.println(e);
		}
	}
}
