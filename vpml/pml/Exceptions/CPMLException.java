package pml.Exceptions;

/**
 * This class implemets a basic PML exception - these exceptions are thrown
 * with respect to syntax or other PML related problems
 *
 * @author Matt Osminer (mosminer@ix.netcom.com)
 * @author Xing Wei
 */
public class CPMLException extends java.lang.Exception
{
    /**
     * Constructor
     *
     * @param e String you wish to issue with this exception
     */
    public CPMLException( String e )
    {
        super( e );   
    }
    
    /**
     * Overriden toString method for custom exception formatting
     */
    public String toString()
    {
       return new String( "[CPMLException] = " + super.toString() );
    }
}

