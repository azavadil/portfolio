/*
 * File: HangmanCanvas.java
 * ------------------------
 * This file keeps track of the Hangman display.
 */

import acm.graphics.*;

public class HangmanCanvas extends GCanvas {
	
	/*Instance variable*/
	private GLine leftArm1; 
	private GLine leftArm2;
	private GLine rightArm1; 
	private GLine rightArm2;
	private GLine leftLeg1; 
	private GLine leftLeg2;
	private GLine rightLeg1; 
	private GLine rightLeg2;
	private GLine leftFoot; 
	private GLine rightFoot; 
	private GOval stickHead; 
	private GLine stickBody; 
	private double l1_x; 
	private double l1_y2; 
	private double l3_y2;
	private double centerX; 
	private GLabel label;
	private GLabel label2; 
	 

/** Resets the display so that only the scaffold appears */
	public void reset() {
		
		
		// remove all the parts that are on the display
		if (leftFoot != null) remove(leftFoot);
		if (rightFoot != null) remove(rightFoot);
		if (leftLeg1 != null)remove(leftLeg1);
		if (leftLeg2 != null)remove(leftLeg2);
		if (rightLeg1 != null)remove(rightLeg1);
		if (rightLeg2 != null)remove(rightLeg2);
		if (leftArm1 != null)remove(leftArm1);
		if (leftArm2 != null)remove(leftArm2);
		if (rightArm1 != null)remove(rightArm1);
		if (rightArm2 != null)remove(rightArm2);
		if (stickHead != null)remove(stickHead);
		if (stickBody != null)remove(stickBody);
		
		
		centerX = getWidth()/2; 
		l1_x = centerX - BEAM_LENGTH;
		double l1_y1 = 0.1*getHeight();
		l1_y2 = l1_y1 + SCAFFOLD_HEIGHT; 
		
		double l2_x1 = l1_x;
		double l2_x2 = centerX; 
		double l2_y = l1_y1; 
		
		double l3_x = centerX;
		double l3_y1 = l1_y1; 
		l3_y2 = l3_y1 + ROPE_LENGTH; 
		
		GLine scaffoldLine = new GLine(l1_x,l1_y1,l1_x,l1_y2); 
		GLine beamLine = new GLine(l2_x1,l2_y,l2_x2,l2_y);
		GLine ropeLine = new GLine(l3_x,l3_y1,l3_x,l3_y2);
		
		add(scaffoldLine);
		add(beamLine); 
		add(ropeLine);
		
		
	}

/**
 * Updates the word on the screen to correspond to the current
 * state of the game.  The argument string shows what letters have
 * been guessed so far; unguessed letters are indicated by hyphens.
 * 
 * param word: combination of hypens and guessed letters representing
 * the game state
 * 
 * param word2: the letters guessed so far
 */
	public void displayWord(String word, String word2) {
		
		if(label != null) remove(label); 
		label = new GLabel(word,l1_x - 10, l1_y2 + 30);
		add(label);
		
		if (label2 != null) remove(label2); 
		label2 = new GLabel(word2, l1_x - 10, l1_y2+40);
		add(label2);
	}

/**
 * Updates the display to correspond to an incorrect guess by the
 * user.  Calling this method causes the next body part to appear
 * on the scaffold and adds the letter to the list of incorrect
 * guesses that appears at the bottom of the window.
 */
	public void noteIncorrectGuess(char letter, int counter) {
		/* You fill this in */
		
		/*calculate coordinates */ 
		double y1 = l3_y2 + 2*HEAD_RADIUS;
		double y2 = y1 + ARM_OFFSET_FROM_HEAD; 
		double y3 = y2 + LOWER_ARM_LENGTH; 
		double y4 = y1 + BODY_LENGTH; 
		double y5 = y4 + LEG_LENGTH; 
		double x2 = centerX - UPPER_ARM_LENGTH; 
		double x3 = centerX + UPPER_ARM_LENGTH;
		double x4 = centerX - HIP_WIDTH/2; 
		double x5 = centerX + HIP_WIDTH/2; 
		double x6 = x4 - FOOT_LENGTH; 
		double x7 = x5 + FOOT_LENGTH; 
		
		switch(counter) { 
		case 7: 
			stickHead = new GOval(centerX - HEAD_RADIUS, l3_y2, 2*HEAD_RADIUS, 2*HEAD_RADIUS); 
			add(stickHead);
			break;
		case 6: 
			stickBody = new GLine(centerX, y1, centerX, y4);
			add(stickBody); 
			break; 
		case 5: 
			leftArm1 = new GLine(centerX,y2, x2,y2); 
			leftArm2 = new GLine(x2,y2,x2,y3); 
			add(leftArm1);
			add(leftArm2);
			break;
		case 4: 
			rightArm1 = new GLine(centerX,y2,x3,y2); 
			rightArm2 = new GLine(x3,y2,x3,y3); 
			add(rightArm1); 
			add(rightArm2); 
			break; 
		case 3: 
			leftLeg1 = new GLine(centerX,y4,x4,y4); 
			leftLeg2 = new GLine(x4,y4,x4,y5); 
			add(leftLeg1); 
			add(leftLeg2); 
			break; 
		case 2: 
			rightLeg1 = new GLine(centerX,y4,x5,y4); 
			rightLeg2 = new GLine(x5,y4,x5,y5);
			add(rightLeg1); 
			add(rightLeg2);
			break; 
		case 1: 
			leftFoot = new GLine(x4,y5,x6,y5); 
			add(leftFoot); 
			break; 
		case 0: 
			rightFoot = new GLine(x5,y5,x7,y5); 
			add(rightFoot); 
			break; 
		default: 
			break; 
		}
		
		
	
	}

/* Constants for the simple version of the picture (in pixels) */
	private static final int SCAFFOLD_HEIGHT = 260;
	private static final int BEAM_LENGTH = 104;
	private static final int ROPE_LENGTH = 13;
	private static final int HEAD_RADIUS = 26;
	private static final int BODY_LENGTH = 104;
	private static final int ARM_OFFSET_FROM_HEAD = 20;
	private static final int UPPER_ARM_LENGTH = 52;
	private static final int LOWER_ARM_LENGTH = 32;
	private static final int HIP_WIDTH = 26;
	private static final int LEG_LENGTH = 78;
	private static final int FOOT_LENGTH = 20;

}
