package pml;

import java.awt.*;
import java.awt.event.*;
import java.awt.Insets;
import java.io.*;
import javax.swing.*;
import javax.swing.filechooser.FileFilter;
import javax.swing.border.*;

class JStatusArea
{
    static private JStatusArea Instance = null;
    static private JPanel statusArea = new JPanel();
    static private JLabel status = new JLabel("");

    private JStatusArea()
    {

    }

    static public JStatusArea getInstance()
    {
      if ( Instance == null )
      {
        Instance = new JStatusArea();
      }
      return Instance;  
    }

    static public void showStatus(String msg)
    {
      status.setText(msg);
    }

    public void createStatusArea(final Container container)
    {
      statusArea.setBorder(BorderFactory.createEtchedBorder());
      statusArea.setLayout(new FlowLayout(FlowLayout.LEFT, 0, 0));
      statusArea.add(status);
      status.setHorizontalAlignment(JLabel.LEFT);
      showStatus("ModelingUI Main Window");

      container.add(statusArea, BorderLayout.SOUTH);
    }
}
