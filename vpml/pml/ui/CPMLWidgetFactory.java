package pml.ui;

import javax.swing.*;
import javax.swing.border.*;
import javax.swing.ImageIcon;
import java.awt.*;

/**
 * This class provides PML widget creation for insertion into the main view 
 * window of the application. This class implements the Factory design pattern.
 * @author Matt Osminer
 * @author Xing Wei
 */
public class CPMLWidgetFactory extends Object
{
    // Declare border enumerations - it's a shame java doesn't have a 
    // non-object based numeration mecanism
    final transient static public int NO_BORDER = 0;
    final transient static public int SINGLE_BORDER = 1;
    final transient static public int DOUBLE_BORDER = 2;
    final transient static public int PROCESS_BORDER = 3;
    
    /*
     * Method for creating PML widgets - really labels with graphic icons
     * @param SymbolName Name that will appear on the widget
     * @param icon icon to insert into the widget
     * @param Border Style the style of border to create see above list
     * @param Alignment @see Component for valid alignments
     * 
     * @return A newly created JLabel according to your input specifications
     */
     static public JLabel createPMLWidget( String SymbolName, Icon icon, 
                                           int BorderStyle, float Alignment )
     {
	    // Create a new label with the icon adjusted to the left

	    JLabel label = new JLabel( SymbolName, icon, JLabel.LEFT );
	    
	    // Set the border accordingly
	    switch ( BorderStyle ) {
	        // Single line black border of weight 2
	        case SINGLE_BORDER:
	        label.setBorder( new LineBorder( Color.black, 2 ) );
			break;
	        
	        // Double black border outside weight 2, inside weight 1
	        case DOUBLE_BORDER:
	        label.setBorder( new CompoundBorder
	                            ( new CompoundBorder
                                         ( new LineBorder( Color.black, 2 ),
	                                   new EmptyBorder( 3 ,3 ,3 ,3 ) ),
	                                   new LineBorder( Color.black, 1 )));
			break;
	        
	        case PROCESS_BORDER:
	        label.setBorder( new CompoundBorder
	                            ( new CompoundBorder
                                        ( new BevelBorder( BevelBorder.RAISED ),
	                                  new EmptyBorder( 4, 4, 4, 4 ) ),
	                                  new EtchedBorder() ) );
			break;
            
	        // Default is no border at all
	        
	    }

		// Set up the alignment
		label.setAlignmentX( Alignment );

	    return label;
    }
}
