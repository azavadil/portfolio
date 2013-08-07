#include <vector>
#include <list>
#include <set>
#include <string>
#include <iostream>
#include <iomanip>
#include "imdb.h"
#include "path.h"
#include <queue> 
using namespace std;

/**
 * Using the specified prompt, requests that the user supply
 * the name of an actor or actress.  The code returns
 * once the user has supplied a name for which some record within
 * the referenced imdb existsif (or if the user just hits return,
 * which is a signal that the empty string should just be returned.)
 *
 * @param prompt the text that should be used for the meaningful
 *               part of the user prompt.
 * @param db a reference to the imdb which can be used to confirm
 *           that a user's response is a legitimate one.
 * @return the name of the user-supplied actor or actress, or the
 *         empty string.
 */

static string promptForActor(const string& prompt, const imdb& db)
{
  string response;
  while (true) {
    cout << prompt << " [or <enter> to quit]: ";
    getline(cin, response);
    if (response == "") return "";
    vector<film> credits;
    if (db.getCredits(response, credits)) return response;
    cout << "We couldn't find \"" << response << "\" in the movie database. "
	 << "Please try again." << endl;
  }
}

/** Implementation note: generateShortestPath
 * ------------------------------------------
 * generateShortestPath uses the standard breadth first search algorithm to 
 * find the shortest path between two actors (actors are represented by strings
 */ 

bool generateShortestPath(imdb &db, const string& source, const string& target, void* goalPath)
{
  set<film> exploredFilms; 
  set<string> exploredCostars; 
  queue<path> frontier;  

  path initialPath(source); 
  frontier.push(initialPath); 
     
  while ( !frontier.empty() ) {

    path nextPath = frontier.front(); 
    frontier.pop(); 
    string costar1 = nextPath.getLastPlayer(); 
    if ( costar1 == target ){
      cout << nextPath << endl; 
      return true;
    }
    
    vector<film> nextCredits; 
    db.getCredits(costar1, nextCredits); 
    for(int i = 0; i < (int)nextCredits.size(); i++) { 
      film currFilm = nextCredits[i];

      if ( exploredFilms.find(currFilm) == exploredFilms.end() ) { 
	exploredFilms.insert(currFilm); 
	vector<string> currCast; 
	db.getCast(currFilm,currCast); 
	for (int j = 0; j < (int)currCast.size(); j++) { 
	  const string costar2 = currCast[j];
	  

	  if( exploredCostars.find(costar2) == exploredCostars.end() ){ 
	    exploredCostars.insert(costar2); 
	    path clonePath = nextPath; 
	    clonePath.addConnection(currFilm, costar2); 
	    frontier.push(clonePath); 
	  }
	}
      }
    }
  }
  return false; 
}


/**
 * Serves as the main entry point for the six-degrees executable.
 * There are no parameters to speak of.
 *
 * @param argc the number of tokens passed to the command line to
 *             invoke this executable.  It's completely ignored
 *             here, because we don't expect any arguments.
 * @param argv the C strings making up the full command line.
 *             We expect argv[0] to be logically equivalent to
 *             "six-degrees" (or whatever absolute path was used to
 *             invoke the program), but otherwise these are ignored
 *             as well.
 * @return 0 if the program ends normally, and undefined otherwise.
 */

int main(int argc, const char *argv[])
{

  imdb db(determinePathToData("/home/compilers/cs107/assn-2-six-degrees-data/little-endian/")); // inlined in imdb-utils.h
  if (!db.good()) {
    cout << "Failed to properly initialize the imdb database." << endl;
    cout << "Please check to make sure the source files exist and that you have permission to read them." << endl;
    exit(1);
  }
  
  while (true) {
    string source = promptForActor("Actor or actress", db);
    if (source == "") break;
    string target = promptForActor("Another actor or actress", db);
    if (target == "") break;
    if (source == target) {
      cout << "Good one.  This is only interesting if you specify two different people." << endl;
    } else {
      // replace the following line by a call to your generateShortestPath routine... 
      path *goalPtr; 
      bool foundSolution = generateShortestPath(db, source, target, &goalPtr); 
      
      if ( !foundSolution )  
	cout << endl << "No path between those two people could be found." << endl << endl;
      else { 
	cout << "found " << endl;  
      }
 
    }
  }
  
  cout << "Thanks for playing!" << endl;
  return 0;
}

