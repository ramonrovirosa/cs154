#ifndef CACHESIM_H
#define CACHEISM_H

//You have a struct that contains all of the information for one cache.
typedef struct _cache{
  int *cacheArray; //array of addresses
  
  int type;//type 1-direct,2-pseudo,3-fourway.
  
  int blocksize;   //
  int totalCacheSize;

  int missesSoFar;
  int accessesSoFar;
  int totalAccessTime;

} Cache;


Cache *createAndInitialize(int blocksize, int cachesize, int type);
//In this function, you create the cache and initialize it, returning a pointer to the struct. Because you are determining the struct, you return a void * to our main. Type 0 is a direct-mapped cache. Type 1 is a pseudo-associative cache. Type 2 is a 4-way set associative cache. 

int accessCache(void *cache, int address);
//In this function, we access the cache with a particular address. If the address results in a hit, return 1. If it is a miss, return 0.

int missesSoFar(void *cache);
//This returns the number of misses that have occurred so far

int accessesSoFar(void *cache);
//This returns the number of accesses that have occurred so far

int totalAccessTime(void *cache);
//This returns the total number of cycles that all of the accesses have taken so far. 

#endif
