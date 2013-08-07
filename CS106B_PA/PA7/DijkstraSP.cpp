/* Implementation note: DijkstraSP
 * -------------------------------
 * Dijkstra shortest paths in implemented as a class with 
 * an API that the client can query. 
 * 
 * The class constructor takes a graph and the source node  
 * and then constructs the distTo and edgeTo arrays. 
 * 
 * The client can then query the DijkstraAP instance for the 
 * shortest path from the source to a destination node through 
 * the getSP API. 
 */


#include "DijkstraSP.h"

DijkstraSP::DijkstraSP(Vector<nodeT *> &G, int source) {
	privateGraph = G; 
	distTo = new double[G.size()]; 
	edgeTo = new arcT[G.size()]; 
	IdxMinPQ pq(G.size()); 
	
	
	// set the distanceTo array to positive infinity
	for (int v = 0; v < G.size(); v++) { 
		distTo[v] = std::numeric_limits<double>::infinity();
	}
	distTo[source] = 0; 
		
	pq.enqueue(source, 0.0); 
		
	while(!pq.isEmpty()) { 
		int v = pq.delMin(); 
		nodeT *node = G[v];
		Vector<arcT *> neighbors = node->outgoing; 
		for (int i = 0; i < neighbors.size(); i++){ 
			relax( neighbors[i], v, pq); 
		} 
	} 
	
	
}	


DijkstraSP::~DijkstraSP()
{
	delete[] distTo; 
	delete[] edgeTo; 
}

/* Implementation note: relax
 * --------------------------
 * private member function. If the distance from the current node v, plus the arc cost is
 * better than the current aggregate cost to node w, then we update the aggregate cost to 
 * node w. 
 *
 */  

void DijkstraSP::relax(arcT *arc, int homeNodeIndex, IdxMinPQ &pq){ 
	
	int v = homeNodeIndex; 
	// get the away node 
	int w = (getToNodeByIndex(arc, v))->indx; 
	
	
	if ( distTo[w] > distTo[v] + arc->cost ) { 
		distTo[w] = distTo[v] + arc->cost; 
		edgeTo[w] = *arc; 
		if ( pq.contains(w) ) pq.decreaseKey(w, distTo[w]); 
		else pq.enqueue(w, distTo[w]); 
	}
}

/* Implementation notes: getSP
 * ---------------------------
 * public member function. This is the client API. 
 * The client can query the shortest path from the source to the 
 * specified node
 */ 

Stack<arcT> DijkstraSP::getSP(int destination){
	
	
	Stack<arcT> path;
	 
	while (true){
		/* Implementation note: building the path
		 * --------------------------------------
		 * The path starts at the destination node and we work our way back to 
		 * the origin. We overwrite 'destination' with the  
		 * next step in the path. Hence, when destination == 0, that 
		 * means we've reached the origin 
		 */ 
		if ( distTo[destination] == 0 ) break;
		arcT arc = edgeTo[destination];
		path.push(arc); 
		destination = (getToNodeByIndex(&arc, destination))->indx; 
	}
	return path; 
}
	
/* Implementation note: getToNodeByIndex
 * ----------------------------------------
 * we often know the node that we are coming from and have 
 * an arc with start and end nodes. This is a convenience function
 * to get the 'to' node (i.e. the node this arc takes us 'to').  
 */ 

nodeT *DijkstraSP::getToNodeByIndex(arcT *arc, int index) 
{
	if((arc->start)->indx != index && (arc->end)->indx != index) Error("getToNodeByIndex: index must match either start or end node");
	
	if((arc->end)->indx == index){
		return arc->start; 
	} else {
		return arc->end; 
	}
}
	
		
