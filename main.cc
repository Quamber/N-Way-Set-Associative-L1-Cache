#include <stdio.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string>
#include <cstring>
#include <stdio.h>
#include <errno.h>
#include <strings.h>
#include <cstdlib>
#include <stdlib.h>
#include <sstream>
#include <string.h>
#include <iomanip>
#include <stddef.h>
#include "common.h"
#include <fstream>
#include <math.h>
#include "funcs.h"
#ifndef __C_H_INCLUDED__
#include "cacheline.cpp"
#endif

extern bool split;
extern int offsetbits_tot;
extern int tagbits_tot;
extern int indexbits_tot;
extern int mem_capacity;
extern int associativity;
extern int totalWords;
extern unsigned int* ramAllocation;
extern int ramSize;
extern int missRead, missWrite;
extern CacheSet *chacheAllocation;
extern CacheSet *chacheAllocationIns;
extern int blockSize;
extern std::string traceFile;
extern int total_Read;
extern int total_Write;
extern int hit, miss;
extern int chacheCapacity, chacheSize;
extern int totalChacheBlocks;
extern int dirtyBlockEvictionCounter;

/*

 */

#define SIZEOFINT sizeof(int)
#define MEM_READ  0
#define MEM_WRITE 1
#define INS_READ  2

void memoryOperations(int ramSize, char *get_argvs[]);
void loadBlock(CacheSet *cacheset, int setNumber, int blockNumber,
		cacheline line, int ramaddress);
void writeBlock(CacheSet *cacheset, int setNumber, int blockNumber,
		cacheline line, int ramaddress);
void updateDataCache(CacheSet *cacheset, int setNumber, int freeBlockNo,
		int offset, unsigned int data, cacheline line, int ramaddress);
void displayStatistics();
int findBlockNumber(CacheSet *cacheset, int setNumber, char* tag) {
	int blockNo = -1;
	for (int i = 0; i < associativity; i++) {
		if (strcmp(cacheset[setNumber].set[i].tag, tag) == 0) {
			blockNo = i;
			cacheset[setNumber].updateLRU(i);
			return blockNo;
		}
	}
	return blockNo;
}
int findVacantBlock(CacheSet *cacheset, int setNumber) {
	int blockNo = -1;
	for (int i = 0; i < associativity; i++) {
		if (cacheset[setNumber].set[i].v == 0) {
			blockNo = i;
			return blockNo;
		}
	}
	return blockNo;
}
int findMinimumLruBlock(CacheSet *cacheset, int setNumber) {

	int blockNo = cacheset[setNumber].minimumLRUBlock();
	return blockNo;
}

void updateDataCache(CacheSet *cacheset, int setNumber, int freeBlockNo,
		int offset, unsigned int data, cacheline line, int ramaddress) {
	try {
		cacheset[setNumber].set[freeBlockNo].v = 1;
		strcpy(cacheset[setNumber].set[freeBlockNo].tag, line.tag);
		cacheset[setNumber].set[freeBlockNo].data[offset] = data;
	int ramIndex = ramaddress;
		cacheset[setNumber].set[freeBlockNo].fromAddress = ramIndex;
	} catch (char* msg) {
		std::cout << "Exception" << msg;
	}

}

void loadBlock(CacheSet *cacheset, int setNumber, int blockNumber,
		cacheline line, int ramaddress) {
	cacheline temp = line;

	int ramIndex = ramaddress - (ramaddress % blockSize);

	for (int i = 0; i < blockSize; i++) {
		updateDataCache(cacheset, binaryToInteger(line.index), blockNumber, i,
				readDataRamInt(ramIndex + (i)), line, ramaddress);
	}
}
void writeBlock(CacheSet *cacheset, int setNumber, int blockNumber,
		cacheline line, int ramaddress) {
	cacheline temp = line;
	int ramIndex = ramaddress - (ramaddress % blockSize);

	for (int i = 0; i < blockSize; i++) {
		updateDataRam((ramIndex + (i)),
				readDataCache(binaryToInteger(line.index), blockNumber, i));
	}
	chacheAllocation[binaryToInteger(line.index)].set[blockNumber].v = 1;
	chacheAllocation[binaryToInteger(line.index)].set[blockNumber].dirty = 0;
	strcpy(chacheAllocation[binaryToInteger(line.index)].set[blockNumber].tag,
			line.tag);

}
int blockToWrite(CacheSet *cacheset, int setNumber, cacheline line) {
	int blockNo = findVacantBlock(cacheset, setNumber);
	if (blockNo == -1) {
		blockNo = findMinimumLruBlock(cacheset, setNumber);
	}
	return blockNo;
}

void memoryOperations(int ramSize, char *get_argvs[]) {
	ramMemmoryAllocation();
	cacheMemmoryAllocation();
	int read_write;
	int address;
	int current_offset = 0;
	unsigned int data;
	try {
		while (!feof(stdin)) {

			bool missFlag = false;
			std::cin >> std::dec >> read_write;
			if (feof(stdin))
				break;
			std::cin >> std::hex >> address;
			char* thirty2Bit = getBinary(address);
			cacheline temp(blockSize, tagbits_tot, indexbits_tot,
					offsetbits_tot);
			parseMemoryAddress(thirty2Bit, temp.tag, temp.index, temp.offset);
			current_offset = binaryToInteger(temp.offset) / 4;
			if (read_write == MEM_WRITE) {
				total_Write++;

				std::cin >> std::hex >> data;
				int blockNo = findBlockNumber(chacheAllocation,
						binaryToInteger(temp.index), temp.tag);
				if (blockNo == -1) {
					blockNo = blockToWrite(chacheAllocation,
							binaryToInteger(temp.index), temp);
					if (chacheAllocation[binaryToInteger(temp.index)].set[blockNo].dirty
							== 1) {

						miss++;
						missFlag = true;
						writeBlock(chacheAllocation,
								binaryToInteger(temp.index), blockNo, temp,
								address);
						dirtyBlockEvictionCounter++;
						if (writeAllocate) {
							loadBlock(chacheAllocation,
									binaryToInteger(temp.index), blockNo, temp,
									address);
							updateDataCache(chacheAllocation,
									binaryToInteger(temp.index), blockNo,
									current_offset, data, temp, address);
							chacheAllocation[binaryToInteger(temp.index)].set[blockNo].dirty =
									1;
							chacheAllocation[binaryToInteger(temp.index)].updateLRU(
									blockNo);
						}
						if (writeNoAllocate) {
							updateDataRam(address, data);
						}
					} else {
						miss++;
						missFlag = true;
						if (writeAllocate) {
							loadBlock(chacheAllocation,
									binaryToInteger(temp.index), blockNo, temp,
									address);
							updateDataCache(chacheAllocation,
									binaryToInteger(temp.index), blockNo,
									current_offset, data, temp, address);
							chacheAllocation[binaryToInteger(temp.index)].set[blockNo].dirty =
									1;
							chacheAllocation[binaryToInteger(temp.index)].updateLRU(
									blockNo);
						}
						if (writeNoAllocate) {
							updateDataRam(address, data);
						}

					}
				} else {
					hit++;
					if (writeBack) {
						loadBlock(chacheAllocation, binaryToInteger(temp.index),
								blockNo, temp, address);
						updateDataCache(chacheAllocation,
								binaryToInteger(temp.index), blockNo,
								current_offset, data, temp, address);
						chacheAllocation[binaryToInteger(temp.index)].set[blockNo].dirty =
								1;
					}
					if (writeThrough) {
						loadBlock(chacheAllocation, binaryToInteger(temp.index),
								blockNo, temp, address);
						updateDataCache(chacheAllocation,
								binaryToInteger(temp.index), blockNo,
								current_offset, data, temp, address);
						chacheAllocation[binaryToInteger(temp.index)].set[blockNo].dirty =
								0;
						updateDataRam(address, data);
					}
				}
				if (missFlag) {
					missWrite++;
				}
			} else if (read_write == MEM_READ) {
				int blockNo = findBlockNumber(chacheAllocation,
						binaryToInteger(temp.index), temp.tag);
				if (blockNo == -1) {
					blockNo = blockToWrite(chacheAllocation,
							binaryToInteger(temp.index), temp);
					loadBlock(chacheAllocation, binaryToInteger(temp.index),
							blockNo, temp, address);
					chacheAllocation[binaryToInteger(temp.index)].updateLRU(
							blockNo);
					miss++;
					missFlag = true;
				} else {
					hit++;
					loadBlock(chacheAllocation, binaryToInteger(temp.index),
							blockNo, temp, address);
				}
				if (missFlag) {
					missRead++;
				}
				total_Read++;
				//std::cout << std::hex << "memory[" << address << "] = " << readDataCache(binaryToInteger(temp.index),binaryToInteger(temp.current_offset))
				//	<< std::endl;
			} else if (split && read_write == INS_READ) {
				if (split) {
					int blockNo = findBlockNumber(chacheAllocationIns,
							binaryToInteger(temp.index), temp.tag);
					if (blockNo == -1) {
						blockNo = blockToWrite(chacheAllocationIns,
								binaryToInteger(temp.index), temp);
						loadBlock(chacheAllocationIns,
								binaryToInteger(temp.index), blockNo, temp,
								address);
						chacheAllocationIns[binaryToInteger(temp.index)].updateLRU(
								(blockNo));

						missFlag = true;
					} else {
						hit++;
						loadBlock(chacheAllocationIns,
								binaryToInteger(temp.index), blockNo, temp,
								address);
						//chacheAllocationIns[binaryToInteger(temp.index)].updateLRU((blockNo));

					}
					if (missFlag) {
						missReadIC++;
					}
					totalICReads++;
				} else if (!split) {
					int blockNo = findBlockNumber(chacheAllocation,
							binaryToInteger(temp.index), temp.tag);
					if (blockNo == -1) {
						blockNo = blockToWrite(chacheAllocation,
								binaryToInteger(temp.index), temp);
						loadBlock(chacheAllocation, binaryToInteger(temp.index),
								blockNo, temp, address);
						chacheAllocation[binaryToInteger(temp.index)].updateLRU(
								blockNo);
						miss++;
						missFlag = true;
					} else {
						hit++;
						loadBlock(chacheAllocation, binaryToInteger(temp.index),
								blockNo, temp, address);
					}
					if (missFlag) {
						missRead++;
					}
					total_Read++;
				}

			}

		}
	} catch (char* msg) {
		std::cout << msg;
	}
}

int main(int arg_count, char *arg_values[]) {

	if (!parseParams(arg_count, arg_values, chacheCapacity, blockSize,
			associativity, traceFile, split)) {
		exit(2);
	}
	chacheSize = chacheCapacity * 1024;
	totalChacheBlocks = chacheSize / blockSize;
	totalCacheSets = totalChacheBlocks / associativity;
	indexbits_tot = log2(totalCacheSets);
	offsetbits_tot = log2(blockSize);
	blockSize = blockSize / SIZEOFINT;
	tagbits_tot = 32 - (indexbits_tot + offsetbits_tot);
	ramSize = ((mem_capacity * 1024 * 1024));
	totalWords = ramSize / blockSize;
	if (split) {
		chacheSize = chacheCapacity * 1024;
		totalChacheBlocks = chacheSize / (blockSize * 2);
		totalCacheSets = totalChacheBlocks / associativity;
		indexbits_tot = log2(totalCacheSets);
		offsetbits_tot = log2(blockSize);
		tagbits_tot = 32 - (indexbits_tot + offsetbits_tot);
		ramSize = ((mem_capacity * 1024 * 1024));
		totalWords = ramSize / blockSize;

	}
//	printf("Let's check the variables first\n");
//	printf("Chache Capacity : %d KB\n", chacheCapacity);
	;
//	printf("RAm Capacity : %d B\n", ramSize);
	;
//	printf("Block Size (optional): %d bytes\n", blockSize);
//	printf("Associativity Size (optional): %d bytes\n", associativity);
//	printf("Split :  %i : ", split);
	memoryOperations(ramSize, arg_values);
	displayStatistics();
	displayCache(chacheAllocation, 0);
	if (split) {
		displayCache(chacheAllocationIns, 1);
	}
	displayMainMemory();
	free(ramAllocation);
	free(chacheAllocation);

}
