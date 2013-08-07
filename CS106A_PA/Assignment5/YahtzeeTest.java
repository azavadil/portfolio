/**
 * Auto Generated Java Class.
 */

import java.util.*; 
import java.lang.*; 
import acm.util.*; 

public class YahtzeeTest {
    
    
    public static void main(String[] args) { 
        
        int[] testArr = new int[5]; 
        
        for(int i = 0; i < 100; i++) { 
            
            fillInDice(testArr); 
            Arrays.sort(testArr); 
            StdOut.println("testArr sorted = " + arrAsString(testArr)); 
            
            boolean res4 = isStraight(testArr, 4); 
            StdOut.println("isStraight4 = " + res4); 
            
            boolean res5 = isStraight(testArr, 5); 
            StdOut.println("isStraight5 = " +  res5); 
        }
                  
    }
    
    private static void fillInDice(int[] arr){
        for (int i = 0; i < arr.length; i++){ 
            Random r = new Random();
            arr[i] = r.nextInt(6) + 1 ; 
        }
    }

    private static boolean isStraight(int[] arr, int numRequired){
        int[] clone = new int[arr.length];
        for (int i = 0; i < arr.length; i++){
            clone[i] = arr[i];
        }
        
        Arrays.sort(clone);
        int count = 0; 
        for(int j = 0; j < clone.length-1; j++){
            
            // if the cards are ascending order, increment the count
            if(clone[j]+1 == clone[j+1]) count++; 
            
            // if cards are not in ascending order or equal, the streak is broken
            if(clone[j]+1 != clone[j+1] && clone[j] != clone[j+1]) break; 
   
            
        }
        boolean result = (count >= numRequired-1);
        return result; 
    }
    
    private static String arrAsString(int [] arr){
        String res = ""; 
        
        for(int elem : arr) { 
            res += Integer.toString(elem); 
        }
        return res; 
    }
}



