/*
 * File: Hangman.java
 * ------------------
 * This program will eventually play the Hangman game from
 * Assignment #4.
 */

import acm.graphics.*;
import acm.program.*;
import acm.util.*;

import java.awt.*;



public class Hangman extends ConsoleProgram {
	
	/*instance variable of gameWord*/
	private String gameWord; 
	
	/*instance variable of dsplyStr*/ 
	private String dsplyStr; 
	
	/*instance variable of rgen*/ 
	private RandomGenerator rgen = RandomGenerator.getInstance(); 
	
	/*instance variable of Hangman canvas*/ 
	private HangmanCanvas canvas; 
	
	/*instance variable of counter*/
	private int counter; 
	
	/*instance variable of unguessed*/
	private int unguessed; 
	
	public void init(){
		canvas = new HangmanCanvas();
		add(canvas); 
	}
	
	private final HangmanLexicon hangmanWords = new HangmanLexicon(); 

    public void run() {
		  
    	canvas.reset(); 
    	
    	String guessedLetters = "";
    	
		// select a random word from our array of words
    	int randomIndex = rgen.nextInt(0,hangmanWords.getWordCount()); 
    	gameWord = hangmanWords.getWord(randomIndex); 
    	dsplyStr = makeDisplay(gameWord); 
    	println("Welcome to Hangman"); 
    	
    	// initialize counter to number of guesses
    	counter = 8;
    	unguessed = gameWord.length(); 
    	
		// continue as long as player hasn't won and we aren't out of turns
    	while (counter > 0 && unguessed > 0) { 
    		println("The word now looks like this :" + dsplyStr); 
    		char nextCh = safeRead();
    		guessedLetters += nextCh; 
    		println("Your guess is " + nextCh); 
    		if (gameWord.indexOf(nextCh)!= -1) { 
        		updateDisplay(nextCh);
    		} else {
    			println("Guess is wrong");
    			counter--;
    			canvas.noteIncorrectGuess(nextCh, counter);
    		}
    		canvas.displayWord(dsplyStr, guessedLetters); 
    	}
    	if (unguessed == 0) {
    		println("You Win");
    	} else {
    		println("You Hang");
    	}
    	
    	
	}
    
	
	/* makeDisplay is a convience method to generate the initial 
	 * string to display. e.g. "------" 
	 */ 
    
	private String makeDisplay(String inputStr) { 
    	String result = ""; 
    	for (int i = 0; i < inputStr.length(); i++) { 
    		result += "-";
    	}
    	return result;
    }
    
	/* safeRead is a convience method to safely read in user input
	 * converts the input to upper case letter and validates 
	 * that we have a single character
	 */ 
    
	private char safeRead() { 
    	String str = readLine("You have " + counter + " guesses left");
    	str = str.toUpperCase();
    	while(!isLegalEntry(str)){ 
    		println("Invalid Input");
    		str = readLine("Reenter guess");    	 
    	}	
    	char ch = str.charAt(0); 
    	return ch; 
    }
    
	
	// isLegalEntry returns true if we have a single letter, false otherwise
    
	private boolean isLegalEntry(String str) { 
    	if (str.length() == 1) return true;
    	return false;
    }
       
	/* updateDisplay updates the string to be displayed 
	 * param ch: the character that we're checking to see if it is in the word
	 */ 
	
    private void updateDisplay(char ch) { 
    	int charIndex = 0; 
    	while(charIndex < gameWord.length() && gameWord.indexOf(ch,charIndex) != -1){ 
    		charIndex = gameWord.indexOf(ch,charIndex);
    		String replace = Character.toString(ch); 
    		dsplyStr = dsplyStr.substring(0,charIndex) + replace + dsplyStr.substring(charIndex+1);  
    		unguessed--;
    		charIndex += 1; 
    	}
    }
    		

}
