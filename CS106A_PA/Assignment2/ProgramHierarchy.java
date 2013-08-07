/*
 * File: ProgramHierarchy.java
 * Name: 
 * Section Leader: 
 * ---------------------------
 * This file is the starter file for the ProgramHierarchy problem.
 */

import acm.graphics.*;
import acm.program.*;
import java.awt.*;

/* ProgramHierarchy is subclass of GraphicsProgram that draws a partial diagram of the acm.program
 * class hierarchy
 */ 
 
public class ProgramHierarchy extends GraphicsProgram {	
		
	public void run() {
		/* You fill this in. */
		
		GLabel lb1 = new GLabel("Programs");
		GLabel lb2 = new GLabel("GraphicsPrograms");
		GLabel lb3 = new GLabel("ConsolePrograms");
		GLabel lb4 = new GLabel("DialogPrograms");
		
		int boxW = getMax((int)lb1.getWidth(),(int)lb2.getWidth(),(int)lb3.getWidth(),(int)lb4.getWidth()) + 10;
		int boxH = (int)lb1.getAscent() + 10;
		
		int centerH = getHeight();
		int centerW = getWidth();
		int boxPadding = 10; 
		int padding1 = (centerW - boxW)/2;
		int padding2 = (centerW - 3*boxW + 2*boxPadding); 
		
		int x1 = centerW - boxW/2;
		
		int x1a = x1 + (int)(boxW - lb1.getWidth())/2;
		int x1b = x1 + (int)(boxW - lb3.getWidth())/2;
		int ya = centerH +boxH - 5; 
		
		int y2 = centerH + 2*boxH;
		int y2a = y2 +boxH - 5; 
		int x2 = x1 - boxPadding - boxW;
		int x2a = x2 + (boxW - (int)lb2.getWidth())/2;
		int x3 = centerW + boxW/2 + boxPadding;
		int x3a = x3 + (boxW - (int)lb4.getWidth())/2;
		
		
		GRect box1 = new GRect(x1,centerH,boxW,boxH);
		GRect box2 = new GRect(x2,y2,boxW,boxH);
		GRect box3 = new GRect(x1,y2,boxW,boxH);
		GRect box4 = new GRect(x3,y2,boxW,boxH);
		
		add(box1);
		add(box2);
		add(box3);
		add(box4);
		
		GLabel lb1_ = new GLabel("Programs",x1a,ya);
		GLabel lb2_ = new GLabel("GraphicsPrograms",x2a,y2a);
		GLabel lb3_ = new GLabel("ConsolePrograms",x1b,y2a);
		GLabel lb4_ = new GLabel("DialogPrograms",x3a,y2a);
		
		add(lb1_);
		add(lb2_);
		add(lb3_);
		add(lb4_);
		
		GLine ln1 = new GLine(centerW,centerH+boxH,centerW,centerH+2*boxH);
		GLine ln2 = new GLine(centerW,centerH+boxH,centerW-boxW-boxPadding,centerH+2*boxH);
		GLine ln3 = new GLine(centerW,centerH+boxH,centerW+boxW+boxPadding,centerH+2*boxH);
		
		add(ln1);
		add(ln2);
		add(ln3);
		
	}
	
	private int getMax(int a, int b, int c, int d) { 
		int max = 0; 
		if (a>max) {
			max = a; 
		} 
		if (b>max){
			max = b; 
		} 
		if (c>max){
			max = c; 
		}
		if (d>max) {
			max = d; 
		}
		return max;
	}
	
	
}

