import java.io.*;
import java.util.Properties;


public class SetupPath{

        /**
         *  Returns the path to the PEOS console application.
         *  @return A string containing the path to the PEOS executable.
         */
	public static String getPeos()
	{
		try {
			return System.getProperty("peos.path");
		}
		catch(Exception e)
		{
			System.err.println(e);
		}
		return "Error";
	}

        /**
         *  A simple function that returns the current directory.
         *  @return The current directory in a String.
         */
	public static String getCurrDir() throws IOException
	{
            return System.getProperty("user.dir");        
	}

        /**
         *  Runs the href received from clicking on a resource name within
         *  the HTML script. Basically, however, this function can be used to 
         *  execute any commandline code.
         *  @param path The string that will be sent to the command line
         *              to be executed.
         */
	public static void displayLink(String path) throws IOException
	{
		Process pwd;
		Runtime r = Runtime.getRuntime();
			
		pwd = r.exec(path);

                try{
                        pwd.waitFor();
                }
                catch(Exception e)
                {
                        System.out.println("kfmclient EXCEPTION");
                        System.err.println(e);
                }
	}

	public static void main(String args[])
	{
		String newPath;
		File f1;
		try
		{	
			Properties peosSrc=new Properties();
			do{
				System.out.println("Please enter pathname for PEOS.");
				BufferedReader stdin = 
					new BufferedReader(new InputStreamReader(System.in));
				newPath=stdin.readLine();
				f1 = new File(newPath);
			}while(f1.canRead() == false);

			peosSrc.setProperty("path", newPath);
			peosSrc.store(new FileOutputStream("peos.properties"), "PeosGUI Properties");
		}

		catch(Exception err)
		{
			System.err.println(err);
		}
	}
        
        public static void delete(int pid) throws IOException {
            Process peos;
            Runtime r=Runtime.getRuntime();
            
            try{
                peos = r.exec(SetupPath.getPeos() + " -d " + pid);
                try{
                    peos.waitFor();
                }
                catch(Exception e) {
                    System.err.println(e);
                }
                
                //m.DeleteProcess(pid);
            }
            catch(Exception ee) {
                System.err.println(ee);
            }
        }
        public static int load(String path) throws IOException
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
