import java.io.*;

public class ActiveList{
	int count;
	public ActiveList() throws IOException
	{
	}
	public String[] listOfActive() throws IOException
	{
		String[] active = new String[11];

		count = 0;
		
		Process peos;
		Runtime r=Runtime.getRuntime();

		peos = r.exec(SetupPath.getPeos() + " -i");
	
		try{
			peos.waitFor();
		}
		catch(Exception e)
		{
			System.out.println("PEOS EXCEPTION");
			System.err.println(e); 
		}

			
		DataInputStream peos_in = new DataInputStream(peos.getInputStream());
		String input;
		
		for (int i = 0; i< 11; i++)
		{
			input = peos_in.readLine();

			if(input.length() <= 3)
			{}	
			else
			{
				active[i] = input;
				count++;
			}

		}
			
		return active;
	}	
	
	public int numActive()
	{
		return count;
	}       
}
