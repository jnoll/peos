import java.io.*;

public class testPO{

	private static	displayPO test1;
	public static boolean testOutput()
	{
		String line1, line2;
		try{
			LoadProcess p1=new LoadProcess("testfiles/oldtimec.pml");
			p1.tryToLoad();
			test1=new displayPO("proc_table.dat.xml");
			test1.convertDOM(0);
			ActionMap testA=test1.getActions();
			testA.setToFirstAction(0);
			if (testA.printCurrentName(0).equals("Fill_name") == false)
				return false;
			while (testA.getNextActionDetails(0) !=null);
			if (testA.printCurrentName(0).equals("Turn_it_in") == false)
				return false;
			return true;
		}
		catch(IOException e)
		{
			System.out.println(e);
			return false;
		}
	}
	public static boolean testGS1()
	{
		try{ 
			ActionMap map = test1.getActions();
			map.reset(0);
			
			String result=test1.getScript(map.getCurrentAction(0),0);	
			if (result == null)
				return true;
			else
				return false;
		}
		catch(Exception e)
		{
			System.out.println(e);
		}
		return false; 
	}
	public static boolean testGS2()
	{
		try{
			ActionMap map=test1.getActions();
			map.setToFirstAction(0);
			String result = test1.getScript(map.getCurrentAction(0),0);
			if (result == null)
				return false;
			else return true;
		}
		catch(Exception e)
		{
			System.out.println(e);
		}
		return false; 
	}
	public static boolean testGS3()
	{
		try{
			ActionMap map=test1.getActions();
			map.setToFirstAction(0);
			String result = test1.getScript(map.getNextActionDetails(0).getElement(),0);
			if (result.equals("Fill in the hours in the time_sheet"))
				return true;
			else return false;
		}
		catch(Exception e)
		{
			System.out.println(e);
		}
		return false;
	}
	public static boolean testPS1()
	{
		ActionMap map = test1.getActions();
		map.setToFirstAction(0);
		String output="Fill in your name and other details in the time_sheet. Make sure you have good_times";
		if (output.equals(test1.getScript(map.getCurrentAction(0),0)))
			return true;
		else return false;
	}
	public static boolean testPS2()
	{
		ActionMap map = test1.getActions();
		map.setToFirstAction(0);
		test1.startSetValue(map.getCurrentAction(0), "whatever", 0);
		String output="Fill in your name and other details in the <a href=\"kfmclient exec whatever\">time_sheet</a>. Make sure you have good_times";
		if (output.equals(test1.getScript(map.getActionByName(0,"Fill_name"),0)))
			return true;
		else return false;
	}
	public static boolean testRRC()
	{
		ActionMap map = test1.getActions();
		ResourceMap rMap = test1.getResources();
		rMap.reset(0);
		map.setToFirstAction(0);
		if(test1.isResValid(rMap.getCurrentResource(0)))
			return true;
		else
			return false;
	}
	public static boolean testPRC()
	{
		ActionMap map = test1.getActions();
		ResourceMap rMap = test1.getResources();
		rMap.nextResource(0);
		map.setToFirstAction(0);
		if(test1.isResValid(rMap.getCurrentResource(0)))
			return false;
		else
			return true;
	}
	public static boolean testGRN()
	{
		ActionMap map = test1.getActions();
		map.setToFirstAction(0);
		String result = test1.getRRName(map.getNextActionDetails(0).getElement());
		if (result.equals("time_sheet"))
			return true;
		else
			return false;

	}
	public static void main(String args[])
	{
		int pass=0, fail=0;
		if (testOutput() == false)
		{	System.out.println("1");
			fail++; }
		else pass++;
		if (testGS1() == false)
		{	System.out.println("2");
			fail++; }
		else pass++;
		if (testGS2() == false)
		{	System.out.println("3");
			fail++;
		}
		else pass++;
		if (testGS3() == false)
		{	System.out.println("4");
			fail++;}
		else pass++;
		if (testGRN() == false)
		{	System.out.println("5");
			fail++;}
		else pass++;
		if (testPS1() == false)
		{	System.out.println("6");
			fail++;}
		else pass++;
		if (testPS2() == false)
		{	System.out.println("7");
			fail++;} 
		else pass++;
		if (testRRC() == false)
		{	System.out.println("8");
			fail++;} 
		else pass++;
		if (testPRC() == false)
		{	System.out.println("9");
			fail++;} 
		else pass++;

		System.out.println("displayPO: Pass: " + pass + " Fail: " + fail);
			
	}
}
