
#include <strings.h>

bool parseParams(int argc, char *argv[], int& mem_capacity, int& blockSize,
		int &assosiativity, std::string &filename, bool &split);
std::string string_to_hex(const  std::string& input);
std::string hex_to_string(const  std::string& input);
char *getBinary(unsigned int num);
int binaryToInteger(char *bin);
void parseMemoryAddress(char *bformatted, char* tag, char* index, char* offset);
void displayMainMemory();
void displayStatistics();
unsigned int readDataCache(int index, int blockNoInSet,int offset);
std::string wordAddress(int i);
void ramMemmoryAllocation();
void cacheMemmoryAllocation();
void updateDataRam(int index, unsigned int data);
char* emptyoffset();
unsigned int readDataRamInt(int index);
void displayCache(CacheSet *cacheSet, int type) ;
std::string execq(const char* cmd);


