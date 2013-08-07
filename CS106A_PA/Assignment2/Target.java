/*
 * File: Target.java
 * Name: 
 * Section Leader: 
 * -----------------
 * This file is the starter file for the Target problem.
 */

import acm.graphics.*;
import acm.program.*;
import java.awt.*;

/* Target draws an image of an archery target. The figure is three GOval objects, 
 * two red and one white, drawn in the correct order. The outer circle has a radius 
 * of one inch (72 pixels), the white circle has a radius of 0.65 inches, and the 
 * inner red circle has a radius of 0.3 inches. The figure is be centered in the 
 * window.
 */ 

public class Target extends GraphicsProgram {	
	public void run() {
		/* You fill this in. */
		int cW = getWidth()/2;
		int cH = getHeight()/2; 
		
		int r1 = 72;
		int r2 = (int)(72*0.65);
		int r3 = (int)(72*0.3);
		
		GOval c1 = new GOval(cW-r1,cH-r1,2*r1,2*r1);		
		c1.setFilled(true);
		c1.setFillColor(Color.RED);
		add(c1);
		
		
		GOval c2 = new GOval(cW-r2,cH-r2,2*r2,2*r2);		
		c2.setFilled(true);
		c2.setFillColor(Color.WHITE);
		add(c2);

		GOval c3 = new GOval(cW-r3,cH-r3,2*r3,2*r3);		
		c3.setFilled(true);
		c3.setFillColor(Color.RED);
		add(c3);
	}
}
