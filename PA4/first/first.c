#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "first.h"

int main(int argc, char** argv) {

	if (argc!= 6) {
		fprintf(stderr,"error\n");
		exit(1);
	}

	checkParameters(argv[1], argv[2], argv[3], argv[4], argv[5]);
	fillCache(argv[1], argv[2], argv[3], argv[4], argv[5]);

	initCache();
	doSimulation(NOPREFETCH);

	initCache();
	doSimulation(PREFETCH);

	return 0;
}

void checkParameters(char* inputCacheSize, char* inputAssociativity, char* inputCachePolicy, char* inputBytesInBlock, char* inputFileName){
	if(isValidNum(inputCacheSize) == FALSE || isValidNum(inputBytesInBlock) == FALSE)
	{
		fprintf(stderr,"error\n");
		exit(1);
	}
	if(isPowerOfTwo(atoi(inputCacheSize)) == FALSE || isPowerOfTwo(atoi(inputBytesInBlock)) == FALSE)
	{
		fprintf(stderr,"error\n");
		exit(1);
	}

	if (strcmp(inputAssociativity,"direct") != 0 && strcmp(inputAssociativity, "assoc") != 0)
	{
		if (strstr (inputAssociativity, "assoc:") != NULL) 
		{
			if(isValidNum(inputAssociativity+strlen("assoc:")) == FALSE)
			{
				fprintf(stderr,"error\n");
				exit(1);
			}
			if(isPowerOfTwo(atoi(inputAssociativity+strlen("assoc:"))) == FALSE)
			{
				fprintf(stderr,"error\n");
				exit(1);
			}
		}
		else 
		{
			fprintf(stderr,"error\n");
			exit(1);
		}
	}

	if(strcmp(inputCachePolicy,"lru") != 0 && strcmp(inputCachePolicy,"fifo") != 0)
	{
		fprintf(stderr,"error\n");
		exit(1);
	}

	FILE *file = fopen(inputFileName, "r");
	if (file == NULL) {
		fprintf(stderr, "error\n");
		exit(1);
	}
	fclose(file);

}

void fillCache(char* inputCacheSize, char* inputAssociativity, char* inputCachePolicy, char* inputBytesInBlock, char* inputFileName) {	
	fileName = inputFileName;

	if (strcmp(inputCachePolicy,"lru")==0){
		theCachePolicy = LRU;
	}
	else if(strcmp(inputCachePolicy,"fifo")==0){
		theCachePolicy = FIFO;
	}
	
	numBytesInCache = atoi(inputCacheSize);
	numBytesInBlock	= atoi(inputBytesInBlock);
	numBlocksInCache = numBytesInCache / numBytesInBlock;
	
	if (strcmp(inputAssociativity, "assoc") == 0){
		numBlocksInSet = numBlocksInCache;
	}
	else if (strcmp(inputAssociativity,"direct") == 0){
		numBlocksInSet = 1;
	}
	else {
		numBlocksInSet = atoi(inputAssociativity+strlen("assoc:"));
	}

	numSetsInCache = numBlocksInCache / numBlocksInSet;		
	tagStartIndex = logBaseTwo(numBytesInBlock) + logBaseTwo(numSetsInCache);		
	tagLength = 48 - logBaseTwo(numBytesInBlock) - logBaseTwo(numSetsInCache);
	indexStartIndex = logBaseTwo(numBytesInBlock);
	indexLength = logBaseTwo(numSetsInCache);

	if (numBytesInCache != numBytesInBlock * numBlocksInCache || numBytesInCache != numBytesInBlock * numBlocksInSet * numSetsInCache){
		fprintf(stderr,"error\n");
		exit(1);
	}

	if (numBytesInCache <= 0 || numBytesInBlock <= 0 || numBlocksInSet <=0 || numBlocksInCache <= 0 || numSetsInCache <= 0) {
		fprintf(stderr,"error\n");
		exit(1);
	}

}

void initCache() {
	cacheBlocks = malloc(sizeof(cacheBlock) * numBlocksInCache);
	cache = malloc(sizeof(cacheSet) * numSetsInCache);	

	int i = 0;
	while (i < numBlocksInCache) {
		cacheBlock* curBlock = cacheBlocks + i;
		curBlock->tag = 0;
		curBlock->valid	= FALSE;
		curBlock->LRUcount = -1;
		i++;
	}

	i = 0;
	while (i < numSetsInCache) {
		cacheSet* curSet = cache + i;
		curSet->blocksInSet = cacheBlocks + i * numBlocksInSet;
		curSet->nextIndexToWrite = 0;
		i++;
	}

	reads = 0;
	writes = 0;
	hits = 0;
	misses = 0;

}

void doSimulation(int prefetch) {
    FILE *file;
	char line[128];
	char programCounter[32];
	char mode;
	char address[32];
	
	for (file = fopen(fileName, "r"); fgets(line, 128, file) != NULL; ) {
		sscanf(line, "%s %c %s", programCounter, &mode, address);
		if (strcmp(programCounter, "#eof") == 0){
			break;
		}
		if (mode=='R' || mode=='r') {
			executeLine(READ, hexToLong(address), prefetch, FALSE);
		}
		else if (mode=='W' || mode=='w') {
			executeLine(WRITE, hexToLong(address), prefetch, FALSE);
		}
	}
	if (file) {
		fclose(file);
	}

	if(prefetch == NOPREFETCH){
		printf("no-prefetch\n");
	}
	else{
		printf("with-prefetch\n");
	}
	printf("Memory reads: %d\n", reads);
	printf("Memory writes: %d\n", writes);
	printf("Cache hits: %d\n", hits);
	printf("Cache misses: %d\n", misses);
}

void executeLine(int mode, unsigned long address, int prefetch, int adjacentBlock) {
	int index = (int) getIndexOrTag(address, INDEX);
	unsigned long tag = getIndexOrTag(address, TAG);
	cacheSet* curSet = cache + index;
	cacheBlock* curBlock;

	int isHit = FALSE;
	int i = 0;
	while (i < numBlocksInSet) {
		curBlock = (curSet->blocksInSet + i);
		if (curBlock->tag == tag && curBlock->valid == TRUE) {
			isHit = TRUE;
			if(adjacentBlock == FALSE){
				if (mode == READ) {
					hits++;
				}
				else if(mode == WRITE){
					hits++;
					writes++;
				}
				if(theCachePolicy == LRU){
					curSet->nextIndexToWrite = getNextIndexToWrite(curSet, i);
				}
			}
			break;
		}
		i++;
	}

	if (isHit == FALSE) {
		reads++;
		curBlock = (curSet->blocksInSet + curSet->nextIndexToWrite);
		curBlock->tag = tag;
		curBlock->valid = TRUE;

		if(adjacentBlock == FALSE){
			if (mode == READ) {
				misses++;
			}
			else if (mode== WRITE){
				misses++;
				writes++;
			}
		}

		if(theCachePolicy == FIFO){
			curSet->nextIndexToWrite++;
			curSet->nextIndexToWrite = curSet->nextIndexToWrite % numBlocksInSet;
		}
		else if(theCachePolicy == LRU){
			curSet->nextIndexToWrite = getNextIndexToWrite(curSet, curSet->nextIndexToWrite);
		}		
		
		if(prefetch == PREFETCH){
			executeLine(mode, address + numBytesInBlock, NOPREFETCH, TRUE);
		}
	}
}

unsigned long hexToLong(char* input) {
	unsigned long output = 0;
	int index = 0;

	if (*(input + index) == '0' && (*(input + index + 1) == 'x' || *(input + index + 1) == 'X') ) {
		index = index + 2;
	}

	while (*(input + index) != 0) {
		char oneChar = *(input + index);
		output = output * 16;
		if (oneChar > '0' && oneChar <= '9') {
			output = output + (oneChar - '0');
		}
		else if (oneChar >= 'a' && oneChar <= 'f') {
			output = output + (oneChar - 'a') + 10;
		}
		else if (oneChar >= 'A' && oneChar <= 'F') {
			output = output + (oneChar - 'A') + 10;
		}
		else {
		}
		index++;
	}
	return output;
}

int isValidNum(char* input){
	int i;
	for(i = 0; i < strlen(input); i++)
	{
		if(isdigit(input[i]) == 0)
			return FALSE;
	}
	return TRUE;
}

int isPowerOfTwo(int numIn){
	int pow = 1;

	while(pow <= numIn){
		if(pow == numIn){
			return TRUE;
		}
		else{
			pow *=2;
		}
	}
	return FALSE;
}

int logBaseTwo(int inputBytes) {
	int i = 0;
	for (i = 0; i < 31; i++) {
		if ((inputBytes >> i) == 1)	{
			break;
		}
	}
	return i;
}
unsigned long getIndexOrTag(unsigned long input, int field) {
	int length;
	int start;
	if(field == INDEX){
		length = indexLength;
		start = indexStartIndex;
	}
	else if(field == TAG){
		length = tagLength;
		start = tagStartIndex;
	}

	unsigned long output;
	if (length > 0) {
		output  = input << (64  - length - start);
		output  = output >> (64  - length);			
		
	}
	else {
		output = 0;	
	}
	return output;
}

int getNextIndexToWrite(cacheSet* curSet, int justAccessedIndex){
	int i = 0;	
	int LRUindex = 0;
	int LRUvalue = 0;
	cacheBlock* curBlock;
	while(i < numBlocksInSet)
	{
		curBlock = curSet->blocksInSet + i;
		if(i == justAccessedIndex){
			curBlock->LRUcount = 0;
		}
		else if(curBlock->LRUcount == -1){
			return i;	
		}
		else{
			curBlock->LRUcount++;
		}
		i++;
	}

	i = 0;
	while(i < numBlocksInSet)
	{
		curBlock = curSet->blocksInSet + i;
		if(curBlock->LRUcount > LRUvalue){
				LRUindex = i;
				LRUvalue = curBlock->LRUcount;
		}
		i++;
	}
	return LRUindex;
}
