/*
 * File: Breakout.java
 * -------------------
 * Name:
 * Section Leader:
 * 
 * This file will eventually implement the game of Breakout.
 */

import acm.graphics.*;
import acm.program.*;
import acm.util.*;

import java.applet.*;
import java.awt.*;
import java.awt.event.*;



public class Breakout extends GraphicsProgram {

/** Width and height of application window in pixels */
	public static final int APPLICATION_WIDTH = 400;
	public static final int APPLICATION_HEIGHT = 600;

/** Dimensions of game board (usually the same) */
	private static final int WIDTH = APPLICATION_WIDTH;
	private static final int HEIGHT = APPLICATION_HEIGHT;

/** Dimensions of the paddle */
	private static final int PADDLE_WIDTH = 60;
	private static final int PADDLE_HEIGHT = 10;

/** Offset of the paddle up from the bottom */
	private static final int PADDLE_Y_OFFSET = 30;

/** Number of bricks per row */
	private static final int NBRICKS_PER_ROW = 10;

/** Number of rows of bricks */
	private static final int NBRICK_ROWS = 10;

/** Separation between bricks */
	private static final int BRICK_SEP = 4;

/** Width of a brick */
	private static final int BRICK_WIDTH =
	  (WIDTH - (NBRICKS_PER_ROW - 1) * BRICK_SEP) / NBRICKS_PER_ROW;

/** Height of a brick */
	private static final int BRICK_HEIGHT = 8;

/** Radius of the ball in pixels */
	private static final int BALL_RADIUS = 10;

/** Radius of the ball in pixels */
	private static final int DIAM_BALL = 2*BALL_RADIUS;
	
/** Offset of the top brick row from the top */
	private static final int BRICK_Y_OFFSET = 70;

/** Number of turns */
	private static final int NTURNS = 3;

/** Number of turns */
	private static final int DELAY = 10;
	
/** Starting number of bricks */
	private int BRICK_TOTAL = NBRICKS_PER_ROW*NBRICK_ROWS;
	
/** Boolean condition for if game is running  */
	private boolean liveBall;
	
	
/* Method: run() */
/** Runs the Breakout program. */
	public void run() {
		setup();
		play();
	}

	/* setup creates the initial game environment 
     * Generates the rows of bricks and the paddle 
     */ 	 
	
	private void setup() { 
		int xPadding = getMargin(BRICK_WIDTH,APPLICATION_WIDTH,NBRICKS_PER_ROW,BRICK_SEP); 
		
		for (int i=0; i < NBRICK_ROWS; i++){ 
			for (int j = 0; j < NBRICKS_PER_ROW; j++){
				GRect nextRect = new GRect(xPadding + j*BRICK_WIDTH + j*BRICK_SEP, BRICK_Y_OFFSET + i*BRICK_SEP + i*BRICK_HEIGHT,BRICK_WIDTH,BRICK_HEIGHT);
				nextRect.setFilled(true);
				nextRect.setFillColor(nextColor(i)); 
				add(nextRect);
			}
		}
		
		bk_paddle = new GRect((APPLICATION_WIDTH-PADDLE_WIDTH)/2,(APPLICATION_HEIGHT - PADDLE_Y_OFFSET), PADDLE_WIDTH,PADDLE_HEIGHT); 
		bk_paddle.setFilled(true);
		bk_paddle.setFillColor(Color.BLACK); 
		add(bk_paddle);
		addMouseListeners(); 
		
	}
	
	/* play runs the game
     * play creates the gameball and keeps track of the number of turns played
     */ 
	 
	private void play() { 
		gameBall =  new GOval(APPLICATION_WIDTH/2 - BALL_RADIUS, APPLICATION_HEIGHT/2 - BALL_RADIUS, BALL_RADIUS, BALL_RADIUS); 
		gameBall.setFilled(true); 
		gameBall.setFillColor(Color.BLACK); 
		
		vx = 1.5; 
		vy = 3.0;
		
		// run the actual game. The player gets 3 turns
		for (int i = 0; i < NTURNS;i++) { 
			
			liveBall = true; 			
			add(gameBall); 
			pause(500); 
		
			/* as long as there are bricks and a live ball the game
			 * continues
			 */
			 
			while (BRICK_TOTAL > 0 && liveBall == true) { 
				gameBall.move(vx,vy);
				checkHit();				//check if ball touched brick 
				checkCollision();       //check if ball touched wall or is out of bounds
				pause(DELAY); 
			}
		}
	}
	
	/* checkCollision checks whether the ball has touched a wall or is out of bounds 
	 * if the ball hits a wall, then we reverse the velocity (i.e. it bounces off the wall
	 * if the ball is out of bounds, then the turn is over and we set liveBall to false and 
     * move the ball back to the center of the screen	 
	 */ 
	 
	private void checkCollision() {  
		double diff = 0; 
		if(gameBall.getY() < 0) { 
			vy = - vy;
			diff = gameBall.getY();
			gameBall.move(0,-2*diff);
		}
		
		if(gameBall.getY() > APPLICATION_HEIGHT - DIAM_BALL){ 
			liveBall = false; 
			gameBall.setLocation(APPLICATION_WIDTH/2-BALL_RADIUS,APPLICATION_HEIGHT/2-BALL_RADIUS);
		}
		
		if(gameBall.getX() < 0) { 
			vx = -vx; 
			diff = gameBall.getX();
			gameBall.move(-2*diff,0); 
		}
		
		if(gameBall.getX() > APPLICATION_WIDTH - DIAM_BALL){ 
			vx = -vx; 
			diff = gameBall.getX() - (APPLICATION_WIDTH - DIAM_BALL); 
			gameBall.move(-2*diff, 0); 
		}
	}
	
	/* check hit checks whether the ball has hit a brick. 
	 * if there is a hit we remove the brick
	 */ 
	private void checkHit() { 
		
		if (getHit() != null) { 
			GObject toRemove = getHit();
			if (toRemove != bk_paddle){
				remove(toRemove);
				BRICK_TOTAL--; 
			}
			vy = -vy; 
		}
	}
	
	/* getHit is a convience method that tests whether a ball 
	 * has hit a brick and returns the appropriate brick 
	 * that was hit.  
	 */ 
	 
	private GObject getHit() { 
		double x1_coord = gameBall.getX();
		double y1_coord = gameBall.getY();
		double x2_coord = x1_coord+DIAM_BALL; 
		double y2_coord = y1_coord + DIAM_BALL; 
		
		if (getElementAt(x1_coord,y1_coord) != null) { 
			return getElementAt(x1_coord,y1_coord); 
		}
		
		if (getElementAt(x1_coord,y2_coord) != null) { 
			return getElementAt(x1_coord,y2_coord); 
		}
		
		if (getElementAt(x2_coord,y1_coord) != null) { 
			return getElementAt(x2_coord,y1_coord); 
		}
		
		if (getElementAt(x2_coord,y2_coord) != null) { 
			return getElementAt(x2_coord,y2_coord); 
		}
		return null;
	}
	
	// convience method to calculate the right and left margin for the bricks	
	private int getMargin(int brickW, int windowW, int numBricks, int brickSep) { 
		return (windowW - numBricks*brickW - (numBricks-1)*brickSep)/2;
	}
	
	private Color nextColor(int counter) {
		GOval result = new GOval(10,10);
		
		switch(counter){ 
		case 0: 
			result.setColor(Color.RED);
			break;
		case 1: 
			result.setColor(Color.RED);
			break;
		case 2:	
			result.setColor(Color.ORANGE);
			break;
		case 3: 
			result.setColor(Color.ORANGE);
			break; 
		case 4: 
			result.setColor(Color.YELLOW);
			break;
		case 5: 
			result.setColor(Color.YELLOW);
			break;
		case 6: 
			result.setColor(Color.GREEN);
			break;
		case 7: 
			result.setColor(Color.GREEN);
			break;
		case 8: 
			result.setColor(Color.CYAN);
			break;
		case 9: 
			result.setColor(Color.CYAN);
			break;
		}
		return result.getColor();
	}
	
	public void mousePressed(MouseEvent e) { 
		last = new GPoint(e.getPoint()); 
		element = getElementAt(last); 
	}
	
	public void mouseDragged(MouseEvent e) { 
		if (element != null && element.getWidth() == PADDLE_WIDTH) { 
			element.move(e.getX() - last.getX(),0); 
			last = new GPoint(e.getPoint());   
		}
	}

	private GOval gameBall; 	
	private GRect bk_paddle; 
	private GObject element; 
	private GPoint last; 
	private RandomGenerator rgen = RandomGenerator.getInstance(); 
	private double vy; 
	private double vx; 
	
	
	
}
 