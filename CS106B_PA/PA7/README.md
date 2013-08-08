This is the [pathfinder assignement](http://see.stanford.edu/materials/icspacs106b/H34-Assign7Pathfinder.pdf) from CS106B. 

The main routine is in pathfinder.cpp. Rather than write the Dijkstra and Kruskal algorithms as part of pathfinder.cpp, I've abstracted Dijkstra and Kruskal into their own classes. The client can then query an  instances of the Dijkstra or Kruskal class for the shortest path or minimum spanning tree. 

pathfinder.cpp uses an adjacency list representation of a graph. The graph is a vector of nodes where each node itself maintains a vector of outgoing edges. 

There is also an unused, legacy file WgtGraph. I initially implemented the graph as a class. I later switched it to the node representation to get more practice with C++ pointers. 

The DijkstraSP class takes a graph and a source node as arguments to the constructor. The client can then query the instance for the shortest path from the source node to a destination node. 

The KruskalMST class takes a graph and a comparison function as arguments to the constructor. The client can query the instance for the minimum spanning tree. 