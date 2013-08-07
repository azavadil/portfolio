/*
 *  WgtGraph.h
 *  PA7
 *
 *  Created by athony on 7/7/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _wgtgraph_h
#define _wgtgraph_h

#include "vector.h"
#include "genlib.h"

class WgtGraph
{
	
public: 		
	WgtGraph();
	~WgtGraph();
	void addArc(arcT *arc);
	void addNode(nodeT *node)
	Vector<arcT *>outgoing(int nodeIndex); 
	int size(); 
	static nodeT *getToNodeByIndex(arcT *, int index); 

private: 
	Vector<nodeT *> G; 
}
	


#endif