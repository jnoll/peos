import java.io.*;

public class DeleteProcess{
	public int pid;
	public DeleteProcess(int pid) throws IOException
	{
		this.pid = pid;
	}

	public void tryToDelete(ActionMap m) throws IOException
	{
		Process peos;
		Runtime r=Runtime.getRuntime();
	
		try{
                	peos = r.exec(SetupPath.getPeos() + " -d " + pid);
			try{
				peos.waitFor();
			}
			catch(Exception e)
			{	
				System.err.println(e);
			}
				
			m.DeleteProcess(pid);
		}
		catch(Exception ee)
		{
			System.err.println(ee);
		}
	}
}
