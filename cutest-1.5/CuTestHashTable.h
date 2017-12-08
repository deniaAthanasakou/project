#ifndef CU_TEST_HASH_TABLE_H
#define CU_TEST_HASH_TABLE_H
#include "CuTest.h"
#include "../hashTable.h"

#define NOOFBUCKETS 4
#define NOOFCELLS 4


void TestCreateLinearHash(CuTest* tc);
void TestDestroyLinearHash(CuTest* tc);
void TestInsertTrieNode_InsertTrieNodeAgain_SplitBucket(CuTest* tc);
void TestLookupTrieNode(CuTest* tc);
void TestInitializeBucket(CuTest* tc);
void TestGetBucketFromHash(CuTest* tc);
void TestLevelUp(CuTest* tc);
void TestDeletionSortBucket(CuTest* tc);
void TestCreateOverflowCells(CuTest* tc);
void TestGetCell(CuTest* tc);
void TestDeletionSortWithoutErasingNextArray(CuTest* tc);

CuSuite* HashTableGetSuite();
#endif

