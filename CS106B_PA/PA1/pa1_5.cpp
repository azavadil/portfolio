/*
 *  File: p5.cpp
 *  ------------------
 *
 *  Created by athony on 3/16/13.
 *
 */

#include "genlib.h"
#include "strutils.h"
#include <iostream>
#include "Vector.h"
#include <fstream> 
#include "simpio.h"


/* The program reads in a data file and generates a histogram of the numbers. 
 * The program first asks the user for the name of the data file containing the score information. 
 * The data file contains a list of scores, one per line. The program reads the scores from the file, 
 * tallies them into buckets, and prints a histogram. 
 */

/* makePrefix generates the prefix for the histogram
 * e.g. 
 * 0-9:
 * 10-19:
 * ... 
 */ 

string makePrefix(int n) { 
	string output; 
	output = IntegerToString(n * 10) + "-" + IntegerToString(n*10+9) + ": "; 
	return output;
}

/* printVector takes a vector as input and prints
 * in specified format
 */ 


void printVector(Vector<int> table) { 
	for (int i = 0; i < 10; i++) { 
		string result; 
		result += makePrefix(i);
		for (int j = 0; j < table[i]; j++) { 
			result += "X"; 
		}
		cout << result << endl;	
	}
}

/* makeHistogram reads in a file line by line and 
 * populates a Vector<int> 
 *
 * The algorithm uses integer division to truncate scores
 * (e.g. 87/10 becomes 8) which we can then use to index
 * into the vector
 */ 

void makeHistogram(Vector<int> &histogram, ifstream &in) { 
	while (true) { 
		string line; 
		getline(in, line); 
		if (in.fail()) break; 
		histogram[StringToInteger(line)/10] += 1;   
	}
}

int main() 
{
	ifstream in; 
	while (true) { 
		cout << "Enter Filename: "; 
		string s = GetLine(); 
		in.open(s.c_str()); 
		if (!in.fail()) break; 
		cout << "File open failed, try again" << endl;
		in.clear(); 
	} 
	
	/*initialize the histogram to a 10 element vector of zeros*/  
	Vector<int> histogram; 
	for (int i = 0; i < 10; i++) { 
		histogram.add(0); 
	} 
	
	
	makeHistogram(histogram, in);
	
	printVector(histogram); 
	
	return 0; 
}

	