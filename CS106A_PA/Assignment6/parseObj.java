

/* parseObj is convience class to keep a 
 * a name and the associated ranks for that 
 * together
 */ 

public class parseObj {
	
	//constructor
	public parseObj(String name, int[] ranks){
		parseName = name; 
		parseRanks = ranks; 
	}
	
	public String getName(){
		return parseName; 
	}
	
	public int[] getRanks(){
		return parseRanks; 
	}
	
	public String toString() { 
		return parseName; 
	}
	
	//instance variables; 
	private String parseName; 
	private int[] parseRanks;

}
