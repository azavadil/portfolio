/* File: boggle.cpp
 * ----------------
 * Your name here!
 */
 
#include "genlib.h"
#include "simpio.h"
#include <iostream>
#include "extgraph.h"
#include "vector.h"
#include "grid.h" 
#include "set.h"
#include "strutils.h" 
#include "random.h"
#include "gboggle.h"
#include "lexicon.h"

string StandardCubes[16]  = 
{"AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS", "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
 "DISTTY", "EEGHNW", "EEINSU", "EHRTVW", "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"};
 
string BigBoggleCubes[25]  = 
{"AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM", "AEEGMU", "AEGMNN", "AFIRSY", 
"BJKQXZ", "CCNSTW", "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT", "DHHLOR", 
"DHLNOR", "EIIITT", "EMOTTT", "ENSSSU", "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"};

void GiveInstructions()
{
    cout << endl << "The boggle board is a grid onto which I will randomly distribute " 
	 << "cubes. These 6-sided cubes have letters rather than numbers on the faces, " 
	 << "creating a grid of letters on which you try to form words. You go first, " 
	 << "entering all the words you can find that are formed by tracing adjoining " 
	 << "letters. Two letters adjoin if they are next to each other horizontally, " 
	 << "vertically, or diagonally. A letter can only be used once in the word. Words "
	 << "must be at least 4 letters long and can only be counted once. You score points "
	 << "based on word length: a 4-letter word is worth 1 point, 5-letters earn 2 "
	 << "points, and so on. After your puny brain is exhausted, I, the super computer, "
	 << "will find all the remaining words and double or triple your paltry score." << endl;
	
    cout << "\nHit return when you're ready...";
    GetLine();
}

static void Welcome()
{
    cout << "Welcome!  You're about to play an intense game of mind-numbing Boggle. " 
	 << "The good news is that you might improve your vocabulary a bit.  The "
	 << "bad news is that you're probably going to lose miserably to this little "
	 << "dictionary-toting hunk of silicon.  If only YOU had a gig of RAM..." << endl << endl;
}


struct pointT { 
	int row; 
	int col; 
};



bool validLength(string s) { return s.length() >= 4; }  

bool validWord(string s, Lexicon &lex) { return lex.containsWord(s); } 

bool validBoggle(string s, Lexicon &lex) { return validLength(s) && validWord(s,lex); } 

/* addLetters
 * ----------
 * addLetters adds the letters to the boggle cubes. 
 * used for testing when we create our own boards. 
 */ 

void addLetters(Grid<char> &board) { 
	for (int i = 0; i < board.numRows() *board.numCols(); i++) {
		int row = i/board.numRows(); int col = i % board.numRows(); 
		LabelCube(row, col, board.getAt(row,col)); 
	}
}


/* highlightPath
 * -------------
 * convience function to highlight the selected cubes
 *
 * param path: vector containing points which represent 
 * the cubes to be highlighted.
 */ 

void highlightPath(Vector<pointT> &path) { 
	for (int i = 0; i < path.size(); i++) {
		HighlightCube(path[i].row, path[i].col, true);
	}
}

/* clearHighlights
 * ---------------
 * clearHighlights is a convience function to set
 * HighlightCube to false for all cubes
 */ 

void clearHighlights(Grid<char> &board) { 
	for (int row = 0; row < board.numRows(); row++) {
		for (int col = 0; col < board.numCols(); col++) { 
			HighlightCube(row,col, false); 
		}
	}
}
	
/* makeDeltas
 * ----------
 * makeDeltas is a convience function to generate the 
 * 8 possible movements on a boggle board horizontal, 
 * vertical, or diagonal
 */ 

Vector<pointT> makeDeltas() { 
	pointT d1, d2, d3, d4, d5, d6, d7, d8; 
	d1.row = -1; d1.col = 0; 
	d2.row = -1; d2.col = 1; 
	d3.row = 0; d3.col = 1; 
	d4.row = 1; d4.col = 1; 
	d5.row = 1; d5.col = 0; 
	d6.row = 1; d6.col = -1; 
	d7.row = 0; d7.col = -1; 
	d8.row = -1; d8.col = -1; 
	
	Vector<pointT> output; 
	output.add(d1); output.add(d2); output.add(d3); output.add(d4); output.add(d5); output.add(d6); output.add(d7); output.add(d8);
	return output; 
}

/* inGrid
 * ------
 * inGrid returns true if the point is within the bounds of the grid and false otherwise
 */ 

bool inGrid(Grid<char> &g, pointT pt) { 
	if ( pt.row >= 0 && pt.row < g.numRows() && pt.col >= 0 && pt.col < g.numCols() ) { 
		return true; 
	} else { 
		return false; 
	}
}


/* successors
 * ----------
 * successors takes a position, board, and a vector of deltas
 * and returns a vector with all possible successors. 
 *
 * successors simply takes all possible moves and does
 * bound checking. If a point is within the bounds of the board
 * it is included in the successors
 */

Vector<pointT> successors(pointT position, Grid<char> &board, Vector<pointT> &deltas) { 
	Vector<pointT> next; 
	
	for (int i =0; i < deltas.size(); i++) { 
		int nextRow = position.row + deltas[i].row; 
		int nextCol = position.col + deltas[i].col; 
		pointT pt; pt.row = nextRow; pt.col = nextCol; 
		if ( inGrid(board, pt) ) { 
			next.add(pt); 
		}
	}
	return next; 
}


/* recurWord
 * ---------
 * recurdWord/checkWord work together to check whether a user input word is a valid word (i.e. both
 * a real word and on the board)
 *
 * recurWord returns true if the given string s is an actual word opn the board and false otherwise. 
 * recurWord is implemented using recursive backtracking. 
 *
 * recurWord generates all possible successor positions on the board. 
 * 
 * recurWord first checks if the successor position matches the current (i.e. first) character of the string.
 * If we have a match, recurWord recurs on the stub of the word (i.e. the first character sliced off) 
 *
 */

bool recurWord(string s, pointT position, Grid<char> &board, Vector<pointT> &deltas, Vector<pointT> &path) { 
	
	if ( s.length() == 0 ) return true; 
	
	Vector<pointT> next = successors(position, board, deltas); 
	
	for (int i = 0; i < next.size(); i++) { 
		pointT nextPt = next[i]; 
		
		if ( board.getAt(nextPt.row, nextPt.col) == s[0] ){ 
			Grid<char> copy = board; 
			copy.setAt(nextPt.row, nextPt.col, 'f'); 
			path.add(nextPt); 
			if ( recurWord(s.substr(1,s.length()-1), nextPt, copy, deltas, path) )  return true; 
		}
	}
	return false; 
}

/* checkWord
 * --------- 
 * checkWord takes a word and returns true if the word is a valid word (i.e. both a real word
 * and on the board) and false otherwise. 
 *
 * checkWord iterates through each position on the board and calls recurWord with the starting 
 * position (i.e. checkWord trys recurWord for each position on the board)
 */ 
				
bool checkWord(string s, Grid<char> &board, Vector<pointT> &deltas, Vector<pointT> &path) { 
	for (int i = 0; i < 16; i++) { 
		int row = i/4; int col = i%4; 
		if ( board.getAt(row,col) == s[0] ) {
			pointT start; start.row = row; start.col = col; 
			path.add(start); 
			if ( recurWord(s.substr(1,s.length()-1), start, board, deltas, path) ) return true; 
		}
	}
	return false;
}


/* findWord
 * --------
 * findWord / findWords are the computer AI that find all the words on a given board
 * 
 * given a string, position, and board, findWord uses recursive backtracking to find a words
 * on the given board
 *
 * If the given string is the boggle lexicon then we add it to the accumulator. 
 *
 * Otherwise, we check all possible successors to the current position, appending the character
 * for the successor position to our boggle word and recuring with the new candiate word. 
 */ 

bool findWord(string sofar, pointT position, Grid<char> &board, Lexicon &lex, Vector<pointT> deltas, Set<string> &acc) { 
	
	if( validBoggle(sofar, lex) ) { 
		acc.add(sofar); 
		return true; 
	}
	
	Vector<pointT> next = successors(position, board, deltas); 
	
	for (int i = 0; i < next.size(); i++) { 
		pointT nextPt = next[i]; 
		char ch = board.getAt(nextPt.row, nextPt.col);
		string prefix = sofar + ch; 
		if ( lex.containsPrefix( prefix ) ) { 
			Grid<char> copy = board; 
			copy.setAt(nextPt.row, nextPt.col, 'f'); 
			if ( findWord(prefix, nextPt, copy, lex, deltas, acc) ) return true; 
		}
	}
	return false; 
}

/* findWords
 * ---------
 * findWords iterates through the boggle board calling its recursive helper
 * method to find if there is a word starting from the current position 
 */

void findWords(Grid<char> &board, Lexicon &lex, Set<string> &words) { 
	
	Vector<pointT> moves = makeDeltas(); 
	
	for (int row = 0; row < board.numRows(); row++) { 
		for (int col = 0; col < board.numCols(); col++) {  
			char ch = board.getAt(row, col);
			string start(1,ch); 
			pointT position; position.row = row; position.col = col;
			findWord(start, position, board, lex, moves, words); 
		} 
	} 
}


/* makeRandomBoard
 * ---------------
 * makeRandomBoard takes an array of strings (each string represents a 
 * standard boggle cube) and creates a random board
 * 
 * makeRandomBoard first shuffles the cubes and then 
 * iterates through the standCubes vector selecting 
 * a random letter from each cube. 
 */ 



Grid<char> makeRandomBoard(string standCubes[]) {
	Grid<char> newGame(4,4); 
					
	for (int i = 0; i < 16; i++){ 
		int randIdx = RandomInteger(0,i); 
		string temp = standCubes[i];
		standCubes[i] = standCubes[randIdx]; 
		standCubes[randIdx] = temp; 
	} 
					
	for (int i =0; i < 16; i++) { 
		int randIdx = RandomInteger(0,5); 
		string s = standCubes[i];
		char ch = s[randIdx]; 
		int row = i/4; 
		int col = i%4; 
		newGame.setAt(row, col, ch); 
	}
	return newGame; 
}


/* makeUserBoard
 * -------------
 * makeUserBoard takes a string a generates a 
 * a 4 x 4 user defined board
 */

Grid<char> makeUserBoard(string s) { 
	if (! s.length() == 16 ) Error("String length must equal 16"); 
				
	Grid<char> newGame(4,4); 
					
	for (int i = 0; i < 16; i++) { 
		int row = i/4;
		int col = i%4; 
		newGame.setAt(row, col, s[i]); 
	}
	return newGame; 
}
				
void testTurn(){ 
	
	Grid<char> board = makeRandomBoard(StandardCubes); 
	Vector<pointT> moves = makeDeltas();

	DrawBoard(4,4); 
	addLetters(board); 
	
	while (true){ 
		cout << "Enter Word: " << endl;
		string s = GetLine(); 
		if (s == "") break; 
		
		cout << "Word entered: " << s << endl;
		Vector<pointT> path; 
		bool res = checkWord(s, board, moves,path); 
		if (res) cout << "checkWord returned true" << endl;
		if (!res) cout << "checkWord returned false" << endl; 
		highlightPath(path); 
		Pause(0.5); 
		clearHighlights(board); 
		
	}
}

void testAI(){ 
	
	Grid<char> board = makeRandomBoard(StandardCubes); 
	
	DrawBoard(4,4); 
	addLetters(board); 
	
	Set<string> results; 
	
	Lexicon lex("lexicon.dat"); 
	
	findWords(board, lex, results); 
	
	Set<string>::Iterator itr = results.iterator(); 
	
	while ( itr.hasNext() ){ 
		string word = itr.next(); 
		cout << "word: " << word << endl; 
	} 
		
}
				
int main()
{
	SetWindowSize(9, 5);
	InitGraphics();
	Welcome();
	GiveInstructions();
	testTurn();
	return 0;
}