
#ifndef FRAME_SELECTER_H
#define FRAME_SELECTER_H
#include "page_table.h"

typedef struct FIFOdata{
	int nextPage;
} fifdata;


struct LRUData{
	int *page_history;
	int *page_bits;
};

void (*getFifo()) (struct page_table*, int*, int*, int*, void*);
void (*getRand()) (struct page_table*, int*, int*, int*, void*);
void (*getCustom()) (struct page_table*, int*, int*, int*, void*);


#endif