
#ifndef FRAME_SELECTER_H
#define FRAME_SELECTER_H
#include "page_table.h"

struct LRUData{
	int *page_history;
	int *page_bits;
	double timestamp;
};


struct frame_table{
	unsigned int *map;
};

void (*getFifo()) 	(struct page_table*, struct frame_table*, int*, void*);
void (*getRand()) 	(struct page_table*, struct frame_table*, int*, void*);
void (*getCustom()) (struct page_table*, struct frame_table*, int*, void*);


#endif