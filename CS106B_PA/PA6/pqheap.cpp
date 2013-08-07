#include "PQueue.h"
#include "simpio.h" 
#include <iostream> 

/* The file implements a heap backed priority queue class
 *
 * Private section of pqueue.h modidied to match below
 * 
 * int numUsed, numAllocated; 
 * int *arr; 
 * 
 * void sink(int n); 
 * void swim(int n); 
 * void doubleCapacity(); 
 * void swap(int a, int b); 
 * bool less(int a, int b); 
 */ 

/* Implementation notes: PQueue
 * ----------------------------
 * priority queue backed by a heap. Values are stored in an 
 * array with a clever scheme for how we interpret the array. 
 * 
 * The heap property is that if A is a parent node of B then 
 * A is ordered with respect to B. The ordering applies across the heap.
 *
 * For this case we gave a max priority queue so the heap invariant is 
 * that the parent is always greater than or equal to the children nodes
 */ 

PQueue::PQueue() { 
	/* 10 + 1, 0 position not used */ 
	arr = new int[11];
	numAllocated = 10; 
	numUsed = 0; 
} 
	
PQueue::~PQueue() { 	
	delete[] arr; 
} 
	
int PQueue::size() { 
	return numUsed; 
}
	
bool PQueue::isEmpty() { 
	return size() == 0; 
} 
	
void PQueue::enqueue(int newElem) { 
	if(numUsed >= numAllocated / 2) doubleCapacity(); 
		
	arr[1 + numUsed++] = newElem; 
	/* because we don't use 0 index numUsed equals the arr index */ 
	swim(numUsed); 
}
	
int PQueue::dequeueMax(){ 
	int top = arr[1]; 
	arr[1] = arr[numUsed--]; 
	sink(1); 
	return top; 
}

/* Implementation notes: swap
 * --------------------------
 * swap is a private helper method that swaps elements in the 
 * internal array
 */ 
	
void PQueue::swap(int a, int b) { 
	int temp = arr[a]; 
	arr[a] = arr[b];
	arr[b] = temp; 
}

/* Implementation notes: less
 * --------------------------
 * less is a private helper method that compares elements in the 
 * internal array
 */ 


bool PQueue::less(int a, int b) { 
	if (arr[a] < arr[b]) return true; 
	else  return false; 
}

/* Implementation notes: swim
 * --------------------------
 * swim is a private helper method that promotes elements in the 
 * internal array.
 */ 


void PQueue::swim(int n){ 
	while ( n > 1 && less(n/2, n) ) { 
		swap(n, n/2); 
		n = n/2; 
	} 
}

/* Implementation notes: skin
 * --------------------------
 * sink is a private helper method that demotes elements in the 
 * internal array.
 */ 


void PQueue::sink(int n) { 
	while ( 2*n <= numUsed ) { 
		int a = 2*n;
		/* we want to promote the larger child. If a < a+1, we take a+1 */ 
		if ( a < numUsed && less(a,a+1) ) a++; 
		if ( !less(n, a) ) break;
		swap(n, a); 
		n = a; 
	}
}

/* Implementation notes: doubleCapacity
 * --------------------------
 * double is a private helper method. We call doubleCapacity when 
 * the array is filled. 
 */ 


void PQueue::doubleCapacity() { 
	int *bigger = new int[numAllocated*2 + 1]; 
	for (int i = 1; i <= numUsed; i++) { 
		bigger[i] = arr[i];
	}
	delete[] arr; 
	arr = bigger; 
	numAllocated *= 2; 
}
	

string PQueue::implementationName()
{
	return "heap pqueue"; 
}

int PQueue::bytesUsed()
{
	return sizeof(*this) + numUsed * sizeof(int); 
}


void PQueue::printDebuggingInfo()
{
	cout << "------------------ START DEBUG INFO ------------------" << endl;
	cout << "Pqueue contains " << numUsed << " entries" << endl;
	for (int i = 0; i < numUsed; i++) 
		cout << "COMPLETE IMPLEMENTATION" << " ";
	cout << endl;
	cout << "------------------ END DEBUG INFO ------------------" << endl;
}
