import java.io.*;

public class LoadProcess{
	private String path;
	public LoadProcess(String pmlPath) throws IOException
	{
		path = pmlPath;
	}
	public int tryToLoad() throws IOException
	{
		File f1 = new File(path);
		FileReader f2;

		if (f1.canRead() == false)
			return -2;
		Process peos;
		Runtime r=Runtime.getRuntime();
		try{
			peos = r.exec(SetupPath.getPeos() + " -c " + path);
			try{
				peos.waitFor();
			}
			catch(Exception e)
			{	System.err.println(e); }
			
			DataInputStream peos_in = new DataInputStream(peos.getInputStream());
			String s1;
			int pid;
			while ((s1=peos_in.readLine()) != null)
			{	
				if (s1.substring(0,7).equals("Created")) 
				{
					pid = Integer.parseInt(s1.substring(14,15));
					return pid;
				}
			}
			return -1;
		}
		catch(Exception ee)
		{
			return (-3);
		}
	}	
}
