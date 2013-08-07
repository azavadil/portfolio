

#ifndef _dijkstraSP_h
#define _dijkstraSP_h

#include "map.h"
#include "IdxMinPQ.h"
#include "stack.h"
#include <limits>
#include <iostream>
#include "simpio.h"
#include "pathfinderStructs.h"

class DijkstraSP
{
	public: 
		DijkstraSP(Vector<nodeT *> &G, int source);
		~DijkstraSP(); 
		Stack<arcT> getSP(int destination); 
		static nodeT *getToNodeByIndex(arcT *arc, int index);
	private: 
		double *distTo; 
		arcT *edgeTo; 
		void relax(arcT *arc, int homeNodeIndex, IdxMinPQ &pq); 
		Vector<nodeT *> privateGraph; 
		
};


#endif

		