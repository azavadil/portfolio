/*
 * File: FacePamphletProfile.java
 * ------------------------------
 * This class keeps track of all the information for one profile
 * in the FacePamphlet social network.  Each profile contains a
 * name, an image (which may not always be set), a status (what 
 * the person is currently doing, which may not always be set),
 * and a list of friends.
 */

import acm.graphics.*;
import java.util.*;

public class FacePamphletProfile implements FacePamphletConstants {
	
	/** 
	 * Constructor takes care of any initialization needed for
	 * the profile.
	 */
	public FacePamphletProfile(String name) {
		profileName = name;
		profileStatus = ""; 
	}

	/** getName returns the name associated with the profile. */ 
	public String getName() {
		return profileName;
	}

	/** 
	 * getImage returns the image associated with the profile.  
	 * If there is no image associated with the profile, the method
	 * returns null. */ 
	public GImage getImage() {
		return profileImage;
	}

	/** setImage sets the image associated with the profile. */ 
	public void setImage(GImage image) {
		profileImage = image; 
	}
	
	/** 
	 * getStatus returns the status associated with the profile.
	 * If there is no status associated with the profile, the method
	 * returns the empty string ("").
	 */ 
	public String getStatus() {
		return profileStatus;
	}
	
	/** setStatus sets the status associated with the profile. */ 
	public void setStatus(String status) {
		profileStatus = status; 
	}

	/** 
	 * addFriend adds the named friend to this profile's list of 
	 * friends.  It returns true if the friend's name was not already
	 * in the list of friends for this profile (and the name is added 
	 * to the list).  The method returns false if the given friend name
	 * was already in the list of friends for this profile (in which 
	 * case, the given friend name is not added to the list of friends 
	 * a second time.)
	 */
	public boolean addFriend(String friend) {
		if(profileFriends.contains(friend)){
			return false; 
		} else { 
			profileFriends.add(friend);
			return true; 
		}
	}

	/** 
	 * removeFriend removes the named friend from this profile's list
	 * of friends.  It returns true if the friend's name was in the 
	 * list of friends for this profile (and the name was removed from
	 * the list).  The method returns false if the given friend name 
	 * was not in the list of friends for this profile (in which case,
	 * the given friend name could not be removed.)
	 */
	public boolean removeFriend(String friend) {
		if(!profileFriends.contains(friend)){
			return false; 
		} else {
			profileFriends.remove(friend); 
			return true; 
		}
	}

	/** 
	 * getFriends returns an iterator over the list of friends 
	 * associated with the profile.
	 */ 
	public Iterator<String> getFriends() {
		return profileFriends.iterator(); 
	}
	
	public ArrayList getFriendList() { 
		return profileFriends; 
	}
	
	/** 
	 * toString returns a string representation of the profile.  
	 * This string is of the form: "name (status): list of friends", 
	 * where name and status are set accordingly and the list of 
	 * friends is a comma separated list of the names of all of the 
	 * friends in this profile.
	 * 
	 * For example, in a profile with name "Alice" whose status is 
	 * "coding" and who has friends Don, Chelsea, and Bob, this method 
	 * would return the string: "Alice (coding): Don, Chelsea, Bob"
	 */ 
	public String toString() {
		String result = profileName + " ( " + profileStatus + " ): "; 
		for (String eachFriend:profileFriends){
			result += eachFriend + ", "; 
		}
		return result.substring(0,result.length()-1); 
	}

	//Instance variables
	private String profileName; 
	private GImage profileImage; 
	private String profileStatus;   
	private ArrayList<String> profileFriends = new ArrayList<String>(); 
}
