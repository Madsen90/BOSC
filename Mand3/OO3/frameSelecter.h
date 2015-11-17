
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


struct frame_table{
	int *map;
};

void (*getFifo()) 	(struct page_table*, struct frame_table*, int*, void*);
void (*getRand()) 	(struct page_table*, struct frame_table*, int*, void*);
void (*getCustom()) (struct page_table*, struct frame_table*, int*, void*);


#endif