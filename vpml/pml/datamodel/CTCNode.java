package pml.datamodel;

import pml.Exceptions.*;
import pml.datamodel.CPMLNode;

/**
 * This class represents the basic TCNode.  
 *
 * @author Matt Osminer
 * @author Xing Wei
 */
public class CTCNode extends CPMLNode
{
    /**
     * Basic Constructor - sets up the default graph node behavior and
     * the starting properties for this process
     *
     * @param TCName - the name of the TC construct.  Serves as the Symbol name      * so no spaces allowed
     * @param TCType - string reprenting the type of this TC. See setType for 
     * valid options
     * @param Comments - string representing any process comments the author
     * wants to include
     * @throws CPMLException
     */
    public CTCNode( String TCName, String TCType, String Comments )
        throws CPMLException
    {
        // No parent node, children are allowed, no child order
        super( null, true, false );
        
        setSymbolName( TCName );
        setType( TCType );
        setComments( Comments );
    }
    
    public void setTCName( String Name )
        throws CPMLException
    {
        // TODO Name Validation
        setSymbolName( Name );
    }
    
    public void setType( String Type )
    {
        // TODO Verify the type is one of sequence, task, branch, selection, 
        // iteration
        m_Type = new String( Type );
    }
    
    public String getTCName()
    {
        return getSymbolName();
    }
    
    public String getType()
    {
        return new String( m_Type );
    }
    
    /**
     * Dump the author (comments are handled by the base class 
     * and the author info to the string.  Then dump the process keyword and 
     * symbol name
     * @param pmlString Reference to string to which we should add this
     * @param SpaceCardinality Number of spaces to add
     */
    public String PMLHeader( int SpaceCardinality )
    {
        String pmlString = new String();
        String ws = buildWhitespace( SpaceCardinality );
        
        // Output the author data
        pmlString += ws + getType() + " " +
                     getSymbolName() + "\n" + ws + "{\n";
        
        return pmlString;
    }
    
    /*
     * Append any closing comments to the input pmlString 
     */
    public String PMLClose( int SpaceCardinality )
    {
        return new String( buildWhitespace( SpaceCardinality ) + "}\n" );
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
    private String m_Type;
}
