import java.io.*;

public class testLoad{
	public static boolean testBadPath()
	{
		try{
			LoadProcess proc1=new LoadProcess("/ver/123/whatever.pml");
			int result=proc1.tryToLoad();
			if (result==-2)
				return true;
			else return false;
		}
		catch(IOException err)
		{
			return false;
		}
	}
	public static boolean testGoodPath()
	{
		try{
			LoadProcess proc1=new LoadProcess("./testfiles/oldtime.pml");
			int s1=proc1.tryToLoad();
			if (s1>=0)
				return true;
			else return false;
		}
		catch(IOException err)
		{
			return false; 
		}
	}
	
	public static boolean testGoodPathBadFile()
	{
		try{
			LoadProcess proc1=new LoadProcess("./testfiles/timesheet.pml");
			int s1=proc1.tryToLoad();
			if (s1==-1)
				return true;
			else return false;
		}
		catch(IOException err)
		{
			return false; 
		}
	}
	
	
	public static void main(String[] args)
	{
		int pass=0;
		int fail=0;
		if (testBadPath() == true)
			pass++;
		else 
		{
			System.out.println("T1");
			fail++;
		}
		if (testGoodPath() == true)
			pass++;
		else 
		{
			System.out.println("T2");
			fail++;
		}
		if (testGoodPathBadFile() == true)
			pass++;
		else{
			fail++;
			System.out.println("T3");
		}
		System.out.println("LoadProcess Pass: "+pass + " Fail: " + fail);
	}
}

