#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "cachesim.h"

//In this function, you create the cache and initialize it, returning a pointer to the struct. Because you are determining the struct, you return a void * to our main. Type 0 is a direct-mapped cache. Type 1 is a pseudo-associative cache. Type 2 is a 4-way set associative cache. 
Cache *createAndInitialize(int blocksize, int cachesize, int type){
   	Cache *newcache = malloc(sizeof(struct _cache));
	
	newcache->type = type;

	newcache->blocksize = blocksize;
	newcache->totalCacheSize = cachesize;

	//fix division not allowed
	newcache->numBlocks= (int)cachesize/blocksize;
	newcache->indexSize= _log2(newcache->numBlocks);
	newcache->offset=_log2(blocksize);

	newcache->missesSoFar=0;
	newcache->accessesSoFar=0;
	newcache->totalAccessTime=0;

	newcache->cacheArray = malloc(sizeof(struct _block)*(newcache->numBlocks));
	int i;
	for(i=0;i<newcache->numBlocks;i++){
	  newcache->cacheArray[i].valid=0;
	}

	if(newcache->cacheArray == NULL)
	  return NULL;

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
  //printf("after addr is shifted by offset: %d\n",(address>> cache->offset));
  int index = (address >> cache->offset) & (_pow2(cache->blocksize)-1);
  int tag = address >> (cache->offset + cache->indexSize);

  cache->accessesSoFar++;

  switch(cache->type){
    //direct
  case 0:

    if(cache->cacheArray[index].valid == 1 && cache->cacheArray[index].tag == tag){
      cache->totalAccessTime+=1;
      return 1;
    }
    else{
      //if(cache->cacheArray[index].valid == 0 ||
      //((cache->cacheArray[index].tag != tag) && cache->cacheArray[index].valid == 1) ){
      cache->cacheArray[index].valid = 1;
      cache->cacheArray[index].tag = tag;
      
      cache->missesSoFar++;
      cache->totalAccessTime+=101;
      
      return 0;	
    }

  //2-associative
  case 1:

    break;

  //4-associative
  case 2:

    break;
  }

  cache->totalAccessTime+=1;
  return 1;
}

//This returns the number of misses that have occurred so far
int missesSoFar(Cache *cache){
  return cache->missesSoFar;
}

//This returns the number of accesses that have occurred so far
int accessesSoFar(Cache *cache){
  return cache->accessesSoFar;
}

//This returns the total number of cycles that all of the accesses have taken so far. 
int totalAccessTime(Cache *cache){
   return cache->totalAccessTime;
}


int _log2( int x )
{
  int ans = 0 ;
  while( x>>=1 ) ans++;
  return ans ;
}

int _pow2( int x )
{
  return 1<<x;
}
