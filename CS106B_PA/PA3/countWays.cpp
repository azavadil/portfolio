/*
 *  File: countWays.cpp
 *  ------------------
 *
 *  Created by athony on 7/1/13.
 *
 */

#include "genlib.h"

/* countWays
 * ---------
 * countWays(int numStairs) that takes a positive numStairs value and 
 * returns the number of different ways to climb a staircase of that 
 * height taking strides of one or two stairs at a time.
 * 
 * 
 */

int countWays(int numStairs) { 
	if ( numStairs < 3 ) return numStairs; 
	else return countWays(numStairs-1) + countWays(numStairs-2); 
}

void printStairs() {
	while (true) { 
		cout << "Number of Stairs: " << endl; 
		int n = GetInteger(); 
		if (n == -1) break;
		cout << IntegerToString(countWays(n)) << endl; 
	}
}
