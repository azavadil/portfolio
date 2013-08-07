/*
 *  File: q2.cpp
 *  ------------------
 *
 *  Created by athony on 3/16/13.
 *
 */

#include "genlib.h"
#include "simpio.h"
#include <iostream> 
#include "strutils.h" 
#include "random.h"



/* simulation performs a voting simulation. 
 * The user is prompted to enter the voting simulation parameters, 
 * then the program performs 500 simulation trials and reports the ratio.
 * (number of trials in which election result was invalid / total number of trial) x 100
 * 
 * param voters: number of voters
 * param spread: percentage spread between the candidates
 * param error: voting error as a percentage
 */

bool simulation(int voters, double spread, double error) { 
	bool voteError; 
	
	int yesVotes = (int)voters * (0.5 + spread); 
	int voteCount = 0;
	
	/* This loop is all the yes votes 
	 * If there IS NO counting error the vote count increases by one
	 *
	 * e.g. if the spread is 1%, then 51% of the votes are yes
	 * and for 10,000 voters we have 5,100 yes votes
	 */ 
	
	
	for (int i = 0; i < yesVotes; i++) { 
		voteError = RandomChance(error); 
		if ( !voteError ) voteCount++; 
	}
	
	/* This loop is all the no votes 
	 * If there IS counting error the vote count increases by one
	 *
	 * e.g. if the spread is 1%, then 51% of the votes are yes
	 * and for 10,000 voters we have 4,900 no votes
	 */ 
	
	for (int i = 0; i < voters-yesVotes; i++){ 
		voteError = RandomChance(error);
		if(voteError) voteCount++;
	}
	
	if( voteCount > voters/2) return true; 
	else return false;
}


/* validPercentage takes user input and validates
 * that user input is within the appropriate range 0 - 1
 */ 

double validPercentage(string displayMessage){ 
	
	double result; 
	while (true) { 
		cout << displayMessage << endl; 
		result = GetReal(); 
		if( result >= 0 && result <= 1 ) break; 
		else cout << "Enter a number between 0 and 1" << endl; 
	}
	return result; 
}


int main() { 

	int voters; 
	int misvote = 0; 
	double spread; double error; 
	
	
	// validate user input. Number of voters must be positive // 
	while (true){ 
		cout << "Enter number of voters: " << endl; 
		voters = GetInteger();
		if(voters > 0) break; 
		else cout << "Enter a positive number" << endl; 
	}
	
	spread = validPercentage("Enter percentage spread between the candidates: "); 
	error = validPercentage("Enter voting error percentage: ");
		
	for (int i = 0; i < 500; i++) { 
		if( !simulation(voters, spread, error) ) misvote++; 
	}
	
	cout << "Chance of an invalid election result after 500 trials = " << RealToString(100*double(misvote)/500) << endl; 
		
	return 0; 
}
	