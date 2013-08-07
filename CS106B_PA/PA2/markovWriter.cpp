#include "genlib.h"
#include <fstream> 
#include <iostream> 
#include "simpio.h" 
#include "map.h"
#include "vector.h" 
#include "random.h"

/* The program reads in a source text, builds an order k Markov model for it, 
 * and generates random output that follows the frequency patterns of the model (i.e. the text) 
 * 
 * The user is prompted for the name of a file to read for the source text 
 * and re-prompted as needed until a valid name is input.
 *
 * The user the inputs the order of Markov model to use (a number from 1 to 10) which 
 * will control what seed length you are working with.
 * 
 * The program then reads in the file character-by-character building up frequency 
 * information (i.e. what character most frequently follows a certain pattern). 
 * 
 * Once the program has read in the sample text, the program outputs 2000 characters of random 
 * text generated from the model. The initial seed is the sequence that 
 * appears most frequently in the source (e.g. if doing an order 4 analysis, the 
 * four-character sequence that is most often repeated in the source is used to 
 * start the random writing). 
 *
 * The initial seed is output and the next character is chosen based on the probabilities of what 
 * followed that seed in the source. The program outputs that character, update the seed, and the 
 * until 2000 characters are generated.
 */


/* getMax
 * ------
 * getMax is a convience function to find the initial seed 
 * (i.e. the most frequently appearing seed)
 * 
 * we already have a map of seed => frequency count 
 * so we just iterate through the map and find the 
 * seed with the highest frequency count
 * 
 * param helper: map of seed => frequency counts
 */ 


string getMax(Map<int> &helper) { 
	int max = 0; 
	string maxKey; 
	
	Map<int>::Iterator itr = helper.iterator(); 
	
	while ( itr.hasNext() ) { 
		string next = itr.next(); 
		if ( helper[next] > max ) { 
			maxKey = next; 
			max = helper[next]; 
		} 
	}
	return maxKey; 
}

/* populateMap
 * -----------
 * populateMap reads in a file character by character and builds up 
 * the frequency infomation. The critical data structure is a  
 * hashmap with seed => vector key/value pairs. 
 * 
 * for example, if we had a markov model of order 4, and we had
 * parsed the first 4 letters of the word 'business', we would have 
 * a seed of 'busi'. The next letter is 'n', so we would add 'n' 
 * to the vector mapped to the seed 'busi'. 
 *
 * In addition to populating the character frequency map,
 * populateMap also returns the initial seed (i.e. the most 
 * frequently appearing seed.
 */ 

string populateMap(Map<Vector<char> > &charFreq, ifstream &in, int markovOrd) { 
	Map<int> helper; 
	string key; 
	
	while (true) { 
		char ch; 
		ch = in.get(); 
		/*assumed to fail because EOF reached*/ 
		if( in.fail() ) break;
		
		/*build up key to initial length*/ 
		if (key.length() < markovOrd ) key += ch; 
		
		/*populate frequencies building up vector*/ 
		if (key.length() == markovOrd) charFreq[key].add(ch); 
		
		/*populate helper to find intial seed
		 * If the key is already in helper, increment by one
		 * If the key is not in helper, insert with an initial value of 1
		 */ 
		if ( helper.containsKey(key) ) { 
			helper[key] += 1; 
		} else { helper.add(key, 1); } 
		
		/* String off the first charater of the key and 
		 * add the new character to create the next seed
		 */ 
		key = key.substr(1, key.length() -1) + ch; 
		
		 
	}
	string res = getMax(helper); 
	return res;
}

/* nextChar
 * --------
 * takes a seed and a map of seed => vector of follwing characters
 * 
 * the vector for each seed contains each letter that followed the seed in the 
 * input text (e.g 'lead' => ['e', 'e', 'e', ' ','b','e',' '])  
 *
 * the program randomly selects one of the characters and returns the character
 *
 */

char nextChar(string seed, Map<Vector<char> > & charFreq) { 
	Vector<char> cVec; 
	int idx; 
	
	cVec = charFreq[seed]; 
	idx = RandomInteger(0,cVec.size() -1); 
	return cVec[idx]; 
}


/* writeText
 * ---------
 * writeText generates random text and writes to an output file 
 * specified by the user.
 * 
 * param seed: string that represents the initial seed
 *
 * param charFreq: the charFreq map represents the frequency that 
 * a character follows a particular seed. 
 * a map of seeds => vector of chars. 
 *
 * param out: file specified by the user to write to 
 */ 

void writeText(string seed, Map<Vector<char> > &charFreq, ofstream &out) { 
	
	// open the output file //
	while (true) { 
		cout << "Continue with write?" << endl; 
		string s = GetLine(); 
		if ( s == "n" ) break; 
		out.open("output.txt"); 
		if ( !out.fail() ) break; 
		cout << "Unable to open output file" << endl; 
		out.clear(); 
	} 
	
	// build up the seed // 
	for (int i = 0; i < seed.length(); i++) { 
		out.put(seed[i]); 
	} 
	
	// generate the output text 
	for (int i = 0; i < 200; i++) { 
		char next = nextChar(seed, charFreq); 
		out.put(next); 
		seed = seed.substr(1, seed.length() -1) + next; 
	} 
	out.close(); 
}

	
	
int main() {
	ifstream filein;
	ofstream fileout; 
	Map<Vector<char> > freqs; 
	int markovOrder = 0; 
	
	
	/*open file*/ 
	while (true) { 
		cout << "Enter Filename: " << endl; 
		string s = GetLine(); 
		filein.open(s.c_str()); 
		if ( !filein.fail() ) break; 
		cout << "File open failed, try again" << endl; 
		filein.clear(); 
	} 
	
	cout << "Enter the Markov Order" << endl; 
	markovOrder = GetInteger(); 
	
	string seed = populateMap(freqs, filein, markovOrder); 
	
	writeText(seed, freqs, fileout);
	return 0; 
}
