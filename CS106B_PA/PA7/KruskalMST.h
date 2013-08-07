

#ifndef _kruskalMST_h
#define _kruskalMST_h

#include "genlib.h"
#include "vector.h" 
#include "map.h" 
#include "MinPQ.h"
#include "set.h"
#include <iostream>
#include "UF.h"
#include "pathfinderStructs.h"

class KruskalMST
{
	public: 
		KruskalMST(Vector<nodeT *> &G, int(cmp)(arcT *, arcT *)); 
		~KruskalMST(); 
		Vector<arcT *> getMST(); 
		
	private:
		Vector<arcT *> mst; 
};

#endif 