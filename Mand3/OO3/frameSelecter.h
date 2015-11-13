
#ifndef FRAME_SELECTER_H
#define FRAME_SELECTER_H

#include "page_table.h"

// struct page_table {
// 	int fd;
// 	char *virtmem;
// 	int npages;
// 	char *physmem;
// 	int nframes;
// 	int *page_mapping;
// 	int *page_bits;
// 	page_fault_handler_t handler;
// };
typedef struct FIFOdata{
	int nextPage;
}fifdata;


struct LRUData *LRUData{
	int *page_history;
	int *page_bits;
};

void (*getFifo())(struct page_table*, int*, int*, int*, void*);
void (*getRand())(struct page_table*, int*, int*, int*, void*);
void (*getCustom())(struct page_table*, int*, int*, int*, void*);

#endif