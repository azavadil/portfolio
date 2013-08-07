#ifndef _minPQ_h
#define _minPQ_h

#include "genlib.h"
#include "cmpfn.h" 

template<typename elemType> 
class MinPQ
{
	
	public: 
		MinPQ(int(*cmp)(elemType,elemType) = OperatorCmp); 
		~MinPQ(); 
		int size(); 
		bool isEmpty(); 
		void enqueue(elemType newElem); 
		elemType delMin(); 
		
	private: 
		int numUsed, numAllocated; 
		elemType *arr; 
		int (*cmpFn)(elemType, elemType); 
	
		void sink(int n); 
		void swim(int n); 
		void doubleCapacity(); 
		void swap(int a, int b); 
		bool greater(int a, int b); 
};

#include "MinPQ.cpp"
	
#endif