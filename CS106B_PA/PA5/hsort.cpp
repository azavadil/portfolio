/*
 *  File: hsort.cpp
 *  ------------------
 *
 *  Created by athony on 3/22/13.
 *
 *  The requirement for this project was to implement a templatized
 *  sorting algorithm of your choice. I chose to implement heapsort
 */

#include "genlib.h"
#include "vector.h"
#include <iostream> 
#include "strutils.h"
#include "cmpfn.h"
#include "random.h"
#include "simpio.h"


/* myless
 * ------
 * myless is a helper function.
 *
 * param i: an integer that represents an index in an array
 * param j: a second integer that represents an index in an array
 * param v: the vector containing the elements to be compared
 * param cmpFn: a function pointer with the comparison function
 */ 

template <typename Type> 
bool myless(int i, int j, Vector<Type> &v, int(cmpFn)(Type one, Type two)){ 
	return cmpFn(v[i-1], v[j-1]) < 0; 
}


/* exch
 * ----
 * exch is the exchange helper function 
 */ 

template <typename Type> 
void exch(Vector<Type> &v, int i, int j ) { 
	Type t = v[i-1]; 
	v[i-1] = v[j-1]; 
	v[j-1] = t; 
}


/* sink
 * ----
 * sink is a helper function that sinks a position maintaining the heap properties
 */
template <typename Type> 
void sink(Vector<Type> &v, int (cmpFn)(Type one, Type two), int k, int N ) { 
		
	while (2*k <= N) { 
		int j = 2*k; 
		if (j < N && myless(j, j+1, v, cmpFn)) j++; 
		if (!myless(k, j, v, cmpFn)) break; 
		exch(v, k, j); 
		k = j; 
	}
}

/* hsort
 * -----
 * hsort is the main sorting routine. hsort takes a vector passed by reference, 
 * a function pointer for the comparison operator for those elements, and 
 * sorts the vector
 */ 

template <typename Type> 
void hsort(Vector<Type> &v, int(cmpFn)(Type one, Type two) = OperatorCmp ){ 
	int N = v.size(); 
	
	for (int k = N/2; k >= 1; k--) { 
		sink(v, cmpFn, k, N); 
	}
	
	while (N > 1) { 
		exch(v, 1, N--); 
		sink(v, cmpFn, 1, N ); 
	} 
}

void test(){
	
	Vector<int> testv; 
	
	for (int i = 0; i < 10000; i++ ) { 
		int r = RandomInteger(0,1000); 
		testv.add(r); 
	}
	
	double start = double (clock()); 
	
	hsort(testv);
	
	double finish = double (clock()); 
	
	double elapsed = finish - start; 
	
	cout << RealToString(elapsed);
	
	for (int i = 9999; i > 9500; i--) { 
		cout << IntegerToString(testv[i]) << endl; 
	}
	
}


int main(){ 
	test();
	return 0; 
}

