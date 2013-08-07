/*
 * File: NameSurfer.java
 * ---------------------
 * When it is finished, this program will implements the viewer for
 * the baby-name database described in the assignment handout.
 */

import acm.program.*;
import java.awt.event.*;
import javax.swing.*;

public class NameSurfer extends Program implements NameSurferConstants {

  /**
   * Init reads in the database and initializes the interactors at the bottom of the window.
   */
	public void init() {
		
		// add the Name label
		add(new JLabel("Name"),SOUTH); 
		
		// add the textfield and listener
		textField1 = new JTextField(10); 
		add(textField1,SOUTH); 
		textField1.addActionListener(this); 
		
		// add buttons Graph and Clear
		add(new JButton("Graph"),SOUTH); 
		add(new JButton("Clear"),SOUTH); 
		addActionListeners(); 
		
		 
		graphCanvas = new NameSurferGraph(); 
		add(graphCanvas); 
		
		// reads the specified file and adds the names to the database
		nameDB = new NameSurferDataBase("names-data.txt");
	}

/* Method: actionPerformed(e) */
   
    /* actionPerformed detects when buttons are clicked and 
     * responds to button clicks with the appropriate actions
     */ 	 
   
	public void actionPerformed(ActionEvent e) {
		
		if(e.getSource() == textField1){  
			
			/* The nameDB is a Hashmap that is populated with Name => NameSurferEntry 
			 * key => value pairs. 
			 * 
			 * We lowercase the input to enable case insensitive matching. 
			 * 
			 * We query the database for the specified name and graph the name 
			 * if the name is found
			 */
			if (nameDB.findEntry(textField1.getText().toLowerCase()) != null){ 
				graphCanvas.addEntry(nameDB.findEntry(textField1.getText().toLowerCase())); 
				graphCanvas.update(); 
			}
		}
		String actionTaken = e.getActionCommand(); 
		if(actionTaken.equals("Graph")){
			if (nameDB.findEntry(textField1.getText().toLowerCase()) != null){ 
				graphCanvas.addEntry(nameDB.findEntry(textField1.getText().toLowerCase())); 
				graphCanvas.update(); 
			}
		} else if (actionTaken.equals("Clear")){
			graphCanvas.clear(); 
			graphCanvas.update(); 
		
		} 
	}
	
	
	/*instance variables*/
	private JTextField textField1; 
	private NameSurferDataBase nameDB;
	private NameSurferGraph graphCanvas; 
	
}
