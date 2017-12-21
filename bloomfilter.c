#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "bloomfilter.h"



BloomFilter* initializeFilter(uint8_t numHashes){
	BloomFilter *filter = malloc(sizeof(struct BloomFilter));
	filter->numHashes = numHashes;
	memset(filter->bitVector,false,HASH_SIZE*sizeof(bool));
	return filter;
}

uint64_t* getHashesMurmur(const char* data,size_t len){
	uint64_t* hash = malloc(2 *sizeof(uint64_t));
	MurmurHash3_x64_128(data, strlen(data), 0, hash);
	return hash;
}

uint64_t kthHash(uint8_t k,uint64_t hashA,uint64_t hashB,uint64_t filterSize) {
    return (hashA + k * hashB) % filterSize;
}


void addFilter(BloomFilter* filter,const char* data, size_t len) {
  uint64_t* hashValues = getHashesMurmur(data, len);
  for (int k = 0; k < filter->numHashes ; k++) {
      filter->bitVector[kthHash(k, hashValues[0], hashValues[1], sizeof(filter->bitVector))] = true;
  }
  
  free(hashValues);
  hashValues=NULL;
}

bool possiblyContains(BloomFilter* filter,const char* data, size_t len) {
  uint64_t* hashValues = getHashesMurmur(data, len);
  for (int k = 0; k < filter->numHashes; k++) {
      if (!filter->bitVector[kthHash(k, hashValues[0], hashValues[1], sizeof(filter->bitVector))]) {
      	  free(hashValues);
  		  hashValues=NULL;
          return false;
      }
  }
  free(hashValues);
  hashValues=NULL;
  return true;
}

void freeFilter(BloomFilter* filter){
	free(filter);
	filter = NULL;
}
