/*
 *  File: p2.cpp
 *  ------------------
 *
 *  Created by athony on 3/17/13.
 *
 */

#include "genlib.h"
#include "simpio.h" 
#include "map.h" 
#include "grid.h" 
#include "maze.h" 
#include "vector.h"
#include "random.h"
#include <iostream>
#include "strutils.h"
#include "graphics.h"
#include "extgraph.h"
#include "stack.h"
#include "queue.h"
#include "set.h" 


/* nextLocation
 * ------------
 * nextLocation returns a randomly selected successor location
 * to the current location. 
 *
 * param mz: a maze that defines the possible locations
 * 
 * param currLoc: the current location in the maze
 *
 */ 

pointT nextLocation(Maze &mz, pointT currLoc) { 
	Vector<pointT> dirs; 
	pointT d1, d2, d3, d4; 
		   
	/* possible movements: up, down, left, right */ 
	d1.row = 1; d1.col = 0;
	d2.row = -1; d2.col = 0; 
	d3.row = 0; d3.col = 1; 
	d4.row = 0; d4.col = -1; 
		   
	dirs.add(d1); dirs.add(d2); dirs.add(d3); dirs.add(d4); 
	pointT newLoc; 
	
	bool found = false; 
	
	while (!found) { 
		int idx = RandomInteger(0,3); 
		pointT delta = dirs[idx]; 
		newLoc.row = 0; newLoc.col = 0; 
			   
		newLoc.row = currLoc.row + delta.row; 
		newLoc.col = currLoc.col + delta.col; 
		
			   
		/* if the position is inbounds and the cell hasn't already been included*/
		if ( newLoc.row >= 0 && newLoc.row < mz.numRows() && 
			newLoc.col >= 0 && newLoc.col < mz.numCols() ) found = true;  
	}
	return newLoc; 
}

/* updateGridValue
 * ---------------
 * updates the value for the specified grid locations to 
 * be true 
 *
 */ 
			
void updateGridValue(Grid<bool> &grd, pointT loc) {
	
	if ( grd.getAt(loc.row, loc.col) == false ) { 
		grd.setAt(loc.row, loc.col, true); 
	} else { 
		Error("markInclued called on tile that was already included"); 
	}
}


/* createIncluded
 * --------------
 * convience function to generate a grid with the size specified 
 * by rows = r and columns = c. 
 * 
 * All locations are initially set to false
 */ 


Grid<bool> createIncluded(int r, int c) {
	Grid<bool> output(r,c); 
	for (int i = 0; i < r; i++) { 
		for (int j = 0; j < c; j++) { 
			output.setAt(i,j,false); 
		}
	}
	return output; 
}

/* makeMake
 * --------
 * makeMake creates a perfect maze. 
 *
 *
 */ 

void makeMaze(Maze &mz) { 
	
	// create a grid that's initially all false 
	Grid<bool> inclGrid = createIncluded(mz.numRows(), mz.numCols()); 
	int included = mz.numRows() * mz.numCols(); 
	pointT loc; loc.row = 0; loc.col = 0; 
	
	while (included > 0) { 
				   
		pointT nextLoc = nextLocation(mz, loc);
		
		/* if the new tile is excluded, remove the wall */ 
		if ( !inclGrid.getAt(nextLoc.row, nextLoc.col) ) { 
			mz.setWall(loc, nextLoc, false);     /*remove the wall*/
			updateGridValue(inclGrid, nextLoc);  
			included--;
		}
		loc = nextLoc; 
	}		   
} 	

/* deltaFactory
 * ------------
 * deltaFactory is a convience function to generate 
 * the four possible movements for a point
 */ 

Vector<pointT> deltaFactory() { 
	Vector<pointT> ds; 
	
	pointT d1, d2, d3, d4; 
	d1.row = 1; d1.col = 0; 
	d2.row = -1; d2.col = 0; 
	d3.row = 0; d3.col = 1; 
	d4.row = 0; d4.col = -1; 
	
	ds.add(d1); ds.add(d2); ds.add(d3); ds.add(d4); 
	
	return ds; 
}

/* isInBounds
 * ---------- 
 * isInBounds takes a maze and point and check whether 
 * the point does bounds checking of the point
 */ 

bool isInBounds(Maze &mz, pointT pt) { 
	if (pt.row >= 0 && pt.row < mz.numRows()  &&
		pt.col >= 0 && pt.col < mz.numCols() ) { 
		return true; 
	} else { return false; } 
}

/* ptCmp
 * -----
 * ptCmp establishes the parameters for comparing two points
 *
 */

int ptCmp(pointT a, pointT b) {
	if (a.row < b.row) return -1;
	else if (b.row < a.row) return 1; 
	else if (a.col < b.col) return -1; 
	else if (b.col < a.col) return 1; 
	else return 0; 
}

/* shortestPath
 * ------------
 * shortestPath takes a maze and finds the shortest path 
 * from the bottom righthand corner to the top lefthand corner.
 * shortestPath uses the standard BFS algorithm
 *
 */ 


Stack<pointT> shortestPath(Maze &mz) { 
	
	Vector<pointT> deltas = deltaFactory(); 
	Queue<Stack<pointT> > frontier; 
	
	/*start from bottom left corner */ 
	pointT startPt; startPt.row = mz.numRows() -1; startPt.col = 0; 
	
	Stack<pointT> startPath; 
	startPath.push(startPt); 
	
	frontier.enqueue(startPath);; 
	
	bool isGoal = false; 
	Stack<pointT> currPath; 
	Set<pointT> explored(ptCmp); 
	
	
	while( !isGoal ) { 
		currPath = frontier.dequeue(); 
		pointT eop = currPath.peek(); 
		
		/* check if we've reach the goal, top right corner */ 
		if (eop.row == 0 && eop.col == mz.numCols() - 1) { 
			isGoal = true;
		}
		
		for (int i = 0; i < 4; i++) { 
			pointT nextPt; 
			nextPt.row = eop.row + deltas[i].row; 
			nextPt.col = eop.col + deltas[i].col; 
			if ( isInBounds(mz, nextPt) && !explored.contains(nextPt) && !mz.isWall(nextPt,eop) ) { 
				explored.add(nextPt); 
				Stack<pointT> nextPath; 
				nextPath = currPath; 
				nextPath.push(nextPt); 
				frontier.enqueue(nextPath); 
			}
		}
	}
	return currPath; 
	
}



int main() {
	cout << "Enter the number of maze rows: " << endl; 
	int mzRows = GetInteger(); 
	cout << "Enter the number of maze columns: " << endl; 
	int mzCols = GetInteger(); 
			   
	Maze mz(mzRows, mzCols, true);
	
	mz.draw();
			   
	makeMaze(mz); 
	
	Stack<pointT> shortest = shortestPath(mz); 
	
	pointT pt; 
	while ( !shortest.isEmpty() ) { 
		pt = shortest.pop(); 
		mz.drawMark(pt,"Red"); 
	}
	mz.draw(); 
	
	
	return 0; 			   
}
		   
