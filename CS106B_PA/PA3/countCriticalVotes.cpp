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