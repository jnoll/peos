/*
 * SpecLoader.java
 *  Contains all the relevant functions and loaders for peos SPEC files.
 *
 */
import java.io.*;
/**
 *
 * @author Steve Beeby
 */
public class SpecLoader {
    
    public static String getSpecFileName(String path)
    {
        if ( (path.equals("")) || (path.length() < 5) )
            return "Not a valid PML file.";
        if (path.substring(path.length() -4).equals(".pml") == false)       
            return "Not a valid PML file.";
        String specPath = path.substring(0,path.length()-4);
        specPath=specPath.concat(".res");
        return specPath;
    }
    
    public static int LoadSpecFile(String path, displayPO dpo, int pidNum)
    //precondition: dpo is a valid instantiation of displayPO.
    //postcondition: If path is valid, all resource:value pairs will be loaded
    //              into PEOS application.
    //returns 0 if ok. 1 if path does not load. 
    //returns 2 if there is an IOException. Returns 3 if the file has invalid entries.
    
    {
        String line;
        File f1 = new File(path);
        if (f1.canRead() == false)        
            return 1;
        try{
            BufferedReader reader = new BufferedReader(new FileReader(path));
            while ((line = reader.readLine()) != null)
            {            
                String[] pair=line.split(":");                
                if (pair.length > 2)                
                    return 3;
                dpo.bindResource(pair[0],pair[1],pidNum);
            }
            return 0;            
        }
        catch(IOException oops)
        {
            System.err.println(oops);
            return 2;
        }                      
    }
}
