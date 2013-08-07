/*
 *  pathfinderStructs.h
 *  PA7
 *
 *  Created by athony on 7/7/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _pathfinderStructs_h
#define _pathfinderStructs_h


/* Implementation notes: data structure
 * ------------------------------------
 * The nodeT and the arcT are the building blocks we use to 
 * make our graph. Fundamentally the graph is dead simple. It's
 * just a vector holding nodeT*. Each nodeT then holds its own 
 * adjacency list of outgoing arcs. 
 */ 

/* Type: coordT
 * ------------
 * Just a simple struct to handle a pair of x,y coordinate values.
 */
struct coordT {
	double x, y;
};



/* Implementation notes: arcT
 * --------------------------- 
 * forward reference of arcT. We use arcT in the definition of 
 * nodeT and we use nodeT in the definition of arcT, so we need 
 * a forward reference for one of the types
 */


struct arcT;		// forward reference


/* Implementation notes: nodeT
 * --------------------------- 
 * we use an adjacency list represenation of the graph nodeT has a 
 * name, an index, a location and a vector of the outgoing arcs 
 */

struct nodeT { 
	string name;
	int indx; 
	coordT location; 
	Vector <arcT *>outgoing; 
};

/* Implementation notes: arcT
 * --------------------------- 
 * param cost: the cost of traversing the arc
 * 
 * param start: the start point of the arc
 * param end: the point the arc takes you to
 */

struct arcT { 
	double cost; 
	nodeT *start, *end; 
}; 



#endif