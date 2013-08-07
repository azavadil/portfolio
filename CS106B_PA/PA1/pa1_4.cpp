/*
 *  File: p4.cpp
 *  ------------------
 *
 *  Created by athony on 3/16/13.
 *
 */

#include "genlib.h"
#include "simpio.h"
#include "strutils.h"
#include <iostream>
#include "map.h" 
#include "vector.h"

/* The Soundex algorithm produces a code for a surname based on the way it sounds, 
 * rather than how it is spelled. The surnames Vaska, Vasque, and Vussky are all 
 * assigned the same Soundex code V200 because of their similar pronunciation, despite 
 * their differences in spellings. The algorithm is only intended to work with English pronunciation
 */

/* makeMap generates the mapping from letter to to a digit
 * used in the soundex translation returning a vector
 */ 

Vector<string> makeMap() { 
	
	string str0 = "aeiouhwy"; 
	string str1 = "bfpv";
	string str2 = "cgjkqsxz";
	string str3 = "dt"; 
	string str4 = "mn"; 
	string str5 = "l"; 
	string str6 = "r"; 
	
	Vector<string> strVec; 
	strVec.add(str0);
	strVec.add(str1); 
	strVec.add(str2); 
	strVec.add(str3); 
	strVec.add(str4); 
	strVec.add(str5); 
	strVec.add(str6);
	
	return strVec; 
	
}	

/* getIntValue takes a character and a vector of strings 
 * and returns the translated value
 * 
 * At this point in the course only vectors have been introduced
 * so we have an inefficient algorithm. 
 * 
 * We iterate through each entry in the vector (each entry is a string)
 * and then we iterate through each character of the entry in that slot
 * looking for a match. 
 * 
 * If there is a match then the vector index is the result for the 
 * given character (i.e. we interpret the position of the array like a hash key)
 */

string getIntValue(char ch, Vector<string> vec) { 
	int res = 0; 
	for (int i = 0; i < vec.size(); i++ ) {
		string temp = vec[i]; 
		if ( temp.find(ch) != -1 ) { 
			res = i;
			break;
		}
	}
	if (res == 0) return ""; 
	else return IntegerToString(res); 
}


/* soundex takes a name and converts the name to its soundex representation
 * 
 * We make 2 passes to generate the output. On the first pass we convert 
 * each character to its soundex representation building up the string. 
 * 
 * On the second pass we remove any duplicates. 
 * 
 * Finally, if the string is less than length 4, we pad the string with zeros. 
 */


string soundex(string name, Vector<string> mapToInt) { 
	string result; 
	result += ConvertToUpperCase(name[0]); 
	
	for (int i = 1; i < name.length(); i++) {
		result += getIntValue(name[i], mapToInt); 
	}
	
	for (int i = result.length(); i > 0; i--) { 
		if (result[i] == result[i-1]) result.erase(i,1); 
	}
	
	if ( result.length() < 4 ) { 
		int len = result.length(); 
		for (int i = 0; i < ( 4 - len ); i++) {
			result += "0"; 
		}
	}
	return result; 
}

int main()
{
	
	string sentinel = "q"; 
	string name; 
	Vector<string> mapping = makeMap(); 
	
	while (true) { 
		cout << "Enter surname: " << endl; 
		name = GetLine(); 
		if (name == sentinel) break; 
		cout << "Soundex code for " << name << " is " << soundex(name, mapping) << endl; 
	}
	return 0;
}
