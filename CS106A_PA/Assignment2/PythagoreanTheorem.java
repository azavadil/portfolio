/*
 * File: PythagoreanTheorem.java
 * Name: 
 * Section Leader: 
 * -----------------------------
 * This file is the starter file for the PythagoreanTheorem problem.
 */

import acm.program.*;
import java.lang.Math; 

/* PythagoreanTheorem is a subclass of ConsoleProgram that accepts values for a and b as ints and then calculates
 * the solution of c as a double.
 */ 

public class PythagoreanTheorem extends ConsoleProgram {
	public void run() {
		
		println("Enter value to computer Pythagorean Theorem");
		
		// read in values and immediately cast to doubles
		double x = (double)readInt("a: ");
		double y = (double)readInt("b: ");
		println("c = " + Math.sqrt(x*x+y*y));
		
	}
}
