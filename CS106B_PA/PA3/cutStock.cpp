/*
 *  File: p3_6.cpp
 *  ------------------
 *
 *  Created by athony on 3/19/13.
 *
 */

#include "genlib.h"
#include "vector.h" 
#include "strutils.h" 
#include <iostream> 

/* The function CutStock(Vector<int> & requests, int stockLength) takes 
 * a vector of the lengths needed and the stock pipe length. It returns the 
 * minimum number of stock pipes needed to service all requests in the vector. 
 * 
 * For example, if the vector contains {4,3,4,1,7,8} and the stock pipe length is 10, 
 * you divide three stock pipes as follows: {4,4,1} {3,7} {8} and have two small 
 * leftover remnants. There are other possible arrangements that also fit into 
 * three stock pipes, but it cannot be done with fewer.
 */

/* vSum
 * ----
 * vSum is recusive fuction that sums a vector
 */ 

int vSum(Vector<int> v) { 
	if ( v.isEmpty() ) return 0; 
	else { 
		int head = v[0];
		Vector<int> tail = v; 
		tail.removeAt(0); 
		return head + vSum(tail); 
	}
}

/* removeReqs
 * ----------
 * removeReqs is a convience function that takes two vectors
 * and removes the entries in the remove vector from the start vector
 * 
 * For example, if start = [1,2,3,4,5] and remove = [2,4], 
 * removeReqs will remove elements 2 and 4 from vector start
 */ 
 

void removeReqs(Vector<int> &start, Vector<int> remove){ 
	
	if (remove.isEmpty()) return; 
	
	else {
		int toDelete = remove[0];
		remove.removeAt(0); 
		for (int j = 0; j < start.size(); j++) { 
			if (start[j] == toDelete) { 
				start.removeAt(j); 
				break; 
			}
		}
		removeReqs(start, remove);
	}
}

/* subsets
 * -------
 * subsets recursively generates all the subsets of a vector
 * using the classic subset pattern
 */ 

void subsets(Vector<int> sofar, Vector<int> rest, Vector<Vector<int> > &acc) { 
	if ( rest.isEmpty() ) acc.add(sofar); 
	else { 
		for (int i = 0; i < rest.size(); i++) { 
			Vector<int> copy = rest; 
			copy.removeAt(i); 
			
			Vector<int> next = sofar; 
			next.add(rest[i]); 
			
			subsets(next, copy, acc); 
			subsets(sofar, copy, acc); 
		}
	}
}

/* printSubsets 
 * ------------
 * printSubsets is used for testing subsets functions
 */ 

void printSubsets(Vector<Vector<int> > ss) { 
	
	for (int i = 0; i < ss.size(); i++) { 
		Vector<int> next = ss[i];
		string acc; 
		for (int j = 0; j < next.size(); j++) { 
			acc += IntegerToString(next[j]); 
		} 
		cout << "Subset: " << acc << endl; 
	}
}

/* recurCut
 * --------
 * recurCut is a recursive helper function for cutStock. 
 * 
 * The recurCut algorithm first calculates the possible subsets
 * based on the requested lengths populating the successors Vectors
 * with the possible subsets. 
 * 
 * recurCut then steps through the various subsets to find the 
 * the combination that will maximize the use of the stockLength
 *
 * Once the best subset is found, the requested lengths contained in 
 * subset are removed from the set of requested lengths. 
 * 
 * Recur on the remaining requested lengths. 
 */ 


int recurCut(Vector<int> reqs, int stockLength, int acc) { 
	
	if ( reqs.isEmpty() ) return acc; 
	
	else{ 
		Vector<Vector<int> > successors; 
		Vector<int> sofar;
		subsets(sofar, reqs, successors); 
		
		int bestSoFar = 0; 
		int bestIdx = 0; 
		
		for (int i = 0; i < successors.size(); i++) { 
			Vector<int> next = successors[i]; 
			int total = vSum(next); 
			if (total > bestSoFar && total <= stockLength) { 
				bestSoFar = total; 
				bestIdx = i; 
			}
		}
		
		Vector<int> toRemove = successors[bestIdx]; 
		
		Vector<int> remaining = reqs; 
		removeReqs(remaining, toRemove); 
		return recurCut(remaining, stockLength, acc + 1); 
	}
}

/* cutStock
 * --------
 * cutStock calculates the minimum number of the stockLength 
 * pipes we need to craft the set of requested lengths. 
 * 
 * cutStock sets the accumulator to zero and then calls
 * recurCut to do the heavy liftings
 */ 

int cutStock(Vector<int> &requests, int stockLength) {
	
	int acc = 0; 
	
	return recurCut(requests, stockLength, acc); 
}

void test1(){ 
	
	Vector<int> test; 
	test.add(4); test.add(3); test.add(4); test.add(1); test.add(7); test.add(8); 
	
	
	Vector<int> remove; 
	remove.add(3); remove.add(1); remove.add(7); 
	
	removeReqs(test, remove); 
	
	for (int i = 0; i < test.size(); i++) { 
		cout << "test[i]: " << i << ", " << IntegerToString(test[i]) << endl; 
	}
}

void test2(){ 
	
	Vector<int> test; 
	test.add(4); test.add(3); test.add(4); test.add(1); test.add(7); test.add(8); 
	
	Vector<int> startEmpty; 
	
	Vector<Vector<int> > accum; 
	
	subsets(startEmpty, test, accum); 
	
	printSubsets(accum); 
	
}

void test3() {

	Vector<int> test; 
	test.add(4); test.add(3); test.add(4); test.add(1); test.add(7); test.add(8); 
	
	
	int res = cutStock(test, 10); 
	
	cout << "Number of stock pipes: " << IntegerToString(res) << endl; 

}

int main(){ 
	test3(); 
	
	return 0; 
} 

