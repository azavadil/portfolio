#include "MinPQ.h"


/* Implementation note: default argument defined in .h file */
template <typename elemType> 
MinPQ<elemType>::MinPQ(int (*cmp)(elemType,elemType) /*(= OperatorCmp*/ ) { 
	/* 10 + 1, 0 position not used */ 
	arr = new elemType[11];
	numAllocated = 10; 
	numUsed = 0; 
	cmpFn = cmp; 
} 

template <typename elemType> 
MinPQ<elemType>::~MinPQ() { 	
	delete[] arr; 
} 

template <typename elemType> 
int MinPQ<elemType>::size() { 
	return numUsed; 
}

template <typename elemType>
bool MinPQ<elemType>::isEmpty() { 
	return size() == 0; 
} 

template <typename elemType>
void MinPQ<elemType>::enqueue(elemType newElem) { 
	if(numUsed >= numAllocated / 2) doubleCapacity(); 
		
	arr[1 + numUsed++] = newElem; 
	/* because we don't use 0 index numUsed equals the arr index */ 
	swim(numUsed); 
}

template <typename elemType>
elemType MinPQ<elemType>::delMin(){ 
	elemType top = arr[1]; 
	arr[1] = arr[numUsed--]; 
	sink(1); 
	return top; 
}

/* Private member function: swap
 * --------------------------
 * swaps elements in the internal array
 */ 
	
template <typename elemType>
void MinPQ<elemType>::swap(int a, int b) { 
	elemType temp = arr[a]; 
	arr[a] = arr[b];
	arr[b] = temp; 
}

/* Private member function: greater
 * --------------------------
 * compares elements in the internal array
 */ 

template <typename elemType>
bool MinPQ<elemType>::greater(int a, int b) { 
	if (cmpFn(arr[a], arr[b]) > 0) return true; 
	else return false; 
}

/* Private member function: swim
 * --------------------------
 * promotes elements in the internal array until the heap property is restored
 */ 

template <typename elemType>
void MinPQ<elemType>::swim(int n){ 
	while ( n > 1 && greater(n/2, n) ) { 
		swap(n, n/2); 
		n = n/2; 
	} 
}

/* Private member function: sink
 * -----------------------------
 * Doubles the current capacity of the queue's internal storage,
 * copying all existing values.
 */


template <typename elemType>
void MinPQ<elemType>::sink(int n) { 
	while ( 2*n <= numUsed ) { 
		int a = 2*n;
		/* we want to promote the larger child. If a < a+1, we take a+1 */ 
		if ( a < numUsed && greater(a,a+1) ) a++; 
		if ( !greater(n, a) ) break;
		swap(n, a); 
		n = a; 
	}
}

/* Private member function: doubleCapacity
 * -----------------------------------------
 * Doubles the current capacity of the queue's internal storage,
 * copying all existing values.
 */

template <typename elemType>
void MinPQ<elemType>::doubleCapacity() { 
	elemType *bigger = new elemType[numAllocated*2 + 1]; 
	for (int i = 1; i <= numUsed; i++) { 
		bigger[i] = arr[i];
	}
	delete[] arr; 
	arr = bigger; 
	numAllocated *= 2; 
}

