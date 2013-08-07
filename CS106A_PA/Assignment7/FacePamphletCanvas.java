/*
 * File: FacePamphletCanvas.java
 * -----------------------------
 * This class represents the canvas on which the profiles in the social
 * network are displayed.  NOTE: This class does NOT need to update the
 * display when the window is resized.
 */


import acm.graphics.*;
import java.awt.*;
import java.util.*;

public class FacePamphletCanvas extends GCanvas 
					implements FacePamphletConstants {
	
	/** 
	 * Constructor
	 * This method takes care of any initialization needed for 
	 * the display
	 */
	public FacePamphletCanvas() {
	}

	
	/** 
	 * showMessage displays a message string near the bottom of the 
	 * canvas.  Every time this method is called, the previously 
	 * displayed message (if any) is replaced by the new message text 
	 * passed in.
	 */
	public void showMessage(String msg) {
		canvasAppMsg = msg; 
		GLabel labelAppMsg = new GLabel(canvasAppMsg);
		if(getElementAt(xMsg,yMsg) != null){ 
			remove(getElementAt(xMsg,yMsg)); 
		}
		xMsg = (getWidth() - labelAppMsg.getWidth())/2; 
		yMsg = getHeight() - BOTTOM_MESSAGE_MARGIN; 
		labelAppMsg.setLocation(xMsg,yMsg);
		add(labelAppMsg);
	}
	
	
	/** 
	 * displayProfile displays the given profile on the canvas.  The 
	 * canvas is first cleared of all existing items (including 
	 * messages displayed near the bottom of the screen) and then the 
	 * given profile is displayed.  The profile display includes the 
	 * name of the user from the profile, the corresponding image 
	 * (or an indication that an image does not exist), the status of
	 * the user, and a list of the user's friends in the social network.
	 */
	public void displayProfile(FacePamphletProfile profile) {
		removeAll(); 
		GLabel header1 = new GLabel(profile.getName(),LEFT_MARGIN, 24 + TOP_MARGIN); 
		header1.setFont(PROFILE_NAME_FONT); 
		header1.setColor(Color.BLUE); 
		add(header1); 
		
		double yImage = 2.0 * TOP_MARGIN + 24; 
		//getImage().getImage(),first call is the profile class getImage, 2nd call is GImage class getImage()
		if(profile.getImage() == null){ 
			GRect blankRect = new GRect(LEFT_MARGIN,yImage,IMAGE_WIDTH,IMAGE_HEIGHT);
			GLabel emptyLabel = new GLabel("No Image"); 
			emptyLabel.setFont(PROFILE_IMAGE_FONT); 
			emptyLabel.setLocation(LEFT_MARGIN + (IMAGE_WIDTH - emptyLabel.getWidth())/2,yImage + IMAGE_HEIGHT/2); 
			add(emptyLabel); 
			add(blankRect); 
		} else { 
			GImage profileImage = new GImage(profile.getImage().getImage(),LEFT_MARGIN,yImage);
			profileImage.setSize(IMAGE_WIDTH,IMAGE_HEIGHT); 
			add(profileImage);
		}
		
		double yStatus = yImage + IMAGE_HEIGHT + TOP_MARGIN; 
		GLabel statusLabel = new GLabel(profile.getStatus(),LEFT_MARGIN,yStatus);
		statusLabel.setFont(PROFILE_STATUS_FONT); 
		add(statusLabel);
		
		GLabel friendHeader = new GLabel("Friends"); 
		friendHeader.setFont(PROFILE_FRIEND_LABEL_FONT);
		double xRightCol = (getWidth() - friendHeader.getWidth())/2;
		friendHeader.setLocation(xRightCol,yImage); 
		add(friendHeader); 
		
		//Display the friends of the currently selected profile
		double xSpacer = 10; 
		int count = 1; 
		//check to see if the profile's friendlist is size zero
		if(profile.getFriendList().size() == 0){
		} else { 
		//if the friendlist is non-zero, iterate through the friends
			Iterator<String> friendIter = profile.getFriends(); 
			while(friendIter.hasNext()){
				GLabel nextLabel = new GLabel(friendIter.next());	
				nextLabel.setFont(PROFILE_FRIEND_FONT); 
				nextLabel.setLocation(xRightCol,yImage + (xSpacer+nextLabel.getHeight())*count); 
				add(nextLabel); 
				count += 1;    
			}
		}	
	}
	
	public void clearProfile() {
		removeAll(); 
	}

	private String canvasAppMsg;
	private double xMsg = 0; 
	private double yMsg = 0; 
}
