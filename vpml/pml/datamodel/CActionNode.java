package pml.datamodel;

import pml.Exceptions.*;
import pml.datamodel.CPMLNode;

/**
 * This class represents the basic Action Node.  
 * For PML representation
 *
 * @author Na Li
 * @author Matt Osminer
 * @author Xing Wei
 */
public class CActionNode extends CPMLNode
{
    /**
     * Basic Constructor - sets up the default graph node behavior and
     * the starting properties for this process
     *
     * @param TCName - the name of the process.  Serves as the process Symbol 
     * name so no spaces allowed.
     * @param Action - 1 means automatic, 0 means manual, -1 means no type
     * @param Comments - string representing any process comments the author 
     * wants to include
     *
     * @throws CPMLException
     */
    public CActionNode( String ActionName, int ActionType, String Comments )
        throws CPMLException
    {
        // No parent node, children are allowed, no child order
        super( null, true, false );
        
        setSymbolName( ActionName );
        setType( ActionType );
        setComments( Comments );
    }
    
    public void setActionName( String Name )
        throws CPMLException
    {
        // TODO Name Validation
        setSymbolName( Name );
    }
    
    public void setType( int Type )
    {
        // TODO Bounds checking here
        m_Type = Type;
    }
    
    public String getActionCName()
    {
        return getSymbolName();
    }
    
    public int getType()
    {
        return m_Type;
    }
    
    public String PMLHeader( int SpaceCardinality )
    {
        String pmlString = new String();
        String ws = buildWhitespace( SpaceCardinality );
        
        // Output the author data
        pmlString += ws + "action " +  getSymbolName();
        
        // Set the type accordinly
        if ( getType() == 1 )
            pmlString += " automatic";
        else if ( getType() == 0 )
            pmlString += " manual";
            
        // now the open scope operator
        pmlString += "\n" + ws + "{\n";
        
        return pmlString;
    }
    
    /*
     * Append any closing comments to the input pmlString 
     */
    public String PMLClose( int SpaceCardinality )
    {
        return new String( "\n" + buildWhitespace( SpaceCardinality ) + "}\n");
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
    private int m_Type;
}
