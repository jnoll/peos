package Utils;

import java.io.*;
import java.util.*;

/*
 * Class Description:  PMLConfig
 *   This class is the is used to read all configuration parameters from
 *   the config file.  It is implemented as a singleton class.  
 */
public class PMLConfig
{  

   private PMLConfig()
   {  
     ConfigMap = new HashMap();
   }

   /*
    *  Initialize the class by reading all of the parameter values.
    *  The parameters and there values are put into a hash map for
    *  quick access.
    */
   public void Initialize(String filename) throws IOException
   {  
     BufferedReader configReader = 
       new BufferedReader(new FileReader(filename));

     String configLine;

     // Read a line at a time until all have been processed 
     while ( (configLine = configReader.readLine()) != null) {

       StringTokenizer t = new StringTokenizer(configLine, ":");

       // The parameter name will be the first token. 
       String paramName = (t.nextToken()).trim();

       // If the parameter name ends with Value (i.e. PEOS_PortValue) then
       // it is assumed to be an integer value.
       if (paramName.endsWith("Value")) {
         Integer paramInt = 
           new Integer((t.nextToken()).trim());
         ConfigMap.put(paramName, paramInt);
       } else {
         String paramString = (t.nextToken()).trim();
         ConfigMap.put(paramName, paramString);
       }
     }

     configReader.close();
   }

   /*
    *  This function is used by users of this class to get a value 
    *  associated with a parameter whose value is a string.
    * 
    */
   public String GetStringValue(String paramName) throws PMLException
   {
     String paramString = null; 
     Object param = ConfigMap.get(paramName);

     if (param != null && param instanceof String) {
       paramString = (String) param;
     } else {
       String errorMessage = "Could not Get Value: " + paramName;
       throw new PMLException(errorMessage);
     }

     return paramString;
   }

   /*
    *  This function is used by users of this class to get a value 
    *  associated with a parameter whose value is a integer.
    * 
    */
   public int GetIntValue(String paramName)  throws PMLException
   {
     Integer paramInt = null;

     Object param = ConfigMap.get(paramName);

     if (param instanceof Integer) {
       paramInt = (Integer) param;
     } else {
       String errorMessage = "Could not Get Value: " + paramName;
       throw new PMLException(errorMessage);
     }

     return paramInt.intValue();
   }

   /*
    *  This function to get the instance for the class.  This implementation
    *  forces this class to be a singleton class
    */
   public static PMLConfig GetConfig()
   {
     if (Instance == null) {
       Instance = new PMLConfig();
     }

     return Instance;
   }
     
   private static PMLConfig Instance = null;  // Instance for class.
   private HashMap ConfigMap;  // Map for parameter, parameter value pairs.
}
