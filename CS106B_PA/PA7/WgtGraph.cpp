/*
 *  WgtGraph.cpp
 *  PA7
 *
 *  Created by athony on 7/7/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include "WgtGraph.h"


WgtGraph::WgtGraph()
{
	//nothing to do here. We're using a vector which is declared as a private member variable
}

WgtGraph::~WgtGraph()
{
}

void WgtGraph::addNode(nodeT *node)
{
	/* Implementation note: validate that the node is in correct order
	 * nodes should only be added once and in ascending order so that the 
	 * index number of the node should match the node's position in the vector
	 */ 
	if(G.size() + 1 != node->indx) Error("WgtGraph::addNode: node index is out of order"); 
	
	G.add(node); 
}

void WgtGraph::addArc(arcT *arc)
{
	
	G[(arc->start)->indx].outgoing.add(arc);
	G[(arc->end)->indx].outgoing.add(arc); 
}


Vector<arcT *>WgtGraph::outgoing(int nodeIndex)
{
	return G[nodeIndex]; 
}

int WgtGraph::size()
{
	return G.size();
}



/* Implementation note: getToNodeByIndex
 * ----------------------------------------
 * we often know the node that we are coming from and have 
 * an arc with start and end nodes. This is a convenience function
 * to get the 'to' node (i.e. the node this arc takes us 'to').  
 */ 

nodeT *WgtGraph::getToNodeByIndex(arcT *, int index) 
{
	if((arc->start)->indx != index && (arc->end)->indx != index) Error("getToNodeByIndex: index must match either start or end node");
	
	if((arc->end)->indx == index){
		return arc->start; 
	} else {
		return arc->end; 
	}
}
