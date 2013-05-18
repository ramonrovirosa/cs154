#ifndef CACHESIM_H
#define CACHEISM_H


typedef struct _block{

  int tag;
  int valid;
	int LRUflag;
}Block;



//You have a struct that contains all of the information for one cache.
typedef struct _cache{
  Block*cacheArray1; //array of addresses
  Block*cacheArray2; //array of addresses
  Block*cacheArray3; //array of addresses
  Block*cacheArray4; //array of addresses
  
  int type;//type 1-direct,2-pseudo,3-fourway.
 
  int lruOrder[4];
 
  int blocksize;   //
  int totalCacheSize;

  int numBlocks;
  int numSets;

  int offset;
  int indexSize;
  
  int missesSoFar;
  int accessesSoFar;
  int totalAccessTime;

} Cache;



Cache *createAndInitialize(int blocksize, int cachesize, int type);
//In this function, you create the cache and initialize it, returning a pointer to the struct. Because you are determining the struct, you return a void * to our main. Type 0 is a direct-mapped cache. Type 1 is a pseudo-associative cache. Type 2 is a 4-way set associative cache. 

int accessCache(Cache *cache, int address);
//In this function, we access the cache with a particular address. If the address results in a hit, return 1. If it is a miss, return 0.

int missesSoFar(Cache *cache);
//This returns the number of misses that have occurred so far

int accessesSoFar(Cache *cache);
//This returns the number of accesses that have occurred so far

int totalAccessTime(Cache *cache);
//This returns the total number of cycles that all of the accesses have taken so far.

int _log2( int x ); 

int _pow2( int x );

int whereIs(Cache* cache, int targetWord);

void reorder(Cache* cache, int foundAt);

#endif
