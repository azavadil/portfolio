/*
 *  File: pa1_pt1.cpp
 *  ------------------
 *
 *  Created by athony on 3/16/13.
 *
 */

#include "genlib.h"
#include "simpio.h"
#include <iostream>
#include "strutils.h"


/* IsPerfect takes an integer and returns true if the 
 * the input is a perfect number and false otherwise
 *
 * Perfect numbers are numbers that are equal to the sum
 * of their proper divisors
 * 
 * e.g. 6 is a perfect number because 1 + 2 + 3 = 6
 * 
 * The alogorithm iterates through all numbers up to 
 * the midpoint summing the divisors. If the sum of the 
 * divisors equals the number then the number is perfect. 
 */ 


bool IsPerfect(int n) { 
	int sum = 0; 
	
	for (int i = 1; i <= n/2; i++) { 
		if (n%i == 0 ) sum += i; 
    }
	return sum == n; 
}

int main() {
	cout << "Enter ceiling" << endl; 
	int ceiling = GetInteger(); 
	for (int i = 1; i < ceiling; i++) { 
		if (IsPerfect(i)) 
			cout << IntegerToString(i) << endl; 
	} 
	return 0;
}