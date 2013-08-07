/*
 * File: NameSurferEntry.java
 * --------------------------
 * This class represents a single entry in the database.  Each
 * NameSurferEntry contains a name and a list giving the popularity
 * of that name for each decade stretching back to 1900.
 */

import acm.util.*;
import java.util.*;

public class NameSurferEntry implements NameSurferConstants {

/* Constructor: NameSurferEntry(line) */
/**
 * Creates a new NameSurferEntry from a data line as it appears
 * in the data file.  Each line begins with the name, which is
 * followed by integers giving the rank of that name for each
 * decade.
 */
	public NameSurferEntry(String line) {
		entry = parseLine(line);
	}

/* Method: getName() */
/**
 * Returns the name associated with this entry.
 */
	public String getName() {
		return entry.getName();
	}

/* Method: getRank(decade) */
/**
 * Returns the rank associated with an entry for a particular
 * decade.  The decade value is an integer indicating how many
 * decades have passed since the first year in the database,
 * which is given by the constant START_DECADE.  If a name does
 * not appear in a decade, the rank value is 0.
 */
	public int getRank(int decade) {
		int[] ranks = entry.getRanks();
		return ranks[(decade-START_DECADE)/10]; 
	}
	
	
	/* parseLine takes a line in the format: name rank1 rank2 rank3 ... rank11 
	 * parseLine splits the line on whitespace filling in an array with 
	 * the resulting strings. 
	 * 
	 * The name and the rankings array are used to initialize a new parseObj
	 * (i.e. an object representation of that line in 'names-data.txt' file) 
	 */
	
	private parseObj parseLine(String line){
		int lastEndPT = line.indexOf(" "); 
		
		/* strip the name off the front of the string, 
		 * lowercase to enable case insensitive matching
		 * and assign to entryName
		 */
		String entryName = line.substring(0,lastEndPT).toLowerCase(); 
		
		
		// parse the remainder of the string filling in the array
		String[] sArr = new String[11]; 
		int index = 0; 
		while (true){
			int nextEndPT = line.indexOf(" ",lastEndPT + 1); 
			if(nextEndPT == -1){
				sArr[index] = line.substring(lastEndPT +1);
				break; 
			}
			sArr[index] = line.substring(lastEndPT + 1, nextEndPT); 
			lastEndPT = nextEndPT; 
			index += 1; 
		}
		
		int[] yrRanks = new int[11];
		for(int i = 0; i < 11; i++){
			yrRanks[i] = Integer.parseInt(sArr[i]); 
		}
		
		return (new parseObj(entryName,yrRanks));  
	}

/* Method: toString() */
/**
 * Returns a string that makes it easy to see the value of a
 * NameSurferEntry.
 */
	public String toString() {
		
		String sRanks = "";
		for (int rank: entry.getRanks()){ 
			sRanks += String.valueOf(rank) + " ";
		}
		return entry.getName() + " [" + sRanks + "]";
	}
	
	// instance variables 
	private parseObj entry; 
	
}

