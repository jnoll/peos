
import pml.JMainFrame;
import java.awt.*;

/**
 * This class serves as a simple launcher for the application not much else
 */
public class ModelingUI 
{
    // Main method for application execution
    public static void main( String argv[] )
    {
        JMainFrame MainFrame = new JMainFrame();
        
        // Show the mainframe window now
        MainFrame.setVisible( true );

        Dimension d;
	    d = Toolkit.getDefaultToolkit().getScreenSize();
        MainFrame.setLocation((d.width - MainFrame.getBounds().width)/2, 
                               (d.height - MainFrame.getBounds().height)/2);
        /* 
         * Start idle time processing and wait for the user to request a 
         * close on the window.
         */

    }
}
