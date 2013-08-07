/* pathfinder.cpp
 * ---------------
 * A starter file with some starting constants and handy utility routines.
 */
 
#include "genlib.h"
#include "extgraph.h"
#include <iostream>
#include <cmath>

#include "simpio.h"
#include <fstream>
#include "scanner.h"
#include "map.h"
#include "simpio.h"
#include "set.h"
#include "KruskalMST.h"
#include "DijkstraSP.h"
#include "vector.h"
#include "MinPQ.h"
#include "pathfinderStructs.h"


/* Constants
 * --------
 * A few program-wide constants concerning the graphical display.
 */
const double CircleRadius =.05;     	// the radius of a node
const int LabelFontSize = 9;          // for node name labels


/* Prototypes
 * ----------
 * 
 */ 
int arcCmp(arcT *a1, arcT *a2);



/* Function: DrawFilledCircleWithLabel
 * Usage:  DrawFilledCircleWithLabel(center, "Green", "You are here");
 * -------------------------------------------------------------------
 * Uses facilities from extgraph to draw a circle filled with
 * color specified. The circle is centered at the given coord has the
 * specified radius.  A label is drawn to the right of the circle.
 * You can leave off the last argument if no label is desired.
 */
void DrawFilledCircleWithLabel(coordT center, string color, string label = "")
{
	MovePen(center.x + CircleRadius, center.y);
	SetPenColor(color);
	StartFilledRegion(1.0);
	DrawArc(CircleRadius, 0, 360);
	EndFilledRegion();
	if (!label.empty()) {
		MovePen(center.x + CircleRadius, center.y);
		SetFont("Helvetica");
		SetPointSize(LabelFontSize);
		DrawTextString(label);
	}
}

/* Function: DrawLineBetween
 * Usage:  DrawLineBetween(coord1, coord2, "Black");
 * -------------------------------------------------
 * Uses facilities from extgraph to draw a line of the
 * specified color between the two given coordinates.
 */
void DrawLineBetween(coordT start, coordT end, string color)
{
	SetPenColor(color);
	MovePen(start.x, start.y);
	DrawLine(end.x - start.x, end.y - start.y);
}


/* Function: GetMouseClick
 * Usage:  loc = GetMouseClick();
 * ------------------------------
 * Waits for the user to click somewhere on the graphics window
 * and returns the coordinate of where the click took place.
 */
coordT GetMouseClick()
{
	coordT where;
	WaitForMouseDown();
	WaitForMouseUp();
	where.x = GetMouseX();
	where.y = GetMouseY();
	return where;
}

/* Function: WithinDistance
 * Usage:  if (WithinDistance(click, pt))...
 * -----------------------------------------
 * Returns true if the Cartesian distance between the two coordinates
 * is <= the specified distance, false otherwise. If not specified,
 * the distance is assumed to be size of the node diameter on screen.
 */
bool WithinDistance(coordT pt1, coordT pt2, double maxDistance = CircleRadius*2)
{
	double dx = pt2.x - pt1.x;
	double dy = pt2.y - pt1.y;
	double distance = sqrt(dx*dx + dy*dy);
	return (distance <= maxDistance);
}





/* Implementation notes: getProgramSelection
 * ------------------------------------
 * getProgramSelection prompts the user to select and action  
 * and validates the selection before returning the selection
 */ 


int getProgramSelection(){
	
	int selection = 0 ;
	
	while (true) { 
		cout << "Please make a selection. Your choices are: " << endl;
		cout << "(1) Choose a new graph data file" << endl;
		cout << "(2) Find shortest path using Dijkstra's algorithm" << endl;
		cout << "(3) Compute the minimum spanning tree using Kruskal's algorithm" << endl; 
		cout << "(4) Quit" << endl; 
		
		selection = GetInteger();  
		if(selection > 0 && selection < 5) break;
		cout << "Invalid selection, try again: "; 
	}
	return selection; 
}





/* Implementation notes: GetMapLocation
 * ------------------------------------
 * GetMapLocation prompts the user to enter a location and 
 * validates the location before returning the selection
 * 
 * GetMapLocation uses two while loops. One to prompt the 
 * user to continue to make selections until a valid location 
 * is selected and a second loop to check if the selection 
 * matches any known locations.
 */ 


string GetMapLocation(Map<coordT *> &coordinates){
	
	string location = "";

	while (true){ 
		bool coordFound = false;
	
		coordT selectedCoord = GetMouseClick(); 
		Map<coordT *>::Iterator itr = coordinates.iterator(); 
		while (itr.hasNext()){ 
			location = itr.next(); 
			if ( WithinDistance(selectedCoord, *coordinates[location]) ) { 
				coordFound = true;    //used to break the outer while loop
				break; 
			}
			
		}
		if (coordFound) break;
		cout << "Coordinate not found, please select again" << endl; 
	}
	return location; 
}


void drawNodesOnMap(Vector<nodeT *> &G)
{
	// draw all the nodes on the map  
	for (int i = 0; i < G.size(); i++) { 
		// we index into pfGraph to get the node, then pull out the location of the node
		DrawFilledCircleWithLabel(G[i]->location, "Black", G[i]->name);
	}
}


/* Implementation note: drawMST
 * ----------------------------
 * Draws the minimum spanning tree
 */ 

void drawMST(Vector<arcT *> &inclArcs, Map<coordT *> &nameToCoordinates){ 
	
	Set<string> drawn; 
	
	for (int i = 0; i < inclArcs.size(); i++) { 
		arcT *arc = inclArcs[i]; 
 
		// get the start and end node name
		string startNodeName = (arc->start)->name; 
		string endNodeName = (arc->end)->name;	
 
		coordT pt_start = *(nameToCoordinates[startNodeName]);
		coordT pt_end = *(nameToCoordinates[endNodeName]); 
 
		DrawLineBetween(pt_start, pt_end, "Black");
 
		if (!drawn.contains(startNodeName)){ 
			drawn.add(startNodeName); 
			DrawFilledCircleWithLabel(pt_start, "Black", startNodeName);
		}
		if (!drawn.contains(endNodeName)){ 
			drawn.add(endNodeName); 
			DrawFilledCircleWithLabel(pt_end,"Black", endNodeName);
		}
	}
}

/* Implementation note: DrawSP
 * ----------------------------		 
 * Draws the shortest path from start node to end node
 */ 
							
void drawSP(Stack<arcT> &inclArcs, Map<coordT *> &nameToCoordinates){
	
	while (!inclArcs.isEmpty()) { 
		arcT arc = inclArcs.pop(); 
		
		// get the start and end node
		coordT pt_start = *(nameToCoordinates[(arc.start)->name]);
		coordT pt_end = *(nameToCoordinates[(arc.end)->name]); 
		
		DrawLineBetween(pt_start, pt_end, "Black");
		cout << (arc.start)-> name << ", " << (arc.end)->name << endl; 
	}
}
 


/* Implementation note: Input
 * -------------------
 * The file is in space delimited format. The format is as follows:
 * 
 * filename		//filename of the picture to be used
 * NODES		//specifies the start of the NODES data
 * nodename coord1 coord2		//ex 'Gates 2.81 2.82'
 * ARCS							// specifies the start of the ARCS data
 * nodename1 nodename2 cost		// ex 'Lakelog Roble 0.040'
 */ 


/* Implementation note: printInput
 * -------------------------------
 * printInput is a short function used for testing. printInput 
 * prints the data for each node entered into the graph
 */ 

void printInput(Vector<nodeT *> v)
{
	cout << "Printing Input: " << endl; 
	for (int i = 0; i < v.size(); i++){ 
		nodeT *n = v.getAt(i); 
		cout << "index = " << n->indx << ", name = " << n->name << ", size of outgoing = " << n->outgoing.size() << endl; 
		for(int j = 0; j < n->outgoing.size(); j++){
			arcT *a = n->outgoing[j]; 
			cout << "arc->start = " << (a->start)->name << ", arc->end = " << (a->end)->name << ", arc->cost = " << a->cost << endl;
		}
		cout << "-------------------------------------" << endl; 
	}
	
}

/* Implementation note: printPQ
 * -------------------------------
 * printPQ is a short function used for testing. printPQ prints
 * the arcs that have been entered into the priority queue
 */ 

void printPQ(Vector<nodeT *> G)
{
	
	MinPQ<arcT *> pq(arcCmp); 
	for(int i = 0; i < G.size(); i++) {
		nodeT *currNode = G[i]; 
		for(int j = 0; j < currNode->outgoing.size(); j++){ 
			pq.enqueue(currNode->outgoing[j]);
		}
	}
	
	while (!pq.isEmpty()) {
		arcT *arc = pq.delMin(); 
		cout << "arc->cost = " << arc->cost << ", arc->start = " << (arc->start)->name << ", arc->end " << (arc->end)->name << endl; 
	}
}

/* Implementation note: getToNodeByIndex
 * ----------------------------------------
 * we often know the node that we are coming from and have 
 * an arc with start and end nodes. This is a convenience function
 * to get the 'to' node (i.e. the node this arc takes us 'to').  
 */ 

nodeT *getToNodeByIndex(arcT *arc, int index) 
{
	if((arc->start)->indx != index && (arc->end)->indx != index) Error("getToNodeByIndex: index must match either start or end node");
	
	if((arc->end)->indx == index){
		return arc->start; 
	} else {
		return arc->end; 
	}
}



/* Implementation note: arcCmp
 * ---------------------------
 * arcCmp is the callback function passed to the priority queue
 * to handle comparisons of arcs. Arcs are compared in terms
 * of their cost
 */ 

int arcCmp(arcT *a1, arcT *a2)
{
	if(a1->cost < a2->cost) return -1; 
	else if ( a1->cost > a2->cost) return 1; 
	else return 0;
}

 
; 
;  
;

/* Implementation note: openFileAndPopulateDataStructures()
 * --------------------------------------------------------
 * openFileAndPopulateDataStructures prompts the user to enter a filename 
 * and populates the dataStructures with the data from that file
 */ 

string openFileAndPopulateDataStructures(Map<coordT *> &nameToCoordinates, 
									   Map<nodeT *> &nameToNode, 
									   Vector<nodeT *> &pfGraph, 
									   Map<int> &nameToIndex, 
									   Vector<arcT *> &arclist)

{
	ifstream in; 
	while (true) { 
		cout << "Enter filename: " << endl; 
		string s = GetLine(); 
		in.open(s.c_str()); 
		if(!in.fail()) break;
		cout << "File didn't open, try again: " << endl; 
		in.clear(); 
	}
	
	bool nodes = false; 
	bool arcs = false; 
	
	string line; 
	getline(in, line); 
	string picname = line; 
	
	Scanner scanner; 
	
	string *scanArr = new string[3];
	int numVertices = 0; 
	
	
	while (true) { 
		
		getline(in, line); 
		if(in.fail()) break;
		
		if(line == "NODES"){ 
			nodes = true;
			scanner.setSpaceOption(Scanner::IgnoreSpaces); 
			scanner.setNumberOption(Scanner::ScanNumbersAsReals);
		}
		
		if(line == "ARCS") {
			arcs = true; 
		}
		
		
		// processing the NODES data
		if(nodes && !arcs && line != "NODES" ) {
			
			scanner.setInput(line); 
			int count = 0; 
			while (scanner.hasMoreTokens()){
				scanArr[count++] = scanner.nextToken(); 
			}
			
			// create the point that represents the location of this node
			coordT *pt = new coordT; 
			pt->x = StringToReal(scanArr[1]); 
			pt->y = StringToReal(scanArr[2]);
			
			// create the new node
			nodeT *node = new nodeT; 
			node->location = *pt; 
			node->indx = numVertices; 
			node->name = scanArr[0]; 
			// IMPLEMENT CLEAN UP
			
			pfGraph.add(node); 
			
			nameToIndex.add(scanArr[0], numVertices++); 
			nameToNode.add(scanArr[0], node); 
			
			nameToCoordinates.add(scanArr[0], pt);
		}	
		
		// processing the ARCS data 
		if(arcs && line != "ARCS") {
			
			scanner.setInput(line); 
			int count = 0; 
			while (scanner.hasMoreTokens()){
				scanArr[count++] = scanner.nextToken();
			}
			
			arcT *arc = new arcT; 
			arc->start = nameToNode.getValue(scanArr[0]);
			arc->end = nameToNode.getValue(scanArr[1]); 
			arc->cost = StringToReal(scanArr[2]); 
			
			arclist.add(arc); 
			
			/* Implementation note
			 * -------------------
			 * we access the node corresponding to the start/end node respectively
			 * using the index for that node. Once we've accessed the node we 
			 * add the end/start node to the respective adjacency list for that node
			 */ 
			nodeT *currNode = pfGraph[nameToIndex[scanArr[0]]];
			currNode->outgoing.add(arc);
			
			currNode = pfGraph[nameToIndex[scanArr[1]]];
			currNode->outgoing.add(arc); 
		}
		
	}
	delete[] scanArr;  //no longer needed
	return picname; 
}



/* Implementation note: cleanUpDataStructures
 * ------------------------------------------
 * cleanUpDataStructures is called when user changes data files. 
 * cleanUpDataStructures go through the data structures deleting the 
 * the elements and freeing dynamically allocated memory
 */ 

void cleanUpDataStructures(Map<coordT *> &nameToCoordinates, 
						   Map<nodeT *> &nameToNode, 
						   Vector<nodeT *> &pfGraph, 
						   Map<int> &nameToIndex, 
						   Vector<arcT *> arclist)
{
	// free the dynamically allocated arcs
	for(int i = arclist.size()-1; i>= 0 ; i--){
		delete arclist[i]; 
		arclist.removeAt(i); 
	}
	
	// free the dynamically allocated nodes
	for(int j = pfGraph.size() -1; j >= 0; j--){ 
		delete pfGraph[j];
		pfGraph.removeAt(j); 
	}
	
	// free the dynamically allocated coords
	Map<coordT *>::Iterator itr = nameToCoordinates.iterator();
	while(itr.hasNext()){
		string key = itr.next(); 
		delete nameToCoordinates.getValue(key); 
		nameToCoordinates.remove(key);
	}
		
	Map<nodeT *>::Iterator itr2 = nameToNode.iterator();
	while(itr2.hasNext()){
		string key = itr.next(); 
		nameToNode.remove(key);
	}	
	
	Map<int>::Iterator itr3 = nameToIndex.iterator();
	while(itr3.hasNext()){
		string key = itr.next(); 
		nameToIndex.remove(key);
	}	
	
}



int main()
{
	InitGraphics();
	SetWindowTitle("CS106 Pathfinder");
    cout << "This masterful piece of work is a graph extravaganza!" << endl
        << "The main attractions include a lovely visual presentation of the graph" << endl
        << "along with an implementation of Dijkstra's shortest path algorithm and" << endl
        << "the computation of a minimal spanning tree.  Enjoy!" << endl;
	
	/* Implementation note: 
	 * -------------------
	 * several maps are maintained to enable quick lookup
	 *
	 * nameToCoordinates: maintain a map of place names to coordinates
	 * nodeToIndex: maintain a mapping of node names to an integer index
	 * nameToNode: maintain a mapping of node names to nodes
	 * arclist: a list of all arcs, this is only maintained for garbage collection
	 * pfGraph: the graph
	 * 
	 */ 
	
	Map<coordT *> nameToCoordinates; 
	Map<nodeT *> nameToNode; 
	Vector<nodeT *> pfGraph;
	Vector<arcT *> arclist; 
	Map<int> nameToIndex;
	
	string picname = "";
	
	
	
	while(true){
		/* Implementation note: getProgramSelection
		 * ----------------------------------------
		 * Promt the user to select an action
		 * (1) Choose a new graph data file
		 * (2) Find shortest path using Dijkstra's algorithm
		 * (3) Compute the minimum spanning tree using Kruskal's algorithm 
		 * (4) Quit 
		 */ 
	
		int selectedAction = getProgramSelection(); 
	
		
		// 1 = Choose a new graph data file
		if(selectedAction == 1) {
		
			// hueristic for whether this is the initial use. If pfGraph has elements its been used previously
			if( pfGraph.size() > 0 ) cleanUpDataStructures(nameToCoordinates, nameToNode, pfGraph, nameToIndex, arclist); 
	
			picname = openFileAndPopulateDataStructures(nameToCoordinates, nameToNode, pfGraph, nameToIndex, arclist); 
		}
	
		/* Implementation note: test the input 
		 * -----------------------------------
		 * testing code, comment out for production
		 */ 
		//printInput(pfGraph); 
	
		DrawNamedPicture(picname);
		drawNodesOnMap(pfGraph); 
		
		// 2 = Find shortest path using Dijkstra's algorithm
		if(selectedAction == 2) { 
	
			cout << "select a start location by clicking on the map" << endl;
			string startCity = GetMapLocation(nameToCoordinates); 
			cout << "startCity = " << startCity << ", index = " << nameToIndex[startCity] << endl; 
	
			/* Implementation note: DijkstraSP
			 * ------------------------------- 
			 * We create an instance of the DijkstraSP class with the 
			 * startCity as the source. We query the DijkstraSP instance
			 * for the shortest path to any location
			 */ 
	
			DijkstraSP solution(pfGraph, nameToIndex[startCity]); 
	
			cout << "select a end location by clicking on the map" << endl;
			string endCity = GetMapLocation(nameToCoordinates); 
			cout << "endCity = " << endCity << ", index = " << nameToIndex[endCity] << endl; 
	
			Stack<arcT> inclArcs = solution.getSP(nameToIndex[endCity]); 
			drawSP(inclArcs, nameToCoordinates); 
			
		}
		
		// 3 = Compute the minimum spanning tree using Kruskal's algorithm 
		if(selectedAction == 3)
		{
			KruskalMST mstSolution(pfGraph, arcCmp);
			Vector<arcT *> mstArcs = mstSolution.getMST(); 
			drawMST(mstArcs, nameToCoordinates); 
		}
		
		// 4 = Quit
		if(selectedAction == 4) break; 
		
	}
	return (0);
}