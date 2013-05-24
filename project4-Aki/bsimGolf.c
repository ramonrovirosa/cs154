#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "bsimGolf.h"

BP *createAndInitialize(int numEntries){ //Allocate a table with numEntries entries, each initialized to the not-taken state. numEntries will be a power of 2.
	BP *newBP = malloc(sizeof(BP));
	*newBP = (BP){.tablesize=0, .accesses=0, .mispredictions=0};
	newBP->ptable = calloc(numEntries,sizeof(int)); // avoids for loop to initialize to 0s
	return newBP;
}
int accessBranchPredictor(BP *bp, int PC){ //Given the PC of a branch, report whether this branch is predicted as taken or not taken. (1 = taken, 0 = not taken)
	int index = (PC & (bp->tablesize - 1));
	int state = bp->ptable[index];
	bp->accesses++;
	return (state == 2 || state == 3) ? 1 : 0; //if state 2 or 3 return 1 else return 0
}
void updateBranchPredictor(BP *bp, int PC, int result){ //Given the PC of the branch and the actual result (1 = taken, 0 = not taken), update the branch predictor for that entry in the table.
	int index = (PC & (bp->tablesize - 1));
	int prediction = bp->ptable[index];
	
	switch(prediction){
		case 0:{
			if(result == 1){
				bp->ptable[index] = 1;
				bp->mispredictions++;	
			} break;
		}case 1:{
			if(result == 1){
				bp->ptable[index] = 3;
				bp->mispredictions++;
			}else{ bp->ptable[index] = 0;}
		break;
		}case 2:{
			if(result == 0){
				bp->ptable[index] = 0;
				bp->mispredictions++;
			}
			else{ bp->ptable[index] = 3;}
		break;
		}case 3:{
			if(result == 0){
				bp->ptable[index] = 2;
				bp->mispredictions++;
			}
		}
	}//end switch	
}	
int numAccesses(BP *bp){ return bp->accesses;}
int numMispredictions(BP *bp){ return bp->mispredictions;}