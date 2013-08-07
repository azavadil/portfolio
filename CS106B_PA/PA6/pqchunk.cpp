/* The file implements the priority queue class as a hybrid 
 * linked-list/array list of integers in decreasing order
 * This makes it easy to dequeue the max (it's right in front), 
 * but hard to enqueue since we have to search for the proper 
 * position to insert the new cell.
 *
 * The key data structure is a struct Cell that has an array field
 * 
 * By chunking the date into arrays we can achieve faster insert times
 * than a linked list because we step through the list an array at a 
 * time instead of one number at a time. 
 *
 * Private section of pqueue.h modidied to match below
 *
 * struct cellT {
 *	int cnt;
 *	int numUsed;
 *	int *arr;
 *	cellT *next;
 *	};
 *
 *	void insertInOrder(int n, cellT *curCell);
 *	void splitArr(cellT *filledCell);
 *	void collectCell( cellT *garbage); 
 *
 *
 *	cellT *head;  
 *	int maxElemsPerBlock; 
 *	int count; 
 */




#include "pqueue.h"s
#include <iostream> 
#include "simpio.h"

PQueue::PQueue()
{	
	head = NULL;
	maxElemsPerBlock = 10; 
	count = 0; 
}

PQueue::~PQueue()
{
	while (head != NULL) {
    	cellT *next = head->next;
    	collectCell(head);
    	head = next;
    }
};


/* Implementation notes: splitArr
 * ------------------------------
 * splitArr is a private helper method that is called
 * when the array field of a cell is full. 
 *
 * splitArr allocates a new cell, xopies the tail half of 
 * the data into the new cell, and splices the new cell 
 * into the list. 
 *
 * Note that splitArr does the splicing and a pointer
 * for the new cell is not explicitly returned. 
 */ 

void PQueue::splitArr(cellT *filledCell) { 
	cellT *tailHalf = new cellT; 
	tailHalf->arr = new int[maxElemsPerBlock]; 
	
	int mid = maxElemsPerBlock / 2; 
	
	for (int i = mid; i < maxElemsPerBlock; i++) { 
		tailHalf->arr[i - mid] = filledCell->arr[i]; 
	} 
	
	filledCell->numUsed =  mid; 
	tailHalf->numUsed = (maxElemsPerBlock % 2 == 0)? mid : mid + 1;  
	
	tailHalf->next = filledCell->next;
	filledCell->next = tailHalf;
}


/* Implementation notes: insertInOrder
 * -----------------------------------
 * insertInOrder is a private helper method that inserts 
 * an element into the array field of a cell
 * 
 */ 
void PQueue::insertInOrder(int n, cellT *curCell) { 
	
	int idx; 
	for (idx = 0; idx < curCell->numUsed; idx++) { 
		if (n > curCell->arr[idx]) break; 
	} 
	
	for (int i = curCell->numUsed - 1; i >= idx; i--) { 
		curCell->arr[i+1] = curCell->arr[i]; 
	} 
	curCell->arr[idx] = n; 
	curCell->numUsed++;	
}		

/* Implementation notes: collectCell
 * ---------------------------------
 * collectCell is a private helper method 
 * that performs the garbage collection for 
 * a cell, deallocating both the array and 
 * then the cell itself
 */ 

void PQueue::collectCell(cellT *garbage) { 
	delete[] garbage->arr; 
	delete garbage; 
	garbage = NULL; 
}


bool PQueue::isEmpty()
{
	return (head == NULL);
}

int PQueue::size()
{
	return count; 
}

/* Implementation notes: enqueue
 * -----------------------------
 * We have to find the proper position which is tricky 
 * with a singly linked list/array hybrid. We walk two pointers
 * one a step behind the other until we find the correct position. 
 * Once we find the correct position, we call the private helper method
 * to make the insertion into the array 
 * 
 * Note the special case of inserting at the head.
 */ 

void PQueue::enqueue(int n)
{
	cellT *cur, *prev; 
		
	for (prev = NULL, cur = head; cur != NULL; prev = cur, cur = cur->next){ 
		if ( n > cur->arr[0] ) break;
	}
	
	if (prev) { 
			
		/* check if the chunk is full */
		if ( prev->numUsed == maxElemsPerBlock) { 
			splitArr(prev); 
			cellT *bottomHalf = prev->next; 
			
			if (n > bottomHalf->arr[0]) {     /*n goes in head half*/ 
 				
				insertInOrder(n, prev); 
			} else {                        /* n goes in tail half */ 
				insertInOrder(n, bottomHalf); 			 
			}
					
		} else {           /* the new value goes in prev */ 
			insertInOrder(n, prev); 
		}
	} else {  
		if (head) {        /* we gave a head cell */ 
			if (cur->numUsed == maxElemsPerBlock ) { 
				splitArr(cur); 
			
				cellT *bottomHalf = cur->next; 
			
				if (n > bottomHalf->arr[0] ) { /*n goes in head half*/ 
					insertInOrder(n, cur); 
				} else {                       /* n goes in tail half */ 
					insertInOrder(n, bottomHalf); 
				}
			} else { 
				insertInOrder(n, cur); 
			}
		} else {         /* no head, this is the first entry so make head cell */ 
			cellT *newOne = new cellT;
			newOne->arr = new int[maxElemsPerBlock];
			newOne->arr[0] = n; 
			newOne->numUsed = 1;
			newOne->next = NULL;
			head = newOne;
		} 
	}
	count++; 
}


/* Implementation notes: dequeueMax
 * --------------------------------
 * The maximum value is kept at the head of the list, so it's easy to find and
 * remove. Note we take care to free the memory for the deleted cell.
 */

int PQueue::dequeueMax()
{
	if (isEmpty()) Error("Tried to dequeueMax from an empty pqueue");
	
	// store away the top value
	int top = head->arr[0];
	// shuffle the values to the front of the array 
	for (int i = 0; i < head->numUsed; i++) { 
		head->arr[i] = head->arr[i+1]; 
	} 
	head->numUsed--; 
	
	if (head->numUsed == 0 ) {	/*head of the list is now empty*/ 
		if (head->next != NULL) { 
			/*store pointer to head	to use for garbage collection */ 
			cellT *old = head; 
			head = head->next; 
			collectCell(old); 
		} else { /*head->next == NULL, entire list is empty*/
			cellT *old = head; 
			head = NULL; 
			collectCell(old); 
		}
	}
	count--; 
	return top;
}


string PQueue::implementationName()
{
	return "chunklist queue"; 
}

int PQueue::bytesUsed()
{
	int total = sizeof(*this);
	for (cellT *cur = head; cur != NULL; cur = cur->next)
		total += sizeof(*cur);
	
	return total;
}


void PQueue::printDebuggingInfo()
{
	cout << "------------------ START DEBUG INFO ------------------" << endl;
	cout << "Pqueue contains " << count << " entries" << endl;
	for (int i = 0; i < count; i++) 
		cout << "COMPLETE IMPLEMENTATION" << " ";
	cout << endl;
	cout << "------------------ END DEBUG INFO ------------------" << endl;
}

