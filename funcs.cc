#ifndef __X_H_INCLUDED__
#endif
#include <stdio.h>
#include <unistd.h> //for the getopt function
#include <stdlib.h> //for atoi
#include <iostream>
#include <string>
#include <strings.h>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <assert.h>
#include <set>
#include <getopt.h>
#include "common.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <strings.h>
#include <cstdlib>
#include <stdlib.h>
#include <sstream>
#include <string.h>
#include <iomanip>
#include <stddef.h>
#include "CacheSet.h"
#define KILOBYTE	1024
#define INT_SIZE	4
#define MEM_READ 	0
#define MEM_WRITE 	1

void updateDataRam(int index, unsigned int data) {
	ramAllocation[index] = data;
}

unsigned int readDataRamInt(int index) {
	return ramAllocation[index];

}
void cacheMemmoryAllocation() {

	chacheAllocation = (CacheSet*) malloc(
			((totalCacheSets) * sizeof(CacheSet)));

	ptrdiff_t k = 0;

	try {
		for (; k < totalCacheSets; k++)
			new (chacheAllocation + k) CacheSet(blockSize, tagbits_tot,
					indexbits_tot, offsetbits_tot, associativity, k);
	}

	catch (...) {
		for (; k > 0; k--)
			(chacheAllocation + k)->~CacheSet();

		throw;
	}
	if (split) {
		chacheAllocationIns = (CacheSet*) malloc(
				((totalCacheSets) * sizeof(CacheSet)));

		ptrdiff_t k = 0;

		try {
			for (; k < totalCacheSets; k++)
				new (chacheAllocationIns + k) CacheSet(blockSize, tagbits_tot,
						indexbits_tot, offsetbits_tot, associativity, k);
		}

		catch (...) {
			for (; k > 0; k--)
				(chacheAllocationIns + k)->~CacheSet();

			// free(chacheAllocation);
			throw;
		}

	}

}
unsigned int readDataCache(int index, int blockNoInSet, int offset) {
	unsigned int num = chacheAllocation[index].set[blockNoInSet].data[offset];
	return num;

}
char* emptyoffset() {
	char* emptyOffset = (char*) malloc(sizeof(char) * (offsetbits_tot + 1));
	memset(emptyOffset, '0', offsetbits_tot + 1);
	emptyOffset[offsetbits_tot] = '\0';
	return emptyOffset;
}
void ramMemmoryAllocation() {
	int i;
	ramAllocation = (unsigned int*) malloc(ramSize * 4);

	for (i = 0; i < (int) (ramSize / INT_SIZE); i++) {
		updateDataRam(i, 0);
	}

}
std::string wordAddress(int i) {
	std::stringstream stream;
	stream << "" << std::setfill('0') << std::setw(INT_SIZE * 2) << std::hex
			<< i;
	std::string t = stream.str();
	stream.str("");
	return t;
}
void displayStatistics() {
	std::cout << "STATISTICS:" << std::endl;
	int totalAccess = (total_Read + total_Write);
	std::cout << "";
	std::cout << "Total Mem Acces = " << totalAccess << std::endl;

	if (!split) {
		printf("Misses: Total %d DataReads %d DataWrites %d\n", (missRead+ missWrite),
				missRead, missWrite);
		printf("Miss Rate: %f %f %f\n", ((float) miss / totalAccess),
				((float) missRead / totalAccess),
				((float) missWrite / totalAccess));
		printf("Number of Dirty Blocks Evicted From the Cache: %d\n",
				dirtyBlockEvictionCounter);
	}
	if (split) {
		printf("L1I Misses: Total %d InstructionReads %d\n", totalICReads,
				missReadIC);
		printf("L1I Miss Rate: %f\n", missReadIC / (float) totalICReads);

		printf("L1D Misses: Total %d DataReads %d DataWrites %d\n", (missRead+ missWrite),
				missRead, missWrite);
		printf("L1D Miss Rate: %f %f %f\n", ((float) miss / totalAccess),
				((float) missRead / totalAccess),
				((float) missWrite / totalAccess));
		printf("OverAll cache Misses %d\n", (missRead+ missWrite+missReadIC));
		printf("Number of Dirty Blocks Evicted From L1D Cache: %d\n",
				dirtyBlockEvictionCounter);
	}

}
int binaryToInteger(char *bin) {
	int b, k, m, n;
	int len, sum;

	sum = 0;
	len = strlen(bin) - 1;

	for (k = 0; k <= len; k++) {
		n = (bin[k] - '0');
		if ((n > 1) || (n < 0)) {
			return 0;
		}
		for (b = 1, m = len; m > k; m--) {
			b *= 2;
		}
		sum = sum + n * b;
	}
	return (sum);
}
void displayCache(CacheSet *cacheSet, int type) {
	if (split) {
		if (type == 0) {
			std::cout << "L1 DATA CACHE CONTENTS:\n";
			std::cout << "Set\tV\tTag\t\tDirty\t\tWords\n";
		}
		if (type == 1) {
			std::cout << "L1 INSTRUCTION CACHE CONTENTS:\n";
			std::cout << "Set\tV\tTag\t\tWords\n";

		}
	} else {
		std::cout << "CACHE CONTENTS:\n";
		std::cout << "Set\tV\tTag\t\tDirty\t\tWords\n";
	}

	for (int i = 0; i < (totalCacheSets); i++) {
		for (int j = 0; j < associativity; j++) {
			std::cout << std::hex << i << "\t" << cacheSet[i].set[j].v << "\t"
					<< std::setfill('0') << std::setw(8) << std::hex
					<< (int) binaryToInteger(cacheSet[i].set[j].tag) << "\t";
			if (type != 1) {
				std::cout << cacheSet[i].set[j].dirty << "\t" << "\t";
			}
			std::cout << " ";
			for (int k = 0; k < blockSize; k++) {
				std::cout << std::setfill('0') << std::setw(8) << std::hex
						<< (int) readDataCache(i, j, k) << " ";
			}
			std::cout << std::endl;
		}
	}
}
void displayMainMemory() {
	int begin_address = strtoul("003f7f00", NULL, 16);
	std::cout << std::endl << "MAIN MEMORY:" << std::endl
			<< "Address      Words";

	for (int i = begin_address / INT_SIZE;
			i < (begin_address + KILOBYTE) / INT_SIZE; i++) {
		if (i % 8 == 0)
			std::cout << std::endl << std::setfill('0') << std::setw(8)
					<< std::hex << i * INT_SIZE << "   ";
		std::cout << "  " << std::setfill('0') << std::setw(8)
				<< ramAllocation[i];
	}
	std::cout << std::endl;
}

void parseMemoryAddress(char *bformatted, char* tag, char* index,
		char* offset) {
	int i = 0;

	assert(tag != NULL);
	tag[tagbits_tot] = '\0';

	for (i = 0; i < tagbits_tot; i++) {
		tag[i] = bformatted[i];
	}

	assert(index != NULL);
	index[indexbits_tot] = '\0';

	for (i = tagbits_tot + 1; i < indexbits_tot + tagbits_tot + 1; i++) {
		index[i - tagbits_tot - 1] = bformatted[i - 1];
	}

	assert(offset != NULL);
	offset[offsetbits_tot] = '\0';

	for (i = indexbits_tot + tagbits_tot + 2;
			i < offsetbits_tot + indexbits_tot + tagbits_tot + 2; i++) {
		offset[i - indexbits_tot - tagbits_tot - 2] = bformatted[i - 2];
	}

	// printf("Tag: %s (%i)\n", tag, binaryToInteger(tag));
	//  printf("Index: %s (%i)\n", index, binaryToInteger(index));
	// printf("Offset: %s (%i)\n", offset, binaryToInteger(offset));
}

char *getBinary(unsigned int num) {
	char* bstring;
	int i;

	/* Calculate the Binary String */

	bstring = (char*) malloc(sizeof(char) * 33);
	assert(bstring != NULL);

	bstring[32] = '\0';

	for (i = 0; i < 32; i++) {
		bstring[32 - 1 - i] = (num == ((1 << i) | num)) ? '1' : '0';
	}

	return bstring;
}
bool checkMemSize(int memSize) {
	switch (memSize) {
	case 4:
	case 8:
	case 16:
	case 32:
	case 64:

		return true;
		break;
	default:
		break;
	}
	return false;
}
bool checkBlockSize(int blockSize) {
	switch (blockSize) {
	case 4:
	case 8:
	case 16:
	case 32:
	case 64:
	case 128:
	case 256:
	case 512:
		return true;
		break;
	default:
		break;
	}
	return false;
}

bool parseParams(int argc, char *argv[], int& mem_capacity, int& blockSize,
		int &assosiativity, std::string &filename, bool &split) {
	//needed for the parsing of command line options
	int c;
	bool c_flag, b_flag, a_flag, t_flag;
	bool errflg = false;

	c_flag = errflg = b_flag = a_flag = t_flag = false;
	int digit_optind = 0;
	extern char *optarg;
	extern int optopt;
	while (1) {
		int this_option_optind = optind ? optind : 1;
		int option_index = 0;
		static struct option long_options[] = { { "wbwa", 0, 0, 0 }, { "wtwn",
				0, 0, 0 },{ "wbwn", 0, 0, 0 },{ "wtwa", 0, 0, 0 }, { 0, 0, 0, 0 } };

		c = getopt_long(argc, argv, "c:a:b:s::0:1:2:3:", long_options,
				&option_index);
		if (c == -1)
			break;

		switch (c) {
		case 0:
				if (strcmp(long_options[option_index].name, "wbwa") == 0) {
				writeBack = true;
				writeThrough = false;
				writeAllocate = true;
				writeNoAllocate = false;
			}
			if (strcmp(long_options[option_index].name, "wtwn") == 0) {
				writeThrough = true;
				writeBack = false;
				writeNoAllocate = true;
				writeAllocate = false;
			}
			if (strcmp(long_options[option_index].name, "wbwn") == 0) {
				writeThrough = false;
				writeBack = true;
				writeNoAllocate = true;
				writeAllocate = false;
			}
			if (strcmp(long_options[option_index].name, "wtwa") == 0) {
				writeThrough = true;
				writeBack = false;
				writeNoAllocate = false;
				writeAllocate = true;
			}

			break;

		case 's':
			split = true;
			break;
		case 't':
			filename = optarg;
			t_flag = true;
			break;
		case 'c':
			mem_capacity = atoi(optarg);
			c_flag = true;
			break;
		case 'b':
			blockSize = atoi(optarg);
			b_flag = true;
			break;
		case 'a':
			assosiativity = atoi(optarg);
			a_flag = true;
			break;
		case ':': //:  -c without operand
			fprintf(stderr, "Option -%c requires an operand\n", optopt);
			errflg++;
			break;
		case '?':
			fprintf(stderr, "Unrecognised option: -%c\n", optopt);
			errflg = true;
			break;
		default:
			printf("?? getopt returned character code 0%o ??\n", c);
		}
	}

	if (optind < argc) {
		printf("non-option ARGV-elements: ");
		while (optind < argc)
			printf("%s ", argv[optind++]);
		printf("\n");
	}

	if (!checkBlockSize(blockSize)) {
		std::cout
				<< "\nPossible Block capacities are 4, 8, 16, 32, 64, 128, 256, or 512.\n";
		errflg=true;
	}
	if (!checkMemSize(mem_capacity)) {
		std::cout << "\nPossible Memmory capacities are 4, 8, 16, 32, or 64.";
		errflg=true;
	}
	//check if we have all the options and have no illegal options
	if (errflg || !c_flag || !b_flag || !a_flag) {
		fprintf(stderr,
				"usage: %s -c<capacity> -b<wordsize> -a<associativity> -s<for Split> < inputTrace.trace > outputFile.txt \nWrite policies are as follows: --wbwa/--wbwn/--wtwa/--wtwn",
				argv[0]);
		return false;
	}

	return true;

}
