
#include "genlib.h"
#include <iostream> 
#include "vector.h" 
#include "set.h"
#include "strutils.h"

/* PuzzleSolver
 * ------------
 * This program takes a puzzle board and returns true if the 
 * board is solvable and false otherwise
 * 
 * see http://see.stanford.edu/materials/icspacs106b/H18-Assign3RecPS.pdf
 * for a description of the problem
 *
 * Example: board 3 6 4 1 3 4 2 5 3 0
 * The goal is to reach the 0. You can move the number of spaces of your
 * current position in either the positive / negative direction
 * Solution for this game is +3, -1, +4, +2, -3, +4
 */ 


/* isGoal
 * ------
 * isGoal returns true if we've reached the goal, false otherwise
 *
 */

bool isGoal(int pos, Vector<int> board) { 
	return board[pos] == 0; 
}

/* successors
 * ----------
 * successors takes a position and a board
 * and returns all the possible successor positions
 * of the board
 */ 

Vector<int> successors(int pos, Vector<int> board) { 
	Vector<int> output; 
	int move = board[pos]; 
	if ( pos + move < board.size() ) output.add(move); 
	if ( pos - move >= 0 ) output.add(-move); 
	
	return output; 
}

/* recurSolve
 * ----------
 * recurSolve is a recursive helper function for Solvable
 * 
 * recurSolve looks at all possible successor moves. If 
 * the move hasn't been explored, then we recur from the 
 * new position
 * 
 * param position: position on the board 
 * param squares: representation of the board
 * param explored: set of the previously explored positions
 */


bool recurSolve(int position, Vector<int> &squares, Set<int> explored) {
	
	if ( isGoal(position, squares) ) return true; 
	
	else { 
		Vector<int> moves = successors(position, squares); 
		
		for (int i = 0; i < moves.size(); i++) { 
			int nextPos = position + moves[i]; 
			
			if ( !explored.contains(nextPos) ) { 
				Set<int> copy = explored; 
				copy.add(nextPos); 
				return recurSolve(nextPos, squares, copy); 
			}
		}
	}
	return false; 
}

/* Solvable
 * --------
 * Solvable returns true if there is a solution to the puzzle
 * false otherwise. 
 * 
 * Solvable call recurSolve to do the heavy lifting
 */ 


bool Solvable(int start, Vector<int> &squares) { 
	
	Set<int> exp; 
	return recurSolve(start, squares, exp); 
}

/* tests
 * -----
 * minimal testing for Solvable function
 */ 

void tests() { 
	Vector<int> v1; 
	v1.add(3); v1.add(1); v1.add(2); v1.add(3); v1.add(0); 
	
	
	bool r1 = Solvable(0, v1); 
	if (!r1) cout << "Puzzle unsolvable" << endl; 
	
	 
	Vector<int> v2; 
	v2.add(3); v2.add(6); v2.add(4); v2.add(1); v2.add(3); v2.add(4); v2.add(2); v2.add(5); v2.add(3); v2.add(0); 
	bool r2 = Solvable(0, v2); 
	
	if (r2) cout << "Puzzle solvable" << endl; 
	if (!r2) cout << "Program foobar" << endl; 
}
	
int main() {
	
	tests(); 
	
	return 0; 
}
	
	