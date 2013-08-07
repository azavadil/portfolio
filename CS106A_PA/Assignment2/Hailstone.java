/*
 * File: Hailstone.java
 * Name: 
 * Section Leader: 
 * --------------------
 * This file is the starter file for the Hailstone problem.
 */

import acm.program.*;

/* Hailstone reads in a number from the user and then displays the
 * Hailstone sequence for that number. The Hailstone sequence is 
 * defined as follows:
 * If n is even, divide it by two.
 * If n is odd, multiply it by three and add one.
 * Continue this process until n is equal to one.
 */ 

public class Hailstone extends ConsoleProgram {
 public void run() {
  /* You fill this in */
  
  int x = readInt("Enter a number: ");
  
  int count  = 0; 
  while (x != 1) { 
   if(x%2 == 0) {
    println(x + " is even so I take half: " + x/2); 
    x = x/2; 
   } else { 
    println(x + " is odd, so I make 3n + 1: " + (3*x+1)); 
    x = 3*x+1; 
   }
   count += 1;  
  }
  println("The process took " + count + " to reach 1"); 
  
 }
}

