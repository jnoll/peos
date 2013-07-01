package pml.datamodel;

import java.util.CGraphNode;

/**
 * This class serves as a base PML class for any PML type construct.
 * Further this class implements basic behavior to convert PML nodes into
 * PML code, as well as some utility methods to assist with that
 * @author Na Li
 * @author M. Osminer
 * @author Xing Wei
 */
abstract public class CPMLNode extends CGraphNode 
{
    /** 
     * @param ParentNode of this PML node
     * @param This node can  have children
     * @param childrenAreOrdered the children must observe a specific order
     * @param Comments Comments associated with this node - may be null
     * @param SymbolName The PML symbol name to generate with this node
     */

    static CPMLNode pmlNodeInst = null;
    public static void setPMLNode(CPMLNode node)
    {
      pmlNodeInst = node;
    }

    public static CPMLNode getPMLNode()
    {
      return pmlNodeInst;
    }

    public CPMLNode( CPMLNode ParentNode,
                     boolean canHaveChildren,
                     boolean childrenAreOrdered )
    {
        // Call the parent constructor
        super( ParentNode, canHaveChildren, childrenAreOrdered );
    }

    // Basic accessors.  If you really need JavaDoc comments for all of these
    // I mean really!

    public String getComments()
    {
        return m_Comments;
    }

    public String getSymbolName()
    {
        return m_SymbolName;
    }

    public void setComments( String Comments )
    {
        if ( Comments != null )
            m_Comments = new String( Comments );
    }

    public void setSymbolName( String SymbolName )
    {
        if ( SymbolName != null )
            m_SymbolName = new String( SymbolName );
    }

    /**
     * This method converts this node to a PML string.  Keep in mind
     * this includes any associated children.  Facility is also provided for
     * increasing tabulature so indents appear nice and neat about each scope
     *
     * @param strPML Reference to the current PML string being built
     * @param SpaceCadinality Current number of spaces to insert for each scope
     */
    public String toPMLString( String strPML, int SpaceCardinality )
    {
        String pml = new String( strPML );
        String ws = buildWhitespace( SpaceCardinality );
        
        // Comments always go first
        pml += CommentsToPML( ws );

        // Output the header for this PML construct - you have to do
        // your own opening scope operator if you want this to work
        pml += PMLHeader( SpaceCardinality );
        
        // Dump the children of this node - if any
        pml += ChildrenToPML( strPML, SpaceCardinality );
        
        // Append any closing remarks you may need - usu. closing scope operator
        pml += PMLClose( SpaceCardinality );
        
        // So you should write your own output format for each construct
        return pml;
    }
    
    /**
     * This method serves as a utility to set comments
     * so that the don't exceed 80 characters in with
     */
    private String CommentsToPML( String Whitespace )
    {
        // Create a new string
        String Comments = new String();
        
        // No comments, no PML
        if ( m_Comments == null )
            return Comments;
            
        // Add our default commentary syntax
        Comments += Whitespace + "// ";
            
        // Keep track of how many characters go on a line
        int CurrentLineCount = Comments.length(); 
        int WordLength = 0;
        String Word;

        // Now iterate over the comments.  Comments begin just like
        // the comments on in this code.  Two forward slashes
        // Basically we're going to read in a word at a time.  If the
        // total line length is greater than 80 (std screen width) then 
        // start a newline until we've completed all the comment conversion
        int nLoopIndex = 0;
        for ( int WordStartIndex = 0; WordStartIndex < m_Comments.length(); ) {
            // First locate the length of the next word looking for spaces here
            nLoopIndex++;
            if ( nLoopIndex == 50000 )
              break;

            while (m_Comments.charAt( WordStartIndex + WordLength++ ) != ' ' &&
                           WordLength + WordStartIndex < m_Comments.length() );

            // If we've exceeded the end of the source string then decrement the
            // WordLen by one so we don't choke trying to remove the substring
            if ( WordLength >= m_Comments.length() )
                WordLength--;
            
            // Now we've got either the end of the Comments, or the end of a 
            // word if we've got a word then check the line length plus the 
            // word length if we exceed 80 characters then start a new line
            if ( CurrentLineCount + WordLength > 79 ) {
                // Add a newline and a standard comment string
                Comments += "\n" + Whitespace + "// ";
                CurrentLineCount = 4 + Whitespace.length();
            }

            // Get the word from the local comments string
            Word = m_Comments.substring( WordStartIndex, WordStartIndex +
                                                                 WordLength );
            
            // Add the word to the current PML formatted comment string we're
            // building
            Comments += Word;
            
            // Add the word length to the word index and chars processed
            WordStartIndex += Word.length();
            
            // Reset the word length for the next go
            WordLength = 0;
            
        }
        
        // Pad the comments with a newline so everything isn't crammed
        // together
        Comments += "\n";
        
        return Comments;
    }

    /** 
     * Method to call for outputting the children of this node into the PML 
     * string. This just recurses the children which recurses their children etc
     * @param pmlString destination string for PML code
     * @param SpaceCardinality The number of spaces to insert on each newline
     */
    public String ChildrenToPML( String pmlString, int SpaceCardinality )
    {
        
        String RetVal = new String();
        
        // If no children allowed then just return
        if ( ChildrenAllowed() == false )
            return RetVal;
            
        // Otherwise recurse the children of this node.  Add 3 to the space card
        // inality so all children are indented by three space in scope
        for ( int ChildNum = 0; ChildNum < NumberOfChildren(); ChildNum++ )
            RetVal += ((CPMLNode)GetAChild( ChildNum )).toPMLString( pmlString,                                                          SpaceCardinality + 3 );
            
        return RetVal;
    }
    
    /**
     * This routine builds a string of empty spaces and is used as a utility
     * routine.  This is really clunky for now.  Perhaps later somebody can 
     * clean it up
     *
     * @param length 
     */
     public String buildWhitespace( int length )
     {
        String whitespace = new String();
        
        while ( length-- != 0 )
            whitespace += " ";
            
        return whitespace;
     }
    /*
     * Override this function in your derived classes - this outputs the custom
     * header info for your specific construct
     * You should include scope operators
     */
    abstract public String PMLHeader( int SpaceCardinality );
    
    /*
     * Append any closing comments to the input pmlString 
     */
    abstract public String PMLClose( int SpaceCardinality );
    
    private String m_Comments;
    private String m_SymbolName;
}
