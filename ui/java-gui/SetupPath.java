import java.io.*;
import java.util.Properties;


public class SetupPath{
//	private static FileWriter output;
	public static String getPeos()
	{
		try {
			Properties peosSrc = new Properties();
			peosSrc.load(new FileInputStream("peos.properties"));
			return peosSrc.getProperty("path");
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
        
}
