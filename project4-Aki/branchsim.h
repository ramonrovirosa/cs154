#ifndef BRANCHSIM_H
#define BRANCHSIM_H

typedef struct _bp{
	int *ptable;
	int tablesize;
	int accesses;
	int mispredictions;
}BP;

BP* createAndInitialize(int numEntries); 
int accessBranchPredictor(BP *bp, int PC); 
void updateBranchPredictor(BP *bp, int PC, int result); 
int numAccesses(BP *bp); 
int numMispredictions(BP *bp); 

#endif
