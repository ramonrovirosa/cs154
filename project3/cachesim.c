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
    if(newcache->type==1){
        newcache->numBlocks= (int)(cachesize/blocksize)>>1;
        //printf("numBlocks for 2 way is %d\n"
    }
    else if(newcache->type==2)
       newcache->numBlocks= (int)(cachesize/blocksize);
      // newcache->numBlocks= (int)(cachesize/blocksize)>>2;
    else
        newcache->numBlocks= (int)(cachesize/blocksize);
    
    //printf("numBlocks is %d\n",newcache->numBlocks);
    newcache->indexSize= _log2(newcache->numBlocks);
    newcache->offset=_log2(blocksize);

    //printf("index size is %d\n",newcache->indexSize);	

    newcache->missesSoFar=0;
    newcache->accessesSoFar=0;
    newcache->totalAccessTime=0;

    newcache->cacheArray1 = malloc(sizeof(struct _block)*(newcache->numBlocks));
    newcache->cacheArray2 = malloc(sizeof(struct _block)*(newcache->numBlocks));
    newcache->cacheArray3 = malloc(sizeof(struct _block)*(newcache->numBlocks));
    newcache->cacheArray4 = malloc(sizeof(struct _block)*(newcache->numBlocks));
    int i;
    for(i=0;i<newcache->numBlocks;i++){
      newcache->cacheArray1[i].valid=0;
      newcache->cacheArray2[i].valid=0;
      newcache->cacheArray3[i].valid=0;
      newcache->cacheArray4[i].valid=0;
    }

    if(newcache->cacheArray1 == NULL || 
       newcache->cacheArray2 == NULL || 
       newcache->cacheArray3 == NULL || 
       newcache->cacheArray4 == NULL)
      return NULL;
    
    newcache->lruOrder[0]=1;
    newcache->lruOrder[1]=2;
    newcache->lruOrder[2]=3;
    newcache->lruOrder[3]=4;
    
    return newcache;
}

//In this function, we access the cache with a particular address. If the address results in a hit, return 1. If it is a miss, return 0.
int accessCache(Cache *cache, int address){
  //printf("addr is shifted by offset %d and masked by %d to get the index\n",(cache->offset), (cache->numBlocks-1));
  //the old way int index = (address >> cache->offset) & (_pow2(cache->blocksize)-1);
  int index = (address >> cache->offset) & (cache->numBlocks-1);
  int tag = address >> (cache->offset + cache->indexSize);
  //printf("The index is %d, the tag is %d\n",index,tag);
  cache->accessesSoFar++;

  switch(cache->type){
    //direct
  case 0:

    if((cache->cacheArray1[index].valid == 1) && (cache->cacheArray1[index].tag == tag)){
      cache->totalAccessTime+=1;
      return 1;
    }
    else{
      //if(cache->cacheArray1[index].valid == 0 ||
      //((cache->cacheArray1[index].tag != tag) && cache->cacheArray1[index].valid == 1) ){
      cache->cacheArray1[index].valid = 1;
      cache->cacheArray1[index].tag = tag;
      
      cache->missesSoFar++;
      cache->totalAccessTime+=101;
      
      return 0;	
    }
    break;
  //2-associative
  case 1:
    if(cache->cacheArray1[index].valid == 0 && cache->cacheArray2[index].valid == 0){
      		cache->cacheArray1[index].valid = 1;
      		cache->cacheArray1[index].tag = tag;
      		cache->cacheArray2[index].tag =-101010;
        cache->cacheArray2[index].valid = 1;
      		cache->missesSoFar++;
          	cache->totalAccessTime+=102;
        return 0;
    }else if(cache->cacheArray1[index].tag == tag || cache->cacheArray2[index].tag == tag){
      cache->totalAccessTime+=1;
        if (cache->cacheArray2[index].tag == tag){
            int temp = cache->cacheArray1[index].tag;
            cache->cacheArray1[index].tag = cache->cacheArray2[index].tag;
            cache->cacheArray2[index].tag =temp;
            cache->totalAccessTime+=1;
        }	
      		return 1;
    }else if(cache->cacheArray1[index].valid == 1 && cache->cacheArray2[index].valid == 1){
        cache->cacheArray2[index].tag = cache->cacheArray1[index].tag;
        cache->cacheArray1[index].tag = tag;
      		cache->missesSoFar++;
          	cache->totalAccessTime+=102;
      		return 0;
    }
    break;

  //4-associative
  case 2:
    //case if nothing at this index
    if(cache->cacheArray1[index].valid == 0){
        cache->cacheArray1[index].valid = 1;
                cache->cacheArray1[index].tag = tag;
                cache->cacheArray2[index].tag =0;
                cache->cacheArray2[index].valid = 1;
                cache->cacheArray3[index].tag =0;
                cache->cacheArray3[index].valid = 1;
                cache->cacheArray4[index].tag =0;
                cache->cacheArray4[index].valid = 1;
                cache->missesSoFar++;
                cache->totalAccessTime+=103;
                return 0;
    }
    //cases if match in any of the words at this index
    else if(cache->cacheArray1[index].tag==tag){
        int foundAt = whereIs(cache,1);
        reorder(cache,foundAt);	
          	cache->totalAccessTime+=3;
        return 1;
    }
    else if(cache->cacheArray2[index].tag==tag){
        int foundAt = whereIs(cache,2);
        reorder(cache,foundAt);	
          	cache->totalAccessTime+=3;
        return 1;
    }
    else if(cache->cacheArray3[index].tag==tag){
        int foundAt = whereIs(cache,3);
        reorder(cache,foundAt);	
          	cache->totalAccessTime+=3;
        return 1;
    }
    else if(cache->cacheArray4[index].tag==tag){
        int foundAt = whereIs(cache,4);
        reorder(cache,foundAt);	
          	cache->totalAccessTime+=3;
        return 1;
    }else{
      
        int theSwap = cache->lruOrder[3];
        if (theSwap==1)
            cache->cacheArray1[index].tag = tag;
        else if (theSwap==2)
            cache->cacheArray2[index].tag = tag;
        else if (theSwap==3)
            cache->cacheArray3[index].tag = tag;
        else if (theSwap==4)
            cache->cacheArray4[index].tag = tag;
        reorder(cache,theSwap);
                cache->missesSoFar++;
                cache->totalAccessTime+=103;
        return 0;
    }

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

int whereIs(Cache* cache, int targetWord){
    int i;
    for(i=0;i<4;i++){
        if(cache->lruOrder[i]==targetWord)
            return i;
    }
}

void reorder(Cache* cache, int foundAt){
    if(foundAt==1){
        int temp = cache->lruOrder[0];
        cache->lruOrder[0] = cache->lruOrder[1];
        cache->lruOrder[1]= temp;
    }else if (foundAt ==2){
        int temp = cache->lruOrder[0];
        cache->lruOrder[0]=cache->lruOrder[2];
        cache->lruOrder[2]=cache->lruOrder[1];
        cache->lruOrder[1]=temp;
    }else if (foundAt == 3){
        int temp = cache->lruOrder[0];
        cache->lruOrder[0]=cache->lruOrder[3];
        cache->lruOrder[3]=cache->lruOrder[2];
        cache->lruOrder[2]=cache->lruOrder[1];
        cache->lruOrder[1]=temp;
    }
}
