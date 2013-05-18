#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "cachesim.h"

int main(int argc, char* argv[]){
	Cache* testDirect = createAndInitialize(4,64,0);
	int printMe;
	printMe = accessCache(testDirect,32);
	printf("access 32: %d\n",printMe);
	printMe = accessesSoFar(testDirect);
	printf("accessesSoFar: %d\n",printMe);
	printMe = accessCache(testDirect,32);
	printf("access 32: %d\n",printMe);
	printMe = accessCache(testDirect,64);
	printf("access 64: %d\n",printMe);
	printMe = accessCache(testDirect,64);
	printf("access 64: %d\n",printMe);
	printMe = accessCache(testDirect,33);
	printf("access 33: %d\n",printMe);
	printMe = accessCache(testDirect,96);
	printf("access 96: %d\n",printMe);
	printMe = accessCache(testDirect,33);
	printf("access 33: %d\n",printMe);
	return 0;
}
