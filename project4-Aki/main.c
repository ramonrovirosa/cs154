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
	printf("prediction @ %d = %d\n", 0, accessBranchPredictor(bp, 0));	
	printf("prediction @ %d = %d\n", 1, accessBranchPredictor(bp, 1));	
	printf("prediction @ %d = %d\n", 2, accessBranchPredictor(bp, 2));	
	free(bp);
}

