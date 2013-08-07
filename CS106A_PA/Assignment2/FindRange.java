/*
 * File: FindRange.java
 * Name: 
 * Section Leader: 
 * --------------------
 * This file is the starter file for the FindRange problem.
 */

/* ConsoleProgram reads in a list of integers, one per line, until a sentinel
 * value is entered. When the sentinel is read, the program displays the smallest and largest values in the list
 * e.g. for list 11, 17, 42, 9, -3, 35, 0 
 * smallest: -3
 * largest: 42 
 * If the user enters only one value before the sentinel, the program should report
 * that value as both the largest and smallest.
 * If the user enters the sentinel on the very first input line, then no values have been 
 * entered, and your program should display a message to that effect
 */ 
 
import acm.program.*;

public class FindRange extends ConsoleProgram {
 private static final int SENTINEL = 0; 
 public void run() {
  println("This program finds the largest and smallest numbers");
  
  Boolean hasInput = false;
  
  // initialize min and max to zero, we'll imediately overwrite min and max the first time through the while loop
  int min = 0; int max = 0; 
  int x; 
  while(true){
   
   x = readInt("?");
   if( x == SENTINEL ) break; 
   
   // first time through we set min & max equal to x
   if (!hasInput) { 
    min = x; 
    max = x; 
   } 
   
   // if we get past the sentinel once we have input
   hasInput = true; 
   
   if(x>max) { 
    max = x; 
   }
   if(x<min) {
    min = x; 
   }
  }
  if (hasInput){ 
   println("smallest: " + min);
   println("largest: " + max);
  } else { 
   println("No input was entered"); 
  } 
 }
}

