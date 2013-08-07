/*
 * File: NameSurferGraph.java
 * ---------------------------
 * This class represents the canvas on which the graph of
 * names is drawn. This class is responsible for updating
 * (redrawing) the graphs whenever the list of entries changes or the window is resized.
 */

import acm.graphics.*;
import java.awt.event.*;
import java.util.*;
import java.awt.*;
import java.lang.Math; 
import acm.program.*; 

public class NameSurferGraph extends GCanvas
 implements NameSurferConstants, ComponentListener {

 /**
 * Creates a new NameSurferGraph object that displays the data.
 */
 public NameSurferGraph() {
  addComponentListener(this);
  lastEntry = null; 
  
 }
 
 /**
 * Clears the list of name surfer entries stored inside this class.
 */
 public void clear() {
  removeAll(); 
  nameArr.clear(); 
 }
 
 /* Method: addEntry(entry) */
 /**
 * Adds a new NameSurferEntry to the list of entries on the display.
 * Note that this method does not actually draw the graph, but
 * simply stores the entry; the graph is drawn by calling update.
 */
 public void addEntry(NameSurferEntry entry) {

  // check that the entry doesn't already exist
  if (!nameArr.contains(entry)) { 
	  nameArr.add(entry); 
  }
 }
 
 /**
 * Updates the display image by deleting all the graphical objects
 * from the canvas and then reassembling the display according to
 * the list of entries. Your application must call update after
 * calling either clear or addEntry; update is also called whenever
 * the size of the canvas changes.
 */
 public void update() {
	 
  removeAll(); 
  makeGraph(); 
  
   
  if (nameArr.size() != 0){
   int nextColor = 1; 
   for (NameSurferEntry entry: nameArr){
    Color lineColor = getNextColor(nextColor);
    makeLine(entry,lineColor); 
    nextColor = nextColorCount(nextColor); 
   }
  }
 }
 
 /* makeGraph draws the initial background for the graph
  * 
  */
 
 public void makeGraph(){
  
  GLine line = new GLine(0,GRAPH_MARGIN_SIZE,getWidth(),GRAPH_MARGIN_SIZE); 
  add(line); 
  line = new GLine(0,getHeight() - GRAPH_MARGIN_SIZE,getWidth(),getHeight()  - GRAPH_MARGIN_SIZE); 
  add(line); 
  
  GLabel yrLabel = new GLabel(String.valueOf(1900),1,getHeight());
  add(yrLabel); 
  
  int xSpacer = getWidth()/NDECADES; 
  for (int i = 1; i < NDECADES; i++){
   line = new GLine(i*xSpacer,0,i*xSpacer,getHeight());
   add(line); 
   yrLabel = new GLabel(String.valueOf(1900 + i*10),i*xSpacer + 1,getHeight());
   add(yrLabel); 
  }
  
 }
 
/* makeLine draws a line representing the popularity of a name over the decades. 
 * makeLine starts at 1900 and uses the helper method makeYCoord to translate
 * a name's ranking into the y coordinate system, creating a point for 
 * each decade. 
 * 
 * param entry: the name that 
 */
 
 
 public void makeLine(NameSurferEntry entry,Color lineColor){
	 
  GLabel label = new GLabel(entry.getName() + String.valueOf(entry.getRank(1900)),0,makeYCoord(entry.getRank(1900))); 
  add(label); 
  
  int xSpacer = getWidth()/NDECADES; 
  for (int i = 0; i < NDECADES - 1; i++){
   int x1 = i*xSpacer; 
   int x2 = x1 + xSpacer; 
   int decade = 1900 + i*10; 
   int nextDecade = decade+10; 
   int y1 = makeYCoord(entry.getRank(decade)); 
   int y2 = makeYCoord(entry.getRank(nextDecade));
   
   GLine line = new GLine(x1,y1,x2,y2); 
   line.setColor(lineColor);
   add(line); 
   label = new GLabel(properCase(entry.getName())+ String.valueOf(entry.getRank(nextDecade)),x2,y2); 
   add(label); 
  }
 }
 
 /* properCase is a convience method that capitalizes the 
  * first character of a string
  */
	
private static String properCase (String s) {
	    
	// Empty strings should be returned as-is.
	if (s.length() == 0) return "";

	// Strings with only one character uppercased.
    if (s.length() == 1) return s.toUpperCase();

    // Otherwise uppercase first letter, lowercase the rest.
	return s.substring(0,1).toUpperCase() + s.substring(1).toLowerCase();
} 
 
 /* makeYCoord takes the rank of a name for a particular year
  * and translates that rank into the y-coordinate system
  * 
  * param nameRank: an integer 
  */
 
 public int makeYCoord(int nameRank){
  int yCoord = 0; 
  if(nameRank == 0){
   yCoord = getHeight(); 
  } else { 
   double percentHeight = (nameRank-1.0)/MAX_RANK;
   yCoord = GRAPH_MARGIN_SIZE + (int)Math.round(percentHeight*(getHeight()-2*GRAPH_MARGIN_SIZE)); 
  }
  return yCoord; 
 }
 
 /* getNextColor is used to cycle through 4 colors (so the lines that 
  * represent names show up as different colors)  
  */
 
 private Color getNextColor(int colorCount){
  Color newColor = Color.BLACK;
  switch(colorCount){
   case 1: 
    newColor = Color.BLACK;
    colorCount = 2; 
    break; 
   case 2: 
    newColor = Color.RED; 
    colorCount = 3;
    break;
   case 3: 
    newColor = Color.BLUE; 
    colorCount = 4; 
    break;
   case 4: 
    newColor = Color.MAGENTA; 
    colorCount = 1; 
    break; 
  }
  return newColor; 
 }
 
 /* nextColorCount is a helper method that returns the 
  * next color. If the current color is 4, then we return 
  * 1, otherwise we return the prior color +1
  */
 
 
 private int nextColorCount(int nextInt){
  if(nextInt == 4){
   return 1;
  } else { 
   nextInt++; 
   return nextInt; 
  }
 }
 
 /* Implementation of the ComponentListener interface */
 public void componentHidden(ComponentEvent e) { }
 public void componentMoved(ComponentEvent e) { }
 public void componentResized(ComponentEvent e) { update(); }
 public void componentShown(ComponentEvent e) { }
 private NameSurferEntry lastEntry;
 private ArrayList<NameSurferEntry> nameArr = new ArrayList<NameSurferEntry>();
}
