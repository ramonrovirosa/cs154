#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "branchsim.h"

int main(int argc, char* argv[]){
	BP *bp = createAndInitialize(3);
	int i;
	for(i=0;i<3;i++){
		printf("bp[%d] = %d\n",i, bp->ptable[i]);
	}
/*	printf("prediction @ %d	= %d\n", 0, accessBranchPredictor(bp, 0));	
	printf("# of accesses	= %d\n",numAccesses(bp)); 
	printf("# of mispredicts= %d\n", numMispredictions(bp));
	printf("prediction @ %d	= %d\n", 1, accessBranchPredictor(bp, 1));	
	printf("# of accesses	= %d\n",numAccesses(bp)); 
	printf("# of mispredicts= %d\n", numMispredictions(bp));
	printf("prediction @ %d	= %d\n", 2, accessBranchPredictor(bp, 2));	
	printf("# of accesses	= %d\n",numAccesses(bp)); 
	printf("# of mispredicts= %d\n", numMispredictions(bp));
*/

	printf("BEFORE bp[%d] =		%d\n",0, bp->ptable[0]);
	printf("prediction @ %d	=	%d\n", 0, accessBranchPredictor(bp, 0));	
	updateBranchPredictor(bp, 0, 0);
	printf("actual result =		%d\n", 0);
	printf("# of mispredicts= %d\n", numMispredictions(bp));
	printf("AFTER bp[%d] =		%d\n",0, bp->ptable[0]);
	printf("\n");
	
	printf("BEFORE bp[%d] =		%d\n",0, bp->ptable[0]);
	printf("prediction @ %d	=	%d\n", 0, accessBranchPredictor(bp, 0));	
	updateBranchPredictor(bp, 0, 1);
	printf("actual result =		%d\n", 1);
	printf("# of mispredicts=	%d\n", numMispredictions(bp));
	printf("AFTER bp[%d] =		%d\n",0, bp->ptable[0]);
	printf("\n");

	printf("BEFORE bp[%d] =		%d\n",0, bp->ptable[0]);
	printf("prediction @ %d	=	%d\n", 0, accessBranchPredictor(bp, 0));	
	updateBranchPredictor(bp, 0, 1);
	printf("actual result =		%d\n", 1);
	printf("# of mispredicts=	%d\n", numMispredictions(bp));
	printf("AFTER bp[%d] =		%d\n",0, bp->ptable[0]);
	printf("\n");
	
	printf("BEFORE bp[%d] = 	%d\n",0, bp->ptable[0]);
	printf("prediction @ %d	= 	%d\n", 0, accessBranchPredictor(bp, 0));	
	updateBranchPredictor(bp, 0, 1);
	printf("actual result =		%d\n", 1);
	printf("# of mispredicts= %d\n", numMispredictions(bp));
	printf("AFTER bp[%d] = 		%d\n",0, bp->ptable[0]);
	printf("\n");

	printf("BEFORE bp[%d] = 	%d\n",0, bp->ptable[0]);
	printf("prediction @ %d	= 	%d\n", 0, accessBranchPredictor(bp, 0));	
	updateBranchPredictor(bp, 0, 0);
	printf("actual result =		%d\n", 0);
	printf("# of mispredicts= %d\n", numMispredictions(bp));
	printf("AFTER bp[%d] = 		%d\n",0, bp->ptable[0]);
	printf("\n");

	printf("BEFORE bp[%d] = 	%d\n",0, bp->ptable[0]);
	printf("prediction @ %d	= 	%d\n", 0, accessBranchPredictor(bp, 0));	
	updateBranchPredictor(bp, 0, 0);
	printf("actual result =		%d\n", 0);
	printf("# of mispredicts= %d\n", numMispredictions(bp));
	printf("AFTER bp[%d] = 		%d\n",0, bp->ptable[0]);
	printf("\n");




free(bp);
}

