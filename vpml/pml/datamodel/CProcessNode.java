package pml.datamodel;

import pml.Exceptions.*;
import pml.datamodel.CPMLNode;

/**
 * This class represents the basic Process Node.  You should always start with
 * this type of node to represent your process.  From here you can add or remove nodes
 * as you need 
 *
 * @author Matt Osminer
 * @author Xing Wei
 */
public class CProcessNode extends CPMLNode
{
    /**
     * Basic Constructor - sets up the default graph node behavior and
     * the starting properties for this process
     *
     * @param ProcessName - the name of the process.  Serves as the process Symbol name so no spaces allowed
     * @param Author - string reprenting the author of this process
     * @param Comments - string representing any process comments the author wants to include
     *
     * @throws CPMLException - see setProcessName, Author, or Comments for more details
     */
    public CProcessNode( String ProcessName, String Author, String Comments )
        throws CPMLException
    {
        // No parent node, children are allowed, no child order
        super( null, true, false );
        
        setSymbolName( ProcessName );
        setAuthor( Author );
        setComments( Comments );
    }
    
    public void setProcessName( String Name )
        throws CPMLException
    {
        // TODO Name Validation
        setSymbolName( Name );
    }
    
    public void setAuthor( String Author )
    {
        m_Author = new String( Author );
    }
    
    public String getProcessName()
    {
        return getSymbolName();
    }
    
    public String getAuthor()
    {
        return new String( m_Author );
    }
    
    /**
     * Dump the author (comments are handled by the base class 
     * and the author info to the string.  Then dump the process keyword and symbol name
     *
     * @param pmlString Reference to string to which we should add this
     * @param SpaceCardinality Number of spaces to add
     */
    public String PMLHeader( int SpaceCardinality )
    {
        String pmlString = new String();
        
        pmlString += "//******************* PML PROCESS SOURCE ********************\n";
        
        // Output the author data
        pmlString += "// AUTHOR:  " + getAuthor() + "\n\n";
        
        // Output the process header
        pmlString += buildWhitespace( SpaceCardinality ) + 
                     "process " + getProcessName() + "\n{\n";
                     
        return pmlString;
    }
    
    /*
     * Append any closing comments to the input pmlString 
     */
    public String PMLClose( int SpaceCardinality )
    {
        return new String( "}\n" );
    }
    
    /**
     * Overridden toPMLString with no arguments required
     */
    public String toPMLString()
    {
        String pml = new String();
        
        // Start off the call chain here
        pml = toPMLString( "", 0 );
        
        return pml;
    }
    
    // Private members
    private String m_Author;
}