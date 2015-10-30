/*
 * CacheSet.h
 *
 *  Created on: Sep 26, 2015
 *      Author: quamber
 */

#ifndef CACHESET_H_
#include "cacheline.cpp"

#include <cstddef>
#define CACHESET_H_

class CacheSet {

public:
	int noofblocks;
		int index;
		cacheline *set;
		unsigned char** lru;
	CacheSet(int blocksize,int tagbits,int indexbits,int offsetbits,int linesinSet, int indexNo);
	virtual ~CacheSet();
	void init(int blocksize,int tagbits,int indexbits,int offsetbits,int sets);
	void updateLRU(int index);
	int getLRU(int index);
	int minimumLRUBlock();

};

#endif /* CACHESET_H_ */
