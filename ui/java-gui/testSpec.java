/*
 * testSpec.java
 * JUnit test for testing spec-file related functions.
 *
 */

import junit.framework.*;
import org.apache.xerces.parsers.DOMParser;
import java.io.*;
import org.w3c.dom.*;
import org.w3c.dom.html.*;
import org.xml.sax.*;

/**
 *
 * @author Steve Beeby
 */
public class testSpec extends TestCase {
    
    private static displayPO dpo;
    private static int pidNum;
    
    public testSpec(java.lang.String testName) {
        super(testName);
    }
    
    public static Test suite() {
        TestSuite suite = new TestSuite(testSpec.class);
        return suite;
    }
    
 
    public static void setuptest()
    {
        try{
            pidNum=SetupPath.load("testfiles/oldtimecmr.pml");
            dpo = new displayPO("proc_table.dat.xml");  
            dpo.convertDOM(pidNum);
        }
        catch(Exception err)
        {
            System.out.println(err);
        }
    }
    public void testGetSpecFile()
    {
        String test1="/home/username/pml/mike.pml";
        assertEquals("/home/username/pml/mike.res",SpecLoader.getSpecFileName(test1));
    }
    public void testGetSpecFileBadName()
    {
        String test1="/home/username/pml/mike.txt";
        assertEquals("Not a valid PML file.",SpecLoader.getSpecFileName(test1));
    }
    
    public void testLoadSpecNotExist()
    {
        String test1="/home/yo/wow/blah/pml/notreal/probablynotanyways/mike.pml";
        assertTrue(SpecLoader.LoadSpecFile(test1, dpo,pidNum) == 1);
    }
    public void testLoadSpecNormal()
    {
        ResourceMap rm = dpo.getResources();
        String test1="testfiles/oldtimecmr.res";
        assertTrue(SpecLoader.LoadSpecFile(test1,dpo,pidNum)==0);
        //assertEquals()
        Element tester=rm.getResource("time_sheet",pidNum);
        if (tester==null)
            System.out.println("WTF MATES");
        assertEquals("time_sheet.doc", tester.getAttribute("value"));
    }
    public static void main(String args[])
    {
        setuptest();        	
        junit.textui.TestRunner.run(suite());
    }
    
}
