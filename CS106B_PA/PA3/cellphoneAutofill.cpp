/*
 *  File: p3_1.cpp
 *  ------------------
 *
 *  Created by athony on 3/18/13.
 *
 */

#include "genlib.h"

#include "vector.h"
#include "simpio.h" 
#include "strutils.h"
#include <iostream>
#include "map.h"
#include "vector.h" 
#include "lexicon.h" 


/* This program mimics the behavior of autofill software for cellphones. 
 * The program takes the numbers that have been entered so far and generates 
 * the possible completions for the permutations of those numbers
 */ 


/* printArg
 * --------
 * printArg takes a map of string => Vector<string> pairs
 * and prints the strings in the vector
 *
 */ 

void printArg(Map<Vector<string> > mapArg){ 
	Map<Vector<string> > testMap = mapArg; 
	
	Map<Vector<string> >::Iterator itr = testMap.iterator(); 
	
	while ( itr.hasNext() ) { 
		Vector<string> nextVec;
		string key = itr.next(); 
		nextVec = testMap[key]; 
		
		cout << "Key: " << key << endl; 
		
		for (int i = 0; i < nextVec.size(); i++) {  
			string toscreen = nextVec[i];
			cout << toscreen << endl; 
		}
	}
}

/* mapFactory
 * ----------
 * mapFactory is a convience function that maps the 
 * numeric keys on a cell phone to certain 
 * alpha characters
 */ 


Map<Vector<string> > mapFactory() { 
	Vector<string> b1,b2,b3,b4,b5,b6,b7,b8,b9; 
	
	b2.add("a");b2.add("b");b2.add("c");
	b3.add("d");b3.add("e");b3.add("f");
	b4.add("g");b4.add("h");b4.add("i"); 
	b5.add("j");b5.add("k");b5.add("l");
    b6.add("m");b6.add("n");b6.add("o"); 
	b7.add("p");b7.add("r");b7.add("s");b7.add("q");
	b8.add("t");b8.add("u");b8.add("v");
	b9.add("w");b9.add("y"); b9.add("z"); 
								   
	Map<Vector<string> > output; 
								   
	output["02"] = b2;
	output["03"] = b3;
	output["04"] = b4;
	output["05"] = b5;
	output["06"] = b6;
	output["07"] = b7;
	output["08"] = b8;
	output["09"] = b9;
								   
	return output; 
}

/* makeAlpha
 * ---------
 * makeAlpha is a convience function to generate
 * a vector with each character in the alphabet
 */ 


Vector<char> makeAlpha() { 
	string alpha = string("abcdefghijklmnopqrstuvwxyz");
	Vector<char> output; 
	
	for (int i = 0; i < alpha.length(); i++) { 
		output.add(alpha[i]); 
	}
	return output; 
}
	
/* recurP
 * ------
 * recurP is a recursive helper function for permutations 
 * 
 *
 */

								   
void recurP(string sofar, Vector<string> rest, Vector<string> &acc, Map<Vector<string> > &keypad, Lexicon &lex) { 
	
	/* Base case: rest is empty (i.e. there are no more numbers)
	 * add the built up string to the accumulator sofar 
	 */ 
	if ( rest.isEmpty() ) { 
		acc.add(sofar); 
	} else { 
		
		Vector<string> copy = rest; 
		copy.removeAt(0);
			
		// extract the key that was pressed
		string keyPressed = rest[0];
		
		// get the vector with all possible alpha chars that maps to the key
		Vector<string> letters = keypad[keyPressed]; 
								   
		/* for each letter, append it to our word so far
		 * if the word is a viable prefix, recur
		 */ 
		for (int j = 0; j < letters.size(); j++) { 
			string next = sofar + letters[j]; 
			if ( lex.containsPrefix(next) ) { 
				recurP(next, copy, acc, keypad, lex);
			}
		}
	}
}

/* permutations
 * ------------
 * permutations takes the number that has been keyed in 
 * and generates all possible permutations of words that 
 * could be created by that number combination
 *
 * permutations calls recurP to do the heavy lifting
 */ 

Vector<string> permutations(int keyedIn, Lexicon lex) { 
	string numEntered = IntegerToString(keyedIn); 
	Vector<string> nums; 
	for (int i = 0; i < numEntered.length(); i++) { 
		string s = string("0") + numEntered[i]; 
		nums.add(s); 
	}
	
	Map<Vector<string> > keypad = mapFactory(); 
	Vector<string> acc; 
	string start; 
	recurP(start, nums, acc, keypad,lex);
	
	return acc; 		
}

/* printPerms
 * ----------
 * printPerms is a quick test of the permutations functions
 * hardcoded to validate that permutations generates the 
 * correct output for the input 72547
 */ 

void printPerms(Lexicon &lex) { 
	
	Vector<string> output = permutations(72547, lex); 
	
	
	for (int i = 0; i < output.size(); i++) {
		if (lex.containsPrefix( output[i] ) )  { 
			cout << output[i] << endl; 
		}
	}
	
}


/* recurPrefix
 * -----------
 * recurPrefix is a recursive helper function for primary function ListCompletions
 *
 * param lex: a lexicon with methods containsWord and containsPrefix
 * param sofar: the sequence of letters that has been built up so far
 * param alpha: a char vector with each letter in the alphabet
 * param acc: an acculator that we populate the possible completions
 */ 


void recurPrefix(Lexicon &lex, string sofar, Vector<char> &alpha, Vector<string> &acc){ 
	
	if ( alpha.isEmpty() ) return; 
	else { 
		
		// iterate through each letter in the alphabet
		for (int i = 0; i < alpha.size(); i++) { 
			string next = sofar + alpha[i];
			
			// if we found a word, add it to the accumulator
			if ( lex.containsWord(next) ) acc.add(next); 
			
			// if the letter sequence is a prefix to some word, recur
			if ( lex.containsPrefix(next) ) recurPrefix(lex, next, alpha, acc); 
		}
	}
}

/* ListCompletions
 * ---------------
 * ListCompletions prints all the words 
 * from the lexicon that can be formed by extending the given digit sequence.
 * 
 * param lex: a lexicon
 * param alphabet: vector with each character of the alphabet
 * param prefixes: all possible prefixes that can be formed by the number entered
 * param completions: an empty vector to be populated with completions
 */


void ListCompletions(Lexicon lex, Vector<char> &alphabet, Vector<string> &prefixes, Vector<string> &completions){
	
	// iterate through each prefix and call recurPrefix
	for (int i = 0; i < prefixes.size(); i++ ) { 
		string next = prefixes[i]; 
		recurPrefix(lex, next, alphabet, completions); 
	}
}

/* printTest
 * ---------
 * printTest is does a quick check of the ListCompletions function
 */ 

		
void printTest(int n){ 

	Lexicon lex("lexicon.dat"); 
	Vector<char> alphabet = makeAlpha(); 
	Vector<string> prefixes = permutations(n, lex); 
	Vector<string> completions; 
	
	
	ListCompletions(lex, alphabet, prefixes, completions); 
	
	for (int i = 0; i < completions.size(); i++) { 
		cout << completions[i] << endl; 
	}
}
	

int main(){ 
	
	cout << "enter number" << endl; 
	int num = GetInteger(); 
	
	printTest(num); 
	return 0; 
}

