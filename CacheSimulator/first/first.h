enum Status{FALSE, TRUE};
enum ReadWrite{READ, WRITE};
enum Policy{FIFO, LRU};
enum Prefetch{NOPREFETCH, PREFETCH};
enum field{TAG, INDEX};

char* fileName;
int theCachePolicy;
int	numBytesInCache;
int	numBlocksInCache;	
int	numSetsInCache;
int	numBytesInBlock;
int	numBlocksInSet;
int tagStartIndex;								
int tagLength;
int indexStartIndex;
int indexLength;
unsigned int hits;
unsigned int misses;
unsigned int reads;
unsigned int writes;

typedef struct cacheBlock {
	unsigned long tag;
	int valid;
	int LRUcount;
} cacheBlock;

typedef struct cacheSet {
	cacheBlock* blocksInSet;
	int	nextIndexToWrite;
} cacheSet;

cacheBlock * cacheBlocks;
cacheSet * cache;

void checkParameters(char* inputCacheSize, char* inputAssociativity, char* inputCachePolicy, char* inputBytesInBlock, char* inputFileName);
void fillCache(char* inputCacheSize, char* inputAssociativity, char* inputCachePolicy, char* inputBytesInBlock, char* inputFileName);
void initCache();
void doSimulation(int prefetch) ;
void executeLine(int mode, unsigned long address, int prefetch, int adjacentBlock) ;
unsigned long hexToLong(char* input);
int isValidNum(char* input);
int isPowerOfTwo(int numIn);
int logBaseTwo(int inputBytes);
unsigned long getIndexOrTag(unsigned long input, int field);
int getNextIndexToWrite(cacheSet* curSet, int justAccessedIndex);
