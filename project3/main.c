#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "cachesim.h"

int main(int argc, char* argv[]){
	Cache* testDirect = createAndInitialize(8,64,1);
	int printMe;
	int i;
	for(i=0;i<109;i+=4){
		printMe = accessCache(testDirect,i);
		printf("%d: Hit? %d\n",i,printMe);
	}
	for(i=0;i<109;i+=4){
		printMe = accessCache(testDirect,i);
		printf("%d: Hit? %d\n",i,printMe);
	}
}
