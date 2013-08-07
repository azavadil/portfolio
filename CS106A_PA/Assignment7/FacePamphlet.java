/* 
 * File: FacePamphlet.java
 * -----------------------
 * When it is finished, this program will implement a basic social network
 * management system.
 */

import acm.program.*;
import acm.graphics.*;
import acm.util.*;
import java.awt.event.*;
import javax.swing.*;

public class FacePamphlet extends Program 
					implements FacePamphletConstants {

	/**
	 * This method has the responsibility for initializing the 
	 * interactors in the application, and taking care of any other 
	 * initialization that needs to be performed.
	 */
	public void init() {
		
		statusField = new JTextField(TEXT_FIELD_SIZE); 
		statusField.setActionCommand("Change Status"); 
		add(statusField, WEST); 
		add(new JButton("Change Status"), WEST);
		add(new JLabel(EMPTY_LABEL_TEXT),WEST); 
		
		pictureField = new JTextField(TEXT_FIELD_SIZE); 
		pictureField.setActionCommand("Change Picture"); 
		add(pictureField,WEST); 
		add(new JButton("Change Picture"),WEST); 
		add(new JLabel(EMPTY_LABEL_TEXT),WEST);
		
		friendField = new JTextField(TEXT_FIELD_SIZE); 
		friendField.setActionCommand("Add Friend");
		add(friendField, WEST); 
		add(new JButton("Add Friend"), WEST); 
		
		add(new JLabel("Name"), NORTH);
		nameField = new JTextField(TEXT_FIELD_SIZE); 
		add(nameField,NORTH     ); 
		add(new JButton("Add"), NORTH); 
		add(new JButton("Delete"), NORTH); 
		add(new JButton("Lookup"), NORTH); 
		
		addActionListeners(); 
		statusField.addActionListener(this);
		pictureField.addActionListener(this);
		friendField.addActionListener(this);
		
		mainCanvas = new FacePamphletCanvas(); 
		mainCanvas.showMessage("Ready");
		add(mainCanvas);
    }
    
  
    /**
     * The FacePamphlet is responsible for detecting when the buttons are
     * clicked or interactors are used. The actionPerformed method specifies
     * how the program responds to those actions.
     */
    public void actionPerformed(ActionEvent e) {
    	
    	String actionTaken = e.getActionCommand(); 
    	String newName = nameField.getText(); 
    	
    	/* action 'Change Status' is triggered
    	 * validate that a profile is selected
    	 */
   
    	if(actionTaken == "Change Status"){
    		if(currentProfile == ""){
    			mainCanvas.showMessage("Select a profile to change status");  
    		} else { 
    			socialNet.getProfile(currentProfile).setStatus(statusField.getText()); 
    			mainCanvas.displayProfile(socialNet.getProfile(currentProfile)); 
    			mainCanvas.showMessage("Status updated to " +socialNet.getProfile(currentProfile).getStatus()); 
    			
    		}
    	} /* action 'Change Picture' triggered
    	   * validate that a profile is selected
    	   */ 
    	  
    	  else if(actionTaken == "Change Picture"){
    		if(currentProfile == ""){
    			appMsg = "Please select a profile"; 
    		} else { 
    			GImage image = null; 
    			try { 
    				image = new GImage(pictureField.getText());
    			} catch(ErrorException ex){
    				appMsg = "Invalide filename"; 
    			} 
    			socialNet.getProfile(currentProfile).setImage(image); 
    		}
    	} /* action 'Add Friend' is triggerd
    	   * validate that a profile is currently selected
    	   *
    	   */
    	
    	  else if(actionTaken == "Add Friend"){
    		if(currentProfile == ""){
    			mainCanvas.showMessage("Select a profile to change status");
    		} else if(!socialNet.containsProfile(friendField.getText())){
    			appMsg = friendField.getText() + " does not exist"; 
    			mainCanvas.showMessage(appMsg); 
    		} else if(socialNet.getProfile(currentProfile).getFriendList().contains(friendField.getText())) { 
    			appMsg = currentProfile + " already has " + friendField.getText() + " as a friend";
    			mainCanvas.showMessage(appMsg); 
    		} else { 
    			socialNet.getProfile(currentProfile).addFriend(friendField.getText()); 
    			socialNet.getProfile(friendField.getText()).addFriend(currentProfile); 
    			mainCanvas.displayProfile(socialNet.getProfile(currentProfile));
    			mainCanvas.showMessage(friendField.getText() + " added as a friend"); 
    		}
    	} 
    	  /* action Add is triggered
    	   *  check that the profile doesn't exist and if profile doesn't exist add the profile
    	   */
    	  else if(actionTaken == "Add"){
    		if(socialNet.containsProfile(newName)){ 
    			mainCanvas.displayProfile(socialNet.getProfile(newName)); 
    			mainCanvas.showMessage("Add: profile for " + newName + " already exists");
    		} else { 
    			socialNet.addProfile(new FacePamphletProfile(newName)); 
    			currentProfile = newName; 
    			mainCanvas.displayProfile(socialNet.getProfile(currentProfile));
    			mainCanvas.showMessage("New profile created");
    
    		}
    	} 
    	  /* action 'Delete selected
    	   * validate that a matching profile exists
    	   */
    	  else if(actionTaken == "Delete"){
    		if(!socialNet.containsProfile(newName)){
    			mainCanvas.showMessage("A profile with the name " + nameField.getText() + " does not exist"); 
    		} else { 
    			socialNet.deleteProfile(newName); 
    			currentProfile = ""; 
    		}
    	} /* action 'Lookup' selected. search for the specified profile, 
    	   * return the profile if found
    	   * return a descriptive message otherwise
    	   */
    	
    	  else if(actionTaken == "Lookup"){
    		if(!socialNet.containsProfile(newName)){
    			mainCanvas.clearProfile(); 
    			mainCanvas.showMessage("A profile with the name " + nameField.getText() + " does not exist"); 
    			currentProfile = ""; 
    		} else { 
    			mainCanvas.displayProfile(socialNet.getProfile(newName)); 
    			mainCanvas.showMessage("Displaying: " + newName); 
    		}
    	}
	}
    
    //Instance variables
    private JTextField statusField; 
    private JTextField pictureField;
    private JTextField friendField;
    private JTextField nameField;
    private FacePamphletDatabase socialNet = new FacePamphletDatabase(); 
    private String currentProfile = "";  
    private String appMsg; 
    private FacePamphletCanvas mainCanvas; 
}
