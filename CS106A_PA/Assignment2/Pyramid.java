/*
 * File: Pyramid.java
 * Name: 
 * Section Leader: 
 * ------------------
 * This file is the starter file for the Pyramid problem.
 * It includes definitions of the constants that match the
 * sample run in the assignment, but you should make sure
 * that changing these values causes the generated display
 * to change accordingly.
 */

import acm.graphics.*;
import acm.program.*;
import java.awt.*;

/* Pyramid is subclass of GraphicsProgram that draws a pyramid consisting of bricks
 * arranged in horizontal rows, so that the number of bricks in each row decreases by
 *	one as you move up the pyramid,
 */ 

public class Pyramid extends GraphicsProgram {

/** Width of each brick in pixels */
	private static final int BRICK_WIDTH = 30;

/** Width of each brick in pixels */
	private static final int BRICK_HEIGHT = 12;

/** Number of bricks in the base of the pyramid */
	private static final int BRICKS_IN_BASE = 14;
	
	public void run() {
		
		int windowWidth = getWidth();
		int windowHeight = getHeight(); 
		int padding = (int)((0.2*windowWidth +(0.8*windowWidth)%BRICK_WIDTH)/2);
		int startIters = (int)((0.8*windowWidth)/BRICK_WIDTH);
		int count = 0;  
		
		while (startIters != 0) {
			for(int i=startIters; i != 0; i--) { 
				int x = padding + (int)(0.5*count*BRICK_WIDTH) + i*BRICK_WIDTH;
				int y = windowHeight - count*BRICK_HEIGHT;
				GRect brick = new GRect(x,y,BRICK_WIDTH,BRICK_HEIGHT);
				add(brick);
			}
			count += 1; 
			startIters -= 1;
		}
			
	}
	
	
	
}

