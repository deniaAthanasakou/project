#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "bloomfilter.h"


BloomFilter* initializeFilter(uint8_t numHashes){
	BloomFilter *filter = malloc(sizeof(struct BloomFilter));
	filter->numHashes = numHashes;
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

  for (int n = 0; n < sizeof(filter->bitVector) ; n++) {
      filter->bitVector[kthHash(n, hashValues[0], hashValues[1], sizeof(filter->bitVector))] = true;
  }
}

bool possiblyContains(BloomFilter* filter,const char* data, size_t len) {
  uint64_t* hashValues = getHashesMurmur(data, len);

  for (int n = 0; n < sizeof(filter->bitVector); n++) {
      if (!filter->bitVector[kthHash(n, hashValues[0], hashValues[1], sizeof(filter->bitVector))]) {
          return false;
      }
  }
  return true;
}
 
bool bloomFilterSeach(BloomFilter* filter,const char* data){
	
	//edo h sto insert ngram??
	addFilter(filter,data,strlen(data));
	
	//anti gia checkifitemexists
	return possiblyContains(filter,data,sizeof(data));

}


/*
bool bloomfilterSearch(char* string){
	uint32_t hash[4];                // Output for the hash 
	uint32_t seed = 1000;              //Seed value for hash 

	//printf("Input: \"%s\"\n", string);

	MurmurHash3_x86_32(string, strlen(string), seed, hashTable);
	printf("x86_32:  %d\n", hashTable[0]);
	
	MurmurHash3_x64_128(string, strlen(string), seed, hashTable);
	printf("x64_128: %d %d %d %d\n",
		 hashTable[0], hashTable[1], hashTable[2], hashTable[3]);

	MurmurHash3_x86_128(string, strlen(string), seed, hash);
	//printf("x86_128: %u %u %u %u\n",
		 //hash[0], hash[1], hash[2], hash[3]);
	
	int counterTrue = 0;
	for(int i=0; i<4;i++){
		if(hashTable[hash[i] % HASH_SIZE]==1){
			counterTrue++;
		}
		else{
			hashTable[hash[i] % HASH_SIZE] = 1;
		}
	}
	
	if(counterTrue == 4){
		return true;
	}
	else{
		return false;
	}

}


void printHash(){
	for(int i=0;i<HASH_SIZE;i++){
		printf("hashTable position: %d is %d\n",i,hashTable[i]);
	
	}

}*/
