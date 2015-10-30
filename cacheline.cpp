/*
 * cache.cpp
 *
 *  Created on: Sep 10, 2015
 *      Author: Muhammad Quamber Ali
 */




#ifndef __C_H_INCLUDED__   // if __C_H_INCLUDED__ hasn't been included yet...
#define __C_H_INCLUDED__   //   #define this so the compiler knows it has been included
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

class cacheline{
public:
	cacheline(int blocksize,int tagbits,int indexbits,int offsetbits){
		v=0;
			dirty=0;
			data=( unsigned int  *) malloc( ((blocksize*(sizeof(unsigned int)))));
			memset(data, 0, ( (blocksize*(sizeof(int)))) );
			tag= (char *) malloc( sizeof(char) * (tagbits) );
			memset(tag, '0', sizeof(char) * (tagbits) );
			tag[tagbits-1]='\0';
			index= (char *) malloc( sizeof(char) * (indexbits)+1 );
			memset(index,'0',sizeof(char) * (indexbits)+1);
			index[indexbits]='\0';
			offset =(char *) malloc( (sizeof(char) *offsetbits)+1);
			memset(offset,'0',(sizeof(char) * offsetbits)+1);
			offset[offsetbits]='\0';
			fromAddress = 0;
	};
	void init(int chacheCapacity,int tagbits, int indexbits,int offsetbits){

};
 int v;
int dirty;

unsigned int *data;
char *index;
char *tag;
char *offset;
int fromAddress;
};
#endif
