#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "cachesim.h"

void main(){

}

//In this function, you create the cache and initialize it, returning a pointer to the struct. Because you are determining the struct, you return a void * to our main. Type 0 is a direct-mapped cache. Type 1 is a pseudo-associative cache. Type 2 is a 4-way set associative cache. 
Cache *createAndInitialize(int blocksize, int cachesize, int type){
   	Cache *newcache = (Cache *) malloc(sizeof(Cache));
	
	newcache->type = type;

	newcache->blocksize = blocksize;
	newcache->totalCacheSize = cachesize;

	//fix division not allowed
	newcache->numBlocks= (int)cachesize/blocksize;
	newcache->indexSize= _log2(newcache->numBlocks);
	newcache->offset=_log2(blocksize);

	newcache->cacheArray = (int*)malloc(sizeof(int)*(newcache->numBlocks));
	if(newcache->cacheArray == NULL)
	  return 0;

	switch(type){
	case 0:
	  //direct
	  break;
	case 1:
	  //2-associative
	   newcache->numSets=cachesize/blocksize/2;
	  break;
	case 2:
	  //4 assosciative
	  newcache->numSets=cachesize/blocksize/4;
	  break;
	}

	return newcache;
}

//In this function, we access the cache with a particular address. If the address results in a hit, return 1. If it is a miss, return 0.
int accessCache(Cache *cache, int address){


  return 0;
}

//This returns the number of misses that have occurred so far
int missesSoFar(Cache *cache){


   return 0;
}

//This returns the number of accesses that have occurred so far
int accessesSoFar(Cache *cache){

   return 0;
}

//This returns the total number of cycles that all of the accesses have taken so far. 
int totalAccessTime(Cache *cache){

   return 0;
}


int _log2( int x )
{
  int ans = 0 ;
  while( x>>=1 ) ans++;
  return ans ;
}
