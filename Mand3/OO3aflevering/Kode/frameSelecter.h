#ifndef FRAME_SELECTER_H
#define FRAME_SELECTER_H
#include "page_table.h"

struct FIFOData { 
	int nextFrame;
};

struct LRUData{
	unsigned int *page_history;
	int *page_bits;
	double timestamp;
};
struct RANDData{
	struct page_table* pt;
};


struct frame_table{
	int *map;
};

void (*getFifo()) 	(struct frame_table*, int, int, int*, void*);
void (*getRand()) 	(struct frame_table*, int, int, int*, void*);
void (*getRandOpt())(struct frame_table*, int, int, int*, void*);
void (*getCustom()) (struct frame_table*, int, int, int*, void*);
struct LRUData* createLRUData(int);
struct frame_table* createFrameTable(int);

#endif