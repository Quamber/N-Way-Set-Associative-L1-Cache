/*
 * CacheSet.cpp
 *
 *  Created on: Sep 26, 2015
 *      Author: quamber
 */

#include "CacheSet.h"
#include <memory>
CacheSet::CacheSet(int blocksize, int tagbits, int indexbits, int offsetbits,
		int linesinSet, int indexNo) {
	noofblocks = linesinSet;
	index = indexNo;
	init(blocksize, tagbits, indexbits, offsetbits, linesinSet);
	lru = new unsigned char*[noofblocks];
	for (int i = 0; i < noofblocks; ++i) {
		lru[i] = new unsigned char[noofblocks];
	}
	for (int i = 0; i < noofblocks; ++i) {
		for (int j = 0; j < noofblocks; ++j) {
			lru[i][j] = 0;
		}
	}

}
void CacheSet::init(int blocksize, int tagbits, int indexbits, int offsetbits,
		int sets) {
	set = (cacheline*) malloc((sizeof(cacheline) * sets));
	ptrdiff_t k = 0;

	try {
		for (; k < sets; k++)
			new (set + k) cacheline(blocksize, tagbits, indexbits, offsetbits);
	}

	catch (...) {
		for (; k > 0; k--)
			(this->set + k)->~cacheline();

		throw;
	}
}
int CacheSet::minimumLRUBlock() {
	int min = 4;
	int temp;
	for (int i = 0; i < noofblocks; ++i)
		{temp=0;
		for(int j = 0; j < noofblocks; ++j)
		{
			temp+=(short)lru[i][j];
		}
		if(temp<min){min=temp;}
	}
return min;
}
void CacheSet::updateLRU(int index) {
for (int i = 0; i < noofblocks; ++i) {
	lru[index][i] = 1;
	lru[i][index] = 0;
}
}
int CacheSet::getLRU(int index) {
for (int i = 0; i < noofblocks; ++i) {
	int temp =0;
	temp=0;
			for(int j = 0; j < noofblocks; ++j)
			{
				temp+=(short)lru[index][j];
			}
			return temp;
}
}
CacheSet::~CacheSet() {
free(set);
}

