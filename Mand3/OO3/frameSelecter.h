
#ifndef FRAME_SELECTER_H
#define FRAME_SELECTER_H
#include "page_table.h"

typedef struct FIFOdata{
	int nextPage;
} fifdata;


struct LRUData{
	int *page_history;
	int *page_bits;
	int *page_frame;
	double timestamp;
};

void (*getFifo()) 	(struct page_table*, int*, void*);
void (*getRand()) 	(struct page_table*, int*, void*);
void (*getCustom()) (struct page_table*, int*, void*);


#endif