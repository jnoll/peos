package pml.datamodel;

import pml.Exceptions.*;
import pml.datamodel.CPMLNode;

/**
 * This class represents the basic SpecNode.  
 *
 * @author Matt Osminer
 * @author Xing Wei
 */
public class CSpecNode extends CPMLNode
{
    /**
     * Basic Constructor - sets up the default graph node behavior and
     * the starting properties for this process
     *
     * @param TCName - the name of the Spec.  Serves as the process Symbol name so no spaces allowed
     * @param TCType - string reprenting the type of this TC. See setType for valid options
     * @param Comments - string representing any process comments the author wants to include
     *
     * @throws CPMLException
     */
    public CSpecNode( String SpecType, String Pathname, String Comments )
        throws CPMLException
    {
        // No parent node, children are allowed, no child order
        super( null, false, false );
        
        setSymbolName( Pathname );
        setSpecType( SpecType );
        setComments( Comments );
    }
    
    public void setSpecType( String Type)
        throws CPMLException
    {
        // TODO Type Validation
        m_SpecType = new String( Type );
    }
    
    public void setPathname( String Pathname )
    {
        setSymbolName( Pathname );
    }
    
    public String getSpecType()
    {
        return new String( m_SpecType );
    }
    
    public String getPathname()
    {
        return getSymbolName();
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
        String ws = buildWhitespace( SpaceCardinality );
        
        // Output the author data
        pmlString += ws + getSpecType() + " { \" " +
                     getPathname() + " \" }";
        
        return pmlString;
    }
    
    /*
     * Append any closing comments to the input pmlString 
     */
    public String PMLClose( int SpaceCardinality )
    {
        return "\n\n";
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
    private String m_SpecType;
}
