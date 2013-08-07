#ifndef _idxminPQ_h
#define _idxminPQ_h

#include "genlib.h"

class IdxMinPQ
{
	
public: 
	IdxMinPQ(int n); 
	~IdxMinPQ(); 
	int size(); 
	bool isEmpty(); 
	void enqueue(int i, double distance); 
	int delMin(); 
	void decreaseKey(int i, double distance); 
	bool contains(int i); 
	void printDebuggingInfo();
	int bytesUsed();
	string implementationName();

		
private: 
	int numUsed, numAllocated; 
	int *pq; 
	int *qp;
	double *dist;
	
	void sink(int n); 
	void swim(int n); 
	void swapElems(int a, int b); 
	bool greater(int a, int b); 
};

	
#endif