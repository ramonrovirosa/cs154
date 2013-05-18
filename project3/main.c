#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "cachesim.h"

int main(int argc, char* argv[]){
	Cache* testDirect = createAndInitialize(4,64,0);
	int printMe;
	printMe = accessCache(testDirect,32);
	printMe = accessCache(testDirect,32);
	printMe = accessCache(testDirect,64);
	printMe = accessCache(testDirect,64);
	printMe = accessCache(testDirect,33);
	printf("accessCache: %d\n",printMe);
	return 0;
}
