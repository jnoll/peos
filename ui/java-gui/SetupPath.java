import java.io.*;
import java.util.Properties;


public class SetupPath{
//	private static FileWriter output;
	public static String getPeos()
	{
		try {
//			Properties peosSrc = new Properties();
//			peosSrc.load(new FileInputStream("peos.properties"));
			return System.getProperty("peos.path");
		}
		catch(Exception e)
		{
			System.err.println(e);
		}
		return "Error";
	}

	public static String getCurrDir() throws IOException
	{
		Process pwd;
		Runtime r = Runtime.getRuntime();
			
		pwd = r.exec("pwd");

                try{
                        pwd.waitFor();
                }
                catch(Exception e)
                {
                        System.out.println("PWD EXCEPTION");
                        System.err.println(e);
                }

		DataInputStream pwd_in = new DataInputStream(pwd.getInputStream());

		return pwd_in.readLine();	
	}

	public static void displayLink(String path) throws IOException
	{
		Process pwd;
		Runtime r = Runtime.getRuntime();
			
//		pwd = r.exec("kfmclient exec README.txt");
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
