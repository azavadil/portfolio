#include "IdxMinPQ.h"

IdxMinPQ::IdxMinPQ(int n) { 
	/* 10 + 1, 0 position not used */ 
	pq = new int[n+1];
	qp = new int[n+1];
	dist = new double[n+1]; 
	numAllocated = n; 
	numUsed = 0; 
	for (int i = 0; i <= n; i++) qp[i] = -1; 
} 

IdxMinPQ::~IdxMinPQ() { 	
	delete[] pq; 
	delete[] qp;
	delete[] dist; 
} 

int IdxMinPQ::size() { 
	return numUsed; 
}

bool IdxMinPQ::isEmpty() { 
	return size() == 0; 
} 

void IdxMinPQ::enqueue(int i, double distance) { 
	if(i<0 || i >= numAllocated) Error("IdxMinPQ: Index out of bounds");
	if( contains(i)) Error("IdxMinPQ: priority queue already contains index"); 

	numUsed++;
	qp[i] = numUsed;
	pq[numUsed] = i; 
	dist[i] = distance; 
	/*we don't use 0 index*/ 
	swim(numUsed); 
}

int IdxMinPQ::delMin(){ 
	int top = pq[1]; 
	swapElems(1,numUsed--); 
	sink(1); 
	qp[top] = -1; 
	dist[pq[numUsed+1]] = NULL;
	pq[numUsed+1] = -1;
	return top; 
}

void IdxMinPQ::decreaseKey(int i, double distance){ 
	if (i < 0 || i >= numAllocated) Error("IdxMinPQ.decreaseKey(): IndexOutOfBoundsException"); 
	if (!contains(i)) Error("IdxMinPQ: index is not in the priority queue");
	if (dist[i] <= distance ) Error("IdxMinPQ: Calling decreaseKey() with given argument would not strictly decrease the key");
	dist[i] = distance; 
	swim(qp[i]); 
}

bool IdxMinPQ::contains(int i) { 
	return qp[i] != -1; 
}

void IdxMinPQ::swapElems(int a, int b) { 
	int temp = pq[a]; pq[a] = pq[b]; pq[b] = temp; 
	qp[pq[a]] = a; qp[pq[b]] = b; 
}

bool IdxMinPQ::greater(int a, int b) { 
	return dist[pq[a]] > dist[pq[b]];  
}

void IdxMinPQ::swim(int n){ 
	while ( n > 1 && greater(n/2, n) ) { 
		swapElems(n, n/2); 
		n = n/2; 
	}
}

void IdxMinPQ::sink(int n) { 
	while ( 2*n <= numUsed ) { 
		int a = 2*n;
		/* we want to promote the larger child. If a < a+1, we take a+1 */ 
		if ( a < numUsed && greater(a,a+1) ) a++; 
		if ( !greater(n, a) ) break;
		swapElems(n, a); 
		n = a; 
	}
}

void IdxMinPQ::printDebuggingInfo()
{
	//implement
}

int IdxMinPQ::bytesUsed()
{
	return sizeof(this) + numUsed*sizeof(int); 
}

string IdxMinPQ::implementationName()
{
	return "indexed priority queue"; 
}

