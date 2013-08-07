/* Implementation note: KruskalMST
 * -------------------------------
 * Kruskal's minimun spanning tree algorithm is implemented as a class with 
 * an API that the client can query. 
 * 
 * The class constructor takes a graph as paramater.
 * 
 * The client can then query the KruskalMST instance for the 
 * minimun spanning tree through the getMST member function. 
 */

#include "KruskalMST.h"



KruskalMST::KruskalMST(Vector<nodeT *> &G, int(*cmp)(arcT *, arcT *)){
	
	int numVertices = G.size(); 
	MinPQ<arcT *> pq(cmp); 
	
	
	for (int i = 0; i < G.size(); i++) { 
		// get the adjacency list
		nodeT *currNode = G.getAt(i); 
		
		// add all the arcs in the adjacenecy list
		for(int j = 0; j < (currNode->outgoing).size(); j++) {
			pq.enqueue(currNode->outgoing[j]);
		}
	}
		
	UF uf(G.size());
	
	while ( !pq.isEmpty() && mst.size() < numVertices - 1 ) { 
		arcT *arc = pq.delMin();
		int v = (arc->start)->indx; 
		int w = (arc->end)->indx; 
		if( !uf.connected(v,w)  ) { 
			uf.unionElems(v,w); 
			mst.add(arc); 
		}
	}
}

KruskalMST::~KruskalMST()
{
}

Vector<arcT *> KruskalMST::getMST(){
	return mst; 
}
	
		
		