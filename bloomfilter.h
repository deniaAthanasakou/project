#ifndef _BLOOMFILTER_H_
#define _BLOOMFILTER_H_

#include <stdbool.h>
#include "./murmur3/murmur3.h"
#define HASH_SIZE 10000


typedef struct BloomFilter{
	bool bitVector[HASH_SIZE];
	uint8_t numHashes;			//number of hashFunctions used
}BloomFilter;

BloomFilter* initializeFilter(uint8_t numHashes);
uint64_t* getHashesMurmur(const char* data,size_t len);
uint64_t kthHash(uint8_t k,uint64_t hashA,uint64_t hashB,uint64_t filterSize);
void addFilter(BloomFilter* filter,const char* data, size_t len);
bool possiblyContains(BloomFilter* filter,const char* data, size_t len);
void freeFilter(BloomFilter* filter);


#endif
