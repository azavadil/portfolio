/*
 * File: Yahtzee.java
 * ------------------
 * This program will eventually play the Yahtzee game.
 */

import acm.io.*;
import acm.program.*;
import acm.util.*;
import java.util.*; 

public class Yahtzee extends GraphicsProgram implements YahtzeeConstants {
 
 public static void main(String[] args) {
  new Yahtzee().start(args);
 }
 
 
 /* run sets up the initial environment and then calls playGame() 
  * to play a round of the game 
  */ 
 
 public void run() {
  IODialog dialog = getDialog();
  nPlayers = dialog.readInt("Enter number of players");
  playerNames = new String[nPlayers];
  for (int i = 1; i <= nPlayers; i++) {
   playerNames[i - 1] = dialog.readLine("Enter name for player " + i);
  }
  display = new YahtzeeDisplay(getGCanvas(), playerNames);
  scorecard = new int[nPlayers][N_CATEGORIES];
  playGame();
 }
 
 
 // playGame plays the actual game
 
 private void playGame(){  
  loadScorecard();
  for (int i = 0; i < 13; i++){
   playRound();
  }
  calcUpper();
  calcLower();
  calcTotal();
  updateFinalScores(UPPER_SCORE);
  updateFinalScores(UPPER_BONUS);
  updateFinalScores(LOWER_SCORE);
  updateFinalScores(TOTAL);
 }
 
 /* playRound plays a single round of yahtzee
  * 1 turn for each player
  */ 
 
 private void playRound(){
  
  for (int i = 1; i <= nPlayers; i++){
   display.waitForPlayerToClickRoll(i);
   roll5(); 
   roll2_3(); 
   int category = safeWaitForPlayerToSelectCategory(numPlayerToArrIndex(i));
   boolean isValidDice = checkSelection(category); 
   if(isValidDice) { 
       display.printMessage("isValidDice = true");
   } else { 
       display.printMessage("isValidDice = false");
   }
   int roundScore = getScore(isValidDice,category);
   scorecard[numPlayerToArrIndex(i)][categoryToArrIndex(category)] = roundScore; 
   display.updateScorecard(category,i,roundScore); 
  }
  
 }

 /* convience function to convert a player index (e.g. 1-5) to 
  * an array index (e.g. 0-4)
  */ 
 
 private int numPlayerToArrIndex(int player){
  return player - 1; 
 }
 
 /* convience function to convert a category index (e.g. 1-5) to 
  * an array index (e.g. 0-4)
  */
 
 private int categoryToArrIndex(int category){
  category = category -1; 
  return category; 
 }
 
 /* loadScorecard sets the initial values for all categories in the 
  * scorecard instance variable to 999. We use 999 to represent a category 
  * that hasn't been played
  */ 
 
 private void loadScorecard(){
  for(int i = 0; i < nPlayers; i++){
   for (int j = 0; j < N_CATEGORIES; j++){
    scorecard[i][j] = 999;
   }
  }
 }
 
 /* updateFinalScores updates the score
  */
 
 private void updateFinalScores(int category){
  for (int i = 1; i <= nPlayers; i++){
   display.updateScorecard(category,i,scorecard[i-1][category-1]);
  }
 }
 
 /* calculate the score for the upper part of the scoreboard
  * if the cumulative score for the upper section of the scoreboard
  * is greater than 63, then there is a 35 point bonus
  */ 
 
 private void calcUpper() { 
  for (int i = 0; i < nPlayers; i++){
   int upperTotal = 0; 
   for (int j=0; j<UPPER_SCORE-1; j++){
    upperTotal += scorecard[i][j];
   }
   scorecard[i][UPPER_SCORE-1] = upperTotal; 
   if(upperTotal >= 63){
    scorecard[i][UPPER_BONUS-1] = 35; 
   } else {
    scorecard[i][UPPER_BONUS-1] = 0;
   }
  }
  
 }
 
 /* calculate the score for the lower part of the scoreboard
  */ 
 
 private void calcLower() { 
  for (int i = 0; i < nPlayers; i++){
   int lowerTotal = 0; 
   for (int j=THREE_OF_A_KIND -1; j<LOWER_SCORE-1; j++){
    lowerTotal += scorecard[i][j];
   }
   scorecard[i][LOWER_SCORE-1] = lowerTotal; 
  }
 }
 
 /* calculate the total score */ 
 
 private void calcTotal(){
  for (int i = 0; i < nPlayers; i++){
   scorecard[i][TOTAL-1] = scorecard[i][UPPER_SCORE-1]+ scorecard[i][UPPER_BONUS-1] + scorecard[i][LOWER_SCORE-1]; 
  }
 }
 
 
 /* precondition: player has rolled the dice 3 times and selected a category
  * The selected category should not have have been previously selected (which we check)
  * 
  * If the category has not been previously selected we return the category
  * otherwise we print an error messsage for the player and wait for the 
     * player to select a valid category. 
  *
  * param player: represents the current player
  */ 
  
 private int safeWaitForPlayerToSelectCategory(int player){
  int category = 0; 
  while(true){
   category = display.waitForPlayerToSelectCategory();
   if(checkCategory(player, categoryToArrIndex(category))) break;
   display.printMessage("Invalid Category");
  }
  return category; 
 }

 /* precondition: category should not be previously updated 
  * we initially set the categories to 999. checkCategory returns
  * true if the category is 999 (indicating it has not been modified) 
  * and false otherwise
  * 
  * param player: the index representing the selected player
  * 
  * param category: the indexing representing the selected category
  */ 
 
 private boolean checkCategory(int player, int category){
  boolean result = false;
  if(scorecard[player][category]==999){
   result = true; 
  }
  return result;
 }
 
 
 // roll5 is used for the players first roll (i.e. all dice are rolled)
 
 private void roll5(){
  for(int i=0; i<N_DICE; i++) {
   diceArr[i] = rgen.nextInt(1,6); 
  }
  display.displayDice(diceArr);
 }
 
 /* roll2_3 is used when it is not the initial roll 
  * roll2_3 waits for the player to select die to reroll
  * before calling reroll()
  */ 
 
 private void roll2_3(){
  for(int i = 0; i < 2; i++){   
   display.waitForPlayerToSelectDice();
   reroll();
   display.displayDice(diceArr);
  }
 }
 
 /* precondition: player has selected dice to reroll 
     * reroll is a helper method for roll2_3
  * once the player has selected the dice reroll is called
  * to roll each die that has been selected
  */
 
 private void reroll(){
  for(int i = 0; i < N_DICE; i++){ 
   if(display.isDieSelected(i)){
    diceArr[i] = rgen.nextInt(1,6);
   }
  }
 }
 
 /* getScore calculates the value  
  */ 
  
 private int getScore(boolean isValid, int category){
  int result = 0; 
  if(!isValid){
   return result;
  }
  switch(category){
   case 1:
    result = getUpper(1);
    break;
   case 2:
    result = getUpper(2);
    break;
   case 3:
    result = getUpper(3);
    break; 
   case 4: 
    result = getUpper(4);
    break;
   case 5:
    result = getUpper(5);
    break;
   case 6: 
    result = getUpper(6);
    break; 
   case 9: 
    result = getSum();
    break;
   case 10: 
    result =  getSum();
    break;
   case 11: 
    result = 25; 
    break; 
   case 12: 
    result = 30; 
    break;
   case 13: 
    result = 40; 
    break;
   case 14: 
    result = 50; 
    break; 
   case 15: 
    result = getSum(); 
    break; 
  }
  return result; 
 }
 
 
 /* getUpper returns the value for the upper part of the scoreboard
  * For the upper part of the scoreboard the score is simply the 
  * cumulative total for the specified die
  * e.g. if die 4 is specified and the player has three 4's, then 
  * the upper score is 12 (i.e. 3 x 4) 
  */ 
 
 private int getUpper(int numCase){
  int result = 0;
  for(int dice: diceArr){
   if(dice == numCase){
    result += dice;
   }
  }
  return result; 
 }
 
 /* getSum returns the cumulative total for all the dice. 
  * getSum applies for cases like Three of a Kind and 
  * Four of a Kind
  */ 
 
 private int getSum(){
  int result = 0; 
  for(int dice: diceArr){
   result += dice; 
  }
  return result; 
 }
 
 /* checkSelection takes the category selected by the player and checks whether 
  * the dice satisfy the requirements of the category 
  * 
  * param category: represents the category selected by the player
  */ 
 
 private boolean checkSelection(int category){
  boolean result = false; 
  switch(category){
   case 1: 
    result = true; 
    break;
   case 2: 
    result = true;
    break; 
   case 3: 
    result = true;
    break;
   case 4:
    result = true; 
    break;
   case 5:
    result = true; 
    break; 
   case 6:
    result = true;
    break;
   case 9:
    result = numOfKind(3);
    break;
   case 10:
    result = numOfKind(4);
    break;
   case 11:
    result = fullHouse() || numOfKind(5);
    break;
   case 12:
    result = isStraight(4);
    break;
   case 13:
    result = isStraight(5); 
    break;
   case 14:
    result = numOfKind(5);
    break;
   case 15: 
    result = true;
    break;
  }
  return result; 
 }
 
 /* isStraight returns true if we have a straight of the specified length
  * and false otherwise
  *
  * implemented by converting into a sorting problem. we clone the dice 
  * and sort the cloned array. Once the array is sorted we can easily check 
  * if we have a straight. we increment count as long as cards are in ascending
  * order 
  *
  * param numRequired: required length of the straight 
  */ 
 
 private boolean isStraight(int numRequired){
  int[] clone = new int[N_DICE];
  for (int i = 0; i < N_DICE; i++){
   clone[i] = diceArr[i];
  }
  Arrays.sort(clone);
  int count = 0; 
  for(int j = 0; j < clone.length-1; j++){
   // if cards are not in ascending order or equal, the streak is broken
   if(clone[j]+1 != clone[j+1] && clone[j] != clone[j+1]) break; 
   
   // if the cards are ascending order, increment the count
   if(clone[j]+1 == clone[j+1]) count++; 
  }
  boolean result = (count >= numRequired - 1);
  return result; 
 }
 
 /* fullHouse returns true if we have a fullHouse, false otherwise
  *
  * fullHouse calculates the frequency counts for an array and returns
  * true if we have a frequency count of 2 & 3 (representing a full house) 
  * and false otherwise
  */ 
 
 private boolean fullHouse(){
  boolean result = false; 
  int[] counts = new int[N_DICE]; 
  for (int i = 0; i < N_DICE; i++){
   counts[i] = arrayCount(diceArr,diceArr[i]);
  }
  boolean twos = false; 
  boolean threes = false; 
  for (int count: counts){
   if(count == 2){
    twos = true; 
   } else if(count == 3){
    threes = true;
   }
  }
  result = (twos && threes);
  return result; 
 }
 
 
 /* arrayCount returns the number of times an element
  * appears in an array
  * 
  * param arr: an array 
  * 
  * param element: the element we're calulating a count of
  */ 
 
 private int arrayCount(int[] arr, int element){
  int count = 0;
  for(int eachElement: arr){
   if(eachElement == element){
    count += 1;
   }
  }
  return count; 
 }
 
 /* numOfKind returns true if we have a frequency count
  * for any element in the dice array that is greater than 
  * or equal to numKind, returns false otherwise. 
  * 
  * param numKind: the frequency count that we are requiring
  * (e.g. numKind = 3 means we require 3 die with the same value
     */   
 
 private boolean numOfKind(int numKind){
  boolean result = false; 
  for(int dice: diceArr){
   if(arrayCount(diceArr,dice)>=numKind){
    result = true; 
   }
  }
  return result;
 }
 
 
 
/* Private instance variables */
 private int nPlayers;
 private String[] playerNames;
 private YahtzeeDisplay display;
 private RandomGenerator rgen = new RandomGenerator();
 private int[] diceArr = new int[N_DICE]; 
 private HashMap<Integer, Integer> categories = new HashMap<Integer, Integer>();
 private int[][] scorecard; 
}
