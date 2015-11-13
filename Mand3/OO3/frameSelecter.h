#ifndef FRAMESELECTER_H
#define FRAMESELECTER_H

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

(void* f(struct page_table*, int*, int*, int*, void*)) getFifo();
(void* f(struct page_table*, int*, int*, int*, void*)) getRand();
(void* f(struct page_table*, int*, int*, int*, void*)) getCustom();

#endif