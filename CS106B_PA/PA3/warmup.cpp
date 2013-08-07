/*
 *  File: warmup.cpp
 *  ------------------
 *
 *  Created by athony on 3/18/13.
 *
 */

#include "genlib.h"

#include "vector.h"
#include "simpio.h" 
#include <iostream> 

/* recurPIB is a recursive helper function for the 
 * function PrintInBinary
 * 
 * recurPIB uses an accumulator to build up the 
 * the binary representation. 
 * 
 */

void recurPIB(int number, string acc) {
	// base case, print the accumulator s
	if (number == 0) { cout << acc << endl; }
	else { 
		if (number%2 == 0) recurPIB(number/2, "0" + acc); 
		else recurPIB(number/2, "1" + acc); 
	} 
} 

/* PrintInBinary(int num) prints the binary representation for a given integer.
 */

void PrintInBinary(int number){ 
	recurPIB(number, "");
}


/* Subset Sum
 * Given a set of integers and a target number, your goal is to find a subset of 
 * those numbers that sum to that target number. For example, given the numbers 
 * {3, 7, 1, 8, -3} and the target sum 4, the subset {3, 1} sums to 4. On the other hand, 
 * if the target sum were 2, the result is false since there is no subset that sums
 */


/* vSum takes a vector of intergers and 
 * recursively calculates the sum of the 
 * vector elements
 */ 
int vSum(Vector<int> v) { 
	if ( v.isEmpty() ) { return 0; } 
	else { 
		int n = v[0]; 
		v.removeAt(0);
		return n + vSum(v); 
		} 
}  

/* recurCMS is a recursive helper function for canMakeSum
 * 
 * recurCMS generates the subsets by taking the subset 
 * with the first element and the subset without the 
 * first element. 
 */ 

bool recurCMS(Vector<int> sofar, Vector<int> rest, int targetSum) { 
	// base case: if rest is empty check if we match the target sum // 
	if (rest.isEmpty()) { return vSum(sofar) == targetSum; } 
	else {
		int n = rest[0]; 
		rest.removeAt(0); 
		// case that includes element 0
		bool r1 = recurCMS(sofar, rest, targetSum);
		sofar.add(n); 
		// case that excludes element 0
		bool r2 = recurCMS(sofar, rest, targetSum);
		return (r1 || r2); 
	}
}

/* canMakeSum initializes an empty vector and makes 
 * call to the recursive helper function
 */ 

bool canMakeSum(Vector<int> &nums, int targetSum) { 
	Vector<int> emptyVec; 
	return recurCMS(emptyVec, nums, targetSum); 
} 

/* printCMS is a short test of the canMakeSum function
 */ 

void printCMS() {
	Vector<int> test; 
	test.add(1); test.add(2); test.add(3); test.add(4);
	if ( canMakeSum(test, 1) ) cout << "1: true" << endl;
	if ( canMakeSum(test, 2) ) cout << "2: true" << endl;
	if ( canMakeSum(test, 10) ) cout << "10: true" << endl;
	if ( canMakeSum(test, 19) ) cout << "shouldn't print" << endl; 
	if ( canMakeSum(test, 6) ) cout << "6:true" << endl; 
}	




/* recurCCV
 * --------
 * 
 * Recursive helper function for CountCriticalVotes
 * 
 * Base case: if there are no other combinations (i.e. rest.isEmpty())
 * then we check whether the block is a critical vote
 * (i.e. the block + sum of the votes so far > 50% threshold) 
 * 
 * Recursion: recur with each subset (i.e. with rest[0] and 
 * without rest[0])
 */

int recurCCV(Vector<int> sofar, Vector<int> rest, int block, int half) { 
	if ( rest.isEmpty() ) { 
		if (vSum(sofar) + block > half && vSum(sofar) < half) return 1; 
		else return 0; 
	}
	else { 
		Vector<int> sofar_with = sofar; 
		sofar_with.add(rest[0]); 
		rest.removeAt(0); 
		return recurCCV(sofar_with, rest, block, half) + 
				recurCCV(sofar, rest, block, half);
	}
}
	
/* CountCriticalVotes
 * ------------------
 * CountCriticalVotes(Vector<int> & blocks, int blockIndex) is given a vector of 
 * block vote counts and an index within that vector. The function counts the number
 * of election outcomes in which the block at the given index has a critical vote. 
 *
 * Example: The vector {4,2,7,4} would represent a voting system with four blocks of 17 total 
 * votes. A majority of 9 votes is required to win the election. If the block index 
 * is 3, the function counts the critical votes for the last block. If the first two 
 * blocks vote for Alice, and the third for Bob, the last block's vote is critical. 
 * If the first block votes for Alice, and the second two for Bob, then the last 
 * block's vote is irrelevant. The last block's vote is critical in just two of the 
 * eight possible situations. Surprisingly, the smaller 2-vote block at index 1 has 
 * the same count of critical votes, giving it equivalent voting power despite its 
 * smaller size. The large 7-vote block has six critical votes, three times as many 
 * as the other blocks, notwithstanding its modest size relative to the others. 
 * 
 */
int CountCriticalVotes(Vector<int> &blocks, int blockIndex) { 
	int half = vSum(blocks)/2 + 1;
	Vector<int> copy = blocks; 
	int block = copy[blockIndex]; 
	copy.removeAt(blockIndex);
	Vector<int> emptyVec; 
	return recurCCV(emptyVec, copy, block, half); 	
}

/* printCCV
 * --------
 *
 * printCCV is a short test of the CountCriticalVotes function
 */

void printCCV() {
	Vector<int> test, test2;  
	test.add(4); test.add(2); test.add(7); test.add(4);
	test2.add(9); test2.add(9); test2.add(7); test2.add(3); test2.add(1); test2.add(1); 
	for (int i = 0; i < test2.size(); i++ ) { 
		int t = CountCriticalVotes(test2,i);
		cout << "Itr: " << IntegerToString(i) << ", Block: " << IntegerToString(test2[i]) << ", Criticals: "  << IntegerToString(t) << endl;
	}
}
		
int main() {
	printCCV();  
	return 0;
}