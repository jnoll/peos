import java.io.*;

public class SetupPath{
	private static FileWriter output;
	public static String getPeos()
	{
		try {
			BufferedReader getPath = 
			new BufferedReader(new FileReader("./PeosApp.cfg"));
			String blah;
			blah = getPath.readLine();
			return blah;
		}
		catch(IOException e)
		{
			System.err.println(e);
		}
		return "whee";
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
			output = new FileWriter("./PeosApp.cfg");
			do{
				System.out.println("Please enter pathname for PEOS.");
				BufferedReader stdin = 
					new BufferedReader(new InputStreamReader(System.in));
				newPath=stdin.readLine();
				f1 = new File(newPath);
			}while(f1.canRead() == false);

			output.write(newPath);
			output.write(" \n"); 
			output.close();
		}

		catch(IOException err)
		{
			System.err.println(err);
		}
	}
}
